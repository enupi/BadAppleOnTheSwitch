#include "draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// エラー時に呼び出す「死なないで待つ関数」
void waitError(struct Graphics* g, int r, int g_val, int b) {
    // 指定した色で塗りつぶす
    SDL_FillRect(g->window_surface, NULL, SDL_MapRGBA(g->window_surface->format, r, g_val, b, 255));
    SDL_UpdateWindowSurface(g->window);
    
    // ★returnせずに無限ループで止める
    while(appletMainLoop()) {
        // とりあえず画面を出したままにする
        SDL_Delay(100); 
    }
}

int main(int argc, char *argv[])
{
	// initialize and get graphics context
	Graphics* g = init();// 画面のセットアップ

	// Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	 
	// Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);
	
	romfsInit();

	// Start SDL with audio support
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		//初期化失敗
		SDL_FillRect(g->window_surface, NULL, SDL_MapRGBA(g->window_surface->format, 0, 255, 0, 255));
        SDL_UpdateWindowSurface(g->window);
        SDL_Delay(5000);
		return -1;
	}
	
	// Load support for the MP3 format
    Mix_Init(MIX_INIT_MP3);

    // open 44.1KHz, signed 16bit, system byte order,
    //  stereo audio, using 4096 byte chunks
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0){
		//音声デバイスが開けなかった
		// SDL_FillRect(g->window_surface, NULL, SDL_MapRGBA(g->window_surface->format, 0, 0, 255, 255));
        // SDL_UpdateWindowSurface(g->window);
        // SDL_Delay(5000);
		// return -1;

		waitError(g, 0, 0, 255);
		goto cleanup;
	}

    // Load sound file to use
    // Sound from https://freesound.org/people/jens.enk/sounds/434610/
    Mix_Music *audio = Mix_LoadMUS("romfs:/Bad_Apple.mp3");
	if (audio == NULL){
		// SDL_FillRect(g->window_surface, NULL, SDL_MapRGBA(g->window_surface->format, 255, 0, 0, 255));
        // SDL_UpdateWindowSurface(g->window);
        // SDL_Delay(5000);

		waitError(g, 255, 0, 0);
		goto cleanup;

	}

	// 少女祈禱中
	background(g, 0x00, 0x00, 0x00);
	drawColorString(g, 160, 350, 4, "Press A to Play Bad Apple!!", 0xFF, 0xFF, 0xFF);
	update(g);

	while(appletMainLoop()){
		padUpdate(&pad);

		u64 kDown = padGetButtonsDown(&pad);

		if (kDown & HidNpadButton_A){
			background(g, 0x00, 0x00, 0x00);
			update(g);
			break;
		}
	}


cleanup:
	if (audio){
		Mix_PlayMusic(audio, 1); //Play the audio file
	}
	//consoleUpdate(NULL);
	
	playVideo(g, &pad, 160, 0);

	// wait 10s
	SDL_Delay(5);

	// Free the loaded sound
    if (audio) Mix_FreeMusic(audio);
	Mix_CloseAudio();

	// clean up graphics
	deinit(g);

	// Deinitialize and clean up resources used by the console (important!)
    romfsExit();
    //consoleExit(NULL);
	return 0;
}
