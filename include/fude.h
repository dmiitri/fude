#ifndef FUDE_H
#define FUDE_H

#include <stdbool.h> // size_t, sizeof(), offsetof()
#include <stdint.h>  // uint32_t
#include <stdbool.h> // bool, true, false

//======================================================================
// Platform Detection
//======================================================================
#if defined(_WIN32) || defined(__WIN32__)
    #define FUDE_PLATFORM_WINDOWS 1
    #define FUDE_PLATFORM_DESKTOP 1
#elif __APPLE__
    #include <TargetConditionals.h>
    #define FUDE_PLATFORM_APPLE 1
    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        #define FUDE_PLATFORM_MOBILE 1
    #elif TARGET_OS_MAC
        #define FUDE_PLATFORM_DESKTOP 1
    #else
        #warn "Unknown apple platform defaulting to desktop environment"
        #define FUDE_PLATFORM_DESKTOP 1
    #endif
    #error "Apple platform is currently not supported"
#elif defined(__ANDROID__)
    #define FUDE_PLATFORM_ANDROID 1
    #define FUDE_PLATFORM_LINUX 1
    #define FUDE_PLATFORM_MOBILE 1
#elif defined(__gnu_linux__) || defined(__linux__)
    #define FUDE_PLATFORM_LINUX 1
    #if !(defined(FUDE_PLATFORM_DESKTOP) || defined(FUDE_PLATFORM_MOBILE))
        #define FUDE_PLATFORM_DESKTOP 1
        #define FUDE_PLATFORM_MOBILE 0
    #endif
#else
    #error "Unsupported platform"
#endif

#ifndef FUDE_PLATFORM_WINDOWS
    #define FUDE_PLATFORM_WINDOWS 0
#endif
#ifndef FUDE_PLATFORM_LINUX
    #define FUDE_PLATFORM_LINUX 0
#endif
#ifndef FUDE_PLATFORM_APPLE
    #define FUDE_PLATFORM_APPLE 0
#endif
#ifndef FUDE_PLATFORM_ANDROID
    #define FUDE_PLATFORM_ANDROID 0
#endif

#if defined(FUDE_PLATFORM_DESKTOP) && FUDE_PLATFORM_DESKTOP
    #ifdef FUDE_PLATFORM_MOBILE
        #undef FUDE_PLATFORM_MOBILE
    #endif
    #define FUDE_PLATFORM_MOBILE 0
#endif
#ifndef FUDE_PLATFORM_DESKTOP
    #define FUDE_PLATFORM_DESKTOP 0
#endif
#ifndef FUDE_PLATFORM_MOBILE
    #define FUDE_PLATFORM_MOBILE 0
#endif

//======================================================================
// Configurations
//======================================================================
#ifdef FUDE_SHAREDLIB
    #ifdef FUDE_EXPORT
        #ifdef _MSC_VER
            #define FAPI __declspec(dllexport)
        #else
            #define FAPI __attribute__((visibility("default")))
        #endif
    #else
        #ifdef _MSC_VER
            #define FAPI __declspec(dllimport)
        #else
            #define FAPI
        #endif
    #endif
#else
    #define FAPI
#endif

#ifdef FUDE_RELEASE
    #define NDEBUG 1
#endif

#define FUDE_EVENT_QUEUE_MAXIMUM_EVENTS 1024 // TODO: minimize this. If this less than 64 will trigger error
#define FUDE_SHADER_MAXIMUM_UNIFORMS 32

//======================================================================
// Types
//======================================================================
typedef enum {
    FUDE_OK = 0,
    FUDE_ERROR,
    FUDE_INVALID_ARGUMENTS_ERROR,
    FUDE_INITIALIZATION_ERROR,
    FUDE_WINDOW_CREATION_ERROR,
} fude_result;

typedef struct { uint8_t r, g, b, a; } fude_color;

typedef struct {
    uint32_t id;
    int width, height;
    int mipmaps, format;
} fude_texture;

typedef struct {
    int location;
    const char* name;
} fude_uniform;

