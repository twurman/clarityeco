#Kaldi#
[Kaldi](http://kaldi.sourceforge.net/) is a speech recognition toolkit written in C++ that is freely available under the Apache license. 

Kaldi has been wraped in RPC framework using [Apache Thrift](http://thrift.apache.org/). 

Two Versions of Open Ephyra are included in ClarityEco, both of which use Apache Thrift for remote procedure calls. The version in this directory belongs to a collection of services that connect with the new Sirus command center

###Basic Setup###

```
$ thrift --gen cpp kaldi.thrift
$ make 
```
Start server:

PORT = the por that the Kaldi Server uses

CMDCENTERPORT = the port used by the command center

In the first one, start up the serve with this command:

`$ ./server (PORT) (CMDCENTER-PORT)` 

