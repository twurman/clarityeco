###To compile:

For sirius version, in sirius/ run
```
thrift --gen cpp service.thrift
make all
```

For example version, in template/ run 
```
thrift --gen cpp service.thrift
make all
```

###To run:
In either folder, run
```
./run_imserver.sh <imm_port> <cc_port>
```
Note that the template version does not have the cc_port argument because it does not register with the command center
