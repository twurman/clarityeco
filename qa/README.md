##Question Answer

The question answering service is currently a wrapper around OpenEphyra,
an open source project from Carnegie Mellon.

Dependencies: You'll need to install thrift and modify java-config.inc
to include the path to your thrift installation. Then visit (thrift)/lib/java
and run `ant`, where (thrift) is the directory where you installed thrift.
For more information, take a look at [this](https://thrift.apache.org/lib/java).
This version has been tested with thrift-0.9.2.

common/ -- Contains the actual OpenEphyra package, with some modifications.
 You shouldn't need to modify this directory.

sirius/ -- Contains an OpenEphyra wrapper that communicates with Sirius.

template/ -- Contains a stand-alone OpenEphyra wrapper for use in any application.

Last Modified: 07/01/15
