# Fude
My own game library (hugely inspired by raylib)

# Example
```c 
#include <fude.h>
int main(void) {
    fude f;
    f_memzero(&f, sizeof(fude));
    fude_config config;
    config.name = "My Game";
    config.width = 800;
    config.height = 600;

    f_expect(f_init(&f, &config) == FUDE_OK, "Failed to initialize %s\n", config.name);

    bool should_quit = false;
    fude_event event;
    while(!should_quit) {
        f_poll_events(&f);
        while(f_next_event(&f, &event)) {
            if(event.type == FUDE_EVENT_QUIT) {
                should_quit = true;
            }
        }
        f_clear(&f);

        f_draw_text(&f, "Hello, World", 0, 0);

        f_present(&f);
    }
}
```


# Cheatsheet
### fude_core.c
```c
fude_result f_init(fude* f, const fude_config* config); // Initialize an instance of fude application
void f_deinit(fude* f);                                 // Deinitialize an instance of fude application
void f_poll_events(fude* fude);                         // Polls all events into the event queue
bool f_next_event(fude* fude, fude_event* event);       // Get the next event from event queue. return true if valid event
void f_present(fude* fude); // Swap the back buffer with the front buffer
void f_flush(fude* fude); // Flush all render data into the back buffer
void f_clear(fude* fude); // Clear the screen
```

### fude_utils.c
```c
void* f_malloc(uint64_t nbytes);                            // make heap allocation
void f_free(void* ptr);                                     // free allocated memory from heap
void* f_memcpy(void* dst, const void* src, size_t nbytes);
void* f_memset(void* ptr, int value, size_t nbytes);
void* f_memzero(void* ptr, size_t nbytes);                  // set nbytes value of memory into 0
void f_trace_log(int log_level, const char* fmt, ...);      // logging
void f_expect(bool condition, const char* fmt, ...);        // assert a condition if it's false then error with message
```

### Types
```c
struct fude; // Fude Application
struct fude_event;
struct fude_color;
struct fude_shader;
struct fude_texture;
```

