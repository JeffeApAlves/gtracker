import os
import time
import json
import threading
import time
import pika
from ..rabbitmq.RabbitMQConfig import RabbitMQConfig
from channels import Channel, Group

QUEUE_TLM   = "TLM%05d"

class Tracker (threading.Thread):

    def __init__(self,ch,nr):
        super(Tracker, self).__init__()
        self._stop_event = threading.Event()
        self._address = nr
        self.reply_channel = ch
        self._connection  = None
        self._closing = False
        self._channel = None
        self._consumer_tag = None
        self._queue_name = QUEUE_TLM % (nr) 
        self.start()

    def stop_consuming(self):
       if self._channel:
            print('Cancelando consumer')
            self._channel.basic_cancel(self.on_cancelok, self._consumer_tag)
        
    def start_consuming(self):
        self.add_on_cancel_callback()
        self._consumer_tag = self._channel.basic_consume(self.on_message_tlm,
                            queue=self._queue_name,
                            no_ack=True)

    def on_message_tlm(self,ch, method, properties, body):
        
        datas = body.decode('utf-8').split(":")

        tlm = {
            'address':  datas[0] ,
            'dest':  datas[1] ,
            'timestamp': datas[2],
            'operation': datas[3],
            'resource': datas[4],
            #'size_pl': datas[5], nao existe a necessidade

            'lat': self.toGrauDecimal(datas[6]) ,
            'lng': self.toGrauDecimal(datas[7]) ,
            'acce':{'X':datas[8],'Y':datas[9],'Z':datas[10]},
            'acce_G':{'X':round(float(datas[11]),2),'Y':round(float(datas[12]),2),'Z':round(float(datas[13]),2)},

            'speed': datas[14],
            'level':datas[15],
            'lock': datas[16],
            'timestamp_tlm': datas[17],
        }
        payload = json.dumps({"telemetry":tlm})
        self.reply_channel.send({"text":payload})
        print("Enviado TLM:" + str(payload))

    def run(self):
        self.connect()
 
    def stopped(self):
        return self._stop_event.is_set()

    def connect(self):
        param = RabbitMQConfig.getConnectionParameters()
        print("Conectando no broker: " + str(param))
        self._connection = pika.SelectConnection(parameters=param,
                                        on_open_callback=self.on_connection_open,
                                        on_open_error_callback=self.on_connection_open_error,
                                        stop_ioloop_on_close=False)
        self._connection.ioloop.start()

    def on_connection_open(self,connection):
        self._connection = connection
        print("Conectado no broker...")
        self.add_on_connection_close_callback()
        self.open_channel()

    def on_connection_open_error(self,a,erro):
        #print(a)
        print(erro)
        print("Erro ao tentar conectar no broker !!!")

    def on_channel_open(self,channel):
        print("Criado o canal no broker")
        self._channel = channel
        self.start_consuming()
        print("Criado o consume da Queue: " + self._queue_name)

    def add_on_connection_close_callback(self):
        self._connection.add_on_close_callback(self.on_connection_closed)

    def on_connection_closed(self, connection, reply_code, reply_text):

        self._channel = None
        if self._closing:
            self._connection.ioloop.stop()
        else:
            print("Conexão com o broker finalizada")
            self._connection.add_timeout(5, self.reconnect)

    def reconnect(self):

        print("Reconectando com o broker ...")

        self._connection.ioloop.stop()

        if not self._closing:
            self.connect()

    def open_channel(self):
        print('Criando o canal no Broker')
        self._connection.channel(on_open_callback=self.on_channel_open)

    def add_on_cancel_callback(self):
        self._channel.add_on_cancel_callback(self.on_consumer_cancelled)

    def on_consumer_cancelled(self, method_frame):
        print("Consumer cancelado")
        if self._channel:
            self._channel.close()

    def on_cancelok(self, unused_frame):
        print('RabbitMQ tem o conhecimento do cancelamento do consumer')
        self.close_channel()

    def close_channel(self):
        self._channel.close()
        print('Canal fechado')

    def stop(self):
        self._closing = True
        self.stop_consuming()
        self._connection.ioloop.start()
        #self._stop_event.set()

    def close_connection(self):
        self._connection.close()

    def toGrauDecimal(self,angulo):
        dest     = float(angulo);
        minutos  = (dest % (100 if dest>=0 else -100));
        graus    = dest - minutos;
        dec      = (minutos / 60.0);
        return round((graus / 100.0) + dec, 4);
