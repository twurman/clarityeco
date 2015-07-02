# -*- coding: utf-8 -*-
"""
This thrift client is created to send natural language texts to the sentiment analysis service, which returns polarity and score for the whole texts. JSON data format is also supported to easily interpret the results.

@author: Hailong Yang (hailong@umich.edu)

"""

import sys, glob, os
sys.path.append('gen-py')
sys.path.insert(0, glob.glob('lib/py/lib.*')[0])

from bs4 import BeautifulSoup

from thrift import Thrift
from sentiment import SentimentAnalysis
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TJSONProtocol
from thrift.server import TServer

if len(sys.argv) != 4:
	print 'Usage: SentimentClient service_ip service_port content_file'
	sys.exit()

try:
  
  transport = TSocket.TSocket(sys.argv[1], int(sys.argv[2]))

  transport = TTransport.TBufferedTransport(transport)

  protocol = TJSONProtocol.TJSONProtocol(transport)

  client = SentimentAnalysis.Client(protocol)

  f = open(sys.argv[3], 'r')
  content = BeautifulSoup(f.read()).get_text()
  f.close()

  transport.open()
  
  print client.doAnalysis(content)

  transport.close()

except Thrift.TException, tx:
  print '%s' % (tx.message)
