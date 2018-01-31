#!/usr/bin/env python

import os
import getpass
from cl_bash import *

"""

@file    repository.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1

Gerenciador dos rpositorios do projeto

"""

class repository(object):

    def clone(src,dest,host=None):

        if host is None:

            bash.execute("git clone %s %s" % (src,dest))
   
        else:
            bash.execute_remote("git clone %s %s" % (src,dest),getpass.getuser(),host)


    def updatesrc,dest,host=None):

        if host is None:

            bash.execute("git update %s %s" % (src,dest))
   
        else:
            bash.execute_remote("git update %s %s" % (src,dest),getpass.getuser(),host)
