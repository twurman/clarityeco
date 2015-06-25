#Command Center
##Responsibilities:
- Maintains a list of registered services.
- Forwards a client's request to the appropriate registered services.
- Returns requested information to the client.


##Starting the command center:
1) Generate the client/server stubs
```
thrift --gen cpp --gen java commandcenter.thrift
```
(see <https://thrift.apache.org/>)

2) Compile the command center
```
make
```

3) Start the command center
```
./ccserver <port>
```


##Sending a request to the server:
1) Generate the command center thrift source code in your language of choice
```
thrift --gen <language> commandcenter.thrift
```

2) Step (1) will create a new directory, gen-<language>/ 
in this directory. Look at the generated code and include them
in your service. These files provide an interface for communicating with
the command center.

3) Create a CommandCenterClient object, and call its handleRequest() method
with your query type (eg. ASR-QA-IMM, ASR-QA), and your data. 
Check commandcenter.thrift or the generated source code if you're not sure how to make this call.


##Registering a service with the command center:
1) Generate the command center thrift source code in your language of choice
```
thrift --gen <language> commandcenter.thrift
```

2) Step (1) will create a new directory, gen-<language>/ 
in this directory. Look at the generated code and include them
in your service. These files provide an interface for communicating with
the command center.

3) Create a CommandCenterClient object, and call its registerService() method
with the appropriate arguments. Check commandcenter.thrift or the generated
source code if you're not sure how to make this call.

4) Link Libraries
(C++ services) When you compile your service, be sure to link in the
command center thrift binaries.

(Java services) When you compile/run your serve, make sure that the command
center java libraries are on your class path.

### Running with the mobile app
The current implementation of the mobile application requires a node.js server as an intermediary between the mobile app and the command center. To run this node server, you must have node installed on your machine as well as npm.
1) Download the thrift node packages in the command-center/ folder using
```
sudo npm install thrift
```
You should see a new folder called node_modules
2) Generate the thrift dependencies using
```
thrift --gen js:node filetransfer_svc.thrift
thrift --gen js:node commandcenter.thrift
```
3) Run the server using
```
node filetransfer_svc.js <fts_port> <cmdcenter_port>
```
Note that depending on how you install node, the command here could be node or nodejs
