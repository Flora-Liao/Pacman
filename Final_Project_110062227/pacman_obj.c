#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
#include "scene_settings.h"
/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset 
static const int draw_region = 30;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
//extern int voiid;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are 
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND2;
//extern int voiid;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern float effect_volume;

/* Declare static function */
static bool pacman_movable(Pacman* pacman, Map* M, Directions targetDirec) {
	// [HACKATHON 1-2]
	// TODO: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in pacman->objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.

	
	int x1 =pacman->objData.Coord.x, y1=pacman->objData.Coord.y;
	
	switch (targetDirec)
	{
	case UP:
		if (is_wall_block(M, x1, y1 - 1) || is_room_block(M, x1, y1 - 1)) {
			return false;
		}
		else return true;
	case DOWN:
		if (is_wall_block(M, x1, y1 + 1) || is_room_block(M, x1, y1 + 1)) {
			return false;
		}
		else return true;
	case LEFT:
		if (is_wall_block(M, x1 - 1, y1) || is_room_block(M, x1 - 1, y1)) {
			return false;
		}
		else return true;
	case RIGHT:
		if (is_wall_block(M, x1 + 1, y1) || is_room_block(M, x1 + 1, y1)) {
			return false;
		}
		else return true;
	default:
		return false;
	}
}

Pacman* pacman_create() {

	/*
		[TODO]
		Allocate dynamic memory for pman pointer;
	*/
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;
	/*
	Pacman* pman = (0, 0);
		if(!pman)
			return NULL;
	*/
	/* Init pman data */
	/* set starting point, Size, */
	/* TODO? */
	/* hint / just put it */
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;

	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;

	//setRecArea(pman->nowrec, 24, 24, block_width, block_height);//lws

	pman->death_anim_counter = al_create_timer(1.0f / 64);
	pman->powerUp = false;
	/* load sprites */
	pman->move_sprite = load_bitmap("Assets/pacman_move.png");
	pman->die_sprite = load_bitmap("Assets/pacman_die.png");
	return pman;

}

void pacman_destory(Pacman* pman) {
	
		//[TODO]
		//free pacman resource
		al_destroy_bitmap( pman->move_sprite);
		al_destroy_timer(pman->death_anim_counter);
		free(pman);
		//...
		//free(pman);
	
}


void pacman_draw(Pacman* pman,Map* M) {
	/*
		[HW-TODO ]
		Draw Pacman and animations
		hint: use pman->objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
	*/
	RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);

	int offset = 0;
	int x = 0;
	float xdir[5][2] = { {0,0},{64,80},{32,48},{0,16},{96,112} };
	int y = 0;
	int draw = 0;
	
	if (game_over) {
		
		al_draw_scaled_bitmap(pman->die_sprite, draw, 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0
		);
	}
	else {
		
			switch(pman->objData.preMove)
			{
			case LEFT:
				x = 2;
				break;
			case RIGHT:
				x = 3;
				break;
			case UP:
				x = 1;
				break;
			case DOWN:
				x = 4;
				break;
			default:
				break;
			}
			if (pacman_movable(pman, M, pman->objData.nextTryMove))
				if (GAME_TICK%2 == 1)y = 1;
			if (y == 1) {
				
				al_draw_scaled_bitmap(pman->move_sprite, xdir[x][0], 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
				);
				y = 0;
			}
			else {
				al_draw_scaled_bitmap(pman->move_sprite, xdir[x][1], 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
				);
				y = 1;
			}
			
			
	
	}
}
void pacman_move(Pacman* pacman, Map* M) {
	if (!movetime(pacman->speed))
		return;
	if (game_over)
		return;

	int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;
	if (pacman_movable(pacman, M, pacman->objData.nextTryMove)) 
		pacman->objData.preMove = pacman->objData.nextTryMove;
	else if (!pacman_movable(pacman, M, pacman->objData.preMove)) 
		return;

	switch (pacman->objData.preMove)
	{
	case UP:
		pacman->objData.Coord.y -= 1;
		pacman->objData.preMove = UP;
		break;
	case DOWN:
		pacman->objData.Coord.y += 1;
		pacman->objData.preMove = DOWN;
		break;
	case LEFT:
		pacman->objData.Coord.x -= 1;
		pacman->objData.preMove = LEFT;
		break;
	case RIGHT:
		pacman->objData.Coord.x += 1;
		pacman->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	pacman->objData.facing = pacman->objData.preMove;
	pacman->objData.moveCD = GAME_TICK_CD;
}
void pacman_eatItem(Pacman* pacman, const char Item,int id) {
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
			//PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
			if (id == 1)
				PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
			if(id==2)
				PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND2, effect_volume);
		break;
	case 'P':
		stop_bgm(PACMAN_MOVESOUND_ID);
		//PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		if (id == 1)
			PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		if(id==2)
			PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND2, effect_volume);
		break;
	default:
		break;
	}
}

void pacman_NextMove(Pacman* pacman, Directions next) {
	pacman->objData.nextTryMove = next;
}

void pacman_die(Pacman* pman,Map*M) {
	RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);
	//int i = 0;
	/*
	al_draw_scaled_bitmap(pman->die_sprite, 0, 0,
		16, 16,
		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
		draw_region, draw_region, 0
	);*/
	//al_rest(0.5);
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}





