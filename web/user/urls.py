from django.conf.urls import url
from .views import *

urlpatterns = [
    url(r'^$', users_list, name='users_list'),
    url(r'^login/$', log_in, name='login'),
    #url(r'^logout/$', log_out, name='logout'),
    url(r'^signup/$', sign_up, name='signup'),     
    url(r'^forgotpw/$', forgot_pw, name='forgotpw'),
]