#include "audio_conv.h"

int Audio_Service::open_output_file(const char * filename,
                                    AVFormatContext * ifmt_ctx,
                                    AVFormatContext ** ofmt_ctx,
                                    AVCodecContext ** enc_ctx){
  AVStream * in_stream;
  AVStream * out_stream;
  AVCodecContext * dec_ctx;
  AVCodec * encoder;
  int ret;
  
  //Allocate an AVFormatContext for an output format and guess the container
  if(avformat_alloc_output_context2(ofmt_ctx,NULL,NULL,filename)<0){ 
    if(EINVAL)
      std::cerr<<"Could not guess the format of the filename\n"<<std::endl;
    else
      std::cerr<<"Could not create output context\n"<<std::endl;
    return AVERROR_UNKNOWN;
  }

  //Find a encoder to encode frames
  encoder = avcodec_find_encoder(AV_CODEC_ID_PCM_S16LE);
  if(encoder == NULL){
    std::cerr<<"Could not locate an encoder"<<std::endl;
    return AVERROR_INVALIDDATA;
  }
  
  //Create a new stream based on output file container
  out_stream = avformat_new_stream(*ofmt_ctx, NULL);
  if(out_stream ==NULL){
     std::cerr<<"Could not create a new stream"<<std::endl;
    return AVERROR_UNKNOWN;
  }
  
  //Setting some values...will use filters to change
  in_stream = ifmt_ctx->streams[0];
  dec_ctx   = in_stream -> codec;
  *enc_ctx  = out_stream -> codec; 
  
  //Set the basic encoders parameters 
  (*enc_ctx) -> sample_rate     = 8000;
  (*enc_ctx) -> codec_type      = AVMEDIA_TYPE_AUDIO;
  (*enc_ctx) -> codec_id        = encoder->id;
  (*enc_ctx) -> bit_rate        = dec_ctx -> bit_rate;;
  (*enc_ctx) -> channel_layout  = dec_ctx -> channel_layout;
  (*enc_ctx) -> channels        = av_get_channel_layout_nb_channels((*enc_ctx )-> channel_layout);
  //Just pick a sample_fmt
  (*enc_ctx) -> sample_fmt      = AV_SAMPLE_FMT_S16;//encoder -> sample_fmts[0]; 
  (*enc_ctx) -> time_base       = (AVRational){1, (*enc_ctx)->sample_rate};
  //Open the encoder for the audio stream to use it later
  ret = avcodec_open2(*enc_ctx,encoder,NULL);
  if(ret < 0){
    std::cerr<<"Could not open codec"<<std::endl;
    return ret;
  }
  std::cout<<"Frame Size: "<<(dec_ctx)->channel_layout<<std::endl; 
  if((*ofmt_ctx)->oformat->flags & AVFMT_GLOBALHEADER){
    (*enc_ctx)->flags |= CODEC_FLAG_GLOBAL_HEADER;
  }

  av_dump_format(*ofmt_ctx,0,filename,1);

  if (!((*ofmt_ctx)->oformat->flags & AVFMT_NOFILE)) {
    ret = avio_open(&(*ofmt_ctx)->pb, filename, AVIO_FLAG_WRITE);
    if (ret < 0) {
      av_log(NULL, AV_LOG_ERROR, "Could not open output file '    %s'", filename);
      return ret;
    }
  }

  //init muxer, write output file header 
  ret = avformat_write_header(*ofmt_ctx, NULL);
  if (ret < 0) {
   // av_log(NULL, AV_LOG_ERROR, "Error occurred when opening outp    ut file\n");
    return ret;
  }

  return 0;
}


//Encode one frame worth of audio to the output file.
int Audio_Service::encode_audio_frame(AVFrame *frame,
                              AVFormatContext *ofmt_ctx,
                              AVCodecContext *enc_ctx){
    std::cout<<"Encoder Frame Size: "<<enc_ctx->frame_size<<std::endl;
    std::cout<<"Frame nb Sample Size: "<<frame->nb_samples<<std::endl;
    // Packet used for temporary storage.
    AVPacket output_packet;
    int ret;
    int * got_pkt = NULL;
    init_packet(&output_packet);
    av_packet_rescale_ts(&output_packet,
        ofmt_ctx->streams[0]->codec->time_base,
        ofmt_ctx->streams[0]->time_base);


    std::cout<<"Frame nb Sample Size: "<<frame->nb_samples<<std::endl;
    // Encode the audio frame and store it in the temporary packet.
    // The output audio stream encoder is used to do this.
    
    ret = avcodec_encode_audio2(enc_ctx, &output_packet,frame, got_pkt);
    if (ret  < 0) {
      std::cerr<<"Could not encode frame \n"<<std::endl;
        av_free_packet(&output_packet);
        return ret;
    }
    
    
    // Write one audio frame from the temporary packet to the output file. 
    if (*got_pkt) {
        if ((ret = av_interleaved_write_frame(ofmt_ctx, &output_packet)) < 0) {
          std::cout<< "Could not write frame \n"<<std::endl;
            av_free_packet(&output_packet);
            return ret;
        }

        //av_free_packet(&output_packet);
    }

    return 0;
}
