source ../../java-config.sh

# Add thrift libraries to class path
export JAVA_CLASS_PATH=$thrift/lib/java/build/libthrift-0.9.2.jar:$thrift/lib/java/build/lib/slf4j-api-1.5.8.jar:$thrift/lib/java/build/lib/slf4j-log4j12-1.5.8.jar:$thrift/lib/java/build/lib/log4j-1.2.14.jar

# Add OpenEphyra libraries to class path
export JAVA_CLASS_PATH=$JAVA_CLASS_PATH:../common/question-answer/bin

echo -e "qa-compile-config.sh: Set compile-time class path for question-answer"
