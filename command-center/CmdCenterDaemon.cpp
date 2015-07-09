#include "CmdCenterDaemon.h"

//---- Utilities ----//
string parseImgFile(const string& immRetVal);
string parseDecodedAudio(const string& asrRetVal);

class CommandCenterHandler : public CommandCenterIf
{
public:
	// ctor: initialize command center's tables
	CommandCenterHandler()
	{
		registeredServices = multimap<string, ServiceData*>();
	}

	//allows services to register with the command center
	virtual void registerService(const string& serviceType, const MachineData& mDataObj)
	{
		cout << "/-----registerService()-----/" << endl;
		cout << "received request from " << mDataObj.name
		     << ":" << mDataObj.port << ", serviceType = " << serviceType
		     << endl;

		registeredServices.insert( 
			pair<string, ServiceData*>(serviceType, new ServiceData(mDataObj.name, mDataObj.port))
		);
	
		cout << "There are now " << registeredServices.size() << " registered services" << endl;
		cout << "LIST OF REGISTERED SERVICES:" << endl;
		multimap<string, ServiceData*>::iterator it;
		for (it = registeredServices.begin(); it != registeredServices.end(); ++it)
		{
			cout << "\t" << it->first << endl;
		}
	}

	//receives requests from a client and determines the workflow needed to process the request 
	virtual void handleRequest(string& _return, const QueryData& data)
	{
		cout << "/-----handleRequest()-----/" << endl;

		//---- Transform data into a form the services can use ----//
		string binary_audio, binary_img;
		if(data.audioB64Encoding) {
			cout << "Decoding audio..." << endl;
			binary_audio = base64_decode(data.audioData);	
		} else {
			binary_audio = data.audioData;
		}
		if(data.imgB64Encoding) {
			cout << "Decoding img..." << endl;
			binary_img = base64_decode(data.imgData);
		} else {
			binary_img = data.imgData;
		}

		//---- Create clients ----//
		AsrServiceData *asr = NULL;
		ImmServiceData *imm = NULL;
		QaServiceData *qa = NULL;
		//---- Kaldi speech recognition client
		if (data.audioData != "") {
			cout << "Getting asr client...\t";
			try {
				asr = new AsrServiceData(assignService("ASR"));
			} catch (AssignmentFailedException exc) {
				cout << exc.err << endl;
				return;
			}
			// asr = new AsrServiceData(sd);
			cout << "AsrServiceData object constructed" << endl;
		}
		//---- Image matching client
		if (data.imgData != "") {
			cout << "Getting imm client...\t";
			try {
				imm = new ImmServiceData(assignService("IMM"));
			} catch (AssignmentFailedException exc) {
				cout << exc.err << endl;
				return;
			}
			// imm = new ImmServiceData(sd);
			cout << "ImmServiceData object constructed" << endl;
		}
		//---- Open Ephyra QA client
		cout << "Getting qa client...\t";
		try {
			qa = new QaServiceData(assignService("QA"));
		} catch (AssignmentFailedException exc) {
			cout << exc.err << endl;
			return;
		}
		// qa = new QaServiceData(sd);
		cout << "QaServiceData object constructed" << endl;

		//---- Set audio and imm fields in service data objects
		// if they were constructed
		if (asr) {
			asr->audio = binary_audio;
		} if (imm) {
			imm->img = binary_img;
		}

		//---- Run pipeline ----//
		string asrRetVal = "";
		string immRetVal = "";
		string question = "";
		if ((data.audioData != "") && (data.imgData != ""))
		{
			cout << "Starting IMM-ASR-QA pipeline..." << endl;
			asr->transport->open();
			asr->client.kaldi_asr(asrRetVal, binary_audio);
			asr->transport->close();
			asrRetVal = parseDecodedAudio(asrRetVal);

			imm->transport->open();
			imm->client.send_request(immRetVal, binary_img);
			imm->transport->close();

			question = asrRetVal + " " + parseImgFile(immRetVal);
			cout << "Your new question is: " << question << endl;
			qa->transport->open();
			qa->client.askFactoidThrift(_return, question);
			qa->transport->close();
		}
		else if (data.audioData != "")
		{
			cout << "Starting ASR-QA pipeline..." << endl;
			asr->transport->open();
			asr->client.kaldi_asr(asrRetVal, binary_audio);
			asr->transport->close();
			asrRetVal = parseDecodedAudio(asrRetVal);

			qa->transport->open();
			qa->client.askFactoidThrift(_return, asrRetVal);
			qa->transport->close();
		}
		else if (data.textData != "")
		{
			cout << "Starting QA test..." << endl;
			qa->transport->open();
			qa->client.askFactoidThrift(_return, data.textData);
			qa->transport->close();
		}
		else if (data.imgData != "")
		{
			imm->transport->open();
			imm->client.send_request(_return, binary_img);
			imm->transport->close();
		}
		else
		{
			cout << "Nothing in the pipeline" << endl;
		}

	}

