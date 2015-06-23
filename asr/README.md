#Kaldi#
[Kaldi](http://kaldi.sourceforge.net/) is a speech recognition toolkit written in C++ that is freely available under the Apache license. 

Kaldi has been wraped in RPC framework using [Apache Thrift](http://thrift.apache.org/). 

`asr/` directory contains several files and folder:
```
common/
sirius/
template/

```
`common/` 		Contains the actual Kaldi package and scripts, with some modifications

`sirius/`		Contains an Kaldi Wrapper that communicates with the Sirius Command Centre

`template/`		Contains a stand-alone Kaldi wrapper for use in any application

###Basic Setup Before Startup###

Some preliminary steps are need before running kaldi in either `sirius/` folder or `template/` folder

```
$ cd common/scripts/
$ sudo ./prepare.sh
$ ./compile-kaldi.sh
```

###NOTES###
1. Make sure to resolve any extra dependencies that may appear before compiling kaldi with `compile-kaldi.sh`
2. Error with Openfst may appear when compiling, to resolve go to the `tools/` and type `make` and then try compiling again with `compile-kaldi.sh`
3. Please refer to the `INSTALL` file in `tools/` file after tools.tar.gz  
