from django.shortcuts import render
from django.contrib.auth import get_user_model, login, logout
from django.contrib.auth.decorators import login_required
from django.contrib.auth.forms import AuthenticationForm, UserCreationForm
from django.core.urlresolvers import reverse
from django.shortcuts import render, redirect
from django.contrib.auth import get_user_model, login, logout
from django.contrib.auth.decorators import login_required

User = get_user_model()

@login_required(login_url='/login/')
def users_list(request):
    users = User.objects.select_related('logged_in_user')
    for user in users:
        user.status = 'Online' if hasattr(user, 'logged_in_user') else 'Offline'
    return render(request, 'user/user_list.html', {'users': users})

def log_in(request):
    form = AuthenticationForm()
    if request.method == 'POST':
        form = AuthenticationForm(data=request.POST)
        if form.is_valid():
            login(request, form.get_user())
            return redirect(reverse('user:users_list'))
        else:
            print(form.errors)
    return render(request, 'user/login.html', {'form': form})

@login_required(login_url='/login/')
def log_out(request):
    logout(request)
    return redirect(reverse('user:login'))

def sign_up(request):
    form = UserCreationForm()
    if request.method == 'POST':
        form = UserCreationForm(data=request.POST)
        if form.is_valid():
            form.save()
            return redirect(reverse('user:login'))
        else:
            print(form.errors)
    return render(request, 'user/register.html', {'form': form})

def forgot_pw(request):
    return render(request, 'user/forgot-password.html')
