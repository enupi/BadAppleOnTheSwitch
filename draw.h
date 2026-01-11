#include <switch.h>

#include <SDL2/SDL.h>

struct Graphics {
	SDL_Window* window;
	SDL_Surface* window_surface;
};

typedef struct Graphics Graphics;

Graphics* init();
void deinit(struct Graphics* g);
void update(struct Graphics* g);
void sleep(int s);
void background(struct Graphics* gr, int r, int g, int b);
void putAPixel(struct Graphics* gr, int xi, int yi, int x, int y, int dot_size, int r, int g, int b);
void drawString(struct Graphics* gr, char* string, int xi, int yi, int dot_size);
void drawColorString(struct Graphics* gr, int xi, int yi, int dot_size, char * string, int r, int g, int b);
void playVideo(struct Graphics* gr, PadState* pad, int xi, int yi);
char* fontLookup(char c);