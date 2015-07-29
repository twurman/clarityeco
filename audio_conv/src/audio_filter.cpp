#include "audio_conv.h"


//TODO: Once everything is working remove the volume filter
int Audio_Service::init_audio_filter(AVCodecContext * dec_ctx, AVCodecContext * enc_ctx, 
                                    AVFilterGraph **graph, AVFilterContext **src,
                                    AVFilterContext **sink){

    AVFilterGraph *filter_graph;
    AVFilterContext *abuffer_ctx;
    AVFilter        *abuffer;
    AVFilterContext *volume_ctx;
    AVFilter        *volume;
    AVFilterContext *aformat_ctx;
    AVFilter        *aformat;
    AVFilterContext *abuffersink_ctx;
    AVFilter        *abuffersink;

    AVDictionary *options_dict = NULL;
    char options_str[1024];
    char ch_layout[64];
    int err;

    // Create a new filtergraph, which will contain all the filters. 
    filter_graph = avfilter_graph_alloc();
    if (!filter_graph) {
        fprintf(stderr, "Unable to create filter graph.\n");
        return AVERROR(ENOMEM);
    }

    // Create the abuffer filter;
    //  it will be used for feeding the data into the graph. 
    abuffer = avfilter_get_by_name("abuffer");
    if (!abuffer) {
        fprintf(stderr, "Could not find the abuffer filter.\n");
        return AVERROR_FILTER_NOT_FOUND;
    }

    abuffer_ctx = avfilter_graph_alloc_filter(filter_graph, abuffer, "src");
    if (!abuffer_ctx) {
        fprintf(stderr, "Could not allocate the abuffer instance.\n");
        return AVERROR(ENOMEM);
    }

    // Set the filter options through the AVOptions API. 
    av_get_channel_layout_string(ch_layout, sizeof(ch_layout), 0, dec_ctx->channel_layout);


    av_opt_set      (abuffer_ctx, "channel_layout", ch_layout, AV_OPT_SEARCH_CHILDREN);

    av_opt_set      (abuffer_ctx, "sample_fmt",     av_get_sample_fmt_name(dec_ctx->sample_fmt), 
                                                    AV_OPT_SEARCH_CHILDREN);

    av_opt_set_q    (abuffer_ctx, "time_base",      (AVRational){ 1, dec_ctx->sample_rate},  
                                                    AV_OPT_SEARCH_CHILDREN);

    av_opt_set_int  (abuffer_ctx, "sample_rate",    dec_ctx->sample_rate,  
                                                    AV_OPT_SEARCH_CHILDREN);
    // Now initialize the filter; we pass NULL options, since we have already
    // set all the options above. 
    err = avfilter_init_str(abuffer_ctx, NULL);
    if (err < 0) {
        fprintf(stderr, "Could not initialize the abuffer filter.\n");
        return err;
    }

    // Create volume filter. 
    volume = avfilter_get_by_name("volume");
    if (!volume) {
        fprintf(stderr, "Could not find the volume filter.\n");
        return AVERROR_FILTER_NOT_FOUND;
    }

    volume_ctx = avfilter_graph_alloc_filter(filter_graph, volume, "volume");
    if (!volume_ctx) {
        fprintf(stderr, "Could not allocate the volume instance.\n");
        return AVERROR(ENOMEM);
    }

    // A different way of passing the options is as key/value pairs in a
    // dictionary. 
    av_dict_set(&options_dict, "volume", AV_STRINGIFY(0.90), 0);
    err = avfilter_init_dict(volume_ctx, &options_dict);
    av_dict_free(&options_dict);
    if (err < 0) {
        fprintf(stderr, "Could not initialize the volume filter.\n");
        return err;
    }

    /* Create the aformat filter;
     * it ensures that the output is of the format we want. */
    aformat = avfilter_get_by_name("aformat");
    if (!aformat) {
        fprintf(stderr, "Could not find the aformat filter.\n");
        return AVERROR_FILTER_NOT_FOUND;
    }

    aformat_ctx = avfilter_graph_alloc_filter(filter_graph, aformat, "aformat");
    if (!aformat_ctx) {
        fprintf(stderr, "Could not allocate the aformat instance.\n");
        return AVERROR(ENOMEM);
    }

    /* A third way of passing the options is in a string of the form
     * key1=value1:key2=value2.... */
    snprintf(options_str, sizeof(options_str),
             "sample_fmts=%s:sample_rates=%d:channel_layouts=0x%" PRIx64,
             av_get_sample_fmt_name(enc_ctx->sample_fmt), enc_ctx->sample_rate,
             (uint64_t)enc_ctx->channel_layout);
    err = avfilter_init_str(aformat_ctx, options_str);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Could not initialize the aformat filter.\n");
        return err;
    }

    /* Finally create the abuffersink filter;
     * it will be used to get the filtered data out of the graph. */
    abuffersink = avfilter_get_by_name("abuffersink");
    if (!abuffersink) {
        fprintf(stderr, "Could not find the abuffersink filter.\n");
        return AVERROR_FILTER_NOT_FOUND;
    }

    abuffersink_ctx = avfilter_graph_alloc_filter(filter_graph, abuffersink, "sink");
    if (!abuffersink_ctx) {
        fprintf(stderr, "Could not allocate the abuffersink instance.\n");
        return AVERROR(ENOMEM);
    }

    /* This filter takes no options. */
    err = avfilter_init_str(abuffersink_ctx, NULL);
    if (err < 0) {
        fprintf(stderr, "Could not initialize the abuffersink instance.\n");
        return err;
    }

    /* Connect the filters;
     * in this simple case the filters just form a linear chain. */
    err = avfilter_link(abuffer_ctx, 0, volume_ctx, 0);
    if (err >= 0)
        err = avfilter_link(volume_ctx, 0, aformat_ctx, 0);
    if (err >= 0)
        err = avfilter_link(aformat_ctx, 0, abuffersink_ctx, 0);
    if (err < 0) {
        fprintf(stderr, "Error connecting filters\n");
        return err;
    }

    /* Configure the graph. */
    err = avfilter_graph_config(filter_graph, NULL);
    if (err < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error configuring the filter graph\n");
        return err;
    }

    *graph = filter_graph;
    *src   = abuffer_ctx;
    *sink  = abuffersink_ctx;

    std::cout<<"Sucessfully filtered"<<std::endl;
    return 0;
}
