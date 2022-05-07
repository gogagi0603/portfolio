import torch
from torch.utils.data import Dataset # 데이터로더
from transformers import ElectraTokenizer
import sys
tokenizer = ElectraTokenizer.from_pretrained("monologg/koelectra-base-discriminator")
sys.path.append("..")
import options

## by태관 / 데이터 파일로부터 데이터를 읽어들임
class ChatbotTextClassificationDataset(Dataset):
  def __init__(self,
               file_path = "../data/deglok_text_classification_train.txt",
               num_label = options.get_label_num(),
               device = 'cpu',
               max_seq_len = 512, # KoBERT max_length
               tokenizer = None
               ):
    self.file_path = file_path
    self.device = device
    self.data =[]

    self.tokenizer = tokenizer if tokenizer is not None else ElectraTokenizer.from_pretrained("monologg/koelectra-base-discriminator")

    file = open(self.file_path, 'r', encoding='utf-8')
    label_dict = {}
    label_dict_len = 0
    while True:
      line = file.readline()
      if not line:
        break
      datas = line.split("\t")
      # by태관 datas[0]은 레이블 data[1]은 질문데이터 data[2]가 만약 있을 경우 그건 답변 데이터
      index_of_words = self.tokenizer.encode(datas[1])
      token_type_ids = [0] * len(index_of_words)
      attention_mask = [1] * len(index_of_words)

      # by 태관 input 데이터의 길이를 동일하게 맞춰주기 위한 패딩
      padding_length = max_seq_len - len(index_of_words)
      index_of_words += [0] * padding_length
      token_type_ids += [0] * padding_length
      attention_mask += [0] * padding_length

      label = label_dict.get(datas[0])

      #by태관 처음보는 레이블일 경우 추가해줌
      if label == None:
        label=label_dict[datas[0]] = label_dict_len
        label_dict_len = label_dict_len +1
        print(label)
        print(datas[0])
      data = {
              'input_ids': torch.tensor(index_of_words).to(self.device),
              'token_type_ids': torch.tensor(token_type_ids).to(self.device),
              'attention_mask': torch.tensor(attention_mask).to(self.device),
              'labels': torch.tensor(label).to(self.device)
             }

      self.data.append(data)

    file.close()

  def __len__(self):
    return len(self.data)
  def __getitem__(self,index):
    item = self.data[index]
    return item
