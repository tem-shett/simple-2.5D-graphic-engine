#include <iostream>
#include "SDL.h"
#include "framebuffer.h"
#include "texture.h"
#include "gamehandler.h"
#include "draw_objects.h"

int main(int argc, char *args[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("", 100, 100, screenW, screenH, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    FrameBuffer fb(screenW, screenH);
    SDL_Texture *framebuffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                                         SDL_TEXTUREACCESS_STREAMING, fb.w, fb.h);
    if (!framebuffer_texture) {
        std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << std::endl;
        return -1;
    }

    GameHandler gh = create_GameHandler();
    auto start = clock();
    int frames_at_all = 0;
    auto prev_time = clock();
    std::vector<uint32_t> frame(screenW * screenH);
    while (true) {
//        if ((float) (clock() - prev_time) / CLOCKS_PER_SEC * 120 < 1) {
//            Sleep(1.0 / 120.0 - (float) (clock() - prev_time) / CLOCKS_PER_SEC);
//        }
        {
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                if (SDL_QUIT == event.type || (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym)) break;
            }
            if (SDL_KEYDOWN == event.type) {
                if ('a' == event.key.keysym.sym) gh.player.ch_y = -10;
                if ('d' == event.key.keysym.sym) gh.player.ch_y = 10;
                if ('w' == event.key.keysym.sym) gh.player.ch_x = 10;
                if ('s' == event.key.keysym.sym) gh.player.ch_x = -10;
                if (SDLK_LEFT == event.key.keysym.sym) gh.player.ch_ang = -2;
                if (SDLK_RIGHT == event.key.keysym.sym) gh.player.ch_ang = 2;
                if (SDLK_UP == event.key.keysym.sym) gh.player.ch_h = 10;
                if (SDLK_DOWN == event.key.keysym.sym) gh.player.ch_h = -10;
            }
            if (SDL_KEYUP == event.type) {
                if ('a' == event.key.keysym.sym || 'd' == event.key.keysym.sym) gh.player.ch_y = 0;
                if ('w' == event.key.keysym.sym || 's' == event.key.keysym.sym) gh.player.ch_x = 0;
                if (SDLK_LEFT == event.key.keysym.sym || SDLK_RIGHT == event.key.keysym.sym) gh.player.ch_ang = 0;
                if (SDLK_UP == event.key.keysym.sym || SDLK_DOWN == event.key.keysym.sym) gh.player.ch_h = 0;
            }
        }
        gh.move((float) (clock() - prev_time) / CLOCKS_PER_SEC);
        prev_time = clock();
        draw_frame(gh, fb);
        frames_at_all++;
        if ((float)(clock() - start) / CLOCKS_PER_SEC > 1) {
            std::cout << frames_at_all / ((float)(clock() - start) / CLOCKS_PER_SEC) << std::endl;
            frames_at_all = 0;
            start = clock();
        }
        {
//            for (int i = 0; i < screenW * screenH; i++) {
//                frame[i] = fb.frame[i];
//            }
            SDL_UpdateTexture(framebuffer_texture, nullptr, reinterpret_cast<void *>(fb.frame.data()), fb.w * 4);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, framebuffer_texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }
    }
    std::cout << gh.player.pos.x << ' ' << gh.player.pos.y << ' ' << gh.player.eye_angle << ' ' << gh.player.h << '\n';
    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
//    std::cout << frames_at_all / ((float)(clock() - start) / CLOCKS_PER_SEC) << std::endl;
    return 0;
}
