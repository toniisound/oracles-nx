#include "platform/window.h"
#include "core/gb.h"
#include <stdio.h>

#define MAX_DEFAULT_SCALE 5

// The largest whole scale up to MAX_DEFAULT_SCALE whose window fits the primary display's usable
// area (in points), leaving room for the title bar.
static int default_scale(void) {
  SDL_Rect r;
  if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &r)) return 4;
  int s = SDL_min((r.w - 32) / FB_W, (r.h - 64) / FB_H);
  return SDL_clamp(s, 1, MAX_DEFAULT_SCALE);
}

static bool saved_size(const char *size_path, int *w, int *h) {
  if (!size_path) return false;
  FILE *f = fopen(size_path, "r");
  if (!f) return false;
  bool ok = fscanf(f, "%d %d", w, h) == 2 && *w >= FB_W && *h >= FB_H && *w <= 16384 && *h <= 16384;
  fclose(f);
  return ok;
}

bool oracles_open_window(const char *title, const char *size_path, SDL_Window **win, SDL_Renderer **ren, SDL_Texture **tex) {
#ifdef __SWITCH__
  // one full-screen window; SDL switches it between 720p handheld and 1080p docked
  (void)size_path; (void)default_scale; (void)saved_size;
  // The Switch's SDL only makes windows on EGL, which it loads for SDL_WINDOW_OPENGL windows alone
  // ("EGL not initialized" otherwise); the renderer is GLES2 on top of it.
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");
  if (!SDL_CreateWindowAndRenderer(title, 1280, 720, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL, win, ren)) return false;
#else
  int w, h;
  if (!saved_size(size_path, &w, &h)) { int s = default_scale(); w = FB_W * s; h = FB_H * s; }
  if (!SDL_CreateWindowAndRenderer(title, w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, win, ren)) return false;
#endif
  SDL_SetWindowMinimumSize(*win, FB_W, FB_H);
  SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
  *tex = SDL_CreateTexture(*ren, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, FB_W, FB_H);
  if (!*tex) return false;
  SDL_SetTextureScaleMode(*tex, SDL_SCALEMODE_NEAREST);
  return true;
}

bool oracles_window_key(SDL_Window *win, const SDL_KeyboardEvent *key) {
  bool toggle = key->scancode == SDL_SCANCODE_F11 || ((key->mod & SDL_KMOD_GUI) && key->scancode == SDL_SCANCODE_F);
  if (!toggle) return false;
  if (!key->repeat) SDL_SetWindowFullscreen(win, !(SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN));
  return true;
}

void oracles_close_window(SDL_Window *win, const char *size_path) {
#ifdef __SWITCH__
  return;
#endif
  if (!size_path || (SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN)) return;
  int w, h;
  if (!SDL_GetWindowSize(win, &w, &h)) return;
  FILE *f = fopen(size_path, "w");
  if (!f) return;
  fprintf(f, "%d %d\n", w, h);
  fclose(f);
}
