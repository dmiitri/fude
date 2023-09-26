#include "fude.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "fude_internal.h"

fude_result f_init(fude* app, const fude_config* config)
{
    fude_result result = FUDE_OK;

    if(!app || !config) return FUDE_INVALID_ARGUMENTS_ERROR;
    f_memzero(app, sizeof(fude));

    result = _fude_init_window(app, config);
    if(result != FUDE_OK) return result;

    result = _fude_init_renderer(app, config);
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

void f_clear(fude* app)
{
    (void)app;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

