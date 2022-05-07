from django.shortcuts import render
from django.http import JsonResponse, HttpResponse
from django.views.decorators.csrf import csrf_exempt
# Create your views here.
import json
import numpy as np
import torch
import torch.nn as nn
import random
import sys
import os
import torch
import numpy as np
from transformers import (
  ElectraConfig,
  ElectraTokenizer
)
sys.path.append("..")
from chatbot.answers import *
from UlsanChatbot.model.koelectra import koElectraForSequenceClassification,koelectra_input
import UlsanChatbot.options as options

# by태관 / 답변 데이터 불러오는 메소드. 인자로 어디 페이지에서 실행되는건지 알려줌
def load_wellness_answer(title):
  root_path = './UlsanChatbot'
  file_path = f"{root_path}/data/"+title+"_text_classification_train.txt"
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
          print(answer[data[0]])
    else:
      if len(data) > 2:
        answer[data[0]] = [data[2][:-1]]
  return category, answer


#by태관 /  첫번째 모델 (휴복학 답변)
root_path='./UlsanChatbot'
checkpoint_path =f"{root_path}/checkpoint"
save_ckpt_path = f"{checkpoint_path}/hubok-text-classification.pth"
model_name_or_path = "monologg/koelectra-base-discriminator"

# by태관 / 답변과 카테고리 불러오기
category, answer = load_wellness_answer("hubok")
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

############################################################################

#by태관 두번째 모델 불러오기
root_path2='./UlsanChatbot'
checkpoint_path2 =f"{root_path2}/checkpoint"
save_ckpt_path2 = f"{checkpoint_path2}/deglok-text-classification.pth"
model_name_or_path2 = "monologg/koelectra-base-discriminator"

#답변과 카테고리 불러오기
category2, answer2 = load_wellness_answer("deglok")

ctx = "cuda" if torch.cuda.is_available() else "cpu"
device = torch.device(ctx)

# 저장한 Checkpoint 불러오기
checkpoint2 = torch.load(save_ckpt_path2, map_location=device)

# Electra Tokenizer
tokenizer2 = ElectraTokenizer.from_pretrained(model_name_or_path2)

electra_config2 = ElectraConfig.from_pretrained(model_name_or_path2)
model2 = koElectraForSequenceClassification.from_pretrained(pretrained_model_name_or_path=model_name_or_path2,
                                                             config=electra_config2,
                                                             num_labels=options.get_label_num())
model2.load_state_dict(checkpoint2['model_state_dict'])

model2.to(device)
model2.eval()

def home(request):
    context = {}
    return render(request, "chathome.html", context)
def deglok(request):
    context={}
    return render(request, "deglok.html", context)
def hubok(request):
    context={}
    return render(request,"hubok.html", context)
def sugang(request):
    context={}
    return render(request,"sugang.html", context)
def welfare(request):
    context={}
    return render(request,"welfare.html", context)

def bot(sent):
    data = koelectra_input(tokenizer, sent, device, 512)
    output = model(**data)
    logit = output[0]
    softmax_logit = torch.softmax(logit, dim=1)
    softmax_logit = softmax_logit.squeeze()
    max_index = torch.argmax(softmax_logit).item()
    max_index_value = softmax_logit[torch.argmax(softmax_logit)].item()
    answer_list = answer[category[str(max_index)]]
    answer_len = len(answer_list) - 1
    answer_index = random.randint(0, answer_len)
    print(f'Answer: {answer_list[answer_index]}, index: {max_index}, softmax_value: {max_index_value}')
    print('-' * 50)
    return answer_list[answer_index]



#by태관 request 쿼리문이 도착하면 질문데이터를 받고 답변을 계산한다음 context로 전달해줌
@csrf_exempt
def chatanswer_hubok(request):
     context = {}
     sent = request.GET["chattext"]
     context["result"] = bot(sent)
     return JsonResponse(context, content_type="application/json")

def chatanswer_deglok(request):
    context={}
    sent = request.GET["chattext"]
    data = koelectra_input(tokenizer2, sent, device, 512)
    output = model2(**data)
    logit = output[0]
    softmax_logit = torch.softmax(logit, dim=1)
    softmax_logit = softmax_logit.squeeze()
    max_index = torch.argmax(softmax_logit).item()
    max_index_value = softmax_logit[torch.argmax(softmax_logit)].item()
    answer_list = answer2[category2[str(max_index)]]
    answer_len = len(answer_list) - 1
    answer_index = random.randint(0, answer_len)
    print(f'Answer: {answer_list[answer_index]}, index: {max_index}, softmax_value: {max_index_value}')
    print('-' * 50)
    context["result"] = answer_list[answer_index]
    return JsonResponse(context, content_type="application/json")

#by태관 복지와 수강신청 답변 메소드 아직 미완
def chatanswer_sugang(request):
    context={}
    sent = request.GET["chattext"]
    context["result"] = descision_sugang(sent)
    if context["result"] != -1:
        return JsonResponse(context, content_type="application/json")
    else:
        context["result"] = bot(sent)
        return JsonResponse(context, content_type="application/json")

def chatanswer_welfare(request):
    context={}
    sent = request.GET["chattext"]
    context["result"] = descision_welfare(sent)
    if context["result"] != -1:
        return JsonResponse(context, content_type="application/json")
    else:
        context["result"] = bot(sent)
        return JsonResponse(context, content_type="application/json")