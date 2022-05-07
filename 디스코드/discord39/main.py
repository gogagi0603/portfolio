import discord
from discord.ext import commands
from youtube_dl import YoutubeDL
import time
import asyncio
import bs4
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
from discord.utils import get
from discord import FFmpegPCMAudio
import requests
token = "OTQ3ODcxMjgwODY5MjkwMDY0.Yhzj6w.3VxnnBvNCaRMDQMz0pw5w8Si8pI"
bot = commands.Bot(command_prefix='!')

user = []
musictitle = []
song_queue = []
musicnow = []
global vo
vo = 0.1
def title(msg):
    global music

    YDL_OPTIONS = {'format': 'bestaudio', 'noplaylist': 'True'}
    FFMPEG_OPTIONS = {'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5', 'options': '-vn'}

    options = webdriver.ChromeOptions()
    options.add_argument("headless")

    chromedriver_dir = r"chromedriver.exe"
    service = Service(chromedriver_dir)
    driver = webdriver.Chrome(service=service, options=options)
    driver.get("https://www.youtube.com/results?search_query=" + msg + "+lyrics")
    time.sleep(0.5)
    source = driver.page_source
    time.sleep(0.5)
    bs = bs4.BeautifulSoup(source, 'lxml')
    entire = bs.find_all('a', {'id': 'video-title'})
    entireNum = entire[0]
    music = entireNum.text.strip()

    musictitle.append(music)
    musicnow.append(music)
    test1 = entireNum.get('href')
    url = 'https://www.youtube.com' + test1
    with YoutubeDL(YDL_OPTIONS) as ydl:
        info = ydl.extract_info(url, download=False)
    URL = info['formats'][0]['url']

    driver.quit()

    return music, URL
def play(ctx):
    global vc
    YDL_OPTIONS = {'format': 'bestaudio', 'noplaylist':'True'}
    FFMPEG_OPTIONS = {'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5', 'options': '-vn'}
    URL = song_queue[0]
    del user[0]
    del musictitle[0]
    del song_queue[0]
    vc = get(bot.voice_clients, guild=ctx.guild)
    if not vc.is_playing():
        vc.play(FFmpegPCMAudio(URL, **FFMPEG_OPTIONS), after=lambda e: play_next(ctx))
        vc.source = discord.PCMVolumeTransformer(vc.source)

        vc.source.volume = vo
        print(vc.source.volume)
def play_next(ctx):
    if len(musicnow) - len(user) >= 2:
        for i in range(len(musicnow) - len(user) - 1):
            del musicnow[0]
    YDL_OPTIONS = {'format': 'bestaudio', 'noplaylist':'True'}
    FFMPEG_OPTIONS = {'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5', 'options': '-vn'}
    if len(user) >= 1:
        if not vc.is_playing():
            del musicnow[0]
            URL = song_queue[0]
            del user[0]
            del musictitle[0]
            del song_queue[0]
            vc.play(discord.FFmpegPCMAudio(URL,**FFMPEG_OPTIONS,executable="D:/Download/ffmpeg-5.0-essentials_build/ffmpeg-5.0-essentials_build/bin/ffmpeg.exe"), after=lambda e: play_next(ctx))
            vc.source = discord.PCMVolumeTransformer(vc.source)
            vc.source.volume = vo
            print(vc.source.volume)
@bot.event
async def on_ready():
    print('다음으로 로그인합니다: ')
    print(bot.user.name)
    print('connection was succesful')
    await bot.change_presence(status=discord.Status.online, activity=discord.Game("제가 바로 MC태관 입니다............."))

@bot.command()
async def leave(ctx):
    try:
        await vc.disconnect()
    except:
        await ctx.send("이미 채널에 속해 있지 않아요")



