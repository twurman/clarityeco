##Common

This directory contains resources shared between
versions of the question-answer service.

question-answer-dev/ -- contains a modified version of the OpenEphyra pkg.

question-answer/ -- contains the original version of the OpenEphyra pkg.

question-answer.tar.gz -- the original OpenEphyra tarball.

####Basic Setup

1) Build OpenEphyra bytecodes

```
cd question-answer
ant
```

2) Build the Wikipedia database

```
wget http://web.eecs.umich.edu/~jahausw/download/wiki_indri_index.tar.gz
tar xzvf wiki_indri_index.tar.gz -C question-answer/
```
