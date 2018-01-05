from django.contrib import admin
from django import forms
from .models import *

class EnderecoForm(forms.ModelForm):
    class Media:
        css = {
            'all': ('route/geo_position.css',)
        }
        js = ('route/geo_position.js',)

@admin.register(Endereco)
class EnderecoAdmin(admin.ModelAdmin):
    form = EnderecoForm
    search_fields = ('rua', 'cidade')
    list_display = ('rua','cidade','estado','bairro','position')
    list_filter = ['estado']
    save_on_top = True


@admin.register(Rota)
class RotaAdmin(admin.ModelAdmin):
    search_fields = ('origem', 'destino',)
    list_display = ('origem', 'destino')
    save_on_top = True
