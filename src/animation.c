#include <pebble.h>
#include "animation.h"

GBitmap *alert_bitmap;
BitmapLayer *alert_bitmap_layer;
GRect from_frame;
GRect to_frame;


void stopped(Animation *anim, bool finished, void *context){
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay){
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
     
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
     
    AnimationHandlers handlers = {
        .stopped = (AnimationStoppedHandler) stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
     
    animation_schedule((Animation*) anim);
}

void showLoading(Window *win){
  from_frame =  GRect(22, 168, 100, 100);
  to_frame = GRect(22, 34, 100, 100);
  
  
  alert_bitmap_layer = bitmap_layer_create(GRect(22, 168, 100, 100));
  alert_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOADING); 
  bitmap_layer_set_bitmap(alert_bitmap_layer, alert_bitmap);
  layer_add_child(window_get_root_layer(win), bitmap_layer_get_layer(alert_bitmap_layer));
  
  animate_layer(bitmap_layer_get_layer(alert_bitmap_layer), &from_frame, &to_frame, 1000, 0);
  // load 
}

void hideLoading(Window *win){

  animate_layer(bitmap_layer_get_layer(alert_bitmap_layer), &to_frame, &from_frame, 1000, 1500);
}