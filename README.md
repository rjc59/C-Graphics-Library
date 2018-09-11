# C-Graphics-Library

Small Graphics Library written in C, using only Linux syscalls.

Implements the following functions:

  | Library Call | System Call(s) used |
  | ------------ | ------------------- |
  | void init_graphics() | open, ioctl, mmap |
  | void exit_graphics() | ioctl |
  | void clear_screen() | write |
  | char getkey() | select, read |
  | void sleep_ms(long ms) | nanosleep |
  | void draw_pixel(int x, int y, color_t color) |  |
  | void draw_rect(int x1, int y1, int width, int height, color_t c) |  |
  | void draw_circle(int x, int y, int r, color_t color) |  |

- void init_graphics()
  - Opens graphics device and maps it into memory (requires direct access to the framebuffer)
  - Disables keypress echo
- void exit_graphcs()
  - Cleans up before program exits and restores keypress echo
- void clear_screen()
  - Clears the terminal
- char getkey()
  - Reads a single key input from the user using non-blocking select() call
- void sleep_ms(long ms)
  - Used to make the program sleep between graphics being drawn
- void draw_pixel(int x, int y, color_t color)
  - Sets the pixel at coordinate (x, y) to the specified color
- void draw_rect(int x1, int y1, int width, int height, color_t c)
  - Using draw_pixel, makes a rectangle with corners (x1, y1), (x1+width,y1), (x1+width,y1+height), (x1, y1+height)
- void draw_circle(int x, int y, int r, color_t color)
  - Uses the midpoint circle algorithm to draw a circle at (x, y) with radius r
