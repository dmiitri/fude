#include "fude.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void _fude_window_pos_callback(GLFWwindow* window, int x, int y);
static void _fude_window_size_callback(GLFWwindow* window, int width, int height);
static void _fude_window_close_callback(GLFWwindow* window);
static void _fude_window_focus_callback(GLFWwindow* window, int focused);
static void _fude_framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void _fude_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void _fude_cursor_pos_callback(GLFWwindow* window, double x, double y);
static void _fude_cursor_enter_callback(GLFWwindow* window, int entered);
static void _fude_scroll_callback(GLFWwindow* window, double x, double y);
static void _fude_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void _fude_char_callback(GLFWwindow* window, unsigned int codepoint);

static fude_result _fude_init_window(fude* app, const fude_config* config)
{
    if(!glfwInit()) {
        return FUDE_INITIALIZATION_ERROR;
    }

    glfwWindowHint(GLFW_RESIZABLE, config->resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    app->window = glfwCreateWindow(config->width, config->height, config->name, NULL, NULL);
    if(app->window == NULL) {
        return FUDE_WINDOW_CREATION_ERROR;
    }

    glfwSetWindowUserPointer(app->window, (void*)&app->event_queue);
    glfwSetWindowPosCallback(app->window, _fude_window_pos_callback);
    glfwSetWindowSizeCallback(app->window, _fude_window_size_callback);
    glfwSetWindowCloseCallback(app->window, _fude_window_close_callback);
    glfwSetWindowFocusCallback(app->window, _fude_window_focus_callback);
    glfwSetFramebufferSizeCallback(app->window, _fude_framebuffer_size_callback);
    glfwSetMouseButtonCallback(app->window, _fude_mouse_button_callback);
    glfwSetCursorPosCallback(app->window, _fude_cursor_pos_callback);
    glfwSetCursorEnterCallback(app->window, _fude_cursor_enter_callback);
    glfwSetScrollCallback(app->window, _fude_scroll_callback);
    glfwSetKeyCallback(app->window, _fude_key_callback);
    glfwSetCharCallback(app->window, _fude_char_callback);

    glfwMakeContextCurrent(app->window);
    return FUDE_OK;
}

static fude_result _fude_init_renderer2d(fude* app, const fude_config* config)
{
    gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
    (void)app;
    (void)config;
    return FUDE_OK;
}

fude_result f_init(fude* app, const fude_config* config)
{
    fude_result result = FUDE_OK;

    if(!app || !config) return FUDE_INVALID_ARGUMENTS_ERROR;
    f_memzero(app, sizeof(fude));

    result = _fude_init_window(app, config);
    if(result != FUDE_OK) return result;

    result = _fude_init_renderer2d(app, config);
    if(result != FUDE_OK) return result;

    return FUDE_OK;
}

void f_deinit(fude* app)
{
    if(!app) return;
    glfwDestroyWindow(app->window);
    f_memzero(app, sizeof(fude));
}

// event handling
void f_poll_events(fude* app)
{
    app->event_queue.head = 0;
    app->event_queue.tail = 0;
    glfwPollEvents();
}

bool f_next_event(fude* app, fude_event* event)
{
    f_memzero(event, sizeof(fude_event));
    if(app->event_queue.head != app->event_queue.tail) {
        *event = app->event_queue.events[app->event_queue.tail];
        app->event_queue.tail = (app->event_queue.tail + 1) % FUDE_EVENT_QUEUE_MAXIMUM_EVENTS;
    }

    return event->type != FUDE_EVENT_NONE;
}

// rendering stuff
void f_present(fude* app)
{
    glfwSwapBuffers(app->window);
}

void f_flush(fude* app)
{
    (void)app;
}

void f_clear(fude* app)
{
    (void)app;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


static fude_event* _fude_new_event(fude_event_queue* eq, int type)
{
    fude_event* event = eq->events + eq->head;
    eq->head = (eq->head + 1) % FUDE_EVENT_QUEUE_MAXIMUM_EVENTS;
    f_expect(eq->head != eq->tail, "Expecting eq->head != eq->tail (%d) at %s (%d)", eq->head, __FILE__, __LINE__);
    f_memzero(event, sizeof(fude_event));
    event->type = type;
    return event;
}


void _fude_window_pos_callback(GLFWwindow* window, int x, int y)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event = _fude_new_event(eq, FUDE_EVENT_WINDOW_MOVED);
    event->window.x = x;
    event->window.y = y;
}

void _fude_window_size_callback(GLFWwindow* window, int width, int height)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event = _fude_new_event(eq, FUDE_EVENT_WINDOW_MOVED);
    event->window.width = width;
    event->window.height = height;
}

void _fude_window_close_callback(GLFWwindow* window)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    _fude_new_event(eq, FUDE_EVENT_QUIT);
}

void _fude_window_focus_callback(GLFWwindow* window, int focused)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    if(focused)
        _fude_new_event(eq, FUDE_EVENT_WINDOW_GAIN_FOCUS);
    else
        _fude_new_event(eq, FUDE_EVENT_WINDOW_LOST_FOCUS);
}

void _fude_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event =_fude_new_event(eq, FUDE_EVENT_FRAMEBUFFER_RESIZED);
    event->framebuffer.width = width;
    event->framebuffer.height = height;
}

void _fude_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event =_fude_new_event(eq, FUDE_EVENT_NONE);
    event->mouse.button = button;
    event->mouse.mods = mods;

    if (action == GLFW_PRESS)
        event->type = FUDE_EVENT_MOUSE_BUTTON_PRESSED;
    else if (action == GLFW_RELEASE)
        event->type = FUDE_EVENT_MOUSE_BUTTON_RELEASED;
}

void _fude_cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event =_fude_new_event(eq, FUDE_EVENT_CURSOR_MOVED);
    event->cursor.x = (int)x;
    event->cursor.y = (int)y;
}

void _fude_cursor_enter_callback(GLFWwindow* window, int entered)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    if(entered)
        _fude_new_event(eq, FUDE_EVENT_CURSOR_ENTERED);
    else
        _fude_new_event(eq, FUDE_EVENT_CURSOR_LEFT);
}

void _fude_scroll_callback(GLFWwindow* window, double x, double y)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event =_fude_new_event(eq, FUDE_EVENT_SCROLL);
    event->scroll.x = x;
    event->scroll.y = y;
}

void _fude_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event =_fude_new_event(eq, FUDE_EVENT_NONE);
    event->keyboard.key = key;
    event->keyboard.scancode = scancode;
    event->keyboard.mods = mods;

    if (action == GLFW_PRESS)
        event->type = FUDE_EVENT_KEY_PRESSED;
    else if (action == GLFW_RELEASE)
        event->type = FUDE_EVENT_KEY_RELEASED;
    else if (action == GLFW_REPEAT)
        event->type = FUDE_EVENT_KEY_REPEATED;
}

void _fude_char_callback(GLFWwindow* window, unsigned int codepoint)
{
    fude_event_queue* eq = (fude_event_queue*)glfwGetWindowUserPointer(window);
    fude_event* event =_fude_new_event(eq, FUDE_EVENT_CODEPOINT);
    event->codepoint = codepoint;
}
