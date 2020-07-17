import requests
import feedparser
import json
#!/usr/bin/env python
import re
import sys
import time
TINHOT= ["19180833","19180832","19180831","19180830","19180829","19180828","19180827","19180826","19180825","19180824","19180823","19180822","19180821","19180820","19180819","19180818","19180817","19180815","19180814","19180813","19180812","19180811","19180810","19180809","19180808"]
T_TINHOT =["19180786","19180785","19180784","19180783","19180782","19180781","19180780","19180779","19180778","19180777","19180776","19180775","19180774","19180773","19180772","19180771","19180770","19180769","19180768","19180767","19180766","19180765","19180764","19180763","19180762"]

patterns = {
    '[àáảãạăắằẵặẳâầấậẫẩ]': 'a',
    '[đ]': 'd',
    '[èéẻẽẹêềếểễệ]': 'e',
    '[ìíỉĩị]': 'i',
    '[òóỏõọôồốổỗộơờớởỡợ]': 'o',
    '[ùúủũụưừứửữự]': 'u',
    '[ỳýỷỹỵ]': 'y'
}

def convert(text):
    """
    Convert from 'Tieng Viet co dau' thanh 'Tieng Viet khong dau'
    text: input string to be converted
    Return: string converted
    """
    output = text
    for regex, replace in patterns.items():
        output = re.sub(regex, replace, output)
        # deal with upper case
        output = re.sub(regex.upper(), replace.upper(), output)
    return output

while (1):    
    headers = {"content-type": "application/x-www-form-urlencoded" }
    NewsFeed = feedparser.parse("https://vnexpress.net/rss/thoi-su.rss")
    i=0
    for item in NewsFeed.entries:
        title=item.title
        title=title.replace("\"","\'")
        xx=[]

        if (len(title)>38):
            prej=0
            j=37
            while (title[j]!=' '):
                    j-=1
            xx.append(title[prej:j])
            xx.append(title[j+1:])
        else:
            xx.append(title)
            xx.append(" ")
        paragram = "\",\"".join(map(str, xx)) 
        paragram="[\""+paragram+"\"]"
        print(paragram)
        r = requests.put('https://api.thingspeak.com/talkbacks/38964/commands/'+T_TINHOT[i]+'.json', data ={'api_key':'JFIC48F3RU95H66U', 'command_string':paragram}, headers=headers)


        content=item.summary;
        content=content.replace("\"","\'")
        idx=content.rfind("/br")
        if (idx>0):
            content=content[idx+4:]
        content=content.replace("\xa0"," ")
        
        xx=[]
        prej=0
        j=0
        while (j!=len(content)):
            if ((prej+38)>=len(content)):
                xx.append(content[j+1:])
                break
            else:
                j=prej+38
                while (content[j]!=' '):
                    j-=1
                xx.append(content[prej:j])
                prej=j+1
        paragram = "\",\"".join(map(str, xx)) 
        paragram="[\""+paragram+"\"]"
        
        r = requests.put('https://api.thingspeak.com/talkbacks/38959/commands/'+TINHOT[i]+'.json', data ={'api_key':'9DD7RLNE6Q5YZV90', 'command_string':paragram}, headers=headers)
        i+=1
        if (i>24): break

    headers = {"content-type": "application/x-www-form-urlencoded" }
    r=requests.get("http://dataservice.accuweather.com/forecasts/v1/hourly/1hour/353412?apikey=JhB8p7JpYqT88S335AKMETvqfGTGpsED&language=vi&details=true&metric=true")
    r=r.json()
    paragram=[]
    print(r[0]["IconPhrase"])
    paragram.append(convert(r[0]["IconPhrase"]))
    print(str(r[0]["Temperature"]["Value"])+"°C")
    paragram.append(str(r[0]["Temperature"]["Value"])+"°C")
    print(str(r[0]["RelativeHumidity"])+'%')
    paragram.append(str(r[0]["RelativeHumidity"])+'%')

    paragram = "\",\"".join(map(str, paragram)) 
    paragram="[\""+paragram+"\"]"
    print(paragram)

    r = requests.put('https://api.thingspeak.com/talkbacks/39112/commands/18906639.json', data ={'api_key':'YWLNNTR9Y3OLSQVC', 'command_string':paragram}, headers=headers)
    time.sleep(3600)
