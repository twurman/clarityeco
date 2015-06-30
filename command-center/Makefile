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

QA_DEPENDENCY = ../qa/sirius/gen-cpp/
ASR_DEPENDENCY = ../asr/sirius/gen-cpp/
IMM_DEPENDENCY = ../imm/sirius/gen-cpp/

OBJECTS_SERVER = $(QA_DEPENDENCY)QAService.o \
		 $(ASR_DEPENDENCY)KaldiService.o \
		$(IMM_DEPENDENCY)ImageMatchingService.o \
		gen-cpp/CommandCenter.o gen-cpp/commandcenter_types.o CmdCenterDaemon.o \
		base64.o

all: thrift $(TARGET_SERVER) $(TARGET_CLIENT)

thrift: 
	thrift --gen cpp --gen java --gen js:node commandcenter.thrift
	thrift --gen cpp --gen js:node filetransfer_svc.thrift

cc_thrift:
	thrift --gen cpp --gen java --gen js:node commandcenter.thrift

fts_thrift:
	thrift --gen cpp --gen js:node filetransfer_svc.thrift

$(TARGET_CLIENT): $(OBJECTS_CLIENT)
	$(CXX) $(OBJECTS_CLIENT) -o $(TARGET_CLIENT) $(LINK_FLAGS)

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CXX) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LINK_FLAGS)

gen-cpp/%.o : gen-cpp/%.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET_SERVER) ccclient ccserver *.o gen-cpp/*.o

.PHONY:	all
