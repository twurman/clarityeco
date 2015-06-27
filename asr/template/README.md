#Kaldi#
[Kaldi](http://kaldi.sourceforge.net/) is a speech recognition toolkit written in C++ that is freely available under the Apache license. 

Kaldi has been wraped in RPC framework using [Apache Thrift](http://thrift.apache.org/). 

Two Versions of Open Ephyra are included in ClarityEco, both of which use Apache Thrift for remote procedure calls. The version in this directory is a stand-alone service for use in any application

###Basic Setup###

```
$ make 
```

###Running Server and Client###
Open a second terminal to this same location.

In the first one, start up the serve with this command:

PORT = the port that the Kaldi Server uses

`$ ./server (PORT)`

In the second one, run the client with a .wav file:

`$ ./client (PORT) ../common/ENG_M.wav`
