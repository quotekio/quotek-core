#!/usr/bin/env python

import requests
import sys
import time
import json


point_data = [ "DAX", "zzzADDEDzzz" , "zzzADDEDzzz", 1, 0, 0, 0, 0, 0, int(time.time()) , int(time.time()) , "zzzADDEDzzz" ]

to_send  = [ { 'name': '__history__',
              'columns':  [ 'indice', 'epic', 'dealid', 'size', 'stop', 'limit', 'open', 'pnl', 'pnl_peak', 'open_date', 'close_date', 'identifier' ],
              'points' : [ point_data ] } ]

if len(sys.argv) > 2: point_data[7] = int(sys.argv[2])

resp = requests.post(sys.argv[1], data=json.dumps(to_send))

print resp.text


