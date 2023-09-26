#include "fude.h"

void _fude_window_pos_callback(GLFWwindow* window, int x, int y);
void _fude_window_size_callback(GLFWwindow* window, int width, int height);
void _fude_window_close_callback(GLFWwindow* window);
void _fude_window_focus_callback(GLFWwindow* window, int focused);
void _fude_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void _fude_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void _fude_cursor_pos_callback(GLFWwindow* window, double x, double y);
void _fude_cursor_enter_callback(GLFWwindow* window, int entered);
void _fude_scroll_callback(GLFWwindow* window, double x, double y);
void _fude_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void _fude_char_callback(GLFWwindow* window, unsigned int codepoint);

fude_result _fude_init_window(fude* app, const fude_config* config);
fude_result _fude_init_renderer(fude* app, const fude_config* config);

