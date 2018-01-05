from channels import route
from .consumers import *

websocket_routing = [
    # Called when WebSockets connect
    route("websocket.connect", ws_connect ),

    # Called when WebSockets get sent a data frame
    route("websocket.receive", ws_receive ),

    # Called when WebSockets disconnect
    route("websocket.disconnect", ws_disconnect),
]

custom_routing = [

    route("chat.receive", chat_join, command="^join$"),
    route("chat.receive", chat_leave, command="^leave$"),
    route("chat.receive", chat_send_echo, command="^send_echo$"),
    route("chat.receive", chat_send_room, command="^send_room$"),
    route("chat.receive", chat_send_allusers, command="^send_all$"),
    route("chat.receive", chat_close_room, command="^close_room$"),
    route("chat.receive", chat_disconnect, command="^disconnect$"),
]
