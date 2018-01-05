from django.apps import AppConfig

#from .workerTLM import workerTLM

#workerTLM = workerTLM();

class MonitorConfig(AppConfig):
    name = 'monitor'

 #   def ready(self):
 #   	workerTLM.start();