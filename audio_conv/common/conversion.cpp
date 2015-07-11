#include "audio_conv.h"

#include <iostream> 
#include <string>
#include <sstream>
#include <fstream>

//WARNING: I CAN GUARNTEE YOU THAT THERE ARE MEMORY LEAKS!!!!!!
//will fix as soon as possible


//This function take the name of the file because of the extension 
//and also its data in the form of a string. 
std::string Audio_Service::conversion(std::string in_filename, std::string audio){
  
  std::ofstream audiofile (in_filename.c_str(), std::ios::binary);
  audiofile.write(audio.c_str(), audio.size());
  audiofile.close();
  
  std::string out_filename = "test.wav";
  int ret;
  int got_pkt;
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
  open_input_file(in_filename.c_str(),&ifmt_ctx,&dec_ctx);

  //Dump input file info to cerr
  file_info(in_filename.c_str(),ifmt_ctx);

  //Open the output file and setup the output format and codec ctx
  open_output_file(out_filename.c_str(), ifmt_ctx, &ofmt_ctx, &enc_ctx);
  //test.open_output_file(argv[2]);
  //Decode the input file
  decode(ifmt_ctx,dec_ctx);

  //Initial Filter Graph 
  init_audio_filter(dec_ctx,enc_ctx,&graph,&src,&sink);

  AVFrame * frame;
  AVPacket pkt;
  init_packet(&pkt);
  // the main filtering loop 
  for (;!decode_queue.empty();) {
    frame = decode_queue.front();
    // Send the frame to the input of the filtergraph. 
    ret = av_buffersrc_add_frame(src, frame);
    if (ret < 0) {
      av_frame_unref(frame);
      fprintf(stderr, "Error submitting the frame to the filtergraph: \n");
      exit(1);
    }
    
    // Get all the filtered output that is available. 
    while ((ret = av_buffersink_get_frame(sink, frame)) >= 0) {
      // now do something with our filtered frame 
      avcodec_encode_audio2(enc_ctx, &pkt,frame,& got_pkt);    
      av_interleaved_write_frame(ofmt_ctx, &pkt);  
      // test.encode_audio_frame(frame,ofmt_ctx,enc_ctx);
      if (ret < 0) {
        fprintf(stderr, "Error processing the filtered frame:");
        exit(1);
      }
            av_frame_unref(frame);
    }
    decode_queue.pop();

    if (ret == AVERROR(EAGAIN)) {
      // Need to feed more frames in. 
      continue;
    } 
    else if (ret == AVERROR_EOF) {
      // Nothing more to do, finish. *
      break;
    } 
    else if (ret < 0) {
      // An error occurred. 
      fprintf(stderr, "Error filtering the data:");
      exit(1);
    }
  }

  std::ifstream converted(out_filename.c_str(), std::ios::binary);
  std::ostringstream ostrm;
  ostrm << converted.rdbuf();
  std::string audio_out(ostrm.str());
  //av_write_trailer(ofmt_ctx);
  //avcodec_close(input_codec_ctx);
  //avformat_close_input(&input_format_ctx);
  //fclose(test.output_file);
  return audio_out;
}
