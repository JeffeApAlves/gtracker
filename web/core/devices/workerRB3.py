import json
from threading import Thread
from .consumers import sendTLM
import time
from raspberryPI3.RBPI3 import RBPI3

UPDATE_INTERVAL = 5

rbpi3 = RBPI3()

class workerTLM (Thread):
    # thread para inicilizacao do subscribe e publish da tlm envio periodico dos dados de telemetria da RaspBerry"""

    def __init__(self):
        Thread.__init__(self)
        self.tlm = TLMConsumer()

    def run(self):

        self.count = 1
        self.rbpi3.init()
        self.rbpi3.loop_start()
    
        while True:
            print("Get TLM")
            self.rbpi3.readValues()
            sendTLM(rbpi3)
            self.count += 1
            time.sleep(UPDATE_INTERVAL)

    def start(self):
        Thread.start(self)