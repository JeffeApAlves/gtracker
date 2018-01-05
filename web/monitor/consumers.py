import os
import sys
import json
from channels import Channel, Group
from channels.sessions import channel_session, enforce_ordering
from channels.auth import channel_session_user, channel_session_user_from_http,channel_session_user_from_http
from channels.security.websockets import allowed_hosts_only
from core.devices.RBPI3 import RBPI3

rbpi3 = RBPI3()

# Permitir apenas os servidores listados no settings.py
@allowed_hosts_only
# Conectado um websocket
@channel_session_user_from_http
def ws_connect(message):
    group = Group("monitors")
    # Adiciona no grupo
    group.add(message.reply_channel)
    # Envia messangem Accept the connection request
    message.reply_channel.send({"accept": True})
    #rbpi3.start()
    print("connect-Monitor")
 
# Conectado em um websocket
@channel_session
def ws_disconnect(message):
    Group("monitors").discard(message.reply_channel)
    #rbpi3.stop()
    print("disconnect-Monitor")

def ws_receive(message):
    payload = json.loads(message['text'])
    payload['reply_channel'] = message.content['reply_channel']
    Channel("monitor.receive").send(payload)
    print("BE(Monitor)-RX:" + str(message.content))

@channel_session_user
@channel_session
def monitor_ping(message):
    payload = json.dumps({"pong": "test"})
    message.reply_channel.send({"text": payload})

@channel_session_user
@channel_session
def monitor_updateTLM(message):
    payload = json.dumps({"telemetry":rbpi3.readTLMChannel()})
    message.reply_channel.send({"text": payload})
    print("BE(Monitor)-TX:" + str(payload))

@channel_session_user
@channel_session
def monitor_disconnect(message):
    print(message)
