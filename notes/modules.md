# Modules

## Widget

On exit:

```c
// callbacks

static void gui_widget_view_draw_callback(Canvas* canvas, void* _model)
static bool gui_widget_view_input_callback(InputEvent* event, void* context)

// standard
Widget* widget_alloc()
void widget_reset(Widget* widget)
void widget_free(Widget* widget)
View* widget_get_view(Widget* widget)

// draw
static void widget_add_element(Widget* widget, WidgetElement* element)
void widget_add_string_multiline_element(Widget* widget, ...)
void widget_add_string_element(Widget* widget, ...)
void widget_add_text_box_element(Widget* widget, ...)
void widget_add_text_scroll_element(Widget* widget... )
void widget_add_button_element(Widget* widget, ...)
void widget_add_icon_element(Widget* widget, uint8_t x, uint8_t y, const Icon* icon)
void widget_add_frame_element(Widget* widget ...)
```