//generated files
#include "gen-cpp/IPAService.h"
#include "gen-cpp/SchedulerService.h"
#include "gen-cpp/types_types.h"

//C++ Libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

//thrift dependencies 
#include <thrift/protocol/TBinaryProtocol.h>             
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TSocket.h>                    
#include <thrift/transport/TBufferTransports.h>          
#include <thrift/transport/TTransportUtils.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char **argv ) {
  //Creating a server port and ip 
  std::string server_ip = "localhost";
  std::string audio_file; 
  //Creating command center port and ip
  std::string cmd_ip="localhost";
  int cmd_port = 8888;
  if(argc==3){
   audio_file = argv[1];
   cmd_port = atoi(argv[2]);
  }
  else if(argc==2){
    audio_file=argv[1];
    std::cout << "Using default port for command center...";
    std::cout << std::endl;
  }
  else{
    std::cerr << "Please provide an audio file to be converted..."; 
    std::cerr << std::endl;
    exit(0);
  }
  //Consulting with the command center for a IP and port of required service
  boost::shared_ptr<TTransport> cmd_socket(new TSocket(cmd_ip,cmd_port));
  
  boost::shared_ptr<TTransport> cmd_transport(new 
    TBufferedTransport(cmd_socket));
  
  boost::shared_ptr<TProtocol> cmd_protocol(new 
    TBinaryProtocol(cmd_transport));
  
  SchedulerServiceClient *cmd_client = new 
    SchedulerServiceClient(cmd_protocol);
  THostPort hostPort; 
  cmd_transport->open();
  try {  
    cmd_client -> consultAddress(hostPort,"audio_conv");
    std::cout << "Command Center returns " << hostPort.ip << ":"; 
    std::cout << hostPort.port << std::endl;
  } catch( TException &tx){
    std::cout << "ERROR: " << tx.what() << std::endl;
  }

  cmd_transport->close();
//Connecting with the service with info from command center
  boost::shared_ptr<TTransport> audio_socket(new 
      TSocket(hostPort.ip,hostPort.port));

 boost::shared_ptr<TTransport> audio_transport(new 
    TBufferedTransport(audio_socket));
  
  boost::shared_ptr<TProtocol> audio_protocol(new 
    TBinaryProtocol(audio_transport));
  
  IPAServiceClient audio_client(audio_protocol);
  
  audio_transport -> open();
 try {
    std::ifstream fin(audio_file.c_str(), std::ios::binary);
    if (!fin) std::cerr << "Could not open the file!" << std::endl;      
    std::ostringstream ostrm;
    ostrm << fin.rdbuf();
    std::string audio_file_to_send(ostrm.str());
    QueryInput queryInput;
    
    QuerySpec query;
    query.__set_name("audio_conv");
    
    queryInput.type="audio";
    queryInput.data.push_back(audio_file_to_send);
    
    query.content.push_back(queryInput);

    queryInput.type="parameter";
    queryInput.data.push_back("8000");
    queryInput.tags.push_back("sample_rate");
    queryInput.__set_tags(queryInput.tags);
    query.content.push_back(queryInput);
    
    queryInput.type="parameter";
    queryInput.data.push_back("0");
    queryInput.tags.push_back("bit_rate");
    queryInput.__set_tags(queryInput.tags);
    query.content.push_back(queryInput);
    
    queryInput.type="parameter";
    queryInput.data.push_back("0");
    queryInput.tags.push_back("channels");
    queryInput.__set_tags(queryInput.tags);
    query.content.push_back(queryInput);

    queryInput.type="parameter";
    queryInput.data.push_back("wav");
    queryInput.tags.push_back("output_format");
    queryInput.__set_tags(queryInput.tags);
    query.content.push_back(queryInput);

    std::string converted_audio;
    std::cout<<query.name<<std::endl;
    audio_client.submitQuery(converted_audio, query);
    audio_transport -> close();
    std::string audio_path = "converted.wav";
    std::ofstream audiofile(audio_path.c_str(), std::ios::binary);
    audiofile.write(converted_audio.c_str(),converted_audio.size());
  audiofile.close();  
} catch (TException &tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  } 

    return 0;
}
