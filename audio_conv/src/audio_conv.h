#ifndef _AUDIO_CONV
#define _AUDIO_CONV

#include <math.h>
#include <string>
#include <queue>
#include <deque>
#include <iostream>

//#ifdef __cplusplus
extern "C" {
#ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
#endif
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/avassert.h>
#include <libavutil/avstring.h>
#include <libavutil/frame.h>
#include <libavutil/opt.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavfilter/avfilter.h> 
#include <libavfilter/buffersrc.h> 
#include <libavfilter/buffersink.h> 
#include <libavutil/dict.h>
}
//#endif

struct audioSpecs{
  int sample_rate;
  int bit_rate;
  int channels;
  std::string output_format;
};

class Audio_Service {
  public:
  // Input file Variables 
  // Index of where the stream starts
  int audio_stream_index;            

  // Output file Variables 
  FILE * output_file; 
  
  // Constructors
  Audio_Service();
  
  // Functions 

  //FILE IO
  int open_input_file(const char * filename,
                      AVFormatContext ** input_format_context,
                      AVCodecContext ** input_codec_context );
  int open_output_file(const char * filename);
  int open_output_file(const char *filename,
                        AVFormatContext *ifmt_ctx,
                        AVFormatContext **ofmt_ctx,
                        AVCodecContext **enc_ctx,
                        const audioSpecs * specs);
  void file_info(const char * filename, AVFormatContext * input_format_context);
  
  //Initialization Functions
  void init_packet(AVPacket * packet);
  int init_frame(AVFrame ** frame);
  int init_output_frame(AVFrame **frame,
                        AVFormatContext *ofmt_ctx,
                        AVCodecContext * enc_ctx);
  int init_audio_filter(AVCodecContext * dec_ctx, AVCodecContext * enc_ctx,
                        AVFilterGraph **graph, AVFilterContext **src,
                        AVFilterContext **sink);
  //Decoding
  int decode_audio_frame(AVFormatContext *input_format_context,
                         AVCodecContext * input_codec_context,
                         AVFrame ** frame, int * got_frame, int *finished);

  void decode(AVFormatContext * input_format_context,
              AVCodecContext  * input_codec_context);
  
  
  //Encoding
  
  int encode_audio_frame(AVFrame *frame, AVFormatContext *output_format_context,
                         AVCodecContext *output_codec_context);

  //Converting
  std::string conversion(const std::string audio, const audioSpecs& specs);

  //Misc
  int get_format_from_sample_fmt(const char **fmt,
                                          enum AVSampleFormat sample_fmt);
  int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt);
  int select_sample_rate(AVCodec *codec);
  int select_channel_layout(AVCodec *codec);
  void printInputStat(AVCodecContext * dec_ctx);
  void printaudioSpecs( const AVCodecContext * dec, const audioSpecs specs);
};


  void init_audioSpecs(audioSpecs * specs, int sample_rate,int bit_rate, int channels,std::string output_format); 

#endif 




