##Open Ephyra

Open Ephyra is an open source framework for question answering
developed at Carnegie Mellon.

Two versions of Open Ephyra are included in ClarityEco, both of which use
Apache Thrift for remote procedure calls. The version in this directory
belongs to a collection of services that connect with the new Sirius command center.

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

CMDCENTERPORT = the port used by the command center

`./start-qa-server-thrift.sh (PORT) (CMDCENTER-PORT)`

####Troubleshooting
Error message:

  `org.apache.thrift.transport.TTransportException: java.net.ConnectException: Connection refused`

Solution:  The QA service automatically tries to register with the command center
  at the port specified in start-qa-server-thrift.sh.
  If the command center is not running, then registration fails,
  and this message is reported.
  
  To solve this problem, first start the command center server
  located at ~/sirius/sirius-application/command-center
