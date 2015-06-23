CXX = g++
PROTO = protoc
# Needed for linking
BOOST_LOCATION = /usr/local/boost_1_58_0
BOOST_LIB = /usr/local/boost_1_58_0/stage/lib
CXX_FLAGS = -I$(BOOST_LOCATION)

LINK_FLAGS = -L$(BOOST_LIB) \
					-lopencv_core \
					-lopencv_highgui \
					-lopencv_imgproc \
					-lopencv_nonfree \
					-lopencv_flann \
					-lopencv_objdetect \
					-lopencv_features2d \
					-lopencv_gpu \
					-lrt \
					-lboost_program_options \
					-lboost_filesystem \
					-lboost_system \
					-lboost_thread \
					-lboost_regex \
					-lprotobuf \
					-ltesseract \
					-lthrift
 
TARGET_SERVER = imserver

CMDOBJECT_SERVER = $(HOME)/sirius/sirius-application/command-center/gen-cpp/CommandCenter.o \
                  $(HOME)/sirius/sirius-application/command-center/gen-cpp/commandcenter_types.o
# CMDOBJECT_SERVER = CommandCenter.o commandcenter_types.o
OBJECTS_SERVER = $(CMDOBJECT_SERVER) \
					./gen-cpp/ImageMatchingService.o \
					../common/detect.o \
					../common/kp_protobuf.o \
					../common/keypoints.pb.cc \
					../common/store_mat.pb.cc \
					IMSDaemon.o

# Needed for finding header files during preprocessing
CC_DIR = $(HOME)/sirius/sirius-application/command-center/gen-cpp
GEN_DIR = ./gen-cpp
COMMON_DIR = ../common

all:
	$(PROTO) --proto_path=$(COMMON_DIR) --cpp_out=$(COMMON_DIR) $(wildcard $(COMMON_DIR)/*.proto)
	$(CXX) -I $(BOOST_LOCATION) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(BOOST_LIB) $(LINK_FLAGS)

# $(TARGET_SERVER): $(OBJECTS_SERVER)
# 	$(CXX) -I $(BOOST_LOCATION) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(BOOST_LIB) $(LINK_FLAGS)

%.o: %.cpp
	$(CXX) -I$(CC_DIR) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET_SERVER) *.o

.PHONY:	all