	void ping()
	{
		cout << "ping!" << endl;
	}

private:
	// registeredServices: a table of all servers that registered with
	// the command center via the registerService() method
	multimap<string, ServiceData*> registeredServices;
	boost::thread heartbeatThread;

	ServiceData* assignService(const string type) {
		multimap<string, ServiceData*>::iterator it;
		it = registeredServices.find(type);
		if (it != registeredServices.end()) {
			return it->second;
		} else {
			string msg = type + " requested, but not found";
			cout << msg << endl;
			throw(AssignmentFailedException(type + " requested, but not found"));
			return NULL;
		}
	}
};

int main(int argc, char **argv) {
	int port = 8081;
	if (argv[1])
	{
		port = atoi(argv[1]);
	}
	else
	{
		cout << "Command center port not specified; using default" << endl;
	}

	boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	boost::shared_ptr<CommandCenterHandler> handler(new CommandCenterHandler());
	boost::shared_ptr<TProcessor> processor(new CommandCenterProcessor(handler));
	boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());

	// initialize the thread manager and factory
	boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(THREAD_WORKS);
	boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();

	// initialize the image matching server
	TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory, threadManager);
	cout << "Starting the command center server on port " << port << "..." << endl;
	server.serve();
	cout << "Done." << endl;
	return 0;
}

string parseImgFile(const string& immRetVal)
{
	// Everything must be escaped twice
	const char *regexPattern = "\\A(/?)([\\w\\-]+/)*([\\w\\-]+)(\\.jpg)\\z";
	cout << "Passing the following pattern to regex engine: " << regexPattern << endl;
	boost::regex re(regexPattern);
	string fmt("$3");
	string outstr = immRetVal;
	if (boost::regex_match(immRetVal, re))
	{
		cout << immRetVal << " matches pattern"  << endl;
		outstr = boost::regex_replace(immRetVal, re, fmt);
		cout << "Input: " << immRetVal << endl;
		cout << "Result: " << outstr << endl;
	}
	else
	{
		cout << "No match for " << immRetVal << "..." << endl;
		throw(BadImgFileException());
	}

	return outstr;
}

string parseDecodedAudio(const string& asrRetVal)
{
	// NOTE: pattern uh-huh matches [uh-huh];
	// pattern (uh-huh|uh) matches [uh-huh];
	// pattern (uh|uh-huh) matches [uh]-h[uh].
	const string fillerWords = "(mhm|mm|yeah|Uh-huh|uh-huh|uh|ah|\\[noise\\]|\\[laughter\\])";
	const string leading = " " + fillerWords;
	const string trailing = fillerWords + " ";
	const string both = " " + fillerWords + " ";
	const string fillerWordPattern = leading + "|" + trailing + "|" + both; 
	const string conjAtStart = "^(but|so)(, | )";
	const string repeatedWordAtStart = "^([^ ]* )(\\1)";
	const string extraSpace = "( ){2,}";
	string fmtFillers = "";
	string fmtSo = "";
	string fmtRepeats = "$2";
	string fmtSpace = " ";

	cout << "Passing the following patterns to regex engine: "
		<< fillerWordPattern << endl << conjAtStart << endl
		<< repeatedWordAtStart << extraSpace << endl;
	boost::regex fw_re(fillerWordPattern);
	boost::regex cs_re(conjAtStart);
	boost::regex rw_re(repeatedWordAtStart);
	boost::regex exsp_re(extraSpace);

	// Changing the order in which the filters are applied
	// WILL change the quality of the parsing.
	string outstr = asrRetVal;
	cout << "Input: " << outstr << endl;
	outstr = boost::regex_replace(
		outstr, fw_re, fmtFillers);
	outstr = boost::regex_replace(
		outstr, cs_re, fmtSo);
	outstr = boost::regex_replace(
		outstr, rw_re, fmtRepeats);
	outstr = boost::regex_replace(
		outstr, exsp_re, fmtSpace);
	cout << "Result: " << outstr << endl;

	return outstr;

}
