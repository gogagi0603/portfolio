import os
import sys
import urllib.request
from ast import literal_eval
from augmentaion.sentence_transformers import similar
client_id = "YMPzFsaIFdERBki7ih52" # 개발자센터에서 발급받은 Client ID 값
client_secret = "WBXI36La1d" # 개발자센터에서 발급받은 Client Secret 값
#by태관 번역. text == 문장, source == 번역할 나라, target == 번역 후 나라
def translate(text, source, target):
    encText = urllib.parse.quote(text)
    # source = ["ko", "en", "zh-CN"]
    data = "source=" + source + "&target=" + target + "&text=" + encText
    url = "https://openapi.naver.com/v1/papago/n2mt"
    request = urllib.request.Request(url)
    request.add_header("X-Naver-Client-Id", client_id)
    request.add_header("X-Naver-Client-Secret", client_secret)
    response = urllib.request.urlopen(request, data=data.encode("utf-8"))
    rescode = response.getcode()
    if (rescode == 200):
        response_body = response.read()
        result = response_body.decode('utf-8')
        result = str(result)
        start_index = int(result.find("""translatedText""")) + 17

        for index, c in enumerate(result[start_index:]):
            if c == ",":
                end_index = start_index + index - 1
                return result[start_index: end_index]
    else:
        print("Error Code:" + rescode)

def augmentation(text,target):
    first_trans = translate(text,"ko",target)
    second_trans = translate(first_trans,target,"ko")
    if similar.compare_sentence(text,second_trans) == True:
        return second_trans
    else:
        return None
