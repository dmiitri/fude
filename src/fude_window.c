#include "fude.h"
#include "fude_internal.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <GLFW/glfw3.h>

static struct {
    uint32_t glfw_window_count;
    int last_failure_code;

    struct {
        Fude_Event events[FUDE_EVENT_QUEUE_CAPACITY];
        size_t head;
        size_t tail;
    } event_queue;
} FUDE = {0};

void fude_set_failure_code(int code)
{
    FUDE.last_failure_code = code;
}

static void fude_event_window_close_callback(GLFWwindow* window);
static void fude_event_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Fude_Window* fude_create_window(const char* title, uint32_t width, uint32_t height, bool resizable)
{
    if(title == NULL)
        return NULL;

    Fude_Window* window = malloc(sizeof(Fude_Window));
    if(window == NULL) {
        fude_set_failure_code(FUDE_MEMORY_ALLOCATION_FAILURE);
    }

    memset(window, 0, sizeof(Fude_Window));

    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(window->glfw_window == NULL) {
        fude_set_failure_code(FUDE_NATIVE_WINDOW_CREATION_FAILURE);
        return NULL;
    }

    glfwSetWindowCloseCallback(window->glfw_window, fude_event_window_close_callback);
    glfwSetKeyCallback(window->glfw_window, fude_event_key_callback);

    FUDE.glfw_window_count += 1;
    return window;
}

void fude_destroy_window(Fude_Window* window)
{
    glfwDestroyWindow(window->glfw_window);
    free(window);
    FUDE.glfw_window_count -= 1;
}

void fude_poll_input_events()
{
    glfwPollEvents();
}

bool fude_get_next_input_event(Fude_Event* event)
{
    if(FUDE.event_queue.head != FUDE.event_queue.tail) {
        *event = FUDE.event_queue.events[FUDE.event_queue.tail];
        FUDE.event_queue.tail = (FUDE.event_queue.tail + 1) % FUDE_EVENT_QUEUE_CAPACITY;
    }
    return event->code != FUDE_EVENT_NONE;
}


Fude_Event* fude_new_event(void)
{
    Fude_Event* event = FUDE.event_queue.events + FUDE.event_queue.head;
    FUDE.event_queue.head = (FUDE.event_queue.head + 1) % FUDE_EVENT_QUEUE_CAPACITY;
    assert(FUDE.event_queue.head != FUDE.event_queue.tail);
    memset(event, 0, sizeof(Fude_Event));
    return event;
}

void fude_event_window_close_callback(GLFWwindow* window)
{
    Fude_Event* event = fude_new_event();
    event->code = FUDE_EVENT_WINDOW_CLOSED;
}

void fude_event_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Fude_Event* event = fude_new_event();
    event->keyboard.key = key;
    event->keyboard.scancode = scancode;
    event->keyboard.mods = mods;

    if(action == GLFW_PRESS)
        event->code = FUDE_EVENT_KEY_PRESSED;
    else if(action == GLFW_RELEASE)
        event->code = FUDE_EVENT_KEY_RELEASED;
    else if(action == GLFW_REPEAT)
        event->code = FUDE_EVENT_KEY_REPEATED;
}

const char* fude_failure_reason(void)
{
    switch (FUDE.last_failure_code)
    {
    case FUDE_MEMORY_ALLOCATION_FAILURE:
        return "Memory allocation failure";
    case FUDE_WINDOWING_SUBSYSTEM_INITIALIZATION_FAILURE:
        return "Windowing subsystem initialization failure";
    case FUDE_NATIVE_WINDOW_CREATION_FAILURE:
        return "Native window creation failure";
    case FUDE_RENDERER_INDICES_OVERFLOW:
        return "Renderer indices overflow";
    case FUDE_RENDERER_VERTICES_OVERFLOW:
        return "Renderer vertices overflow";
    case FUDE_INVALID_TEXTURE_SLOT:
        return "Invalid texture slot";
    case FUDE_OPENGL_INVALID_ENUM:
        return "OpenGL Invalid enum";
    case FUDE_OPENGL_INVALID_VALUE:
        return "OpenGL Invalid value";
    case FUDE_OPENGL_INVALID_OPERATION:
        return "OpenGL Invalid operation";
    case FUDE_OPENGL_STACK_OVERFLOW:
        return "OpenGL Stack overflow";
    case FUDE_OPENGL_STACK_UNDERFLOW:
        return "OpenGL Stack underflow";
    case FUDE_OPENGL_OUT_OF_MEMORY:
        return "OpenGL Out of memory";
    case FUDE_OPENGL_INVALID_FRAMEBUFFER_OPERATION:
        return "OpenGL Invalid framebuffer operation";
    case FUDE_OPENGL_UNKNOWN_INVALID_CODE:
        return "OpenGL Unknown invalid code";
    case FUDE_OPENGL_VERTEX_SHADER_COMPILATION_FAILURE:
        return "OpenGL vertex shader compilation failure";
    case FUDE_OPENGL_FRAGMENT_SHADER_COMPILATION_FAILURE:
        return "OpenGL fragment shader compilation failure";
    case FUDE_OPENGL_SHADER_PROGRAM_LINKING_FAILURE:
        return "OpenGL shader program linking failure";

    default:
        return NULL;
    }
}
