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
 
TARGET_SERVER = asrserver

CMDOBJECT_SERVER = $(HOME)/sirius/sirius-application/command-center/gen-cpp/CommandCenter.o \
                  $(HOME)/sirius/sirius-application/command-center/gen-cpp/commandcenter_types.o
# CMDOBJECT_SERVER = CommandCenter.o commandcenter_types.o
OBJECTS_SERVER = $(CMDOBJECT_SERVER) 

# Needed for finding header files during preprocessing
CC_DIR = $(HOME)/sirius/sirius-application/command-center/gen-cpp
GEN_DIR = ./gen-cpp
COMMON_DIR = ../common

all: $(TARGET_SERVER)

$(TARGET_SERVER): $(OBJECTS_SERVER)
	$(CXX) -I$(CC_DIR) -I$(GEN_DIR) -I$(COMMON_DIR) -c $(CXX_FLAGS) $(EXTRA_FLAGS) $(COMMON_DIR)/Kaldi_Server.cpp -o $(TARGET_SERVER) $(LINK_FLAGS)
	# $(CXX) $(CXX_FLAGS) $(OBJECTS_SERVER) -o $(TARGET_SERVER) $(LINK_FLAGS)

compile:
	sudo ../common/scripts/compile_kaldi.sh

prepare:
	sudo ../common/scripts/prepare.sh

clean:
	rm -rf 

.PHONY:	all
