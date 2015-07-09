#include "audio_conv.h"

#include <iostream> 
#include <string>

int main(int argc , char ** argv){
  int ret;
  int got_pkt;
  //Initialize all codecs
  av_register_all();

  //Initial the filters
  avfilter_register_all();

  //FFMPEG Wapper in a class
  Audio_Service test;
  
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
  test.open_input_file(argv[1],&ifmt_ctx,&dec_ctx);

  //Dump input file info to cerr
  test.file_info(argv[1],ifmt_ctx);

  //Open the output file and setup the output format and codec ctx
  test.open_output_file(argv[2], ifmt_ctx, &ofmt_ctx, &enc_ctx);
  //test.open_output_file(argv[2]);
  //Decode the input file
  test.decode(ifmt_ctx,dec_ctx);

  //Initial Filter Graph 
  test.init_audio_filter(dec_ctx,enc_ctx,&graph,&src,&sink);

 /* AVFrame * testo;
  int i=0;
  while(!test.decode_queue.empty()){
    std::cout<<i<<std::endl;
    i++;
    testo=test.decode_queue.front();
    // std::cout<<decode_queue.front()->pkt_pts<<std::endl;
   // size_t unpadded_linesize = test.decode_queue.front()->nb_samples * av_get_bytes_per_sample(
    //    static_cast<AVSampleFormat>(test.decode_queue.front()->format));
     size_t unpadded_linesize = testo->nb_samples * av_get_bytes_per_sample(
        static_cast<AVSampleFormat>(testo->format));
    //fwrite(decode_queue.front().extended_data[0], 1, unpadded_linesize,output_file);
    //std::cout<<"Padded Line Size: " <<unpadded_linesize<<std::endl; 
    //std::cout<<"Frame: "<<frame_test->pkt_pts<<std::endl;
    fwrite(testo->extended_data[0], 1, unpadded_linesize,test.output_file);
    //fwrite(test.decode_queue.front()->extended_data[0], 1, unpadded_linesize,test.output_file);
    test.decode_queue.pop();
  } 
*/
  
  AVFrame * frame;
  AVPacket pkt;
  test.init_packet(&pkt);
    // the main filtering loop 
    for (int i=0;!test.decode_queue.empty();++i) {
        frame = test.decode_queue.front();
       // test.encode_audio_frame(test.decode_queue.front(),ofmt_ctx,enc_ctx);
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
          std::cout<<i<<std::endl;
         avcodec_encode_audio2(enc_ctx, &pkt,frame,& got_pkt);    
         av_interleaved_write_frame(ofmt_ctx, &pkt);  
         // test.encode_audio_frame(frame,ofmt_ctx,enc_ctx);
            if (ret < 0) {
                fprintf(stderr, "Error processing the filtered frame:");
                exit(1);
            }
            av_frame_unref(frame);
        }
        test.decode_queue.pop();

        if (ret == AVERROR(EAGAIN)) {
            // Need to feed more frames in. 
            continue;
        } else if (ret == AVERROR_EOF) {
            // Nothing more to do, finish. *
            break;
        } else if (ret < 0) {
            // An error occurred. 
            fprintf(stderr, "Error filtering the data:");
            exit(1);
        }
    }
av_write_trailer(ofmt_ctx);
 // avcodec_close(input_codec_ctx);
  //avformat_close_input(&input_format_ctx);
  //fclose(test.output_file);
  return 0;
}
