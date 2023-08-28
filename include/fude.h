#ifndef FUDE_H
#define FUDE_H

#include <stdbool.h>
#include <stdint.h>

#define FUDE_MAXIMUM_VERTICES 1024
#define FUDE_MAXIMUM_INDICES (FUDE_MAXIMUM_VERTICES / 4)
#define FUDE_MAXIMUM_TEXTURES 8
#define FUDE_EVENT_QUEUE_CAPACITY 1024

typedef enum {
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
} Fude_Event_Code;

typedef struct Fude_Event {
    Fude_Event_Code code;
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
} Fude_Event;

const char* fude_failure_reason(void);

bool fude_init(void);
void fude_deinit(void);

typedef struct _Fude_Window Fude_Window;

Fude_Window* fude_create_window(const char* title, uint32_t width, uint32_t height, bool resizable);
void fude_destroy_window(Fude_Window* window);
void fude_poll_input_events(void);
bool fude_get_next_input_event(Fude_Event* event);
bool fude_window_should_close(Fude_Window* window);

typedef struct _Fude_Renderer Fude_Renderer;

Fude_Renderer* fude_create_renderer(Fude_Window* window);
void fude_destroy_renderer(Fude_Renderer* ren);

void fude_present_renderer(Fude_Renderer* ren);
void fude_clear_renderer(Fude_Renderer* ren);
void fude_set_draw_color(Fude_Renderer* ren, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void fude_draw_rectangle(Fude_Renderer* ren, int32_t x, int32_t y, int32_t w, int32_t h);
void fude_draw_triangle(Fude_Renderer* ren, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);

typedef struct _Fude_Texture Fude_Texture;

Fude_Texture* fude_create_texture(Fude_Renderer* ren, uint32_t w, uint32_t h, const uint8_t* data, int comp);
void fude_destroy_texture(Fude_Texture* tex);

void fude_update_texture(Fude_Texture* tex, uint32_t w, uint32_t h, const uint8_t* data, int comp);
void fude_draw_texture(Fude_Renderer* ren, Fude_Texture* tex, 
int sx, int sy, uint32_t sw, uint32_t sh, 
int dx, int dy, uint32_t dw, uint32_t dh);

#endif // FUDE_H
