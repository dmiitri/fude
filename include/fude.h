#ifndef FUDE_H
#define FUDE_H

#include <stdbool.h>
#include <stdint.h>

#define FUDE_MAXIMUM_VERTICES 1024
#define FUDE_MAXIMUM_INDICES (FUDE_MAXIMUM_VERTICES / 4)
#define FUDE_MAXIMUM_TEXTURES 8

const char* fude_failure_reason(void);

bool fude_init(void);
void fude_deinit(void);

typedef struct _FudeWindow FudeWindow;

FudeWindow* fude_create_window(const char* title, uint32_t width, uint32_t height, bool resizable);
void fude_destroy_window(FudeWindow* window);
void fude_poll_input_events(void);
bool fude_window_should_close(FudeWindow* window);

typedef struct _FudeRenderer FudeRenderer;

FudeRenderer* fude_create_renderer(FudeWindow* window);
void fude_destroy_renderer(FudeRenderer* ren);

void fude_present_renderer(FudeRenderer* ren);
void fude_clear_renderer(FudeRenderer* ren);
void fude_set_draw_color(FudeRenderer* ren, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void fude_draw_rectangle(FudeRenderer* ren, int32_t x, int32_t y, int32_t w, int32_t h);
void fude_draw_triangle(FudeRenderer* ren, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);

typedef struct _FudeTexture FudeTexture;

FudeTexture* fude_create_texture(FudeRenderer* ren, uint32_t w, uint32_t h, const uint8_t* data, int comp);
void fude_destroy_texture(FudeTexture* tex);

void fude_update_texture(FudeTexture* tex, uint32_t w, uint32_t h, const uint8_t* data, int comp);
void fude_draw_texture(FudeRenderer* ren, FudeTexture* tex, 
int sx, int sy, uint32_t sw, uint32_t sh, 
int dx, int dy, uint32_t dw, uint32_t dh);

#endif // FUDE_H