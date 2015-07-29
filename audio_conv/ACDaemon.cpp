//IPAserivce header files
#include "IPAService.h"
#include "SchedulerService.h"
#include "service_constants.h"
#include "service_types.h"
#include "types_constants.h"
#include "types_types.h"

//C++ Libraries
#include <iostream>
#include <string>
#include <cstdlib>

//Boost Libraries
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

//Audio Conv Header Files
#include "audio_conv.h"

//Apache Thrift Header files
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/TSocket.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::concurrency;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

#define THREAD_WORKS 16

class AudioConversionServiceHandler : virtual public IPAServiceIf {
 public:
  AudioConversionServiceHandler(std::string service_name, std::string service_ip, int service_port,
                    std::string scheduler_ip, int scheduler_port) : 
                    SERVICE_NAME(service_name),SERVICE_IP(service_ip),
                    SERVICE_PORT(service_port), SCHEDULER_IP(scheduler_ip),
                    SCHEDULER_PORT(scheduler_port){}


  void submitQuery(std::string& _return, const ::QuerySpec& query) {
    std::string audio_file=query.content[0].data[0];;
    audioSpecs specs;
    specs.sample_rate = atoi(query.content[1].data[1].c_str());
    specs.bit_rate = atoi(query.content[2].data[2].c_str());
    specs.channels = atoi(query.content[3].data[3].c_str());
    specs.output_format = query.content[4].data[4];

    Audio_Service test;
    _return = test.conversion(audio_file,specs); 
  }

  //Initialize is to register with the command center
  void initialize(){
    boost::shared_ptr<TTransport> socket(new TSocket(SCHEDULER_IP,SCHEDULER_PORT));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    SchedulerServiceClient client(protocol);
    transport->open();

    THostPort hostPort;
    hostPort.ip=this->SERVICE_IP;
    hostPort.port=this->SERVICE_PORT;
    RegMessage regMessage;
    regMessage.app_name=this->SERVICE_NAME;
    regMessage.endpoint=hostPort;
    client.registerBackend(regMessage);
        
    std::cout << "registering to command center runnig at " << this->SCHEDULER_IP << ":" 
      <<this->SCHEDULER_PORT << std::endl;
    transport->close();
  }


  private:
  std::string SERVICE_NAME;
  std::string SERVICE_IP;   //Audio Conversion IP 
  int SERVICE_PORT;    //Audio Conversion Port 
  std::string SCHEDULER_IP; //Command Center IP
  int SCHEDULER_PORT;  //Command Center Port
};

int main(int argc, char **argv) {
  //Gathering info about service and command center
  std::string service_name;
  std::string service_ip;
  int service_port;
  std::string scheduler_ip;
  int scheduler_port;

  //Using boost program options to parse cmd args
  namespace po = boost::program_options;
  po::options_description desc("Allowed Options");
  desc.add_options()
    ("help,h", "Produce help message")
    ("port,p", po::value<int>()->default_value(9092),
      "Service port number")
    ("ccport,c", po::value<int>()->default_value(8888),
      "Command Center port number")
    ("ccip,i", po::value<std::string>()->default_value("localhost"),
      "Command Center IP address")
    ("svip,s", po::value<std::string>()->default_value("localhost"),
      "Service IP address");

  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);

  if (vm.count("help")){
    std::cout << desc << std::endl;
    return 1;
  }

  if(vm.count("port")){
    service_port = vm["port"].as<int>();
  }
  else {
    std::cout << "Using default service port" << std::endl;
  }
  if(vm.count("ccport")){
    scheduler_port = vm["ccport"].as<int>();
  }
  else {
    std::cout << "Using default scheduler port" << std::endl;
  }

  if(vm.count("ccip")){
    service_ip = vm["ccip"].as<std::string>();
  }
  else {
    std::cout << "Using default service ip" << std::endl;
  }

  if(vm.count("svip")){
   scheduler_ip  = vm["svip"].as<std::string>();
  }
  else {
    std::cout << " Using default scheduler ip" << std::endl;
  }
  

  //Creating new AudioConversion Pointer
  AudioConversionServiceHandler * AudioConversionService = 
      new AudioConversionServiceHandler(service_name, service_ip, 
      service_port, scheduler_ip,scheduler_port);
  
  //Setup the socket server and the service processor and handler
  boost::shared_ptr<AudioConversionServiceHandler> handler
    (AudioConversionService);
  boost::shared_ptr<TProcessor> processor(new IPAServiceProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport
    (new TServerSocket(service_port));

  //Setup the protocol and layered transport factories
  boost::shared_ptr<TTransportFactory> transportFactory(
    new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(
    new TBinaryProtocolFactory());

 
  //Setup the thread manager and thread factory
  boost::shared_ptr<ThreadManager> threadManager = 
    ThreadManager::newSimpleThreadManager(THREAD_WORKS);
  boost::shared_ptr<PosixThreadFactory> threadFactory = 
    boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);  
  
  threadManager->start();

  TThreadPoolServer server(processor, serverTransport, 
    transportFactory, protocolFactory, threadManager);

  std::cout << "Starting the audio conversion service..." << std::endl;

  //Create the Thread 
  boost::thread * server_thread = new boost::thread(boost::bind(&TThreadPoolServer::serve,&server));

  //Start the server
  AudioConversionService->initialize();
  
  std::cout << "audio conversion service is ready..."<<std::endl;

  server_thread->join(); 
  return 0;
}

