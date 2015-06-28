# NOTE ABOUT CLASSPATHS:
# Classpaths contain jar files and paths to the TOP of package hierarchies.
# For example, say a program imports the class info.ephyra.OpenEphyra
# Now javac knows to look for the class info.ephyra.OpenEphyra in the directory info/ephyra/
# However, javac still needs the classpath to the package.

# Rather than forking a subshell, execute all commands
# in java-config.sh in the current shell.
source ../../java-config.sh

echo "Using thrift directory $thrift..."

# Add thrift libraries to class path
export JAVA_CLASS_PATH=$thrift/lib/java/build/libthrift-0.9.2.jar:$thrift/lib/java/build/lib/slf4j-api-1.5.8.jar:$thrift/lib/java/build/lib/slf4j-log4j12-1.5.8.jar:$thrift/lib/java/build/lib/log4j-1.2.14.jar

# Add command center to class path
export JAVA_CLASS_PATH=$JAVA_CLASS_PATH:../../command-center/gen-java

# Add OpenEphyra libraries to class path
export JAVA_CLASS_PATH=$JAVA_CLASS_PATH:../common/question-answer/bin

# Use cp flag to avoid cluttering up the CLASSPATH environment variable
javac -cp $JAVA_CLASS_PATH QADaemon.java QAServiceHandler.java gen-java/qastubs/QAService.java
