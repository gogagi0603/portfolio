import bs4
import requests
ul = "https://www.youtube.com/results?search_query=" + "stronger" + "+lyrics"
response = requests.get(url=ul)
print(ul)
source = response.text
bs = bs4.BeautifulSoup(source, 'html.parser')
result = bs.find("div",{"id":"container"})
reulst = result.find_all({"id":"primary"})
print(len(result))

for i in result:
    print(i)
print(result)