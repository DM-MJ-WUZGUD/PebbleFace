#include <pebble.h>

static Window *s_main_window; //makes an element; therefore, it must be destroyed
static TextLayer *s_time_layer; //makes an element; therefore, it must be destroyed
static TextLayer *s_time_layer2; //makes an element; therefore, it must be destroyed

//Adds custom bitmap into watchface
static BitmapLayer *s_background_layer; //is going to use GBitmap and make child layer added to window_layer
static GBitmap *s_background_bitmap;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) { //Time change call function

}

static void update_time() { //calls tick_time (created by tick_handler: see above) and does stuff with it that i haf no idea
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);                                   
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(                                                      
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50)
  );  
  s_time_layer2 = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58,30), bounds.size.w, 20)
  );

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);                           
  text_layer_set_text_color(s_time_layer, GColorBlack);                                                                            
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));    
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);                   

  text_layer_set_background_color(s_time_layer2, GColorClear);                           
  text_layer_set_text_color(s_time_layer2, GColorRed);        
  text_layer_set_text(s_time_layer2, "U SUK SUKIT AHHH");
  text_layer_set_font(s_time_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_14));    
  text_layer_set_text_alignment(s_time_layer2, GTextAlignmentCenter);   
  
  // Create GBitmap
s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer)); 
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer2)); 
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);               
  text_layer_destroy(s_time_layer2);                                                                                      
}

static void init() {
    // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


