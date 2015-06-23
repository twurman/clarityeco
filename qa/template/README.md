##OpenEphyra

OpenEphyra is an open source framework for question answering
developed at Carnegie Mellon.

Two versions of OpenEphyra are included in ClarityEco, both of which use
Apache Thrift for remote procedure calls. The version in this directory
is a stand-alone service for use in any application.

####Basic Setup
(1) Generate client/server stubs:

```
thrift --gen cpp qaservice.thrift
thrift --gen java qaservice.thrift
```

(2) Generate OpenEphyra bytecodes:

```
cd ../common/question-answer
ant
```

(3) Compile server:

`./compile-qa-server-thrift.sh`

(4) Start server:

PORT = the port that the OpenEphyra server uses

`./start-qa-server-thrift.sh (PORT)`
