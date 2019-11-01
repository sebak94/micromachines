//
// Created by fperezboco on 20/10/19.
//

#include <iostream>
#include "Record.h"
#include "Error.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}
Record::Record(const std::string &filePath,
               const int &fps) {
    //3 because of RGB (3 bytes for each color)
    initScaleAndColorContext();
    allocContext();
    allocBuffer();
    allocFrame();
    allocPacket();
    av_register_all();
    avcodec_register_all();
    setFormat(filePath);
    setCodecH264();
    setFrame();
    openFile(filePath);
}

void Record::initScaleAndColorContext() {
    sContext = sws_getContext(WINDOW_W,
                              WINDOW_H,
                              AV_PIX_FMT_RGB24,
                              WINDOW_W,
                              WINDOW_H,
                              AV_PIX_FMT_YUV420P,
                              0,
                              0,
                              0,
                              0);
}

void Record::allocContext(){
    formatContext = avformat_alloc_context();
    if ( !formatContext )
        throw Error("Alloc format context");
}

void Record::allocBuffer(){
    size = 3*WINDOW_H*WINDOW_W;
    buffer.reserve(3*WINDOW_H*WINDOW_W);
}

void Record::openFile (const std::string & filePath) {
    recordedFile = fopen(filePath.c_str(), "wb");
    if ( !recordedFile )
        throw Error("Opening file");
}

void Record::allocPacket() {
    packet = av_packet_alloc();
    if ( !packet )
        throw Error("Alloc av_packet");
}

void Record::allocFrame() {
    frame = av_frame_alloc();
    if ( !frame )
        throw Error("Alloc av_frame");
}

void Record::setCodecH264() {
    format->video_codec = AV_CODEC_ID_H264;
    AVCodec * codec = avcodec_find_encoder(format->video_codec);
    if ( !codec )
        throw Error("Invalid Codec");
    codecContextInit(codec);
}

void Record::setFormat(const std::string & filePath) {
    format = av_guess_format(nullptr, filePath.c_str(), nullptr);
    if ( !format )
        format = av_guess_format("mpeg", nullptr, nullptr);
    if ( !format )
        throw Error("Invalid output format");
}

void Record::setFrame() {
    frame->format = codecContext->pix_fmt;
    frame->width  = codecContext->width;
    frame->height = codecContext->height;
    av_frame_get_buffer(frame, 0);
    framesQuantity = 0;
}

// Resolution must be multiple of 2
void Record::codecContextInit(AVCodec *codec) {
    codecContext = avcodec_alloc_context3(codec);
    codecContext->width = WINDOW_W;
    codecContext->height = WINDOW_H;
    codecContext->time_base = {1, fps};
    codecContext->framerate = {fps,1};
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 2;
    if (codec->id == AV_CODEC_ID_H264) {
        codecContext->profile = FF_PROFILE_H264_BASELINE;
        av_opt_set(codecContext->priv_data, "preset", "slow", 0);
    }
    avcodec_open2(codecContext, codec, nullptr);
}

void Record::encode(AVCodecContext *context, AVFrame *fr, AVPacket *pckt,
                    FILE *filePath) {
    int result_code = sendFrame(context, fr);  // sends to encoder
    while (result_code >= 0) {
        result_code = avcodec_receive_packet(context, pckt);  // receives from encoder
        if (result_code == AVERROR(EAGAIN) || result_code == AVERROR_EOF)
            return;
        else if (result_code < 0) {
            throw Error("In encoding");
        }
        fwrite(pckt->data, 1, pckt->size, filePath);
        av_packet_unref(pckt);
    }
}

void Record::writeFrame(SDL_Renderer *renderer) {
    // 3 because of RGB (3 bytes for each color)
    int res = SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGB24, buffer.data(), 3*WINDOW_W);
    if (res)
        throw Error("Error Writing %s", SDL_GetError());
    const uint8_t* aux = (const uint8_t*) buffer.data();
    int width = 3*WINDOW_W; // 3 because of RGB (3 bytes for each color)
    sws_scale(sContext, &aux, &width, 0, frame->height, frame->data, frame->linesize);
    encode(codecContext, frame, packet, recordedFile);
    frame->pts = framesQuantity;
    framesQuantity++;
}

int Record::sendFrame(AVCodecContext * context, AVFrame * fr) {
    int sent = avcodec_send_frame(context, fr);
    if (sent < 0)
        throw Error("Sending frame: %d", sent);
    return sent;
}

void Record::close() {
    encode(codecContext, nullptr, packet, recordedFile);
    uint8_t trail[] = { 0, 0, 1, 0xb7 };  // to read it as mpeg
    fwrite(trail, 1, sizeof(trail), recordedFile);
}

/* stops/starts recording */
void Record::changeState() {
    std::cout << "Record button clicked" << std::endl;
    recording = !recording;
}

bool Record::isRecording() {
    return recording;
}

SDL_Texture * Record::getSDLRecordTexture(SDL_Renderer * renderer) {
    return SDL_CreateTexture(renderer,
                             SDL_PIXELFORMAT_RGB24,
                             SDL_TEXTUREACCESS_TARGET,
                             WINDOW_W,
                             WINDOW_H);
}

Record::~Record() {
    avcodec_close(codecContext);
    avcodec_free_context(&codecContext);
    avformat_free_context(formatContext);
    sws_freeContext(sContext);
    fclose(recordedFile);
}
