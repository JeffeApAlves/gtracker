
class Route(object):
        
    def __init__(self, adict):
        self.__dict__.update(adict)
        for k, v in adict.items():
            if isinstance(v, dict):
                self.__dict__[k] = Route(v)

def get_object(adict):
    return Route(adict)