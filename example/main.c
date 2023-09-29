#include "fude.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(void) 
{
    fude* f = f_malloc(sizeof(fude));
    fude_shader shader;
    fude_texture cute;
    fude_config config;
    config.name = "My Game";
    config.width = 800;
    config.height = 600;

    f_expect(f_init(f, &config) == FUDE_OK,
            "Failed to initialize %s", config.name);
    f_expect(f_create_shader_from_file(&shader, "./example/main.vert", "./example/main.frag") == FUDE_OK, 
            "Failed to create shader");

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load("./resources/cute.jpg", &width, &height, &channels, 0);
    f_expect(f_create_texture(&cute, data, width, height, channels) == FUDE_OK,
            "Failed to create cute texture");
    stbi_image_free(data);

    bool should_quit = false;
    fude_event event;
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

        // // Colored Rectangle
        // f_begin(f, FUDE_MODE_QUADS, shader);
        //     f_color(f, FUDE_RED);
        //     f_vertex2f(f, -0.5f, -0.5f);
        //     f_color(f, FUDE_GREEN);
        //     f_vertex2f(f, +0.5f, -0.5f);
        //     f_color(f, FUDE_BLUE);
        //     f_vertex2f(f, +0.5f, +0.5f);
        //     f_color(f, FUDE_GREEN);
        //     f_vertex2f(f, -0.5f, +0.5f);
        // f_end(f);

        f_begin(f, FUDE_MODE_QUADS, shader);
            f_texture(f, cute, 0.0f, 0.0f, 1);
            f_vertex2f(f, -0.5f, -0.5f);
            f_texture(f, cute, 1.0f, 0.0f, 1);
            f_vertex2f(f, +0.5f, -0.5f);
            f_texture(f, cute, 1.0f, 1.0f, 1);
            f_vertex2f(f, +0.5f, +0.5f);
            f_texture(f, cute, 0.0f, 1.0f, 1);
            f_vertex2f(f, -0.5f, +0.5f);
        f_end(f);

        f_flush(f);
        f_present(f);
    }

    f_destroy_shader(shader);
    f_free(f);
}