@bot.command()
async def 링크(ctx, *, url):
    YDL_OPTIONS = {'format': 'bestaudio','noplaylist':'True'}
    FFMPEG_OPTIONS = {'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5', 'options': '-vn'}
    try:
        global vc
        vc = await ctx.message.author.voice.channel.connect()
    except:
        try:
            await vc.move_to(ctx.message.author.voice.channel)
        except:
            await ctx.send("채널에 유저가 접속해 있지 않아요")
    if not vc.is_playing():
        with YoutubeDL(YDL_OPTIONS) as ydl:
            info = ydl.extract_info(url, download=False)
        URL = info['formats'][0]['url']
        vc.play(FFmpegPCMAudio(URL, **FFMPEG_OPTIONS,executable="D:/Download/ffmpeg-5.0-essentials_build/ffmpeg-5.0-essentials_build/bin/ffmpeg.exe"))
        vc.source = discord.PCMVolumeTransformer(vc.source)
        vc.source.volume = vo
        print(vc.source.volume)
        await ctx.send(embed = discord.Embed(title= "노래 재생", description = "현재 " + url + "을(를) 재생하고 있습니다.", color = 0x00ff00))
    else:
        await ctx.send("노래가 이미 재생되고 있습니다!")


@bot.command()
async def 재생(ctx, *, msg):
    emb = discord.Embed(color = 0x00ff00)
    emb.add_field(name="로딩중..", value="[" + msg + "] 로딩 중 입니다.")
    await ctx.send(embed=emb)
    try:
        global vc
        vc = await ctx.message.author.voice.channel.connect()

    except:
        try:
            channel_name = ctx.message.author.voice.channel
            print(channel_name)
            if channel_name == "방역준수채널":
                await vc.move_to(channel_name)
            
        except:
            await ctx.send("채널에 유저가 접속해 있지 않아요")

    if not vc.is_playing():


        options = webdriver.ChromeOptions()
        options.add_argument("headless")
        global entireText
        YDL_OPTIONS = {'format': 'bestaudio', 'oplaylist': 'True'}
        FFMPEG_OPTIONS = {'before_options': '-reconnect 1 -reconnect_streamed 1 -reconnect_delay_max 5',
                          'options': '-vn'}

        chromedriver_dir = "chromedriver.exe"
        service = Service(chromedriver_dir)
        driver = webdriver.Chrome(service=service, options=options)
        driver.implicitly_wait(3)
        ul = "https://www.youtube.com/results?search_query=" + msg
        driver.get(ul)
        time.sleep(0.5)
        source = driver.page_source

        bs = bs4.BeautifulSoup(source, 'lxml')
        entire = bs.find_all('a', {'id': 'video-title'})
        entireNum = entire[0]
        entireText = entireNum.text.strip()

        musicurl = entireNum.get('href')
        url = 'https://www.youtube.com' + musicurl
        driver.quit()
        musicnow.insert(0,entireText)
        with YoutubeDL(YDL_OPTIONS) as ydl:
            info = ydl.extract_info(url, download=False)
        URL = info['formats'][0]['url']
        vc.play(FFmpegPCMAudio(URL, **FFMPEG_OPTIONS,
                               executable="D:/Download/ffmpeg-5.0-essentials_build/ffmpeg-5.0-essentials_build/bin/ffmpeg.exe"),
                after=lambda e: play_next(ctx))
        vc.source = discord.PCMVolumeTransformer(vc.source)
        vc.source.volume = vo

        emb = discord.Embed(color = 0x00ff00)
        emb = emb.add_field(name="노래 재생", value="현재 " + musicnow[0] + "을(를) 재생하고 있습니다.")
        await ctx.send(embed=emb)
       # await ctx.send(
        #    embed=discord.Embed(title="노래 재생", description="현재 " + musicnow[0] + "을(를) 재생하고 있습니다.", color=0x00ff00))
    else:
        user.append(msg)
        result, URLTEST = title(msg)
        song_queue.append(URLTEST)
        await ctx.send(embed=discord.Embed(color = 0x00ff00).add_field(name="노래 재생",value="노래 목록에 " + result+"을(를) 대기열로 추가시켰어요"))
@bot.command()
async def np(ctx):
    if not vc.is_playing():
        await ctx.send("지금은 노래가 재생되지 않네요..")
    else:
        await ctx.send(embed = discord.Embed(title = "지금노래", description = "현재 " + musicnow[0] + "을(를) 재생하고 있습니다.", color = 0x00ff00))


@bot.command()
async def add(ctx, *, msg):
    user.append(msg)
    result, URLTEST = title(msg)
    song_queue.append(URLTEST)
    await ctx.send(result + "를 재생목록에 추가했어요!")
