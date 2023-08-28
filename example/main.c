#include "fude.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(void) 
{
    if(!fude_init())
        return -1;

    int waifu_image_width, waifu_image_height, waifu_image_comp;
    uint8_t* data = stbi_load("res/waifu.jpg", &waifu_image_width, &waifu_image_height, 
    &waifu_image_comp, STBI_rgb_alpha);
    int waifu_dst_width = waifu_image_width * 0.4;
    int waifu_dst_height = waifu_image_height * 0.4;


    Fude_Window* window = fude_create_window("My Waifu", waifu_dst_width, waifu_dst_height, false);
    if(window == NULL) {
        fprintf(stderr, "ERROR: %s\n", fude_failure_reason());
        return 0;
    }

    Fude_Renderer* ren = fude_create_renderer(window);
    if(ren == NULL) {
        fprintf(stderr, "ERROR: %s\n", fude_failure_reason());
        return 0;
    }

    Fude_Texture* waifu = fude_create_texture(ren, waifu_image_width, 
    waifu_image_height, data, waifu_image_comp);
    stbi_image_free(data);

    bool quit = false;

    while(!quit) {
        fude_poll_input_events();
        Fude_Event event = {0};
        while(fude_get_next_input_event(&event)) {
            if(event.code == FUDE_EVENT_WINDOW_CLOSED) {
                quit = true;
                break;
            }
        }

        fude_set_draw_color(ren, 255, 255, 255, 255);
        fude_clear_renderer(ren);
        fude_draw_texture(ren, waifu, 0, 0, 1, 1, 0, 0, 
        waifu_dst_width, waifu_dst_height);
        fude_present_renderer(ren);
    }

    fude_destroy_texture(waifu);
    fude_destroy_renderer(ren);
    fude_destroy_window(window);
    fude_deinit();
}
