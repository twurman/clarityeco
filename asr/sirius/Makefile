# Makefile for generating the kaldi-thrift service
CXX = g++
FLAGS = -g --std=c++11
TARGET_CLIENT = client
TARGET_SERVER = server
LINK_FLAGS = -lthrift -lsox

# Needed for finding header files during preprocessing
CC_DIR = ../../command-center/gen-cpp 
CMDOBJECT_SERVER = ../../command-center/gen-cpp/CommandCenter.o \
                  ../../command-center/gen-cpp/commandcenter_types.o

OBJECTS_CLIENT = gen-cpp/KaldiService.o Kaldi_Client.o
OBJECTS_SERVER = gen-cpp/KaldiService.o Kaldi_Server.o $(CMDOBJECT_SERVER)

all: thrift  $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_CLIENT): $(OBJECTS_CLIENT)
	$(CXX) $(FLAGS) $(OBJECTS_CLIENT) -o $(TARGET_CLIENT) $(LINK_FLAGS)

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CXX) $(FLAGS) -I$(CC_DIR) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LINK_FLAGS)
thrift:
	thrift --gen cpp kaldi.thrift

%.o: %.cpp
	$(CXX) $(FLAGS) -I$(CC_DIR) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET_CLIENT) $(TARGET_SERVER) gen-cpp *.o *~ *wav

.PHONY:	all
