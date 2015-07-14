#include "audio_conv.h"

//Encode one frame worth of audio to the output file.
int Audio_Service::encode_audio_frame(AVFrame *frame,
                              AVFormatContext *ofmt_ctx,
                              AVCodecContext *enc_ctx){
    // Packet used for temporary storage.
    AVPacket output_packet;
    int ret;
    int got_pkt;
    init_packet(&output_packet);
    av_packet_rescale_ts(&output_packet,
        ofmt_ctx->streams[0]->codec->time_base,
        ofmt_ctx->streams[0]->time_base);


    // Encode the audio frame and store it in the temporary packet.
    // The output audio stream encoder is used to do this.
    ret = avcodec_encode_audio2(enc_ctx, &output_packet,frame, &got_pkt);
    if (ret  < 0) {
      std::cerr<<"Could not encode frame \n"<<std::endl;
        av_free_packet(&output_packet);
        return ret;
    }
    
    // Write one audio frame from the temporary packet to the output file. 
    if (got_pkt) {
      if ((ret = av_interleaved_write_frame(ofmt_ctx, &output_packet)) < 0) {
        std::cerr<< "Could not write frame \n"<<std::endl;
        av_free_packet(&output_packet);
        return ret;
      }
    }
    av_free_packet(&output_packet);
    
    return 0;
}
