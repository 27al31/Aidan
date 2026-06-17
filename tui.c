// +--> x
// |     
// v     
// y     

typedef enum {
 TUIColorBlack,
 TUIColorRed,
 TUIColorGreen,
 TUIColorBlue,
 TUIColorAqua,
} TUIColor;

typedef struct {
 int num_rows;
 int num_cols;
 int num_pixels;
 TUIColor *buffer;
} TUI;

void tui_set_bg_color(TUIColor color) {
 if (0) {
 } else if (color == TUIColorBlack) {
  printf("\033[40m"); 
 } else if (color == TUIColorRed) {
  printf("\033[41m"); 
 } else if (color == TUIColorGreen) {
  printf("\033[42m"); 
 } else if (color == TUIColorBlue) {
  printf("\033[44m"); 
 } else if (color == TUIColorAqua) {
  printf("\033[46m"); 
 } else {
  ASSERT(0);
 }
}

void tui_move_cursor_to_top_left() {
 printf("\033[20A");
}

TUI tui_new(
 int num_x,
 int num_y
) {
 int num_rows = num_y;
 int num_cols = num_x;
 TUI tui;
 tui.num_rows = num_rows;
 tui.num_cols = num_cols;
 tui.num_pixels = num_rows * num_cols;
 tui.buffer = calloc(tui.num_pixels, sizeof(TUIColor));
 return(tui);
}

void tui_clear(TUI *tui, TUIColor color) {
 for (int pixel_index = 0; pixel_index < tui->num_pixels; ++pixel_index) {
  tui->buffer[pixel_index] = color;
 }
}

int _tui_get_pixel_index_row_col(TUI *tui, int row, int col) {
 ASSERT(row >= 0);
 ASSERT(col >= 0);
 ASSERT(row < tui->num_rows);
 ASSERT(col < tui->num_cols);
 int pixel_index = (col * tui->num_rows) + row;
 return(pixel_index);
}

int _tui_get_pixel_index_x_y(TUI *tui, int x, int y) {
 int row = y;
 int col = x;
 int pixel_index = _tui_get_pixel_index_row_col(tui, row, col);
 return(pixel_index);
}

void tui_set_pixel_color(TUI *tui, int x, int y, TUIColor color) {
 int pixel_index = _tui_get_pixel_index_x_y(tui, x, y);
 tui->buffer[pixel_index] = color;
}

void tui_flip(TUI *tui) {
 for (int row = 0; row < tui->num_rows; ++row) {
  for (int col = 0; col < tui->num_cols; ++col) {
   int pixel_index = _tui_get_pixel_index_row_col(tui, row, col);
   int color = tui->buffer[pixel_index];
   tui_set_bg_color(color);
   printf(" ");
  }
  printf("\n");
 }
}

void tui_begin_frame(TUI *tui) {
 SLEEP(100);
 tui_move_cursor_to_top_left();
 tui_flip(tui);
 tui_clear(tui, TUIColorBlue);
}
