#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static struct tm *t;
#define KEY_WORD 0
#define KEY_TOWORD 1

#define BUFFER_SIZE 44
///CHANGE
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char word_buffer[32];
  static char to_word_layer_buffer[32];
  char textLine4[BUFFER_SIZE];
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_WORD:
    
      snprintf(textLine4, sizeof(textLine4),t->value->cstring);
      APP_LOG(APP_LOG_LEVEL_INFO, "value : %s",t->value->cstring );
      //snprintf(word_buffer, sizeof(word_buffer),"EN|%s", t->value->cstring);
      break;
    case KEY_TOWORD:
      //snprintf(to_word_layer_buffer, sizeof(to_word_layer_buffer), "ES|%s", t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }
    



    // Look for next item
    t = dict_read_next(iterator);
  }
  
  //APP_LOG(APP_LOG_LEVEL_INFO, "############  linea 4 %s", textLine4);
 //   if (needToUpdateLine(&line4, line4Str, textLine4)) {
 //   APP_LOG(APP_LOG_LEVEL_INFO, "need to update Line");
 //   updateLineTo(&line4, line4Str,  textLine4); 
  //}
  
  // Assemble full string and display
  //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
  text_layer_set_text(text_layer, textLine4);
 // text_layer_set_text(s_to_word_layer, to_word_layer_buffer);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  t = tick_time;
  //display_time(tick_time);
  
  // Get weather update every 9 minutes
  if(tick_time->tm_min % 1 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

  //char textLine4[BUFFER_SIZE];
  //if (needToUpdateLine(&line4, line4Str, textLine4)) {
    //updateLineTo(&line4, line4Str, textLine4);  
  //}
    // Send the message!
    app_message_outbox_send();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

//END

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
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
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
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

//CHANGE
 tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);

 // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  //END


  window_stack_push(window, animated);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
