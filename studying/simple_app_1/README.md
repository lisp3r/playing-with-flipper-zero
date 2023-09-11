# Simple App 1

What to learn here?

* How to print a string
* How to add an icon to the app
* How to log shit

![screenshot](Screenshot-20230402-193400.png)

## Structure

App:

```c
typedef struct {
    Gui* gui;
    ViewPort* view_port;
} App;
```

...

## Highlights

If `context` attr is not used you shoud use `UNUSED()` function.

```c
static void draw_callback(Canvas* canvas, void* ctx) {
    // App* app = ctx;
    UNUSED(ctx);
```
