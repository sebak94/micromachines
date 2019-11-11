//
// Created by fperezboco on 20/10/19.
//

#ifndef TP_RECORD_H
#define TP_RECORD_H

#include <vector>
#include "../../client/include/sdl/SdlWindow.h"
//#include "../../client/include/Drawer.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#define RECORD_FPS 60

class Record {
private:
    int size{};
    std::vector<char> lastFrame;
    FILE* recordedFile{};
    AVStream* videoStream{};
    AVPacket* packet{};
    AVFrame* frame{};
    int framesQuantity{};
    AVOutputFormat* format{};
    AVCodecContext* codecContext{};
    AVFormatContext * formatContext{};
    SwsContext * sContext{};
    bool recording = false;
    int fps = 30;
    int width;
    int height;

public:
    explicit Record(const std::string &filePath, const int &fps, int width, int height);
    ~Record();
    void codecContextInit(AVCodec *codec);
    void setFrame();
    void setFormat(const std::string & filePath);
    void setCodecH264();
    void allocFrame();
    void allocPacket();
    void openFile(const std::string & filePath);
    void allocBuffer();
    void allocContext();
    void encode(AVCodecContext *context, AVFrame *fr, AVPacket *pckt, FILE *filePath);
    static int sendFrame(AVCodecContext *context, AVFrame *fr);
    void writeFrame();
    void close();
    void changeState();
    bool isRecording();

    void initScaleAndColorContext();

    SDL_Texture *getSDLRecordTexture(SDL_Renderer *renderer);

    void setLastFrame(std::vector<char> frame);
};


#endif //TP_RECORD_H
