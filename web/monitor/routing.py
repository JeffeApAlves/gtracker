from channels import route
from .consumers import *

websocket_routing = [
    # Called when WebSockets connect
    route("websocket.connect", ws_connect),

    # Called when WebSockets get sent a data frame
    route("websocket.receive", ws_receive),

    # Called when WebSockets disconnect
    route("websocket.disconnect", ws_disconnect),
]

custom_routing = [

    route("monitor.receive", monitor_ping, command="^ping$"),
    route("monitor.receive", monitor_updateTLM, command="^update_monitor$"),
]
