var thrift = require('thrift');
var FileTransferSvc = require('./gen-nodejs/FileTransferSvc');
var CCDaemon = require('./gen-nodejs/CommandCenter');
var ttypes = require('./gen-nodejs/commandcenter_types');
var responses = [];

var ftsHandler = {
	ping: function(result) {
		console.log("Ping");
		result(null, "Hello from server");
	},

	send_file: function(data, uuid) {
		console.log("Client message recieved");
		//data = data.replace(data.substr(0, data.search(",") + 1), "");
		// console.log(data);
		var buffer = new Buffer(data, 'base64');
		// console.log(buffer);
		var connection = thrift.createConnection("localhost", cmdcenterport);
		var client = thrift.createClient(CCDaemon, connection);

		/*var qType = new QueryType();
		qType.ASR = true;
		qType.QA = false;
		qType.IMM = false;

		var audioFile = new File();
		audioFile.file = data;
		audioFile.b64format = true;

		var txtFile = new File();
		txtFile.file = '';
		txtFile.b64format = false;

		var immFile = new File();
		immFile.file = '';
		immFile.b64format = false;

		var qData = new QueryData();
		qData.audioFile = audioFile;
		qData.textFile = txtFile;
		qData.imgFile = immFile;
		*/

		client.handleRequest(
			data,
			function(err, response) {
			  console.log('ANSWER = ' + response);
			  if(!response) {
			  	console.log("Response is empty");
			  	response = "I'm not sure";
			  }
			  responses[uuid] = response;
			  connection.end();
		});

		//result(null, "");
	},

	get_response: function(uuid, result) {
		//while(!responses[uuid]) {} //this is going to prevent anyone else from using the server
		result(null, responses[uuid]);
		responses[uuid] = null;
	},
}

var ftService = {
	transport: thrift.TBufferedTransport,
	protocol: thrift.TJSONProtocol,
	processor: FileTransferSvc,
	handler: ftsHandler
};

var ServerOptions = {
	files: ".",
	services: {
		"/fts": ftService,
	}
}

var server = thrift.createWebServer(ServerOptions);
var port = process.argv[2]; //8585
var cmdcenterport = process.argv[3]; //8081
server.listen(port);
console.log("Http/Thrift Server running on port: " + port);
