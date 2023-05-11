#ifndef FUDE_H
#define FUDE_H

#include <stdbool.h>
#include <stdint.h>

#define FUDE_MAXIMUM_VERTICES 1024
#define FUDE_MAXIMUM_INDICES (FUDE_MAXIMUM_VERTICES / 4)
#define FUDE_MAXIMUM_TEXTURES 8
#define FUDE_EVENT_QUEUE_CAPACITY 1024

typedef enum FudeEventType {
    FUDE_EVENT_NONE,
    FUDE_EVENT_WINDOW_MOVED,
    FUDE_EVENT_WINDOW_RESIZED,
    FUDE_EVENT_WINDOW_CLOSED,
    FUDE_EVENT_WINDOW_REFRESH,
    FUDE_EVENT_WINDOW_FOCUSED,
    FUDE_EVENT_WINDOW_DEFOCUSED,
    FUDE_EVENT_WINDOW_ICONIFIED,
    FUDE_EVENT_WINDOW_UNICONIFIED,
    FUDE_EVENT_FRAMEBUFFER_RESIZED,
    FUDE_EVENT_BUTTON_PRESSED,
    FUDE_EVENT_BUTTON_RELEASED,
    FUDE_EVENT_CURSOR_MOVED,
    FUDE_EVENT_CURSOR_ENTERED,
    FUDE_EVENT_CURSOR_LEFT,
    FUDE_EVENT_SCROLLED,
    FUDE_EVENT_KEY_PRESSED,
    FUDE_EVENT_KEY_REPEATED,
    FUDE_EVENT_KEY_RELEASED,
    FUDE_EVENT_CODEPOINT_INPUT,
    FUDE_EVENT_MONITOR_CONNECTED,
    FUDE_EVENT_MONITOR_DISCONNECTED,
    FUDE_EVENT_FILE_DROPPED,
    FUDE_EVENT_JOYSTICK_CONNECTED,
    FUDE_EVENT_JOYSTICK_DISCONNECTED,
    FUDE_EVENT_WINDOW_MAXIMIZED,
    FUDE_EVENT_WINDOW_UNMAXIMIZED,
    FUDE_EVENT_SCALE_CHANGED,
} FudeEventType;

typedef struct FudeEvent {
    FudeEventType type;
    union {
        struct { int x, y;  } pos;
        struct { int width, height; } size;
        struct { double x, y; } scroll;
        struct { int key, scancode, mods; } keyboard;
        struct { int button, mods; } mouse;
        unsigned int codepoint;
        struct { char** paths; int count; } file;
        struct { float x, y; } scale;
    };
} FudeEvent;

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