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
}
//#endif

class Audio_Service {
  public:
  /** Input file Variables */
  int audio_stream_index;             //index of where stream starts
  /** Queue to store decoded raw frames*/
  std::queue<AVFrame * > decode_queue;

  /** Output file Variables */
  FILE * output_file; 
  
  /** Constructors */
  Audio_Service();
  
  /** Functions */
  int open_input_file(const char * filename,
                      AVFormatContext ** input_format_context,
                      AVCodecContext ** input_codec_context );
  int open_output_file(const char * filename);
  void file_info(const char * filename, AVFormatContext * input_format_context);
  void init_packet(AVPacket * packet);
  int init_frame(AVFrame ** frame);
  int decode_audio_frame(AVFormatContext *input_format_context,
                         AVCodecContext * input_codec_context,
                         AVFrame * frame,AVPacket * pkt, 
                         int * got_frame, int cached);
  void decode(AVFormatContext * input_format_context,
              AVCodecContext  * input_codec_context);
  int get_format_from_sample_fmt(const char **fmt,
                                          enum AVSampleFormat sample_fmt);
   int open_output_file(const char *filename,
                        AVFormatContext *ifmt_ctx,
                        AVFormatContext **ofmt_ctx,
                        AVCodecContext **enc_ctx);
  int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt);
int init_output_frame(AVFrame **frame,
                      AVFormatContext *ofmt_ctx,
                      AVCodecContext * enc_ctx);
int encode_audio_frame(AVFrame *frame,
                       AVFormatContext *output_format_context,
                       AVCodecContext *output_codec_context);
int load_encode_and_write(AVFormatContext *output_format_context,
                         AVCodecContext *output_codec_context);

void audio_encode_example(const char *filename);
int select_sample_rate(AVCodec *codec);
int select_channel_layout(AVCodec *codec);

int init_audio_filter(AVCodecContext * dec_ctx, AVCodecContext * enc_ctx,
                                       AVFilterGraph **graph, AVFilterContext **src,
                                       AVFilterContext **sink);

int av_frame_ref1(AVFrame *dst, const AVFrame *src);

void printInputStat(AVCodecContext * dec_ctx);
};



#endif 




