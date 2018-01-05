from channels.routing import route
from channels import include

channel_routing = [
	
	include("chat.routing.websocket_routing", path=r"^/chat/stream"),
    include("chat.routing.custom_routing"),

    include("monitor.routing.websocket_routing", path=r"^/monitor/stream"),
    include("monitor.routing.custom_routing"),

    #include("tracker.routing.websocket_routing", path=r"^/tracker/stream"),
    include("tracker.routing.websocket_routing", path=r"^/ws/tracker"),
    include("tracker.routing.custom_routing"),
]