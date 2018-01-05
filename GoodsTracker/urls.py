from django.conf.urls import url,include
from django.contrib import admin

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^', include('home.urls', namespace='home')),
    url(r'^chat/', include('chat.urls', namespace='chat')),
    url(r'^route/', include('route.urls', namespace='route')),
    url(r'^history/', include('history.urls', namespace='history')),
    url(r'^service/', include('service.urls', namespace='service')),
    url(r'^tracker/', include('tracker.urls', namespace='tracker')),
    url(r'^dashboard/', include('dashboard.urls', namespace='dashboard')),
    url(r'^monitor/', include('monitor.urls', namespace='monitor')),
    url(r'^user/', include('user.urls', namespace='user')),
    url(r'^graphics/', include('graphics.urls', namespace="graphics")),
]
