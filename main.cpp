#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>
#include "Car.h"
#include "Window.h"
#include "error.h"
#include "Button.h"
#include "Record.h"
#include <libswscale/swscale.h>

#define CAR_SPRITE_PATH "../assets/img/car.png"
#define WINDOW_NAME "ffmpeg test"

int main(int argc, char * argv[]) {
    if (argc != 2){
        std::cout << "Call: ./tp <output_video_file>" << std::endl;
        return -1;
    }

    Window game;
    Car car;
    try {
        game.startGUI(WINDOW_NAME);
        car.carTexture = car.loadTexture(CAR_SPRITE_PATH, game.renderer);
        SDL_Rect recordButtonArea = {recordButtonArea.x = WINDOW_W * 7 / 8,
                                     recordButtonArea.y = WINDOW_H / 10,
                                     recordButtonArea.w = 32,
                                     recordButtonArea.h = 32};
        Button recordButton(255, 0, 0, 255, recordButtonArea);
        Record video(argv[1], RECORD_FPS);
        SDL_Texture* videoTexture = video.getSDLRecordTexture(game.renderer);
        bool quit = false;
        SDL_Event event;
        uint32_t startTime = SDL_GetTicks();

        while ( !quit ) {
            while ( SDL_PollEvent( &event ) ) {
                if ( event.type == SDL_QUIT )
                    quit = true;
                recordButton.updateEvent(&event);
                car.move(15, &event);
            }
            //------window render--------
            SDL_SetRenderTarget(game.renderer, nullptr);
            game.fillBackground(255, 255, 255, 0);
            recordButton.draw(game.renderer);
            car.draw(game.renderer);
            SDL_RenderPresent(game.renderer);

            //------record render--------
            //button changes states only when button goes OFF->ON
            if (recordButton.isClicked())
                video.changeState();  // resumes (or pauses) recording
            if (video.isRecording()) {
                SDL_SetRenderTarget(game.renderer, videoTexture);
                game.fillBackground(255, 255, 255, 0);
                recordButton.draw(game.renderer);
                car.draw(game.renderer);
                SDL_RenderPresent(game.renderer);
                uint32_t time = SDL_GetTicks()-startTime;
                if (time > 1000/RECORD_FPS){
                    video.writeFrame(game.renderer);
                    startTime = SDL_GetTicks();
                }
            }
        }
        video.close();
    } catch (Error & e) {
        e.printError();
    }


    return 0;

}
