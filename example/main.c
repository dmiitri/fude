#include "fude.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(void) 
{
    if(!fude_init())
        return -1;
    Fude_Window* window = fude_create_window("The Thousands time I create Pong", WINDOW_WIDTH, WINDOW_HEIGHT, false);


    Fude_Renderer* ren = fude_create_renderer(window);

    bool quit = false;
    int entt_size_w = 30;
    int entt_size_h = 100;
    int entt_x_offset = 30;

    int player_pos_y = (WINDOW_HEIGHT - entt_size_h)/2;
    int player_pos_x = entt_x_offset;
    int enemy_pos_y = player_pos_y;
    int enemy_pos_x = WINDOW_WIDTH - entt_x_offset;

    while(!quit) {
        fude_poll_input_events();
        Fude_Event event = {0};
        while(fude_get_next_input_event(&event)) {
            if(event.code == FUDE_EVENT_WINDOW_CLOSED) {
                quit = true;
                break;
            }
        }

        fude_set_draw_color(ren, 0, 0, 255, 255);
        fude_draw_rectangle(ren, player_pos_x, player_pos_y, entt_size_w, entt_size_h);
        fude_set_draw_color(ren, 255, 0, 0, 255);
        fude_draw_rectangle(ren, enemy_pos_x, enemy_pos_y, entt_size_w, entt_size_h);

        fude_set_draw_color(ren, 255, 255, 255, 255);
        fude_clear_renderer(ren);
        fude_present_renderer(ren);
    }

    fude_destroy_renderer(ren);
    fude_destroy_window(window);
    fude_deinit();
}
