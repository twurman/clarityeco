#Audio Conversion#
[FFmpeg](http://ffmpeg.org/) is a audio/video conversion api written in C that is freely available from FFmpeg's website. 

FFmpeg has been wraped in RPC framework using [Apache Thrift](http://thrift.apache.org/). 

The Audio Conversion service converts a audio file(via string) and 
converts the file to a user specified format and the output is 
also a string

###Installation/Dependencies###
A custom installation of ffmpeg is required for this service to work a script is provided in `src/srcripts`

**Make sure that all previous installations of ffmpeg is uninstalled!**

`$ ./ffmpegscript.sh`

**Install any extra dependencies that may not be available**

To test the installation of ffmpeg type:

`$ ffmpeg -version`

The version number you want is 2.7.1

###Compiling###
The command below will make compile both the server and the client. 
```
$ make 
```

###Running Server and Client###
**Make sure a command center(ipa version) is running first**
Open a second terminal to this same location.

In the first terminal, start up the server with this command:

`$ ./acserver` -> use default connection settings
`$ ./acserver --help` to see the help menu

In the second one, run the client with a .wav file:

`$ ./acclient (CMDPORT) android_test.3gpp` -> use default connection settings

CMDPORT = port command center is running, default is 8888 



