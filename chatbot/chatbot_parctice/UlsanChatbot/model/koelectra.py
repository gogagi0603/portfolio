import torch
import torch.nn as nn
from torch.nn import CrossEntropyLoss, MSELoss
from transformers.activations import get_activation
from transformers import (
  ElectraPreTrainedModel,
  ElectraModel,
)
import sys
sys.path.append("..")
from UlsanChatbot.model.Focal_loss import focal_loss, label_to_one_hot_label
import UlsanChatbot.options as options
# MODEL_CLASSES = {
#     "koelectra-base": (ElectraConfig, koElectraForSequenceClassification, ElectraTokenizer),
#     "koelectra-small": (ElectraConfig, koElectraForSequenceClassification, ElectraTokenizer),
#     "koelectra-base-v2": (ElectraConfig, koElectraForSequenceClassification, ElectraTokenizer),
#     "koelectra-small-v2": (ElectraConfig, koElectraForSequenceClassification, ElectraTokenizer),
# }
#by 태관 fine tune 내용
class ElectraClassificationHead(nn.Module):
  """Head for sentence-level classification tasks."""

  def __init__(self, config, num_labels):
    super().__init__()
    self.dense = nn.Linear(config.hidden_size, 4*config.hidden_size)
    self.dropout = nn.Dropout(config.hidden_dropout_prob)
    self.out_proj = nn.Linear(4*config.hidden_size,num_labels)

  def forward(self, features, **kwargs):
    x = features[:, 0, :]  # take <s> token (equiv. to [CLS])
    x = self.dropout(x)
    x = self.dense(x)
    x = get_activation("gelu")(x)  # although BERT uses tanh here, it seems Electra authors used gelu here
    x = self.dropout(x)
    x = self.out_proj(x)
    return x
#by 태관 model = koelectraForSequenceClassification.for_pretrained(~~)
class koElectraForSequenceClassification(ElectraPreTrainedModel):
  def __init__(self,
               config,
               num_labels):
    super().__init__(config)
    self.num_labels = num_labels
    self.electra = ElectraModel(config)
    self.classifier = ElectraClassificationHead(config, num_labels)
    self.init_weights()
  def forward(
          self,
          input_ids=None,
          attention_mask=None,
          token_type_ids=None,
          position_ids=None,
          head_mask=None,
          inputs_embeds=None,
          labels=None,
          output_attentions=None,
          output_hidden_states=None,

  ):
    r"""
    labels (:obj:`torch.LongTensor` of shape :obj:`(batch_size,)`, `optional`, defaults to :obj:`None`):
        Labels for computing the sequence classification/regression loss.
        Indices should be in :obj:`[0, ..., config.num_labels - 1]`.
        If :obj:`config.num_labels == 1` a regression loss is computed (Mean-Square loss),
        If :obj:`config.num_labels > 1` a classification loss is computed (Cross-Entropy).
    """
    discriminator_hidden_states = self.electra(
      input_ids,
      attention_mask,
      token_type_ids,
      position_ids,
      head_mask,
      inputs_embeds,
      output_attentions,
      output_hidden_states,
    )

    sequence_output = discriminator_hidden_states[0]
    logits = self.classifier(sequence_output)

    outputs = (logits,) + discriminator_hidden_states[1:]  # add hidden states and attention if they are here
# 분류 레이블이 2개면 다중 분류 mean square err, 그 이상이면 crossentropy
    if labels is not None:
      if self.num_labels == 1:
        #  We are doing regression
        loss_fct = MSELoss()
        loss = loss_fct(logits.view(-1), labels.view(-1))
      else:
# crossentropy는 데이터 불균형 문제에서 취약. 논문참조 focal loss
        #loss_fct = CrossEntropyLoss()
        #loss = loss_fct(logits.view(-1, self.num_labels), labels.view(-1))
        loss = focal_loss(logits.view(-1, self.num_labels),labels.view(-1),alpha=0.5,gamma=2.0, reduction='mean', eps=5e-6, ignore_index=options.get_label_num())
      outputs = (loss,) + outputs


    return outputs  # (loss), (logits), (hidden_states), (attentions)

#by 태관, 문장을 input data 형식으로 만듦
def koelectra_input(tokenizer, str, device = None, max_seq_len = 512):
  index_of_words = tokenizer.encode(str)
  attention_mask = [1] * len(index_of_words)

  # Padding Length
  padding_length = max_seq_len - len(index_of_words)
  # Zero Padding
  index_of_words += [0] * padding_length
  attention_mask += [0] * padding_length

  data = {\
    'input_ids': torch.tensor([index_of_words]).to(device),
    'attention_mask': torch.tensor([attention_mask]).to(device),
  }
  return data
