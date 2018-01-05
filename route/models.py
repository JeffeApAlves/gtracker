from django.db import models
from geoposition.fields import GeopositionField
from localflavor.br.br_states import STATE_CHOICES

class Endereco(models.Model):
    
    rua = models.CharField(max_length=255, verbose_name=u'Rua', help_text='Para uma melhor localização no mapa, preencha sem abreviações. Ex: Rua Martinho Estrela,  1229') 
    bairro = models.CharField(max_length=255,)
    cidade = models.CharField(max_length=255,help_text="Para uma melhor localização no mapa, preencha sem abreviações. Ex: Belo Horizonte")
    estado = models.CharField(max_length=2, null=True, blank=True,choices=STATE_CHOICES)
    position = GeopositionField(verbose_name=u'Geolocalização', help_text="Não altere os valores calculados automaticamente de latitude e longitude")

    class Meta:
        verbose_name, verbose_name_plural = u"Endereço" , u"Endereços"
        ordering = ('rua',)

    def __unicode__(self):
        return u"%s" % self.rua 


class Rota(models.Model):

    origem = Endereco()
    destino = Endereco()


    class Meta:
        verbose_name, verbose_name_plural = u"Rota" , u"Rotas"


    def __unicode__(self):
        return u"%s to % s" % self.origem.rua,self.destino.rua
