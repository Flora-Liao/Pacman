#define _CRT_SECURE_NO_WARNINGS
#include "scene_setting_object.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ButtonSet button_create_set(float x, float y, float w, float h, const char* default_image_path, const char* hovered_image_path) {
    ButtonSet button;
    memset(&button, 0, sizeof(ButtonSet));

    button.default_img = load_bitmap(default_image_path);
    if (hovered_image_path) {
        button.hovered_img = load_bitmap(hovered_image_path);
    }

    if (!button.default_img) {
        game_log("failed loading button image!");
    }

    button.body.x = x;
    button.body.y = y;
    button.body.w = w;
    button.body.h = h;

    return button;
}

void drawButtonset(ButtonSet button) {
    ALLEGRO_BITMAP* _img = button.hovered_img ?
        button.hovered ?
        button.hovered_img :
        button.default_img :
        button.default_img;
    al_draw_scaled_bitmap(
        _img,
        0, 0,
        al_get_bitmap_width(_img), al_get_bitmap_height(_img),
        button.body.x, button.body.y,
        button.body.w, button.body.h, 0
    );
}

bool buttonHoverset(ButtonSet button, int mouse_x, int mouse_y) {
    return pnt_in_rect(mouse_x, mouse_y, button.body);

    return false;
}