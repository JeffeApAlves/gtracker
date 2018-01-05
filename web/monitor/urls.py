from django.conf.urls import url
from .views import *

urlpatterns = [
    url(r'^$', index, name='index'),
    url(r'^pressure/$', pressure, name='pressure'),
    url(r'^humidity/$', humidity, name='humidity'),
    url(r'^temperature/$', temperature, name='temperature'),
    url(r'^cpu/$', cpu, name='cpu'),
    url(r'^memory/$', memory, name='memory'),
    url(r'^disk/$', disk, name='disk'),
]