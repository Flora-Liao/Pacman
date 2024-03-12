#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"

static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW;
static int gameTitleH;
int voiid;//lwsvoiid

#include "scene_settings.h"
static Button btnSettings;
static Button add1;
static Button minus1;
static Button volume;
static Button add2;
static Button minus2;
static Button soundeffect;
//static Button effecttype1;
//static Button effecttype2;


static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	// [HACKATHON 3-3]
	// TODO: Draw button
	// Uncomment and fill the code below
	drawButton(btnSettings);
	drawButton(add1);
	drawButton(minus1);
	drawButton(volume);
	drawButton(add2);
	drawButton(minus2);
	drawButton(soundeffect);
	//drawButton(effecttype1);
	//drawButton(effecttype2);

}
// The only function that is shared across files.

static void init() {
	volume = button_create(25, 160, 500, 500, "Assets/MUSIC_VOLUME(2).png", "Assets/MUSIC_VOLUME(2).png");
	soundeffect = button_create(395, 160, 500, 500, "Assets/EFFECT_VOLUME(2).png", "Assets/EFFECT_VOLUME(2).png");
	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	minus1 = button_create(80, 300, 50, 50, "Assets/MINUS.png", "Assets/MINUS.png");
	add1 = button_create(325, 300, 50, 50, "Assets/ADD.png", "Assets/ADD.png");
	minus2 = button_create(450, 300, 50, 50, "Assets/MINUS.png", "Assets/MINUS.png");
	add2 = button_create(695, 300, 50, 50, "Assets/ADD.png", "Assets/ADD.png");
	//effecttype1 = button_create(450, 500, 50, 50, "Assets/ADD.png", "Assets/ADD.png");
	//effecttype2 = button_create(695, 500, 50, 50, "Assets/ICON(3).png", "Assets/ICON(3).png");

	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);

}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
	add1.hovered = buttonHover(add1, mouse_x, mouse_y);
	minus1.hovered = buttonHover(minus1, mouse_x, mouse_y);
	add2.hovered = buttonHover(add2, mouse_x, mouse_y);
	minus2.hovered = buttonHover(minus2, mouse_x, mouse_y);
	//effecttype1.hovered = buttonHover(effecttype1, mouse_x, mouse_y);
	//effecttype2.hovered = buttonHover(effecttype2, mouse_x, mouse_y);

}

static void on_mouse_down() {
	if (btnSettings.hovered)
		game_change_scene(scene_menu_create());
	if (add1.hovered) {
		music_volume+=0.8;
	}
	if (minus1.hovered) {
		if(music_volume>=0)
			music_volume-=0.1;
	}
	if (add2.hovered) {
		effect_volume+=0.8;
	}
	if (minus2.hovered) {
		//if(effect_volume)
		if(effect_volume>=0)
			effect_volume-=0.1;
	}
	
	//if (effecttype2.hovered) {
		//if (voiid == 1)voiid = 2;
		//else if (voiid == 2)voiid = 1;
	//}
	/*if (!effecttype2.hovered) {
		voiid = 1;
	}*/
}

static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(btnSettings.hovered_img);
	al_destroy_bitmap(btnSettings.default_img);

}

static void on_key_down(int keycode) {

	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		game_change_scene(scene_main_create());
		break;
	default:
	case ALLEGRO_KEY_M:
		game_change_scene(scene_menu_create());
		break;
		break;
	}
}

Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Setting";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.

	scene.on_mouse_down = &on_mouse_down;

	// -------------------------------------
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Setting scene created");
	return scene;
}




