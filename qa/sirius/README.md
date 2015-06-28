##Sirius

The question-answer program in sirius/ belongs to a collection of services that connect with Sirius.

####Basic Setup

1) Compile server: `./compile-qa.sh`

2) Start server: `./start-qa.sh (PORT) (SIRIUS-PORT)`

####Troubleshooting

Error message:

  `org.apache.thrift.transport.TTransportException: java.net.ConnectException: Connection refused`

Solution:  The QA service automatically tries to register with the Sirius
  at the port specified in start-qa-server-thrift.sh.
  If the Sirius is not running, then registration fails,
  and this message is reported.
  
  To solve this problem, first start the Sirius server
  located at ~/sirius/sirius-application/command-center
