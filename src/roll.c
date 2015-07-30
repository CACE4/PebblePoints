#include <pebble.h>
#include "roll.h"

static Window *window;

// alex wasn't here; yet
static TextLayer *text_layer; // this comment
static TextLayer *text_layer2;
Layer *dice_layer;
static GBitmap *up_icon;
static GBitmap *down_icon;
static ActionBarLayer *action_bar;

void push_roll(){
  window_stack_push(window, true);
}
int score=6;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
  
  int r = rand() % score;
  
  static char buffer[5];
  
  snprintf(buffer, sizeof(buffer), "%d", r + 1); // Adds one to remove 0 from possibilities
  
  // int r = rand() % 7;
  
  text_layer_set_text(text_layer, buffer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  score++;
  static char buffer2[25];
  snprintf(buffer2, sizeof(buffer2), "number of dice sides: %d",  score);
  text_layer_set_text(text_layer2, buffer2);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  score--;
  static char buffer3[25];
  snprintf(buffer3, sizeof(buffer3), "number of dice sides: %d",  score);
  text_layer_set_text(text_layer2, buffer3);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
void dice_rectangle_draw(struct Layer *layer, GContext *ctx){
graphics_draw_rect(ctx,(GRect) { .origin = { 30,30 }, .size = { 50,50 } });

}
static void window_load(Window *window) {
 
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  dice_layer=layer_create(bounds);
  layer_set_update_proc(dice_layer, dice_rectangle_draw);
  
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    GRect bounds2 = layer_get_bounds(window_layer);

  text_layer2 = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer2, "number of dice sides");
  text_layer_set_text_alignment(text_layer2, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_layer2));
  layer_add_child(window_layer, dice_layer);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}
void init_roll() {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
 // window_stack_push(window, animated);
}


void deinit_roll() {
  window_destroy(window);
}

/*int main(void) {
  init();
  app_event_loop();
  deinit();
}*/