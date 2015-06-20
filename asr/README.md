#Kaldi#
[Kaldi](http://kaldi.sourceforge.net/) is a speech recognition toolkit written in C++ that is freely available under the Apache license. 

Kaldi has been wraped in RPC framework using [Apache Thrift](http://thrift.apache.org/). 

Kaldi directory contains several files and folder:
```
kaldi-thrift/
scripts/
src.tar.gz
tools.tar.gz

```
`kaldi-thrift/` contains the Thrift components

`scripts/`			contains scripts

`src.tar.gz`		contains source code in a tar file

`tools.tar.gz`		contains tools for Kaldi in a tar file

###Basic Setup###



```
$ make thrift
$ make 
```

Open a second terminal to this same location.

In the first one, start up the serve with this command:

`$ ./server`

In the second one, run the client with a .wav file:

`$ ./client ../../../inputs/questions/what.is.the.speed.of.light.wav`
