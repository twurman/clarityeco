include ../Makefile.config

CXX = g++
CXX_FLAGS = -I$(BOOST_LOCATION)

LINK_FLAGS =    -L$(BOOST_LIB) \
				-lboost_program_options \
				-lboost_filesystem \
				-lboost_system \
				-lboost_thread \
				-lboost_regex \
				-lthrift

TARGET_SERVER = ccserver

QA_DEPENDENCY = ../qa/sirius/
ASR_DEPENDENCY = ../asr/sirius/
IMM_DEPENDENCY = ../imm/sirius/

OBJECTS_SERVER = gen-cpp/QAService.o gen-cpp/KaldiService.o gen-cpp/ImageMatchingService.o \
		gen-cpp/CommandCenter.o gen-cpp/commandcenter_types.o CmdCenterDaemon.o base64.o

all: 
	thrift --gen cpp $(QA_DEPENDENCY)qaservice.thrift
	thrift --gen cpp $(ASR_DEPENDENCY)kaldi.thrift
	thrift --gen cpp $(IMM_DEPENDENCY)service.thrift
	thrift --gen cpp --gen java --gen js:node commandcenter.thrift
	thrift --gen cpp --gen js:node filetransfer_svc.thrift
	$(CXX) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LINK_FLAGS)

thrift: thrift_dep thrift_cc $(TARGET_SERVER)

thrift_dep: 
	thrift --gen cpp $(QA_DEPENDENCY)qaservice.thrift
	thrift --gen cpp $(ASR_DEPENDENCY)kaldi.thrift
	thrift --gen cpp $(IMM_DEPENDENCY)service.thrift
	
thrift_cc:
	thrift --gen cpp --gen java --gen js:node commandcenter.thrift
	thrift --gen cpp --gen js:node filetransfer_svc.thrift

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CXX) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LINK_FLAGS)

gen-cpp/%.o : gen-cpp/%.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET_SERVER) ccclient ccserver *.o gen-cpp/*.o

.PHONY:	all
