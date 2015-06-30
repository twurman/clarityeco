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

OBJECTS_SERVER = $(QA_DEPENDENCY)gen-cpp/QAService.o \
		 $(ASR_DEPENDENCY)gen-cpp/KaldiService.o \
		$(IMM_DEPENDENCY)gen-cpp/ImageMatchingService.o \
		gen-cpp/CommandCenter.o gen-cpp/commandcenter_types.o CmdCenterDaemon.o \
		base64.o

all: thrift $(TARGET_SERVER)

thrift: 
	thrift --gen cpp --gen java --gen js:node commandcenter.thrift
	thrift --gen cpp --gen js:node filetransfer_svc.thrift
	thrift --gen cpp QA_DEPENDENCY/qaservice.thrift
	thrift --gen cpp ASR_DEPENDENCY/kaldi.thrift
	thrift --gen cpp IMM_DEPENDENCY/service.thrift

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CXX) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LINK_FLAGS)

$(QA_DEPENDENCY)gen-cpp/%.o : $(QA_DEPENDENCY)gen-cpp/%.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

$(ASR_DEPENDENCY)gen-cpp/%.o : $(ASR_DEPENDENCY)gen-cpp/%.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

$(IMM_DEPENDENCY)gen-cpp/%.o : $(IMM_DEPENDENCY)gen-cpp/%.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@	 

gen-cpp/%.o : gen-cpp/%.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET_SERVER) ccclient ccserver *.o gen-cpp/*.o

.PHONY:	all