@bot.command()
async def 넘기기(ctx):
    if vc.is_playing():
        vc.stop()
        await ctx.send(embed = discord.Embed(title= "노래끄기", description = musicnow[0]  + "을(를) 종료했습니다.", color = 0x00ff00))
    else:
        await ctx.send("지금 노래가 재생되지 않네요.")

@bot.command()
async def delete(ctx, *, number):
    try:
        ex = len(musicnow) - len(user)
        del user[int(number) - 1]
        del musictitle[int(number) - 1]
        del song_queue[int(number) - 1]
        del musicnow[int(number) - 1 + ex]

        await ctx.send("대기열이 정상적으로 삭제되었습니다.")
    except:
        if len(list) == 0:
            await ctx.send("대기열에 노래가 없어 삭제할 수 없어요!")
        else:
            if len(list) < int(number):
                await ctx.send("숫자의 범위가 목록개수를 벗어났습니다!")
            else:
                await ctx.send("숫자를 입력해주세요!")


@bot.command()
async def list(ctx):
    if len(musictitle) == 0:
        await ctx.send("아직 아무노래도 등록하지 않았어요.")
    else:
        global Text
        Text = ""
        for i in range(len(musictitle)):
            Text = Text + "\n" + str(i + 1) + ". " + str(musictitle[i])

        await ctx.send(embed=discord.Embed(title="노래목록", description=Text.strip(), color=0x00ff00))


@bot.command()
async def clear(ctx):
    try:
        ex = len(musicnow) - len(user)
        del user[:]
        del musictitle[:]
        del song_queue[:]
        while True:
            try:
                del musicnow[ex]
            except:
                break
        await ctx.send(
            embed=discord.Embed(title="목록초기화", description="""목록이 정상적으로 초기화되었습니다. 이제 노래를 등록해볼까요?""", color=0x00ff00))
    except:
        await ctx.send("아직 아무노래도 등록하지 않았어요.")
@bot.command()
async def 도움말(ctx):
    await ctx.send(embed = discord.Embed(title='도움말',description="""
검색하는 속도는 고려 안한거라 검색 느림. 하리보 터졌을때만 사용 ㄱㄱ\n
    
\n!help -> 뮤직봇의 모든 명령어를 볼 수 있습니다.
\n!안녕 -> 뮤직봇을 자신이 속한 채널로 부릅니다.
\n!leave -> 뮤직봇을 자신이 속한 채널에서 내보냅니다.
\n!링크 [노래링크] -> 유튜브URL를 입력하면 뮤직봇이 노래를 틀어줍니다.
(목록재생에서는 사용할 수 없습니다.)
\n!재생 [노래이름] -> 뮤직봇이 노래를 검색해 틀어줍니다.
\n!넘기기 -> 현재 재생중인 노래를 끕니다.
\n!np -> 지금 재생되고 있는 노래의 제목을 알려줍니다.
\n!list -> 이어서 재생할 노래목록을 보여줍니다.
!clear -> 목록에 추가된 모든 노래를 지웁니다.
\n!add [노래] -> 노래를 대기열에 추가합니다.
!delete [숫자] -> 대기열에서 입력한 숫자에 해당하는 노래를 지웁니다.
\n!볼륨 [숫자] -> 볼륨을 조절 합니다. 기본값은 100. 범위는 1~200
""", color = 0x00ff00))

@bot.command()
async def 볼륨(ctx, *, vol):
    if float(vol)>0 and float(vol)<=200:
        try:
            await vc.move_to(ctx.message.author.voice.channel)
            try:
                before = vc.source.volume

                vc.source.volume = float(vol)/1000
                global vo
                vo = float(vol)/1000
                await ctx.send("볼륨이 " + str(before*1000) + "에서 " + str(vo*1000) +"으로 변경 되었어요.\n기본값은 100")
            except:
                await ctx.send("노래가 실행 중이지 않아요")
        except:
            await ctx.send("유저가 음성 채널에 접속해 있지 않아요")
    else:
        await ctx.send("볼륨 조절 범위를 확인해 주세요 (1~200)")


bot.run(token)
