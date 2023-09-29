#ifndef FUDE_H
#define FUDE_H

#include <stddef.h> // size_t, sizeof(), offsetof()
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

#define FUDE_POSITION_VERTEX_ATRIBUTE_NAME "a_position"
#define FUDE_COLOR_VERTEX_ATRIBUTE_NAME "a_color"
#define FUDE_TEX_COORDS_VERTEX_ATRIBUTE_NAME "a_tex_coords"
#define FUDE_TEX_INDEX_VERTEX_ATRIBUTE_NAME "a_color"
#define FUDE_OBJECT_ID_VERTEX_ATRIBUTE_NAME "a_object_id"

#define FUDE_TEXTURE_SAMPLER_UNIFORM_NAME "u_texture_samplers"
#define FUDE_MATRIX_MVP_UNIFORM_NAME "u_mvp"
#define FUDE_MATRIX_PROJECTION_UNIFORM_NAME "u_projection"
#define FUDE_MATRIX_VIEW_UNIFORM_NAME "u_view"
#define FUDE_MATRIX_MODEL_UNIFORM_NAME "u_model"

#define FUDE_EVENT_QUEUE_MAXIMUM_EVENTS 512 // TODO: reconsider this value
#define FUDE_RENDERER_MAXIMUM_VERTICES (32*1024)
#define FUDE_RENDERER_MAXIMUM_INDICIES (FUDE_RENDERER_MAXIMUM_VERTICES*6/4)
#define FUDE_RENDERER_MAXIMUM_TEXTURES 8
#define FUDE_SHADER_RETRIEVE_ALL_LOCATIONS 0

//======================================================================
// Types
//======================================================================
#ifndef GM_H // if "gm.h" is not included
    typedef union V2f {
        float elements[2];
        struct {
            union { float x, r, s, u; };
            union { float y, g, t, v; };
        };
    } V2f;

    typedef union V3f {
        float elements[3];
        struct {
            union { float x, r, s, u; };
            union { float y, g, t, v; };
            union { float z, b, p, w; };
        };
    } V3f;

    typedef union V4f {
        float elements[4];
        struct {
            union { float x, r, s; };
            union { float y, g, t; };
            union { float z, b, p; };
            union { float w, a, q; };
        };
    } V4f;

    typedef union M3f {
        V4f rows[3];
        float elements[3*3];
    } M3f;

    typedef union M4f {
        V4f rows[4];
        float elements[4*4];
    } M4f;

#endif // GM_H

typedef enum {
    FUDE_OK = 0,
    FUDE_ERROR,
    FUDE_INVALID_ARGUMENTS_ERROR,
    FUDE_INITIALIZATION_ERROR,
    FUDE_WINDOW_CREATION_ERROR,
    FUDE_SHADER_CREATION_ERROR,

    FUDE_ATTRIBUTE_LOCATION_NOT_FOUND_ERROR,
    FUDE_UNIFORM_LOCATION_NOT_FOUND_ERROR,
} fude_result;

typedef struct { uint8_t r, g, b, a;  } fude_color;
#define FUDE_RED   (fude_color){ .r=0xFF, .g=0x00, .b=0x00, .a=0xFF }
#define FUDE_BLUE  (fude_color){ .r=0x00, .g=0x00, .b=0xFF, .a=0xFF }
#define FUDE_GREEN (fude_color){ .r=0x00, .g=0xFF, .b=0x00, .a=0xFF }
#define FUDE_BLACK (fude_color){ .r=0x00, .g=0x00, .b=0x00, .a=0xFF }
#define FUDE_WHITE (fude_color){ .r=0xFF, .g=0xFF, .b=0xFF, .a=0xFF }

typedef struct { int x, y, width, height; } fude_rect;
typedef struct { int x0, y0, x1, y1, x2, y2; } fude_triangle;

// graphics
typedef struct {
    uint32_t id;
} fude_texture;

typedef enum {
    FUDE_UNIFORM_TEXTURE_SAMPLERS_LOC,
    FUDE_UNIFORM_MATRIX_MVP_LOC,
    FUDE_UNIFORM_MATRIX_PROJECTION_LOC,
    FUDE_UNIFORM_MATRIX_VIEW_LOC,
    FUDE_UNIFORM_MATRIX_MODEL_LOC,
    FUDE_COUNT_UNIFORM_LOC,
} fude_shader_uniform_loc;

typedef enum {
    FUDE_ATTR_POSITION_LOC,
    FUDE_ATTR_COLOR_LOC,
    FUDE_ATTR_TEX_COORDS_LOC,
    FUDE_ATTR_TEX_INDEX_LOC,
    FUDE_ATTR_OBJECT_ID_LOC,
} fude_shader_attribute_loc;

