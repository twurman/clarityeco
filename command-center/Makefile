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

OBJECTS_SERVER = ../qa/sirius/gen-cpp/QAService.o \
		 ../asr/sirius/gen-cpp/KaldiService.o \
		../imm/sirius/gen-cpp/ImageMatchingService.o \
		gen-cpp/CommandCenter.o gen-cpp/commandcenter_types.o CmdCenterDaemon.o \
		base64.o

all: thrift $(TARGET_SERVER) $(TARGET_CLIENT)

thrift: cc_thrift fts_thrift

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

# gen-cpp/commandcenter_types.o : gen-cpp/commandcenter_types.cpp
# 	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

# CmdCenterDaemon.o : CmdCenterDaemon.cpp
# 	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

base64.o : base64.cpp
	$(CXX) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET_SERVER) ccclient ccserver *.o gen-cpp/*.o

.PHONY:	all
