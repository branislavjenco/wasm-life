// Author: Mario Talevski
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm> 

using namespace std;

void update_state(Uint32 * grid, Uint32 * compareGrid, unsigned int w, unsigned int h);
Uint32 idx(unsigned int w, int i, int j);

Uint32 black = 0x00000000;
Uint32 white = 0xffffffff;
Uint32 red = 0x00ff0000;

int main() {
  int x, y, n;
  unsigned int w = 100;
  unsigned int h = 100;
  unsigned int size = w * h;
  Uint32 * pixels = new Uint32[size];
  Uint32 * pixels_temp = new Uint32[size];
  std::fill_n(pixels, size, black); 

  string nc;
  string start;
  string filename = "glider_gun.txt";
  while (true) {

    // cout << "Enter name of file to read from: " << endl;
    // cin >> filename;

    ifstream readfile(filename);
    if (readfile.is_open()) {
      string fileline, xx, yy;

      while (getline(readfile, fileline)) {
        stringstream ss(fileline);

        getline(ss, xx, ' ');
        getline(ss, yy, ' ');

        x = stoi(xx);
        y = stoi(yy);

        pixels[idx(w, x, y)] = white;
      }
      break;
    } else {
      cout << "No such file, try again." << endl;
    }
  }
  
  cout << "File loaded." << endl;
  bool quit = false;
  SDL_Event event;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 0);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, w, h);
 
  SDL_Rect SrcR = {0, 0, 100, 100};
  SDL_Rect DestR = {0, 0, 1000, 1000};

  while (!quit) {
    unsigned char* lockedPixels = nullptr;
    int pitch = 100 * sizeof(Uint32);
    SDL_LockTexture(texture, NULL, reinterpret_cast< void** >( &lockedPixels ), &pitch );
    update_state(pixels, pixels_temp, w, h);
    memcpy(lockedPixels, pixels, w * h);
    SDL_UnlockTexture(texture);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
    
  
    usleep(100000);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();

  delete[] pixels;
  delete[] pixels_temp;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
}

Uint32 idx(unsigned int w, int i, int j) {
   return w * i + j;
}

void update_state(Uint32 * grid, Uint32 * compareGrid, unsigned int w, unsigned int h) {
  memcpy(compareGrid, grid, w * h);

  for (int a = 1; a < h; a++) {
    for (int b = 1; b < w; b++) {
      int alive = 0;
      for (int c = -1; c < 2; c++) {
        for (int d = -1; d < 2; d++) {
          if (!(c == 0 && d == 0)) {
            if (compareGrid[idx(w, a + c, b + d)]) {
              ++alive;
            }
          }
        }
      }
        
      if (alive < 2) {
        grid[idx(w, a, b)] = black;
      } else if (alive == 3) {
        grid[idx(w, a, b)] = white;
      } else if (alive > 3) {
        grid[idx(w, a, b)] = black;
      }
    }
  }
}
