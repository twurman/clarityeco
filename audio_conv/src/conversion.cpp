#include "audio_conv.h"

#include <iostream> 
#include <string>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>


std::string Audio_Service::conversion(const std::string audio, const audioSpecs& specs){
  
  //Preparing the input file for transcoding
  struct timeval tp;
  gettimeofday(&tp,NULL);
  long int timestamp = tp.tv_sec * 100 + tp.tv_usec / 1000;
  std::string audio_path = "input-"+ std::to_string(timestamp);
  std::ofstream audiofile(audio_path.c_str(), std::ios::binary);
  audiofile.write(audio.c_str(), audio.size());
  audiofile.close();

  //Name of the ouptut audio file
  std::string audio_outfile = "output-" + std::to_string(timestamp)+
   "."+specs.output_format;

  int ret;
  int got_pkt;
  int got_frame=0;
  int finished=0;
  
  AVFrame *frame;
  AVFrame *filteredFrame;
  AVPacket pkt;
  //Initialize all codecs
  av_register_all();

  //Initial the filters
  avfilter_register_all();

  //Input File info 
  AVFormatContext * ifmt_ctx=NULL;
  AVCodecContext *dec_ctx=NULL;

  //Output File info
  AVFormatContext * ofmt_ctx=NULL;
  AVCodecContext *enc_ctx=NULL;
  
  //Filter Graph 
  AVFilterGraph  *  graph;

  //Entry point for graph
  AVFilterContext * src;

  //Exit point for graph
   AVFilterContext * sink;

  //Open the input file for demuxing and decoding
  open_input_file(audio_path.c_str(),&ifmt_ctx,&dec_ctx);

  //Output info about the input file
  //file_info(audio_path.c_str(),ifmt_ctx);
  
  //Output info about the specs struct
  //printaudioSpecs(dec_ctx,specs);
  
  //Open the output file and setup the output format and codec ctx
  open_output_file(audio_outfile.c_str(), ifmt_ctx, &ofmt_ctx, &enc_ctx, &specs);
  //open_output_file(audio_outfile);

  //Initial Filter Graph 
  init_audio_filter(dec_ctx,enc_ctx,&graph,&src,&sink);

  //Initialize packets and frame
  init_packet(&pkt);
  init_frame(&frame);
  //Main loop 
  while(!finished){
    //Grab one audio frame
    if(decode_audio_frame(ifmt_ctx,dec_ctx,&frame,&got_frame,&finished)<0){
      std::cerr << "Error Decoding Audio" <<std::endl;
    }
      
    
     // If we are at the end of the file and there are no more samples
     // in the decoder which are delayed, we are actually finished.
     // This must not be treated as an error.
    if (finished && !got_frame) {
      break;
    }

    if(got_frame){
      //Add the frame to the filter
      ret=av_buffersrc_add_frame(src,frame);
      if (ret < 0) {
        av_frame_unref(frame);
        std::cerr<< "Error submitting the frame to the filtergraph"<<std::endl;;
        exit(1);
      }
      
      //Pull the filtered frame from the graph
      for(;;){
        init_frame(&filteredFrame);
        ret = av_buffersink_get_frame(sink,filteredFrame);
        if((ret == AVERROR(EAGAIN))|| (ret == AVERROR_EOF)){
          break;
        }
        if(ret<0){
          std::cerr<<"Error while getting filtered frames from filtergraph"<<std::endl;;
          return "error occured with filtering";
        }  

        //Send the filtered frame to be encoded and written to the file
        ret=encode_audio_frame(filteredFrame, ofmt_ctx, enc_ctx);
        if(ret < 0){
          std::cerr << "Error while encoding frame" << std::endl;
          av_frame_unref(frame);
          return "Error occured with encoding";
        }
        
        av_frame_unref(frame);

      }
    }
  }

  //Flushing the encoder
  std::cout<<"Flushing the encoder"<<std::endl;
  got_pkt = 1;
  while(got_pkt){
    AVPacket encodedP;
    init_packet(&encodedP);
    ret=avcodec_encode_audio2(enc_ctx, &encodedP, NULL,&got_pkt);

    if(got_pkt){
      if ((ret = av_interleaved_write_frame(ofmt_ctx, &encodedP)) < 0) {
        std::cerr<< "Could not write frame \n"<<std::endl;
        av_free_packet(&encodedP);
        return "error";
      }
    }
    av_free_packet(&encodedP);
  }

  //Final touches on the file
  av_write_trailer(ofmt_ctx);
  
  //Cleanup
  avformat_free_context(ofmt_ctx);
  avcodec_close(dec_ctx);
  avformat_close_input(&ifmt_ctx);
  //fclose(test.output_file);
  
  //Preparing the file for transporting back
  std::ifstream fin(audio_outfile.c_str(), std::ios::binary);
  std::ostringstream ostrm;
  ostrm <<fin.rdbuf();
  std::string audio_out(ostrm.str());
  std::cout<<"Now returning converted audio"<<std::endl;
  return audio_out;
}
