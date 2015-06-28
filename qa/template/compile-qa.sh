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
# 'source' command: Rather than forking a subshell, execute all commands
# in the current shell.
cd ../common
	./compile-openephyra.sh
	source ./qa-compile-config.sh
cd ../template
echo -e "Using thrift directory $thrift...\n"

# Use cp flag to avoid cluttering up the CLASSPATH environment variable
echo -e "Compiling QA server...\n"
javac -cp $JAVA_CLASS_PATH QADaemon.java QAServiceHandler.java gen-java/qastubs/QAService.java
