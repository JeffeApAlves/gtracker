# -*- coding: utf-8 -*-
"""
Created on Fri May 12 08:31:16 2017

@author: Jefferson
"""

import pika

class RabbitMQConfig(object):
    
    HOST = '192.168.42.1'
    USER = "senai_ws"
    PW = "senai_ws"
    VHOST = "/"
    PORT = 5672
 
    @staticmethod    
    def getConnectionParameters():    

        credentials = pika.PlainCredentials(RabbitMQConfig.USER, RabbitMQConfig.PW)
        return pika.ConnectionParameters( host = RabbitMQConfig.HOST,
                                            port = RabbitMQConfig.PORT,
                                            virtual_host = RabbitMQConfig.VHOST,
                                            credentials  = credentials,
                                            socket_timeout = 1000,
                                            heartbeat_interval = 200)

