#include "audio_conv.h"

Audio_Service::Audio_Service():audio_stream_index(-1){}


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

int Audio_Service::open_output_file(const char * filename){
  output_file = fopen(filename, "wb");
    if (!output_file) {
      fprintf(stderr, "Could not open %s\n",filename );
      return -1;
    }
   return 0;
}


//initialize one data packet for reading or writing. 
void Audio_Service::init_packet(AVPacket * packet){
  av_init_packet(packet);
  // Set the packet data and size so that it is recognized as being empty. 
  (packet)->data = NULL;
  (packet)->size = 0;
}

// Initialize one audio frame for reading from the input file 
int Audio_Service::init_frame(AVFrame ** frame){
  if (!((*frame) = av_frame_alloc())) {
    fprintf(stderr, "Could not allocate input frame\n");
    return AVERROR(ENOMEM);
  }
  return 0;
}


/*Wrapper around av_dump_format, which displays info about the file via stdcerr*/
void Audio_Service::file_info(const char * filename, AVFormatContext * input_format_context){
  av_dump_format(input_format_context, 0,filename,0);
}


void Audio_Service::decode(AVFormatContext * input_format_context,
                           AVCodecContext * input_codec_context){
  int ret=0;
  int got_frame =0;
  // Packet used for temporary storage. 
  AVPacket  pkt;

  AVFrame * frame=NULL;
  
  // initialize packet, set data to NULL, let the demuxer fill it
  init_packet(&pkt);

  // Initialize temporary storage for one input frame.
  init_frame(&frame);

  //Read frames from the file
  while (av_read_frame(input_format_context, &pkt) >=0){
    //Grab a decoded frame
    do{
     ret=decode_audio_frame(input_format_context,input_codec_context,frame,&pkt,&got_frame,0);
     if(ret <0)
      break;
     pkt.data +=ret;
     pkt.size -= ret;

    }while(pkt.size > 0);
  }
   //flush cached frames
  pkt.data = NULL;
  pkt.size = 0;

  do {
     decode_audio_frame(input_format_context,input_codec_context,
                            frame,&pkt,&got_frame,1);
  }while(got_frame);
/*  int i=0;
  while(!decode_queue.empty()){
    std::cout<<i<<std::endl;
    i++;
    // std::cout<<decode_queue.front()->pkt_pts<<std::endl;
    size_t unpadded_linesize = decode_queue.front()->nb_samples * av_get_bytes_per_sample(static_cast<AVSampleFormat>(decode_queue.front()->format));
   //fwrite(decode_queue.front().extended_data[0], 1, unpadded_linesize,output_file);
   //std::cout<<"Padded Line Size: " <<unpadded_linesize<<std::endl; 
  //d::cout<<"Frame: "<<frame_test->pkt_pts<<std::endl;
   fwrite(decode_queue.front()->extended_data[0], 1, unpadded_linesize,output_file);
    decode_queue.pop();
  }*/
 printf("Demuxing succeeded.\n");
enum AVSampleFormat sfmt = (input_codec_context)->sample_fmt;
        int n_channels = (input_codec_context)->channels;
        const char *fmt;

        if (av_sample_fmt_is_planar(sfmt)) {
            const char *packed = av_get_sample_fmt_name(sfmt);
            printf("Warning: the sample format the decoder produced is planar "
                   "(%s). This example will output the first channel only.\n",
                   packed ? packed : "?");
            sfmt = av_get_packed_sample_fmt(sfmt);
            n_channels = 1;
        }

        if ((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0)
            exit(1);

        printf("Play the output audio file with the command:\n"
               "ffplay -f %s -ac %d -ar %d %s\n",
               fmt, n_channels, (input_codec_context)->sample_rate,
               "ai");

       
 // av_frame_free(&frame);

}

int Audio_Service::decode_audio_frame(AVFormatContext *input_format_context,
                                      AVCodecContext * input_codec_context,
                                      AVFrame * frame,AVPacket * pkt,
                                      int * got_frame,int cached){
  int ret=0;
  int decoded = (pkt)->size;
   // Decode the audio frame stored in the temporary packet.
  // The input audio stream decoder is used to do this.
  // If we are at the end of the file, pass an empty packet to the decoder
  // to flush it.
  if ((ret = avcodec_decode_audio4(input_codec_context, frame,
                                     got_frame,pkt)) < 0) {
      av_log(NULL, AV_LOG_ERROR, "Error decoding audio frame\n");
      av_free_packet(pkt);
      return ret;
  }

   // Some audio decoders decode only part of the packet, and have to be
   // called again with the remainder of the packet data.
   // Sample: fate-suite/lossless-audio/luckynight-partial.shn
   // Also, some decoders might over-read the packet. */
    decoded = FFMIN(ret, (pkt)->size);

    frame -> pts = av_frame_get_best_effort_timestamp(frame);
  if(*got_frame){
   // size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(static_cast<AVSampleFormat>(frame->format));
  //std::cout<<"INside: "<<frame->pkt_pts<<std::endl; 
  //std::cout<<"Inside linesize: "<< unpadded_linesize<<std::endl;  
  

 
   AVFrame * frame2 = av_frame_clone(frame);
   if(frame2 == NULL)
      exit(1);

   // std::cout<<frame2<<std::endl;
    
   // size_t unpadded_linesize2 = frame2->nb_samples * av_get_bytes_per_sample(static_cast<AVSampleFormat>(frame2->format));
  //std::cout<<"INside1: "<<frame2->pkt_pts<<std::endl;
  //std::cout<<"Inside linesize2: "<<unpadded_linesize2<<std::endl;
    
 decode_queue.push(frame2);
 /*rintf("audio_frame%s n:%d nb_samples:%d pts:%s\n",
         cached ? "(cached)" : "",
         audio_frame_count++, frame->nb_samples,
         "monkey");*/
         //av_ts2timestr(frame->pts, &input_codec_context->time_base));
   
  // Write the raw audio data samples of the first plane. This works
    // fine for packed formats (e.g. AV_SAMPLE_FMT_S16). However,
    // most audio decoders output planar audio, which uses a separate
    //  plane of audio samples for each channel (e.g. AV_SAMPLE_FMT_S16P).
    //  In other words, this code will write only the first audio channel
    //  in these cases.
    //  You should use libswresample or libavfilter to convert the frame
    //  to packed data. 
  //  fwrite(frame2->extended_data[0], 1, unpadded_linesize2,output_file);
  
 // av_frame_unref(frame);
  }
 return decoded;

}

 int Audio_Service::get_format_from_sample_fmt(const char **fmt,
                                      enum AVSampleFormat sample_fmt)
{
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
        { AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
        { AV_SAMPLE_FMT_S16, "s16be", "s16le" },
        { AV_SAMPLE_FMT_S32, "s32be", "s32le" },
        { AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
        { AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
    };
    *fmt = NULL;

    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }

    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}

void Audio_Service::printInputStat (AVCodecContext * dec_ctx){
 
  std::cout<<"Sample Format: "<<dec_ctx->sample_fmt<<std::endl;
  std::cout<<"BIT RATE: "<<dec_ctx->bit_rate<<std::endl;
  std::cout<<"SAMPLE RATE: "<<dec_ctx->sample_rate<<std::endl;
  std::cout<<"CHANNEL LAYOUT: "<<dec_ctx->channel_layout<<std::endl;
  std::cout<<"CHANNELs: "<<dec_ctx->channels<<std::endl;
 

}
