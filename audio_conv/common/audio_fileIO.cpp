#include "audio_conv.h"

//This function opens the input file, gathers information about the
//file and determines the stream index and 
//decoder for the input file


int Audio_Service::open_input_file(const char * filename,
                  AVFormatContext ** input_format_context,
                  AVCodecContext ** input_codec_context){
  int ret;
  AVCodec * input_codec; 
  AVDictionary *opts = NULL;

  /*Open an input stream and read the header. */
  if ((ret = avformat_open_input(input_format_context,filename, 
                                NULL, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
      return ret;
  }

  /* Gather Information about the stream an store it in input_FO */
  if ((ret = avformat_find_stream_info(*input_format_context, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
      return ret;
  }
  // select the audio stream via best stream function
  // returns the non-negative stream number in case of success 
  ret = av_find_best_stream(*input_format_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  //audio_stream = ret;
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find a audio stream in the input file\n");
    return ret;
  }
  
  //Set the codec context 
  *input_codec_context=(*input_format_context)->streams[ret]->codec;
  
  // Find a decoder for the audio stream
  input_codec = avcodec_find_decoder((*input_format_context)->streams[ret]->codec->codec_id);
  if (!input_codec) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find decoder for the input file\n");
    avformat_close_input(input_format_context);
    return AVERROR_EXIT;
  }


  // init the audio decoder
  av_dict_set(&opts, "refcounted_frames", "1", 0); 
  if ((ret = avcodec_open2(*input_codec_context, input_codec, &opts)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open audio decoder\n");
    return ret;
  }
/* Return Successful*/
  
  //Sometime ffmpeg can't figure out the channel layout
  (*input_codec_context)->channel_layout = av_get_default_channel_layout((*input_codec_context)->channels);
  
  return 0;
}



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

int Audio_Service::open_output_file(const char * filename){
  output_file = fopen(filename, "wb");
    if (!output_file) {
      fprintf(stderr, "Could not open %s\n",filename );
      return -1;
    }
   return 0;
}


