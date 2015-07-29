#include <pebble.h>
#include "roll.h"

static Window *window;

// alex wasn't here; yet
static TextLayer *text_layer; // this comment

static GBitmap *up_icon;
static GBitmap *down_icon;
static ActionBarLayer *action_bar;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
  
  int r = rand() % 7;
  
  static char buffer[7];
  
  snprintf(buffer, sizeof(buffer), "%d", r + 1); // Adds one to remove 0 from possibilities
  
  // int r = rand() % 7;
  
  text_layer_set_text(text_layer, buffer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  up_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_UP_ICON);
  down_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DOWN_ICON);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  // Initialize the action bar:
  action_bar = action_bar_layer_create();
  // Associate the action bar with the window:
  action_bar_layer_add_to_window(action_bar, window);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(action_bar,
                                             click_config_provider);
  
  action_bar_layer_set_background_color(action_bar, GColorBlack);

  // Set the icons:
  // The loading of the icons is omitted for brevity... See gbitmap_create_with_resource()
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, up_icon);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, down_icon);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

/*int main(void) {
  init();
  app_event_loop();
  deinit();
}*/