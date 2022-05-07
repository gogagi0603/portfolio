import torch
import torch.nn as nn
import random
import sys
import os
import numpy as np
from transformers import (
  ElectraConfig,
  ElectraTokenizer
)

sys.path.append("..")
from model.koelectra import koElectraForSequenceClassification,koelectra_input
import options
def load_wellness_answer():
  root_path = '..'
  file_path = f"{root_path}/data/deglok_text_classification_train.txt"

  f_f = open(file_path,'r',encoding='utf-8')

  file_lines = f_f.readlines()
  category = {}
  answer = {}

  k = 0
  for data in file_lines:
    data = data.split('\t')
    category_values = category.values()
    if data[0] in category_values:
      pass
    else:
      category[str(k)] = data[0]
      k = k+1

    answer_keys = answer.keys()


    if data[0] in answer_keys:
      if len(data) > 2:
        if len(data[2][:-1]) > 0:
          answer[data[0]] += [data[2][:-1]]
    else:
      if len(data) > 2:
        answer[data[0]] = [data[2][:-1]]

  for i in answer:
    print(category.keys())
  return category, answer


if __name__ == "__main__":
  root_path='..'
  checkpoint_path =f"{root_path}/checkpoint"
  save_ckpt_path = f"{checkpoint_path}/deglok-text-classification.pth"
  model_name_or_path = "monologg/koelectra-base-discriminator"

  #답변과 카테고리 불러오기
  category, answer = load_wellness_answer()

  ctx = "cuda" if torch.cuda.is_available() else "cpu"
  device = torch.device(ctx)

  # 저장한 Checkpoint 불러오기
  checkpoint = torch.load(save_ckpt_path, map_location=device)

  # Electra Tokenizer
  tokenizer = ElectraTokenizer.from_pretrained(model_name_or_path)

  electra_config = ElectraConfig.from_pretrained(model_name_or_path)
  model = koElectraForSequenceClassification.from_pretrained(pretrained_model_name_or_path=model_name_or_path,
                                                             config=electra_config,
                                                             num_labels=options.get_label_num())
  model.load_state_dict(checkpoint['model_state_dict'])

  model.to(device)
  model.eval()


  while 1:
    sent = input('\nQuestion: ') # '질문!'
    data = koelectra_input(tokenizer,sent, device,512)
    # print(data)

    output = model(**data)
    #print("output : ", output)
    logit = output[0]
    #print("logit : ",logit)
    softmax_logit = torch.softmax(logit, dim=1)
    print("softmax_logit : ", softmax_logit)

    softmax_logit = softmax_logit.squeeze()
    print("softmax_logit squeeze : ", softmax_logit)
    max_index = torch.argmax(softmax_logit).item()
    max_index_value = softmax_logit[torch.argmax(softmax_logit)].item()
    print(answer)
    answer_list = answer[category[str(max_index)]]
    answer_len = len(answer_list) - 1
    answer_index = random.randint(0, answer_len)
    print(f'Answer: {answer_list[answer_index]}, index: {max_index}, softmax_value: {max_index_value}')
    print('-' * 50)
  # print('argmin:',softmax_logit[torch.argmin(softmax_logit)])





