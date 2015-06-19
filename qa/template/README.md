##Open Ephyra

Open Ephyra is an open-source framework for question answering
developed at Carnegie Mellon.

Two versions of Open Ephyra are included in ClarityEco, both which use
Apache Thrift for remote procedure calls. The version in this directory
is a stand-alone service for use in any application.

####Basic Setup
Generate client/server stubs:

```
thrift --gen cpp qaservice.thrift
thrift --gen java qaservice.thrift
```

Compile server:

`./compile-qa-server-thrift.sh`

Start server:

PORT = the port that the Open Ephyra server uses

`./start-qa-server-thrift.sh (PORT)`
