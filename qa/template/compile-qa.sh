# NOTE ABOUT CLASSPATHS:
# Classpaths contain jar files and paths to the TOP of package hierarchies.
# For example, say a program imports the class info.ephyra.OpenEphyra
# Now javac knows to look for the class info.ephyra.OpenEphyra in the directory info/ephyra/
# However, javac still needs the classpath to the package.

# Generate thrift files
echo -e "Compiling thrift source code...\n"
thrift --gen java --gen cpp qaservice.thrift

# Compile client
echo -e "Making QA client...\n"
make

# Compile server
# Rather than forking a subshell, execute all commands
# in java-config.sh in the current shell.
source ../../java-config.sh
cd ../common
../common/compile-openephyra.sh
cd ../template

echo -e "Using thrift directory $thrift...\n"

# Add thrift libraries to class path
export JAVA_CLASS_PATH=$thrift/lib/java/build/libthrift-0.9.2.jar:$thrift/lib/java/build/lib/slf4j-api-1.5.8.jar:$thrift/lib/java/build/lib/slf4j-log4j12-1.5.8.jar:$thrift/lib/java/build/lib/log4j-1.2.14.jar

# Add OpenEphyra libraries to class path
export JAVA_CLASS_PATH=$JAVA_CLASS_PATH:../common/question-answer/bin

# Use cp flag to avoid cluttering up the CLASSPATH environment variable
echo -e "Compiling QA server...\n"
javac -cp $JAVA_CLASS_PATH QADaemon.java QAServiceHandler.java gen-java/qastubs/QAService.java
