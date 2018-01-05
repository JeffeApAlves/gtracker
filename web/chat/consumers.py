import json
from channels import Channel, Group
from channels.sessions import channel_session, enforce_ordering
from channels.auth import channel_session_user, channel_session_user_from_http,channel_session_user_from_http
from channels.security.websockets import allowed_hosts_only

#Permitir apenas os servidores listados no settings.py
@allowed_hosts_only
# Conectado em um websocket
@channel_session_user_from_http
def ws_connect(message):
    group = Group("users")
    # adiciona no grupo
    group.add(message.reply_channel)
    # Envia messangem Accept the connection request
    message.reply_channel.send({"accept": True})
    print("accept")
 
# Conectado em um websocket
@channel_session
def ws_disconnect(message):
    Group("chat-%s" % message.channel_session['room']).discard(message.reply_channel)


def ws_receive(message):
    payload = json.loads(message['text'])
    payload['reply_channel'] = message.content['reply_channel']
    Channel("chat.receive").send(payload)
    #Debug
    print(message['text'])

# Sessao do usuario
@channel_session_user
# Conectado em um websocket
@channel_session
def chat_join(message):
    room_name = message["room"]
    # Salva a sala na sessao e adiona o canal na sala
    message.channel_session['room'] = room_name
    payload = json.dumps({
                "join": room_name,
                "title": room_name})
    
    message.reply_channel.send({"text":payload})
    Group("chat-%s" % room_name).add(message.reply_channel)
    #Debug
    print("Entrou do grupo chat-%s channel:%s" % (room_name,message.reply_channel))

@channel_session_user
@channel_session
def chat_leave(message):
    room_name = message["room"]
    Group(room_name).discard(message.reply_channel)

    payload = json.dumps({
                "leave": room_name,
                "title": room_name})
    
    message.reply_channel.send({"text":payload})
    #Debug
    print("Saiu do grupo chat-%s channel:%s" %  (room_name,message.reply_channel))

@channel_session_user
@channel_session
def chat_send_echo(message):
    payload = json.dumps({
                "message": message.content})
    message.reply_channel.send({"text": payload})
    #Debug
    print("Echo %s" % payload)

@channel_session_user
@channel_session
def chat_send_room(message):
    group = message["room"]
    payload = json.dumps({
                "message":message["chat_msg"]})
    group.send({"text": payload})
    print(message['chat_msg'])

@channel_session_user
@channel_session
def chat_send_allusers(message):
    group = Group("users")
    group.send(message["chat_msg"])
    print(message['chat_msg'])

# Connected to chat-messages
def msg_consumer(message):

    # Save to model
    room = message.content['room']
    ChatMessage.objects.create(
        room=room,
        message=message.content['message'],
    )
    # Broadcast to listening sockets
    Group("chat-%s" % room).send({
        "text": message.content['message'],
    })

@channel_session_user
def chat_close_room(message):
    room_name = message["room"]
    Group(room_name).discard(message.reply_channel)

    payload = json.dumps({
                "close": room_name,
                "title": room_name})
    
    message.reply_channel.send({"text":payload})
    #Debug
    print("Finalizou o  grupo chat-%s channel:%s" %  (room_name,message.reply_channel))


def sendStatusUser(msg,status_user,room_name):

    group = "chat-%s" % room_name

    Group(group).send({
        'text': json.dumps({
            'username': msg,
            'is_logged_in': status_user
        })
    })

@channel_session_user
@channel_session
def chat_disconnect(message):
    pass