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
#include "scene_gameover.h"
#include "utility.h"
#include "shared.h"

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW;
static int gameTitleH;

static Button btnSettings;
static Button restart;
static Button gomenu;

static void init() {

	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	restart = button_create(0, 300, 500, 500, "Assets/RESTART(2).png", "Assets/RESTART(2).png");
	gomenu = button_create(400, 300, 500, 500, "Assets/MENU(2).png", "Assets/MENU(2).png");

	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic2, music_volume);

}

static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	drawButton(btnSettings);
	drawButton(restart);
	drawButton(gomenu);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"GAME OVER"
	);

}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	
	btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
	restart.hovered = buttonHover(restart, mouse_x, mouse_y);
	gomenu.hovered = buttonHover(gomenu, mouse_x, mouse_y);
}

static void on_mouse_down() {
	if (btnSettings.hovered)
		game_change_scene(scene_settings_create());
	if (restart.hovered)
		game_change_scene(scene_main_create());
	if (gomenu.hovered)
		game_change_scene(scene_menu_create());
}


static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(gameTitle);
	//	[HACKATHON 3-10]
	//	TODO: Destroy button images
	//	Uncomment and fill the code below

	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);

}

Scene scene_gameover_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "GameOver";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.

	scene.on_mouse_down = &on_mouse_down;

	// -------------------------------------
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("GameOver scene created");
	return scene;
}
