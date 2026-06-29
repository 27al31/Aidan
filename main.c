#include "aidan.c"
#include "tui.c"
#include <stdlib.h>
#include <time.h>

// BIG QUESTION
// physics -?-> graphics

typedef struct {
    double x;
    double y;
} vec2;

// // Constructor
// NOTE if the "{ x, y }" version is not working,
//      try using "(vec2) { x, y }" or "vec2_new(x, y)"
vec2 vec2_new(double x, double y) {
    vec2 result = { x, y };
    return(result);
}

#define UNUSED(expr) do { (Void)(expr); } while (0)

vec2 vec2_add(vec2 a, vec2 b) {
    vec2 result = {0};
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec2 vec2_sub(vec2 a, vec2 b) {
    vec2 result = {0};
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

void vec2_add_equals(vec2 *a, vec2 b) {
    *a = vec2_add(*a, b);
}

double double_square(double a) {
    return a * a;
}

double vec2_squared_distance(vec2 a, vec2 b) {
    return double_square(a.x - b.x) + double_square(a.y - b.y);
}

vec2 vec2_abs(vec2 a) {
    vec2 result = {0};
    result.x = (a.x < 0) ? -a.x : a.x;
    result.y = (a.y < 0) ? -a.y : a.y;
    return result;
}

vec2 vec2_mult(vec2 a, vec2 b) {
    vec2 result = {0};
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

double vec2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

vec2 vec2_scalar_mult(vec2 a, double s) {
    vec2 result = {0};
    result.x = a.x * s;
    result.y = a.y * s;
    return result;
}

vec2 vec2_reflect(vec2 v, vec2 normal) {
    return vec2_sub(v, vec2_scalar_mult(normal, 2.0 * vec2_dot(v, normal)));
}

typedef struct {
    vec2 c;
    vec2 v;
    vec2 a;
    double radius;
    TUIColor color;
} Ball;

typedef struct {
    vec2 position;
    vec2 normal;
} Wall;

#define LIST_STRUCT(ELEMENT_TYPE, LIST_TYPE) \
typedef struct { \
 int length; \
 int capacity; \
 ELEMENT_TYPE *_; \
} LIST_TYPE

#define LIST_FOR(TYPE, ELEMENT, LIST) \
for ( \
 TYPE ELEMENT = (LIST)->_; \
 (ELEMENT != NULL) && (ELEMENT != &(LIST)->_[(LIST)->length]); \
 ++ELEMENT \
)

#define LIST_APPEND(LIST, ELEMENT) \
do { \
 if ((LIST)->length == (LIST)->capacity) { \
  (LIST)->capacity = ( \
   !(LIST)->_ \
   ? 7 \
   : 2 * (LIST)->capacity \
  ); \
  (LIST)->_ = realloc((LIST)->_, (LIST)->capacity * sizeof((LIST)->_[0])); \
 } \
 \
 (LIST)->_[(LIST)->length++] = (ELEMENT); \
} while (0)

LIST_STRUCT(Ball, List_Ball);
LIST_STRUCT(vec2, List_Vec2);
LIST_STRUCT(Wall, List_Wall);

Ball ball_make(vec2 c, vec2 v, vec2 a, double radius, TUIColor color) {
    Ball ball = {c, v, a, radius, color};
    return ball;
}

Wall wall_make(vec2 position, vec2 normal) {
    Wall wall = {position, normal};
    return wall;
}

Ball ball_random(double world_length, double world_height) {
    double radius = rand() % 4 + 2;
    vec2 c = {radius + rand() % ((int)world_length - 2*(int)radius), radius + rand() % ((int)world_height - 2*(int)radius)};
    vec2 v = {(rand() % 7) - 3, (rand() % 7) - 3};
    vec2 a = {0, 0}; // {(rand() % 3) - 1, (rand() % 3) - 1};
    TUIColor color = (TUIColor)((rand() % 4) + 1);
    return ball_make(c, v, a, radius, color);
}

/* before wall struct:
void ball_update(Ball *ball, double world_length, double world_height) {
    vec2 next_v = vec2_add(ball->v, ball->a);
    // update physics
    if (ball->c.x + next_v.x + ball->radius >= world_length - 1) {
        ball->c.x = world_length - 1 - (next_v.x - (world_length - 1 - (ball->c.x + ball->radius))) - ball->radius;
        ball->v.x = -next_v.x;
    }
    else if (ball->c.x + next_v.x - ball->radius <= 0) {
        ball->c.x = -next_v.x - (ball->c.x - ball->radius) + ball->radius;
        ball->v.x = -next_v.x;
    }
    else if (ball->c.y + next_v.y + ball->radius >= world_height - 1) { // need to fix eventually, shouldn't be else if
        ball->c.y = world_height - 1 - (next_v.y - (world_height - 1 - (ball->c.y + ball->radius))) - ball->radius;
        ball->v.y = -next_v.y;
    }
    else if (ball->c.y + next_v.y - ball->radius <= 0) {
        ball->c.y = -next_v.y - (ball->c.y - ball->radius) + ball->radius;
        ball->v.y = -next_v.y;
    }
    else {
        vec2_add_equals(&ball->c, next_v);
        ball->v = next_v;
    }
}
*/

void ball_update(Ball *ball, List_Wall walls) {
    vec2 next_v = vec2_add(ball->v, ball->a);
    bool bounced = false;
    //bool check_all = true;
    //while(check_all) {
    //    check_all = false;
        LIST_FOR(Wall *, wall, &walls) {
            vec2 p = wall->position;
            vec2 n = wall->normal;
            vec2 c = vec2_add(ball->c, next_v);
            vec2 d = vec2_sub(c, vec2_scalar_mult(n, ball->radius));
            vec2 f = vec2_sub(d, p);
            if (vec2_dot(n, f) <= 0) {
                double ball_disp_from_wall = vec2_dot(vec2_sub(ball->c, p), n) - ball->radius;
                double hit_time = -ball_disp_from_wall/vec2_dot(next_v, n);
                vec2 ball_at_wall = vec2_add(ball->c, vec2_scalar_mult(next_v, hit_time));
                vec2 remaining_disp = vec2_scalar_mult(next_v, 1.0 - hit_time);
                ball->c = vec2_add(ball_at_wall, vec2_reflect(remaining_disp, n));
                ball->v = vec2_reflect(next_v, n);
                bounced = true;
    //            check_all = true;
            }
        }
    //}
    if (!bounced) {
        vec2_add_equals(&ball->c, next_v);
        ball->v = next_v;
    }
}

// render graphics
// for each ball
//  for x
//   for y
//    if pixel = (x, y) in circle
//     set pixel color
void ball_draw(TUI *tui, Ball *ball) {
    for (int i = (ball->c.x - ball->radius); i <= (ball->c.x + ball->radius); ++i) {
        for (int j = (ball->c.y - ball->radius); j <= (ball->c.y + ball->radius); ++j) {
            vec2 p = {i, j};
            double sd = vec2_squared_distance(ball->c, p);
            if (sd <= double_square(ball->radius)) {
                tui_set_pixel_color(tui, i, j, ball->color);
            }
        }
    }
}

void walls_draw(TUI *tui, List_Wall walls, double world_length, double world_height) {
    for (int i = 0; i < world_length; ++i) {
        for (int j = 0; j < world_height; ++j) {
            vec2 pixel = {i, j};
            bool all = true;
            LIST_FOR (Wall *, wall, &walls) {
                if (vec2_dot(wall->normal, vec2_sub(pixel, wall->position)) <= 0) {
                    all = false;
                }
            }
            if (all) {
                tui_set_pixel_color(tui, i, j, TUIColorWhite);
            }
        }
    }
}

int main() {
 al_init();

 // physics state
 // - TODO make all of these variables double's
 // - TODO figure out how to turn a double back into an int in order to draw it (ideally, round not truncate)
 // - TODO to convince yourself that this works, add an acceleration of (0, -g) where g is whatever number you like (perhaps 1?)
 // - TODO look up Symplectic Euler update or similar on Wikipedia (i think that's the one you want)
 // - TODO (Jim, next meeting): introduce a vec2 struct
 double world_length = 80;
 double world_height = 40;

 srand(time(NULL)); // to randomize each time program is run
 int num_balls = 1;
 List_Ball balls = {0};
 for (int k = 0; k < num_balls; ++k) {
    LIST_APPEND(&balls, ball_random(world_length, world_height));
    }

List_Wall walls = {0};
LIST_APPEND(&walls, wall_make((vec2){0, world_height/2},                (vec2){1, 0}));
LIST_APPEND(&walls, wall_make((vec2){world_length - 1, world_height/2}, (vec2){-1, 0}));
LIST_APPEND(&walls, wall_make((vec2){world_length/2, 0},                (vec2){0, 1}));
LIST_APPEND(&walls, wall_make((vec2){world_length/2, world_height - 1}, (vec2){0, -1}));

// graphics state
 TUI tui = tui_new((int) (world_length + 0.5), (int) (world_height + 0.5)); // - NOTE this line will need to change
 while (true) {
  tui_begin_frame(&tui, TUIColorBlack);
  walls_draw(&tui, walls, world_length, world_height);

  LIST_FOR(Ball *, ball, &balls) {
    ball_update(ball, walls);
    ball_draw(&tui, ball);
  }
 }

 return(0);
}
