#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ghost.h"
#include "map.h"
#include "pacman_obj.h"

/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
const int cage_grid_x=22, cage_grid_y=11;

/* shared variables. */
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern const int block_width,  block_height;
/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
// [ NOTE - speed again ]
// Again, you see this notaficationd. If you still want to implement something 
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on 
// working on animation of ghosts and pacman. // Once you finished the animation 
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 2;

Ghost* ghost_create(int flag,int id) {//lws_ghost_create

	// NOTODO
	Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
	if (!ghost)
		return NULL;

	ghost->typeFlag = flag;
	ghost->objData.Size.x = block_width;
	ghost->objData.Size.y = block_height;

	ghost->objData.nextTryMove = NONE;
	ghost->speed = basic_speed;
	ghost->status = BLOCKED;

	ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
	ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

	switch (ghost->typeFlag) {
	case Blinky:
		//ghost->objData.Coord.x = cage_grid_x;
		//ghost->objData.Coord.y = cage_grid_y;
		if (id == 0) {
			ghost->objData.Coord.x = cage_grid_x+1;//lws_2ghosts
			ghost->objData.Coord.y = cage_grid_y+1;
			ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		}
			
		else if (id == 1) {
			ghost->objData.Coord.x = cage_grid_x;
			ghost->objData.Coord.y = cage_grid_y;
			ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
		}
			
		ghost->move_script = &ghost_red_move_script;
		break;
	default:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		if(id==0)
			ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		else if(id==1)
			ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	}
	return ghost;
}
void ghost_destory(Ghost* ghost) {
	free(ghost);
	/*
		[TODO]
		free ghost resource

		al_destory_bitmap(...);
		...
		free(ghost);
	*/
}
void ghost_draw(Ghost* ghost,Map* M) {
	int x = 0,y=0;
	float xdir[5][2] = { {0,0},{64,80},{32,48},{0,16},{96,112} };
	// getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
	RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);
	switch (ghost->objData.preMove)
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
	if (ghost_movable(ghost, M, ghost->objData.preMove,true))
		if (GAME_TICK % 2 == 1)y = 1;
	if (y == 1) {

		al_draw_scaled_bitmap(ghost->move_sprite, xdir[x][0], 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0
		);
		y = 0;
	}
	else {
		al_draw_scaled_bitmap(ghost->move_sprite, xdir[x][1], 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0
		);
		y = 1;
	}
	//int f[5] = { 0,64,32,0,96 };
	//Draw default image
	/*
	al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
		16, 16,
		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
		draw_region, draw_region, 0
	);*///lws

	
		//[TODO]
		//Draw ghost according to its status
		//hint : use ghost->objData.moveCD value to determine which frame of the animation to draw.

			//A not so good way is:
	/*
			if (ghost->objData.preMove == UP) {
				al_draw_scaled_bitmap(ghost->move_sprite, 64, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
				);
			}
			else if(ghost->objData.preMove == DOWN){
				al_draw_scaled_bitmap(ghost->move_sprite, 96, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
				);
			}
			else if (ghost->objData.preMove == LEFT) {
				al_draw_scaled_bitmap(ghost->move_sprite, 32, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
				);
			}
			else if (ghost->objData.preMove == RIGHT) {
				al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
				);
			}
			*/
			
			//since modulo operation is expensive, better avoid using it.
	

	int bitmap_x_offset = 0;
	// [TODO] below is for animation usage, change the sprite you want to use.
	if (ghost->status == FLEE) {
		
		al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0, 16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0);//lwsflee
		
	}
	else if (ghost->status == GO_IN) {
		/*
		switch (ghost->objData.facing)
		{
		case LEFT:
			...
		*/
	}
	else {
		/*
		switch (ghost->objData.facing)
		{
		case LEFT:
			...
		}
		*/
	}

}
void ghost_NextMove(Ghost* ghost, Directions next) {
	ghost->objData.nextTryMove = next;
}
void printGhostStatus(GhostStatus S) {

	switch(S){
	
	case BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GO_IN:
		game_log("GO_IN");
		break;
	case FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirec, bool room) {
	// [HACKATHON 2-3]
	// TODO: Determine if the current direction is movable.
	// Basically, this is a ghost version of `pacman_movable`.
	// So if you have finished (and you should), you can just "copy and paste"
	// and do some small alternation.

	
	int x2 = ghost->objData.Coord.x , y2 = ghost->objData.Coord.y;

	switch (targetDirec)
	{
	case UP:
		if (is_wall_block(M, x2, y2 - 1) || (room && is_room_block(M, x2, y2 - 1))) {
			return false;
		}
		else return true;
	case DOWN:
		if (is_wall_block(M, x2, y2 + 1) || (room && is_room_block(M, x2, y2 + 1))) {
			return false;
		}
		else return true;
	case LEFT:
		if (is_wall_block(M, x2-1, y2) || (room && is_room_block(M, x2-1, y2))) {
			return false;
		}
		else return true;
	case RIGHT:
		if (is_wall_block(M, x2+1, y2) || (room && is_room_block(M, x2+1, y2))) {
			return false;
		}
		else return true;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
	}
}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {
	// [TODO]
	// TODO: Here is reserved for power bean implementation.
	// The concept is "When pacman eats the power bean, only
	// ghosts who are in state FREEDOM will change to state FLEE.
	// For those who are not (BLOCK, GO_IN, etc.), they won't change state."
	// This implementation is based on the classic PACMAN game.
	// You are allowed to do your own implementation of power bean system.
	
	//lws power bean 1
		if(setFLEE){
			if(ghost->status == FREEDOM){
				ghost->status = FLEE;
				int speed = 4;
			}
		}else{
			if (ghost->status == FLEE)
				ghost->status = FLEE;

		}
	
}

void ghost_collided(Ghost* ghost) {
	if (ghost->status == FLEE) {
		ghost->status = GO_IN;
		ghost->speed = 4;
	}
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	// [TODO]
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.
	static Directions proba[4]; // possible movement
	int cnt = 0;
	for (Directions i = 1;i <= 4;i++) {
		if (i != shortestDirection) {
			proba[cnt++] = i;
		}
	}
	ghost_NextMove(ghost, proba[generateRandomNumber(0, cnt - 1)]);//lws power bean 2

}

