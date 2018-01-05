from django.contrib.auth import get_user_model, login, logout
from django.contrib.auth.forms import AuthenticationForm, UserCreationForm
from django.core.urlresolvers import reverse
from django.shortcuts import render, redirect
from django.contrib.auth import get_user_model, login, logout
from django.contrib.auth.decorators import login_required
from core.devices.RBPI3 import RBPI3

rbpi3 = RBPI3()

@login_required(login_url='../user/login/')
def index(request):
    return render(request, 'monitor/index.html')

def pressure(request):    
    return HttpResponse("Pressao: %s %%" % str(rbpi3.getPressure()))

def humidity(request):
    return HttpResponse("Humidade: %s %%" % str(rbpi3.getHumidity()))

def temperature(request):
    return HttpResponse("Temperatura: %s Celsus" % str(rbpi3.getCPUtemperature()))
    
def cpu(request):
    return HttpResponse("CPU: %s %%" % str(rbpi3.getCPU()))

def memory(request):
    return HttpResponse("Memoria: %s MB" % str(rbpi3.getMemory()))

def disk(request):
    return HttpResponse("Disco: %s GB" % str(rbpi3.getDisk()))
