from __future__ import print_function
import os
import paho.mqtt.client as mqtt
import time
from core.thingspeak.ThingSpeak import ThingSpeak
from ast import literal_eval
import json
import threading
import time

#ThingSpeak
# ID do canal do ThingSpeak
TLM_CHANNEL_ID = "315831"
# Chave de escrita da API para o canal
TLM_READ_API_KEY = "TF63H3V6I2UWO954"
#Host mqtt do ThingSpeak
mqttHost = "mqtt.thingspeak.com"

VALUE_TEMPERATURE = 'field1'
VALUE_DISK = 'field2'
VALUE_MEMORY = 'field4'
VALUE_CPU = 'field3'
VALUE_PRESSURE = 'field5'
VALUE_HUMIDITY = 'field6'

#Porta 
tPort = 80

#class RBPI3 (threading.Thread):
class RBPI3 (object):
    

    def __init__(self):
#        super(RBPI3, self).__init__()
#        self._stop_event = threading.Event()
        self.count = 0
        self.temperature = 0
        self.cpu = 0
        self.memory = 0
        self.disk=''
        self.pressure = 0
        self.humidity = 0
        self.ts = ThingSpeak()
        self.values = None

    def init(self):
        pass
#        self.client = mqtt.Client()
#        self.client.connect(mqttHost,tPort)
 
    def getCPUtemperature(self):
        return self.values.temperature
  
    def getCPU(self):
        return self.values.cpu

    def getMemory(self):
        return self.values.memory

    def getDisk(self):
         return self.values.disk

    def getPressure(self):
        return self.values.pressure

    def getHumidity(self):
        return self.values.humidity

    def readTLMChannel(self):
        self.values = self.ts.readChannel(channel=TLM_CHANNEL_ID,key=TLM_READ_API_KEY)

        return {
            'temperature':  self.values[VALUE_TEMPERATURE] ,
            'humidity':  self.values[VALUE_HUMIDITY] ,
            'memory': self.values[VALUE_MEMORY],
            'disk': self.values[VALUE_DISK],
            'cpu': self.values[VALUE_CPU],
            'pressure': round(float(self.values[VALUE_PRESSURE]),1),
        }
           
    def loop_start(self):
        pass
#        self.client.loop_start()

    def run(self):
        pass
#        while(self.stopped() is not True):
#            self.values = self.ts.readChannel(channel=TLM_CHANNEL_ID,key=TLM_READ_API_KEY)
#            print("Leitura do canal do ThingSpeak")
#            time.sleep(5)

    def stop(self):
        self._stop_event.set()

    def stopped(self):
        return self._stop_event.is_set()