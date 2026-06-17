#include "kac.c"
#include "tui.c"

// BIG QUESTION
// physics -?-> graphics

int main() {
 kac_init();

 // physics state
 // TODO make all of these variables double's
 // TODO figure out how to turn a double back into an int in order to draw it (ideally, round not truncate)
 // TODO to convince yourself that this works, add an acceleration of (0, -g) where g is whatever number you like (perhaps 1?)
 // TODO look up Symplectic Euler update or similar on Wikipedia (i think that's the one you want)
 // TODO (Jim, next meeting): introduce a vec2 struct
 int world_length = 60;
 int world_height = 20;
 int x = world_length/2 - 1;
 int y = world_height/2 - 1;
 int velocityX = 1;
 int velocityY = 1;

 // graphics state
 TUI tui = tui_new(world_length, world_height); // NOTE this line will need to change
 while (true) {
  tui_begin_frame(&tui);

  // update physics
  // NOTE these collision conditions and "corrections"(?)/updates will need to change
  if (x == world_length - 1 || x == 0) {
   velocityX *= -1;
  }
  if (y == world_height - 1 || y == 0) {
   velocityY *= -1;
  }
  x += velocityX;
  y += velocityY;

  // render graphics
  tui_set_pixel_color(&tui, x, y, TUIColorAqua);

  // for each ball
  //  for x
  //   for y
  //    if pixel = (x, y) in circle
  //     set pixel color
 }

 return(0);
}
