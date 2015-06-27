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

# Add my classes to class path
# NOTE: these class paths are only necessary when running the program
export JAVA_CLASS_PATH=$JAVA_CLASS_PATH:`pwd`:`pwd`/gen-java

# Add OpenEphyra libraries to class path; start from top directory (question-answer)
cd ../common/question-answer

export JAVA_CLASS_PATH=$JAVA_CLASS_PATH:bin:lib/ml/maxent.jar:lib/ml/minorthird.jar:lib/nlp/jwnl.jar:lib/nlp/lingpipe.jar:lib/nlp/opennlp-tools.jar:lib/nlp/plingstemmer.jar:lib/nlp/snowball.jar:lib/nlp/stanford-ner.jar:lib/nlp/stanford-parser.jar:lib/nlp/stanford-postagger.jar:lib/qa/javelin.jar:lib/search/bing-search-java-sdk.jar:lib/search/googleapi.jar:lib/search/indri.jar:lib/search/yahoosearch.jar:lib/util/commons-logging.jar:lib/util/gson.jar:lib/util/htmlparser.jar:lib/util/log4j.jar:lib/util/trove.jar:lib/util/servlet-api.jar:lib/util/jetty-all.jar:lib/util/commons-codec-1.9.jar

export INDRI_INDEX=`pwd`/wiki_indri_index/
#export THREADS=$(cat /proc/cpuinfo | grep processor | wc -l)
#if [ $THREADS -lt 8 ]; then
#  export THREADS=8
#fi

# Use cp flag to avoid cluttering up the CLASSPATH environment variable
java -cp $JAVA_CLASS_PATH -Djava.library.path=lib/search/ -server -Xms1024m -Xmx2048m QADaemon $1