typedef struct {
    uint32_t id;
    int uniform_loc[FUDE_COUNT_UNIFORM_LOC];
} fude_shader;

typedef struct {
    V3f position;
    V4f color;
    V2f tex_coords;
    float tex_index;
    float object_id;
} fude_vertex;

typedef enum {
    FUDE_MODE_TRIANGLES = 0,
    FUDE_MODE_QUADS,
} fude_draw_mode;

typedef struct {
    M4f projection_matrix;
    M4f view_matrix;
} fude_camera;

typedef struct {
    uint32_t id;
    fude_shader shader;
    uint32_t vbo, ibo;
    struct {
        fude_vertex data[FUDE_RENDERER_MAXIMUM_VERTICES];
        uint32_t count;
    } vertices;
    struct {
        uint32_t data[FUDE_RENDERER_MAXIMUM_INDICIES];
        uint32_t count;
    } indices;
    struct {
        fude_texture data[FUDE_RENDERER_MAXIMUM_TEXTURES];
        int samplers[FUDE_RENDERER_MAXIMUM_TEXTURES];
    } textures;

    fude_shader default_shader;
    fude_texture default_texture;

    struct {
        fude_vertex vertex;
        fude_draw_mode mode;
        uint32_t count;
    } working;
} fude_renderer;

// core
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

typedef struct GLFWwindow GLFWwindow;

typedef struct {
    GLFWwindow* window;
    fude_event_queue event_queue;
    fude_renderer renderer;
} fude;

typedef struct {
    const char* name;
    uint32_t width, height;
    bool resizable;
} fude_config;

//======================================================================
// Functions
//======================================================================
// fude_core.c
FAPI fude_result f_init(fude* f, const fude_config* config);
FAPI void f_deinit(fude* f);
FAPI void f_poll_events(fude* f);
FAPI bool f_next_event(fude* f, fude_event* event);
FAPI void f_present(fude* f);
FAPI void f_clear(fude* f);

// fude_graphics.c
FAPI void f_flush(fude* f);

FAPI void f_begin(fude* f, fude_draw_mode mode, fude_shader shader);
FAPI void f_end(fude* f);
FAPI void f_texture(fude* f, fude_texture texture, float u, float v, uint32_t index);
FAPI void f_color4f(fude* f, float r, float g, float b, float a);
FAPI void f_color(fude* f, fude_color color);
FAPI void f_vertex2f(fude* app, float x, float y);
FAPI void f_vertex3f(fude* app, float x, float y, float z);

FAPI void f_triangle(fude* f, fude_triangle triangle, uint32_t color);
FAPI void f_rectangle(fude* f, fude_rect rect, uint32_t color);
FAPI void f_rectangle_tex(fude* f, fude_rect rect, fude_texture texture);

FAPI fude_result f_create_shader(fude_shader* shader, const char* vert_src, const char* frag_src);
FAPI fude_result f_create_shader_from_file(fude_shader* shader, const char* vert_path, const char* frag_path);
FAPI void f_destroy_shader(fude_shader shader);
FAPI fude_result f_get_shader_uniform_location(fude_shader shader, int* location, const char* name);
FAPI fude_result f_set_shader_uniform(fude_shader shader, int location, int data_type, int count, const void* data, bool transpose);

FAPI fude_result f_create_texture(fude_texture* texture, const void* data, int width, int height, int channels);
FAPI void f_destroy_texture(fude_texture texture);
FAPI void f_update_texture(fude_texture texture, const void* data, int width, int height, int channels);

FAPI fude_result f_create_camera2d(fude_camera* camera, uint32_t width, uint32_t height);
FAPI fude_result f_create_camera3d(fude_camera* camera);

// fude_utils.c
FAPI void* f_malloc(uint64_t nbytes);
FAPI void f_free(void* ptr);
FAPI void* f_memcpy(void* dst, const void* src, size_t nbytes);
FAPI void* f_memset(void* ptr, int value, size_t nbytes);
FAPI void* f_memzero(void* ptr, size_t nbytes);
FAPI void* f_load_file_data(const char* file_path, size_t* file_size);
FAPI void f_unload_file_data(void* data);
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

enum fude_shader_data_type {
    FUDE_SHADERDT_FLOAT = 0,
    FUDE_SHADERDT_VEC2,
    FUDE_SHADERDT_VEC3,
    FUDE_SHADERDT_VEC4,
    FUDE_SHADERDT_INT,
    FUDE_SHADERDT_IVEC2,
    FUDE_SHADERDT_IVEC3,
    FUDE_SHADERDT_IVEC4,
    FUDE_SHADERDT_MAT4,
};

#endif // FUDE_H
