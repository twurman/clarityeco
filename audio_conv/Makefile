#include ../../Makefile.config

CXX = g++
CXXFLAGS = -Wall -g --std=c++11
CXX_LIB = -L/usr/local/lib

TARGET_CLIENT = acclient
TARGET_SERVER = acserver

GEN_DIR = ./gen-cpp
COMMON_DIR =./src

OBJECTS = $(patsubst %.cpp,%.o,$(wildcard $(COMMON_DIR)/*.cpp))


OBJECTS_CLIENT =  $(GEN_DIR)/IPAService.o $(GEN_DIR)/SchedulerService.o \
									$(GEN_DIR)/service_types.o $(GEN_DIR)/service_constants.o \
									$(GEN_DIR)/types_types.o $(GEN_DIR)/types_constants.o ACClient.o


OBJECTS_SERVER  = $(OBJECTS) $(GEN_DIR)/IPAService.o $(GEN_DIR)/SchedulerService.o \
									$(GEN_DIR)/service_types.o $(GEN_DIR)/service_constants.o \
									$(GEN_DIR)/types_types.o $(GEN_DIR)/types_constants.o ACDaemon.o


FFMPEG_LIBS= libavdevice		\
						 libavformat		\
						 libavfilter		\
						 libavcodec  		\
						 libswresample	\
						 libswscale     \
						 libavutil      



CXX_DIR = $(shell pkg-config --cflags $(FFMPEG_LIBS)) 
LIBS	= $(shell pkg-config --libs $(FFMPEG_LIBS))  
LIBS	+= -lthrift -lm -lboost_system -lboost_thread -lboost_program_options


all: thrift	$(TARGET_SERVER) $(TARGET_CLIENT) 

#all: thrift	$(TARGET_SERVER)  

thrift:
	thrift -r --gen cpp service.thrift 

$(TARGET_SERVER):	$(OBJECTS_SERVER) 
	$(CXX) $(CXXFLAGS) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LIBS)

$(TARGET_CLIENT):	$(OBJECTS_CLIENT) 
	$(CXX) $(CXXFLAGS) $(OBJECTS_CLIENT) -o $(TARGET_CLIENT) $(LIBS)


%.o:	%.cpp
	$(CXX) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXXFLAGS) $< -o $@

nuke: clean
	rm -rf gen-cpp/ $(COMMON_DIR)/*.o
clean: 
	rm -rf  $(TARGET_SERVER) $(TARGET_CLIENT) input* *.o *.wav *~ .*.swp  


.PHONY: all
