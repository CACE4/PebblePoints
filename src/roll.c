#include <pebble.h>
#include "roll.h"
#include "animation.h"
  
#define NUM_MENU_SECTIONS 1
#define NUM_SCORES 6  
  
static Window *window;
static Window *history_window;
static TextLayer *text_layer;
static TextLayer *text_layer2;
Layer *dice_layer;
//static GBitmap *up_icon;
//static GBitmap *down_icon;
//static ActionBarLayer *action_bar;
int roll_history[6];

int sides=6;

SimpleMenuLayer *history_list;
SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
SimpleMenuItem s_history_menu_items[NUM_SCORES];

void push_roll(){
  window_stack_push(window, true);
}

void push_history(){
  window_stack_push(history_window, true);
}

void reset_history(){
  for (int i = 0; i < 6; i++)
  {
    roll_history[i] = 0;
  }
}

void updateHistory(){
  int i;
  for (i = 0; i < 6; i++)
  {
    
    // Player Name Handler
    static char namebuffer[6][10];
    snprintf(namebuffer[i], sizeof(namebuffer[i]), "Recent  %d", roll_history[i]);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d", i);
    printf("Address: %p", namebuffer[i]);
    
    s_history_menu_items[i] = (SimpleMenuItem) {
      
      .title = namebuffer[i], // buffer
      //.subtitle = scorebuffer[i],
      //.callback = menu_select_callback,
      
    };
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", s_history_menu_items[i].title);
}
}
  
void add_to_history(int numbah){
  showLoading(window);
  //roll_history[6] = roll_history[5];
  roll_history[5] = roll_history[4];
  roll_history[4] = roll_history[3];
  roll_history[3] = roll_history[2];
  roll_history[2] = roll_history[1];
  roll_history[1] = roll_history[0];
  roll_history[0] = numbah;
  
  updateHistory();
  hideLoading(window);
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
  
  int r = rand() % sides;
  
  static char buffer[7];
  
  snprintf(buffer, sizeof(buffer), "%d", r + 1); // Adds one to remove 0 from possibilities
  
  add_to_history(r + 1);
  
  // int r = rand() % 7;
  
  text_layer_set_text(text_layer, buffer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  sides++;
  static char buffer2[25];
  snprintf(buffer2, sizeof(buffer2), "# of sides: %d",  sides);
  text_layer_set_text(text_layer2, buffer2);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    sides--;
    if(sides <= 0){
      sides = 1;
    }
    static char buffer3[25];
    snprintf(buffer3, sizeof(buffer3), "# of sides: %d",  sides);
    text_layer_set_text(text_layer2, buffer3);
  
}


void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void dice_rectangle_draw(struct Layer *layer, GContext *ctx){
graphics_draw_rect(ctx,(GRect) { .origin = { 48, 62 }, .size = { 50, 50 } });

}

void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  dice_layer=layer_create(bounds);
  layer_set_update_proc(dice_layer, dice_rectangle_draw);
  
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 50 } });
  text_layer_set_text(text_layer, "Roll");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    GRect bounds2 = layer_get_bounds(window_layer);

  text_layer2 = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer2, "# of sides: 6");
  text_layer_set_text_alignment(text_layer2, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_layer2));
  layer_add_child(window_layer, dice_layer);
}

void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

void history_window_load(Window *history_window){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading widnow");
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Recent Rolls",
    .num_items = NUM_SCORES,
    .items = s_history_menu_items,
  };
  
  Layer *window_layer = window_get_root_layer(history_window);
  GRect bounds = layer_get_frame(window_layer);

  history_list = simple_menu_layer_create(bounds, history_window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_get_root_layer(history_window), simple_menu_layer_get_layer(history_list)); 
}

void history_window_unload(Window* history_window){
  // do stuff
  simple_menu_layer_destroy(history_list);
}

void init_roll() {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = window_load,
    .unload = window_unload,
  });
 
  history_window = window_create();
  window_set_window_handlers(history_window, (WindowHandlers){
    .load = history_window_load,
    .unload = history_window_unload,
  });
}

void deinit_roll() {
  window_destroy(window);
  window_destroy(history_window);
}