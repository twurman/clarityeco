#!/bin/bash

#Remove and older installations if used by package manager.
#sudo apt-get -y remove ffmpeg x264 libav-tools libvpx-dev libx264-dev

#Dependencies
sudo apt-get update

sudo apt-get -y --force-yes install autoconf automake build-essential libass-dev libfreetype6-dev \
	libsdl1.2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev \
	libxcb-xfixes0-dev pkg-config texi2html zlib1g-dev mercurial cmake cmake-curses-gui build-essential \
	libspeex-dev libvo-aacenc-dev libopencore-amrnb-dev frei0r-plugins-dev libgnutls-dev ladspa-sdk\
	libbluray-dev libbs2b-dev flite1-dev libflite1 libgme-dev libgsm1-dev libmodplug-dev libsox-dev libsoxr-dev\
	libwavpack-dev libwebp-dev libzmq3-dev libopenal-dev libopencv-dev git

#An assembler for x86 optomizations
#sudo apt-get install yasm
echo "Opus audio decoder and encoder"
sudo apt-get -y install libopus-dev
echo "H.264 video encoder"
sudo apt-get -y install libx264-dev
echo "libx265: H.265/HEVC video encoder"
mkdir ffmpeg_sources
cd ffmpeg_sources
hg clone https://bitbucket.org/multicoreware/x265
cd x265/build/linux
cmake ../../source
make
sudo make install
cd ../../../
#libfdk-acc: Acc audio encoder (Comment it out due to licensing issues)
#wget -O fdk-aac.tar.gz https://github.com/mstorsjo/fdk-aac/tarball/master
#tar xzvf fdk-aac.tar.gz
#cd mstorsjo-fdk-aac*
#./autogen.sh
#./configure --disable-shared
#make 
#sudo make install 
#cd ../

echo "libvidstab: video stablization"
wget -O georgmartius-vid.stab-3b35b4d.tar.gz https://github.com/georgmartius/vid.stab/tarball/release-0.98b
tar zxf georgmartius-vid.stab-3b35b4d.tar.gz
cd georgmartius-vid.stab-3b35b4d  
cmake .
make 
sudo make install
cd ../



echo "libvpx: VP8/VP9 video encoder and decoder"
git clone --depth 1 https://chromium.googlesource.com/webm/libvpx 
cd libvpx
./configure --enable-shared
make
sudo make install
cd ../
#Install mp3 encoder
sudo apt-get -y install libmp3lame-dev
sudo apt-get -y install libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev
#install ffmpeg
wget ffmpeg.org/releases/ffmpeg-2.7.1.tar.gz
tar zxvf ffmpeg-2.7.1.tar.gz
cd ffmpeg-2.7.1
./configure --enable-avresample --enable-gpl --enable-version3 --enable-fontconfig --enable-gnutls --enable-ladspa --enable-libbluray --enable-libbs2b --enable-libflite --enable-libfribidi --enable-libgme --enable-libgsm --enable-libmodplug --enable-libwavpack --enable-libwebp --enable-opengl --enable-x11grab --enable-libxvid --enable-libzmq --enable-frei0r --enable-libsoxr --enable-openal --enable-libopencv --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libpulse --enable-libx264 --enable-libx265 --enable-libvorbis --enable-libmp3lame --enable-libopus --enable-libvpx --enable-libspeex --enable-libass --enable-avisynth --enable-libxvid --enable-libvo-aacenc --enable-libvidstab --enable-libfreetype --enable-pthreads --enable-shared --enable-pic
make 
sudo make install
hash -r

#final step just reload ldconfig
sudo ldconfig


#http://stackoverflow.com/questions/18122724/setting-installing-up-opencv-2-4-6-1-on-ubuntu-12-04-02
#//github.com/jayrambhia/Install-OpenCV
