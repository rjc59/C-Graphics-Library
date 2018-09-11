typedef unsigned short color_t;

color_t encode_color(unsigned short r, unsigned short g, unsigned short b);
void init_graphics();
void exit_graphics();
void clear_screen();
char getkey();
void sleep_ms(long ms);
void draw_pixel(int x, int y, color_t color);
void draw_rect(int x1, int y1, int width, int height, color_t c);
void draw_circle(int x0, int y0, int r, color_t c);
