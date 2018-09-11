#include <stdlib.h>
#include <stdio.h>
#include "library.h"

int main(){
  init_graphics();
  clear_screen();
  printf("enter s for square, c for circle, q to quit:\n");
  char key;
  while(1){
    key = getkey();
    if (key == 's'){
      draw_rect(100,100,100,100,15);
    }
    else if (key == 'c'){
      draw_circle(300,300,100,30);
    }
    else if (key == 'q')
      break;
  }
  sleep_ms(5000);
  clear_screen();
  exit_graphics();
  return 0;
}
