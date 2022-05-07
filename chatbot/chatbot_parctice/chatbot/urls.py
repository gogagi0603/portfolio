from django.contrib import admin
from django.urls import path
from django.conf.urls import url, include
from . import views

urlpatterns = [
    path('', views.home,name="home"),
    #path('chattrain', views.chattrain, name='chattrain'),
    path('chatanswer_hubok', views.chatanswer_hubok, name='chatanswer_hubok'),
    path('chatanswer_deglok', views.chatanswer_deglok, name='chatanswer_deglok'),
    path('deglok',views.deglok, name="deglok"),
    path('hubok', views.hubok, name="hubok"),
    path('sugang', views.sugang, name="sugang"),
    path('chatanswer_sugang', views.chatanswer_sugang,name='chatanswer_sugang'),
    path('welfare', views.welfare, name="welfare"),
    path('chatanswer_welfare', views.chatanswer_welfare, name='chatanswer_welfare')

]
