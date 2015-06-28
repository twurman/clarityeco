#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <getopt.h> //for getopt_long()

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

// Thrift-generated stubs for RPC handling
#include "gen-cpp/CommandCenter.h"

using namespace std; 
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace cmdcenterstubs;

///////////////////////////////////////////////////////////////////////////////

void printHelpMsg();
void runQaTest(int port);
void runAsrQaTest(int port);
void runImmAsrQaTest(int port);

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
	// getopt_long() for options parsing: Getopt makes it easier
	// to have options that may take arguments, as well as long and
	// short options that share the same meaning.
	bool qa_test = false;
	bool asrqa_test = false;
	bool immasrqa_test = false;
	int c;
	while (1)
	{
		static struct option long_options[] =
		{
			// These options don't set a flag
			{"qa", no_argument, 0, 'q'},
			{"asrqa", no_argument, 0, 'a'},
			{"immasrqa", no_argument, 0, 'i'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
		};
		// getopt_long stores the option index here.
		int option_index = 0;

		c = getopt_long(argc, argv, "qaih", long_options, &option_index);
		
		// Detect end of options
		if (c == -1)
			break;

		switch (c)
		{
			case 'q':
				cout << "Client will run QA test" << endl;
				qa_test = true;
				break;
			case 'a':
				cout << "Client will run ASR-QA test" << endl;
				asrqa_test = true;
				break;
			case 'i':
				cout << "Client will run IMM-ASR-QA test" << endl;
				immasrqa_test = true;
				break;
			case 'h':
				printHelpMsg();
				return 0;
			case '?':
				// getopt_long has already printed an error message
				break;
			default:
				abort();
		}
	} // End while loop for options parsing

	// Collect the port number after options have been parsed
	int port = -1;
	if (optind < argc)
	{
		port = atoi(argv[optind]);
		cout << "Port = " << port << endl;
	}
	else
	{
		cout << "Please specify a port number" << endl;
		printHelpMsg();
		exit(1);
	}

	// Run tests specified by the user
	if (!qa_test && !asrqa_test && !immasrqa_test)
	{
		cout << "Please tell me what test to run" << endl;
		printHelpMsg();
		exit(1);
	}
	if (qa_test)
	{
		cout << "Running QA test..." << endl;
		runQaTest(port);
	}
	if (asrqa_test)
	{
		cout << "Running ASR-QA test..." << endl;
		runAsrQaTest(port);
	}	
	if (immasrqa_test)
	{
		cout << "Running IMM-ASR-QA test..." << endl;
		runImmAsrQaTest(port);
	}
	return 0;
}

void printHelpMsg()
{
	cout << "Usage: ./ccclient [--qa|--asrqa|--immasrqa] (PORT)" << endl;
}

void runQaTest(int port)
{
	string answer;
	// Initialize thrift RPC objects
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", port));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	CommandCenterClient client(protocol);

	try
	{
		QueryData directed_inception;
		directed_inception.audioData = "";
		directed_inception.textData = "Who directed inception?";
		directed_inception.imgData = "";

		// Talk to Sirius
		transport->open();
		cout << "///// QA /////" << endl;
		client.handleRequest(answer, directed_inception);
		cout << "ANSWER = " << answer << endl;
		transport->close();
	}
	catch (TException &tx) //NOTE: execution resumes AFTER try-catch block
	{
                cout << "CLIENT ERROR: " << tx.what() << endl;
	}
}

void runAsrQaTest(int port)
{
	string answer;
	// Initialize thrift RPC objects
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", port));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	CommandCenterClient client(protocol);

	try
	{
		//Audio File
		ifstream fin_audio("what.is.the.capital.of.italy.wav",ios::binary);
		ostringstream ostrm_audio;
		ostrm_audio << fin_audio.rdbuf();
		std::string capital_italy_audio_file(ostrm_audio.str());

		QueryData capital_italy;
		capital_italy.audioData = capital_italy_audio_file;
		capital_italy.textData = "";
		capital_italy.imgData = "";

		// Talk to Sirius
		transport->open();
		cout << "///// ASR-QA /////" << endl;
		client.handleRequest(answer, capital_italy);
		cout << "ANSWER = " << answer << endl;
		transport->close();
	}
	catch (TException &tx)
	{
                cout << "CLIENT ERROR: " << tx.what() << endl;
	}
}

void runImmAsrQaTest(int port)
{
	string answer;
	// Initialize thrift RPC objects
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", port));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	CommandCenterClient client(protocol);
	try
	{
		// Audio File
		ifstream fin_audio01("how.tall.is.this.wav",ios::binary);
		ostringstream ostrm_audio01;
		ostrm_audio01 << fin_audio01.rdbuf();
		std::string how_tall_audio_file(ostrm_audio01.str());	
	
		//Image File
		ifstream fin_image("test.jpg", ios::binary);
		ostringstream ostrm_image;
		ostrm_image << fin_image.rdbuf();
		string image_file(ostrm_image.str());

		QueryData how_tall;
		how_tall.audioData = how_tall_audio_file;
		how_tall.textData = "";
		how_tall.imgData = image_file;

		// Talk to Sirius
		transport->open();
		cout << "\n///// ASR-QA-IMM /////" << endl;
		client.handleRequest(answer, how_tall);
		cout << "ANSWER = " << answer << endl;
		transport->close();
	}
	catch (TException &tx)
	{
                cout << "CLIENT ERROR: " << tx.what() << endl;
	}
}
