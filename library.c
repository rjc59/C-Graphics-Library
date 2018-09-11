#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include "library.h"

color_t* frame_buffer;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
int frame_fd;
int len;

color_t encode_color(unsigned short r, unsigned short g, unsigned short b){
  color_t code;
  code = (r & 0x1f) << 11;
  code != (g & 0x3f) << 5;
  code != (b & 0x1f);
  return code;
}

void init_graphics(){
  int term_fd;
  struct termios terminal;

  frame_fd = open("/dev/fb0", O_RDWR);
  if (frame_fd == -1){
    perror("Error opening framebuffer file");
    _exit(1);
  }

  if (ioctl(frame_fd, FBIOGET_VSCREENINFO, &vinfo) == -1){
    perror("Error reading screen information");
    _exit(1);
  }
  if (ioctl(frame_fd, FBIOGET_FSCREENINFO, &finfo) == -1){
    perror("Error reading screen information");
    _exit(1);
  }

  len = vinfo.yres_virtual * finfo.line_length;
  frame_buffer = mmap(NULL, len, PROT_WRITE, MAP_SHARED, frame_fd, 0);

  term_fd = open("/dev/tty", O_RDWR);
  if (term_fd == -1){
    perror("Error opening terminal file");
    _exit(1);
  }

  if (ioctl(term_fd, TCGETS, &terminal) == -1){
    perror("Error reading console info");
    _exit(1);
  }

  terminal.c_lflag &= ~ICANON;
  terminal.c_lflag &= ~ECHO;

  if (ioctl(term_fd, TCSETS, &terminal) == -1){
    perror("Error setting console changes");
    _exit(1);
  }

  close(term_fd);
}

void exit_graphics(){
  int term_fd;
  struct termios terminal;

  term_fd = open("/dev/tty", O_RDWR);
  if (term_fd == -1){
    perror("Error opening terminal file");
    _exit(1);
  }

  if (ioctl(term_fd, TCGETS, &terminal) == -1){
    perror("Error reading console info");
    _exit(1);
  }

  /* set flags in the struct back to re-enable keypress echo after
  the changes are added to the terminal file */
  terminal.c_lflag |= ICANON;
  terminal.c_lflag |= ECHO;

  if (ioctl(term_fd, TCSETS, &terminal) == -1){
    perror("Error setting console changes");
    _exit(1);
  }

  close(term_fd);

  /* unmap the framebuffer and close the corresponding file descriptor*/
  munmap(frame_buffer, len);
  close(frame_fd);
}

void clear_screen(){
  /* write ANSI escape sequence to tell terminal to clear itself */
  const char code[] = "\033[2J";
  write(STDOUT_FILENO, code, sizeof(code)-1);
}

char getkey(){
  fd_set rfds;
  struct timeval tv;
  int retval;
  char key;
  char buf[1];

  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds);

  /* set both fields of the timeval struct to 0 so select returns
  immediately (for polling) */
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  retval = select(1, &rfds, NULL, NULL, &tv);

  if (retval == -1){
    perror("Select() returned an error");
    _exit(1);
  }
  else if (retval){
    read(STDIN_FILENO, buf, 1);
  }

  key = buf[0];
  return key;
}

void sleep_ms(long ms){
  struct timespec req;

  req.tv_sec = 0;
  req.tv_nsec = ms * 1000000;

  if (nanosleep(&req, NULL) == -1){
    perror("nanosleep() returned an error");
    _exit(1);
  }
}

void draw_pixel(int x, int y, color_t color){
  unsigned long vert = (finfo.line_length / 2) * y;
  unsigned long horiz = x;
  *(frame_buffer + vert + horiz) = color;
}

void draw_rect(int x1, int y1, int width, int height, color_t c){
  int x;
  int y;
    for (x = x1; x < (x1 + width); x++){
       for (y = y1; y < (y1 + height); y++){
          if (x == x1 || x == x1 + width - 1){
             draw_pixel(x, y, c);
          }
          if (y == y1 || y == y1 + height - 1){
             draw_pixel(x, y, c);
          }
       }
    }
}

void draw_circle(int x0, int y0, int r, color_t c){
  int x = r;
  int y = 0;
  int err = 0;

  while (x >= y){
    draw_pixel(x0 + x, y0 + y, c);
    draw_pixel(x0 + y, y0 + x, c);
    draw_pixel(x0 - y, y0 + x, c);
    draw_pixel(x0 - x, y0 + y, c);
    draw_pixel(x0 - x, y0 - y, c);
    draw_pixel(x0 - y, y0 - x, c);
    draw_pixel(x0 + y, y0 - x, c);
    draw_pixel(x0 + x, y0 - y, c);

    y += 1;
    if (err <= 0){
      err += 2*y + 1;
    }
    if (err > 0){
      x -= 1;
      err -= 2*x + 1;
    }
  }
}
