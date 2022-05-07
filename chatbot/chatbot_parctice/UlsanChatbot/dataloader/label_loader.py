import pandas as pd
def load():
    arr = []
    file_path= "../data/(original)wellness_dialog_for_text_classification_train.txt"
    file_path = file_path
    file = open(file_path, 'r', encoding='utf-8')
    while True:
        line = file.readline()
        if not line:
            break
        datas = line.split("\t")
        if datas[0] == "군복학":
            arr.append(datas[1])
    result = pd.DataFrame(arr)
    result.to_csv("aug.csv")
load()