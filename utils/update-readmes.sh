#!/bin/bash
# Bash script to update a file's timestamp after editing it

# Description:
# First, open the file for editing. Then, do one of the following:
# 1) If there is no timestamp, add one.
# 2) If there is a timestamp, update it.

# Usage:
# You could start every vim session by running this script, but that's
# inconvenient. Why not add an alias to ~/.bashrc instead?

###############################################################################

vim $1

###############################################################################

# NOTE on quoting: Double quoting prevents
# the expansion of '*', while single quoting
# also prevents the expansion of variables.
#
# The POSIX extended regular expression pattern
# that will be used to search for the timestamp.
tagpattern="Last Modified:"
datepattern="[[:digit:]]{2}/[[:digit:]]{2}/[[:digit:]]{2}"
timepattern="[[:digit:]]{2}:[[:digit:]]{2}:[[:digit:]]{2}"
pattern="$tagpattern[[:space:]]$datepattern[[:space:]]*$timepattern"

echo -e "Examining $1..."
# Prepare a timestamp
date_lastmodified=`date -r "$1" +%m/%d/%y`
time_lastmodified=`date -r "$1" +%H:%M:%S`
timestamp="Last Modified: $date_lastmodified $time_lastmodified" 

lastline=`sed -n '$'p $1`
# If I used sed instead of grep, I would need to use
# POSIX basic regexes, which are not as nice.
# The grep options are:
#	-q = silent output
#	-E = extended regex
#	-e = regex pattern to search for
if echo "$lastline" | grep -q -E -e "$pattern"
then
	echo -e "new timestamp = $timestamp"
	echo -e "old timestamp = $lastline"
	# NOTE: you can use any delimiter with sed,
	# with the caveat that the delimiter cannot
	# appear in the regex pattern or the replacement.
	# The format of this command is:
	# sed "<Address=$> <Command=s>" <Filename>
	# The address, '$', ensures that the 's' command
	# is only executed on the last line
	# of the file.
	sed -i "$ s@$lastline@$timestamp@" $1 
else
	echo -e "TIMESTAMP NOT FOUND"
	echo -e "\n$timestamp" >> $1
fi
