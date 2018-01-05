from django.conf.urls import url

from waypoints import views

urlpatterns = [	
		url(r'^$', views.index, name='waypoints-index'),
    	url(r'^save$', views.save, name='waypoints-save'),
    	url(r'^search$', views.search, name='waypoints-search'),
    	url(r'^upload$', views.upload, name='waypoints-upload'),
]
