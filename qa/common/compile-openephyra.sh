# Rather than forking a subshell, execute all commands
# in java-config.sh in the current shell.
source ../../java-config.sh

# Build OpenEphyra
cd ../common/question-answer
ant

# Build Wikipedia database
if [ ! -d "wiki_indri_index" ]; then
	echo "DRY RUN"
	#wget http://web.eecs.umich.edu/~jahausw/download/wiki_indri_index.tar.gz
	#tar xzvf wiki_indri_index.tar.gz -C question-answer/
fi

echo "OpenEphyra done"
