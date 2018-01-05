import json
from urllib.request import urlopen

class ThingSpeak (object):

    def readChannel(self,channel,key):
        conn = urlopen("http://api.thingspeak.com/channels/%s/feeds/last.json?api_key=%s" \
                           % (channel,key))
        response = conn.read()
        #Debug
        print("ThingSpeak http status code=%s" % (conn.getcode()))
        data=json.loads(response)
        conn.close()
        return data