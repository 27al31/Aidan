#include "aidan.c"
#include "tui.c"

// BIG QUESTION
// physics -?-> graphics

int main() {
 al_init();

 // physics state
 // - TODO make all of these variables double's
 // - TODO figure out how to turn a double back into an int in order to draw it (ideally, round not truncate)
 // - TODO to convince yourself that this works, add an acceleration of (0, -g) where g is whatever number you like (perhaps 1?)
 // - TODO look up Symplectic Euler update or similar on Wikipedia (i think that's the one you want)
 // - TODO (Jim, next meeting): introduce a vec2 struct
 double world_length = 60;
 double world_height = 20;
 double x = world_length/2 - 1;
 double y = world_height/2 - 1;
 double velocityX = 1;
 double velocityY = 1;
 double accelerationX = 0;
 double accelerationY = 0;
 double radius = 6;

 // graphics state
 TUI tui = tui_new((int) (world_length + 0.5), (int) (world_height + 0.5)); // - NOTE this line will need to change
 while (true) {
  tui_begin_frame(&tui);

  double next_velocityX = velocityX + accelerationX;
  double next_velocityY = velocityY + accelerationY;
  // update physics
  //  - NOTE these collision conditions and "corrections"(?)/updates will need to change
  if (x + next_velocityX + radius >= world_length - 1) {
    x = world_length - 1 - (next_velocityX - (world_length - 1 - (x + radius))) - radius;
    velocityX = -next_velocityX;
  }
  else if (x + next_velocityX - radius <= 0) {
    x = -next_velocityX - (x - radius) + radius;
    velocityX = -next_velocityX;
  }
  else if (y + next_velocityY + radius >= world_height - 1) { // need to fix eventually, shouldn't be else if
    y = world_height - 1 - (next_velocityY - (world_height - 1 - (y + radius))) - radius;
    velocityY = -next_velocityY;
  }
  else if (y + next_velocityY - radius <= 0) {
    y = -next_velocityY - (y - radius) + radius;
    velocityY = -next_velocityY;
  }
  else {
    x += next_velocityX;
    y += next_velocityY;
    velocityX = next_velocityX;
    velocityY = next_velocityY;
  }

  // render graphics
  // tui_set_pixel_color(&tui, x, y, TUIColorAqua);

  // for each ball
  //  for x
  //   for y
  //    if pixel = (x, y) in circle
  //     set pixel color
  for (int i = (x - radius); i <= (x + radius); ++i) { // could be a potential truncation issue here
    for (int j = (y - radius); j <= (y + radius); ++j) {
        double sd = (i - x)*(i - x) + (j - y)*(j - y);
        if (sd <= (radius*radius)) {
            tui_set_pixel_color(&tui, i, j, TUIColorRed);
        }
    }
  }
  tui_set_pixel_color(&tui, x, y, TUIColorAqua);
 }

 return(0);
}
