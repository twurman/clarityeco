include ../../Makefile.config

CXX = g++
CXXFLAGS = -Wall -g --std=c++11
CXX_LIB = -L/usr/local/lib

TARGET_CLIENT = acclient
TARGET_SERVER = acserver

GEN_DIR = ./gen-cpp
COMMON_DIR =../common

OBJECTS = $(patsubst %.cpp,%.o,$(wildcard $(COMMON_DIR)/*.cpp))


OBJECTS_CLIENT =  $(OBJECTS) $(GEN_DIR)/AudioService.o ACClient.o
OBJECTS_SERVER  = $(OBJECTS) $(GEN_DIR)/AudioService.o ACDaemon.o


FFMPEG_LIBS= libavdevice		\
						 libavformat		\
						 libavfilter		\
						 libavcodec  		\
						 libswresample	\
						 libswscale     \
						 libavutil      



CXX_DIR = $(shell pkg-config --cflags $(FFMPEG_LIBS)) 
LIBS	= $(shell pkg-config --libs $(FFMPEG_LIBS))  
LIBS	+= -lthrift -lm


all: thrift	$(TARGET_SERVER) $(TARGET_CLIENT) 

thrift:
	thrift --gen cpp audio.thrift 

$(TARGET_SERVER):	$(OBJECTS_SERVER) 
	$(CXX) $(CXXFLAGS) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LIBS)

$(TARGET_CLIENT):	$(OBJECTS_CLIENT) 
	$(CXX) $(CXXFLAGS) $(OBJECTS_CLIENT) -o $(TARGET_CLIENT) $(LIBS)


%.o:	%.cpp
	$(CXX) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXX_FLAGS) $< -o $@

clean: 
	rm -rf  gen-cpp/ $(TARGET_SERVER) $(TARGET_CLIENT) *.o *.wav *~ .*.swp $(COMMON_DIR)/*.o 


.PHONY: all
