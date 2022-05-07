import pandas as pd
import papago
data =pd.read_csv("aug.csv")
data = data["0"]
data =data.values

aug = []

for i in data:
    result = papago.augmentation(i,"en")
    if result != None:
        aug.append(result)
d = pd.DataFrame(aug)
d.to_csv("군복학aug.csv")