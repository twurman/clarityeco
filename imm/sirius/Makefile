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

proto:
	$(PROTO) --proto_path=$(COMMON_DIR) --cpp_out=$(COMMON_DIR) $(wildcard $(COMMON_DIR)/*.proto)

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CXX) $(CXX_FLAGS) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(BOOST_LIB) $(LINK_FLAGS)

common:
	$(CXX) -I$(CC_DIR) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $(COMMON_DIR)/detect.cpp -o $(COMMON_DIR)/detect.o
	$(CXX) -I$(CC_DIR) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $(COMMON_DIR)/kp_protobuf.cpp -o $(COMMON_DIR)/kp_protobuf.o

%.o: %.cpp
	$(CXX) -I$(CC_DIR) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $< -o $@

all: 
	$(MAKE) proto
	$(MAKE) common
	$(MAKE) $(TARGET_SERVER)

clean:
	rm -rf $(TARGET_SERVER) *.o

.PHONY:	all
