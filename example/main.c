#include "fude.h"

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

        f_present(f);
    }

    f_free(f);
}