typedef struct {
    uint32_t id;
    fude_uniform uniforms[FUDE_SHADER_MAXIMUM_UNIFORMS];
} fude_shader;

typedef struct {
    int type;
    struct { int width, height; } framebuffer;
    struct { int x, y, width, height; } window;
    struct { int x, y; } cursor;
    struct { double x, y; } scroll;
    struct { int key, scancode, mods; } keyboard;
    struct { int button, mods; } mouse;
    uint32_t codepoint;
} fude_event;

typedef struct {
    fude_event events[FUDE_EVENT_QUEUE_MAXIMUM_EVENTS];
    size_t tail, head;
} fude_event_queue;

typedef struct {
    uint32_t vao;
} fude_renderer2d;

typedef struct GLFWwindow GLFWwindow;

typedef struct {
    GLFWwindow* window;
    fude_event_queue event_queue;
    fude_renderer2d ren2d;
} fude;

typedef struct {
    const char* name;
    uint32_t width, height;
    bool resizable;
} fude_config;

//======================================================================
// Functions
//======================================================================
FAPI fude_result f_init(fude* f, const fude_config* config);
FAPI void f_deinit(fude* f);

// event handling
FAPI void f_poll_events(fude* f);
FAPI bool f_next_event(fude* f, fude_event* event);

// rendering stuff
FAPI void f_present(fude* f);
FAPI void f_flush(fude* f);
FAPI void f_clear(fude* f);

// shader
FAPI fude_result f_load_shader(fude* f, fude_shader* shader, const char* vert_src, const char* frag_src);
FAPI fude_result f_load_shader_from_file(fude* f, fude_shader* shader, const char* vert_path, const char* frag_path);
FAPI void f_unload_shader(fude* f, fude_shader shader);

// texture
FAPI fude_result f_load_texture(fude* f, fude_texture* texture, const void* data, int width, int height, int format);

// 2D
FAPI void f_draw_point(fude* f, fude_color color, int x, int y);
FAPI void f_draw_rectangle(fude* f, fude_color color, int x, int y, uint32_t width, uint32_t height);
FAPI void f_draw_triangle(fude* f, fude_color color, int x1, int y1, int x2, int y2, int x3, int y3);

FAPI void f_draw_texture(void);
FAPI void f_draw_text(fude* f, const char* text, int x, int y);

FAPI void* f_malloc(uint64_t nbytes);
FAPI void f_free(void* ptr);
FAPI void* f_memcpy(void* dst, const void* src, size_t nbytes);
FAPI void* f_memset(void* ptr, int value, size_t nbytes);
FAPI void* f_memzero(void* ptr, size_t nbytes);
FAPI void f_trace_log(int log_level, const char* fmt, ...);
FAPI void f_expect(bool condition, const char* fmt, ...);

//======================================================================
// Enums
//======================================================================
enum fude_event_type {
    FUDE_EVENT_NONE = 0,
    FUDE_EVENT_QUIT,
    FUDE_EVENT_WINDOW_MOVED,
    FUDE_EVENT_WINDOW_RESIZED,
    FUDE_EVENT_WINDOW_GAIN_FOCUS,
    FUDE_EVENT_WINDOW_LOST_FOCUS,
    FUDE_EVENT_FRAMEBUFFER_RESIZED,
    FUDE_EVENT_MOUSE_BUTTON_PRESSED,
    FUDE_EVENT_MOUSE_BUTTON_RELEASED,
    FUDE_EVENT_CURSOR_MOVED,
    FUDE_EVENT_CURSOR_ENTERED,
    FUDE_EVENT_CURSOR_LEFT,
    FUDE_EVENT_SCROLL,
    FUDE_EVENT_KEY_PRESSED,
    FUDE_EVENT_KEY_RELEASED,
    FUDE_EVENT_KEY_REPEATED,
    FUDE_EVENT_CODEPOINT,
};

enum fude_log_level {
    FUDE_LOG_INFO,
    FUDE_LOG_WARNING,
    FUDE_LOG_ERROR,
};

#endif // FUDE_H
