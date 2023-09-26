#include "fude.h"
#include "glad/glad.h"

int main(void) 
{
    fude* f = f_malloc(sizeof(fude));
    fude_config config;
    config.name = "My Game";
    config.width = 800;
    config.height = 600;

    f_expect(f_init(f, &config) == FUDE_OK, "Failed to initialize %s\n", config.name);

    bool should_quit = false;
    fude_event event;
    fude_color red = (fude_color){ .r = 255, .g = 0, .b = 0, .a = 255 };

    fude_shader shader;
    f_expect(f_load_shader_from_file(&shader, "./example/main.vert", "./example/main.frag") == FUDE_OK, 
            "Failed to load shader\n");
    f_trace_log(FUDE_LOG_INFO, "TEST SHADER LOADED\n");


    while(!should_quit) {
        f_poll_events(f);
        while(f_next_event(f, &event)) {
            if(event.type == FUDE_EVENT_QUIT) {
                should_quit = true;
            }
            if(event.type == FUDE_EVENT_NONE) {
                f_trace_log(FUDE_LOG_INFO, "Event NONE");
            }
        }
        f_clear(f);

        f_begin(f, FUDE_MODE_QUADS);
            f_color(f, red);
            f_vertex2f(f, -0.5f, -0.5f);
            f_color(f, red);
            f_vertex2f(f, +0.5f, -0.5f);
            f_color(f, red);
            f_vertex2f(f, +0.5f, +0.5f);
            f_color(f, red);
            f_vertex2f(f, -0.5f, +0.5f);
        f_end(f);

        f_present(f);
    }

    f_free(f);
}
