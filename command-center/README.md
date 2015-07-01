#Command Center
##Responsibilities:
- Maintains a list of registered services.
- Forwards a client's request to the appropriate registered services.
- Returns requested information to the client.


##Starting the command center:
1) Compile the thrift files
```
make thrift
```
2) Compile the command center
```
make
```
3) Start the command center
```
./ccserver <port>
```

##Using Sirius services
You can use the supplied Sirius services in the clarity ecosystem in conjunction with the command center. These services (asr, qa, imm) are in each services sirius/ subdirectory. The command center must be compiled before you can compile the sirius versions of these services.


### Running with SiriusMobile v0.1
The current implementation of the mobile application requires a node.js server as an intermediary between the mobile app and the command center. To run this node server, you must have node installed on your machine as well as npm.
1) Download the thrift node packages in the command-center/ folder using
```
sudo npm install thrift
```
You should see a new folder called node_modules

2) Run the server using
```
node filetransfer_svc.js <fts_port> <cmdcenter_port>
```
Note that depending on how you installed node, this command could be node or nodejs
