#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include "utility.h"
#include "shared.h"
#include "game.h"

typedef struct ButtonSet {
	RecArea body;
	ALLEGRO_BITMAP* default_img;
	ALLEGRO_BITMAP* hovered_img;
	bool hovered;
}ButtonSet;

ButtonSet button_create_set(float, float, float, float, const char*, const char*);
void drawButtonset(ButtonSet button);
bool buttonHoverset(ButtonSet, int, int);

#endif
