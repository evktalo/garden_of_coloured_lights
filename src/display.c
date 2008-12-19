/*
 * This file is part of 'Garden of coloured lights'.
 *
 * 'Garden of coloured lights' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 'Garden of coloured lights' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 'Garden of coloured lights'.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "config.h"

#include "allegro.h"

#include "globvars.h"
#include "palette.h"
#include "display.h"
#include "game.h"
#include "stuff.h"

#define TRANS_MODE drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
#define END_TRANS_MODE drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

extern RGB palet[2][256];


BITMAP *display;

extern volatile int frames_per_second;
extern int slacktime;
extern int long_slacktime_store;

BITMAP *underlay;

struct RLE_STRUCT eRLE_flower[5][50];
RLE_SPRITE *platform_RLE[PLATFORM_RLES];
int platform_tile[9][PLAT_Y];
int platform_position;
int platform_step;
// externed in game.cc

RLE_SPRITE *island1_RLE[ISLAND1_RLES];
int island1_tile[9][ISLAND1_Y];
int island1_position;
int island1_step;

int star_x[NO_STARS][3];
int star_y[NO_STARS][3];
int star_y_speed[NO_STARS][3];
//int star_col [NO_STARS] [3];

RLE_SPRITE *bone_RLE[BONE_RLES];

RLE_SPRITE *trans_RLE[TRANS_RLES];


void draw_background (void);
void draw_background2 (void);
void draw_background3 (void);
void draw_background4 (void);

void shift_message (void);

int damage_per_time;



void indicate_fps (BITMAP * bmp, int play);

void draw_player (int show);
void draw_HUD (void);
int detect_player_collision (void);
int check_pixel (int x, int y);

void draw_enemies (void);
void draw_an_enemy (int e);
void draw_clouds (void);
void draw_a_cloud (int c);
void draw_darter1 (int x, int y, int xa, int ya);
void draw_darter2 (int x, int y, int out2, int out1);
void draw_stopper1 (int x, int y, int front_down, int fx, int fy, int mx,
                    int my, int bx, int by);
void draw_carrier1 (int x, int y, int out);
void draw_carrier1_part (int x, int y, int out, int out2);
void draw_diver1 (int x, int y, int xa, int ya);
void draw_pod1 (int x, int y, int out, int up_out, int e);

void draw_ebullets (void);
void draw_an_ebullet (int b);
void draw_dart (int x, int y, int angle, int out, int timeout, int cord_col,
                int col);
void draw_diamond (int x, int y, int angle, int out1, int out2,
                   int cord_colour);

void draw_pbullets (void);
void draw_a_pbullet (int b);

void fcircle (int x, int y, int size, int col);

void pline (BITMAP * bmp, int x1, int y1, int x2, int y2, int colour);
void poly4 (BITMAP * target, int x1, int y1, int x2, int y2, int x3, int y3,
            int x4, int y4, int col);

struct RLE_STRUCT dart_bullet[SMALL_ROTATIONS][9];
struct RLE_STRUCT diamond_bullet[SMALL_ROTATIONS][9];
struct RLE_STRUCT sdart_bullet[SMALL_ROTATIONS][9];

struct RLE_STRUCT RLE_beam[SMALL_ROTATIONS];

//struct RLE_STRUCT splinter [SMALL_ROTATIONS];

RLE_SPRITE *RLE_circle[RLE_CIRCLES][9];

struct RLE_STRUCT RLE_mshock[4][20];
struct RLE_STRUCT RLE_lshock[4][50];
struct RLE_STRUCT RLE_hshock[4][50];
struct RLE_STRUCT RLE_blade[SMALL_ROTATIONS];
struct RLE_STRUCT RLE_lwbeamshock[15];
struct RLE_STRUCT RLE_rshock[5][20];
struct RLE_STRUCT RLE_mg_shock[10];
struct RLE_STRUCT RLE_ring[20];

struct RLE_STRUCT petal1[SMALL_ROTATIONS];
struct RLE_STRUCT petal2[SMALL_ROTATIONS];
struct RLE_STRUCT petal3[SMALL_ROTATIONS];


RLE_SPRITE *eRLE_small1[S_ENEMY_RLES];
RLE_SPRITE *eRLE_large1[L_ENEMY_RLES];

struct RLE_STRUCT RLE_multi[WPN_TYPES][8];

int points4[8];

void print_number (int x, int y, int n);

#ifdef TRUE


#define TRACK_SIZE 10

struct trackstruct
{
	int type;
	int multiple;
	int count;
	int count2;
//  int next_track;
	int delay;
	int total;
	int x[TRACK_SIZE + 1];
	int y[TRACK_SIZE + 1];
	int reverse;

	int style;

	int x_target[MAX_TARGETS];
	int y_target[MAX_TARGETS];
	int target_time;

};


extern struct trackstruct track[TRACK_SIZE];

extern int next_track;
extern int overtrack_pos;
extern int overtrack_pos2;

#endif

void run_display (int show)
{
	clear_to_color (display, COL_OUTLINE);

	if (show == 1)
	{
		switch (arena.level)
		{
		default:
			draw_background ();
			break;
		case 2:
			draw_background2 ();
			break;
		case 3:
			draw_background3 ();
			break;
		case 4:
			draw_background4 ();
			break;
		}
		draw_enemies ();
	}
	if (player.in_play == 1)
		draw_player (show);
	draw_ebullets ();

	if (player.in_play == 1 && player.grace == 0)
	{
		if (detect_player_collision () == 1)
			player_hit ();
	}

	if (show == 0)
		return;			// skip the frame


	draw_pbullets ();
	draw_clouds ();

	draw_HUD ();
//#define SDFSDFG
//  int i, count = 0, ypos = 100;
#ifdef SDFSDFG
	textprintf_ex (display, font, 10, ypos, -1, -1, "%i fps",
	               frames_per_second);
	textprintf_ex (display, font, 10, ypos + 15, -1, -1,
	               "Press Escape to quit");
	for (i = 0; i < NO_ENEMIES; i++)
	{
		if (enemy[i].type != ENEMY_NONE)
			count++;
	}
	textprintf_ex (display, font, 10, ypos + 30, -1, -1, "e %i", count);
	count = 0;
	for (i = 0; i < NO_EBULLETS; i++)
	{
		if (ebullet[i].type != EBULLET_NONE)
			count++;
	}
	textprintf_ex (display, font, 10, ypos + 45, -1, -1, "b %i", count);
	count = 0;
	for (i = 0; i < NO_PBULLETS; i++)
	{
		if (pbullet[i].type != PBULLET_NONE)
			count++;
	}
	textprintf_ex (display, font, 10, ypos + 60, -1, -1, "p %i", count);
	count = 0;
	for (i = 0; i < NO_CLOUDS; i++)
	{
		if (cloud[i].type != CLOUD_NONE)
			count++;
	}
	textprintf_ex (display, font, 10, ypos + 75, -1, -1, "c %i", count);

	/*  print_number(10, ypos + 90, player.wxp [0]);
	  print_number(10, ypos + 105, player.wxp [1]);
	  print_number(10, ypos + 120, player.wxp [2]);*/
	print_number (10, ypos + 180, damage_per_time);

	print_number (10, ypos + 90, track[0].count);
	print_number (10, ypos + 105, track[1].count);
	print_number (10, ypos + 120, track[2].count);
	print_number (10, ypos + 135, next_track);
	print_number (10, ypos + 150, overtrack_pos);
	print_number (10, ypos + 165, overtrack_pos2);
#endif
//  textprintf_ex(display, font, 10, 10, COL_WHITE, -1, "%i fps", frames_per_second);

	if (arena.level_finished > 0 && arena.level_finished < 140)
	{

		int y = 140 - arena.level_finished;
		if (y > 17)
			y = 17;
		if (arena.level_finished < 17)
			y = arena.level_finished;
		int x = 140 - arena.level_finished;
		x *= 4;
		if (x > 110)
			x = 110;
		if (arena.level_finished < 20)
		{
			x = arena.level_finished * 4;
			if (x > 110)
				x = 110;
		}

		rectfill (display, 278, 188 - y * 3, 292, 188 + y * 3, TRANS_B3_IN - 1);
		rect (display, 277, 187 - y * 3, 293, 189 + y * 3, TRANS_B3_OUT - 1);

		rectfill (display, 348, 188 - y * 3, 362, 188 + y * 3, TRANS_B3_IN - 1);
		rect (display, 347, 187 - y * 3, 363, 189 + y * 3, TRANS_B3_OUT - 1);

		rectfill (display, 240, 188 - y * 2, 250, 188 + y * 2, TRANS_B2_IN - 1);
		rect (display, 239, 187 - y * 2, 251, 189 + y * 2, TRANS_B2_OUT - 1);

		rectfill (display, 390, 188 - y * 2, 400, 188 + y * 2, TRANS_B2_IN - 1);
		rect (display, 389, 187 - y * 2, 401, 189 + y * 2, TRANS_B2_OUT - 1);


		rectfill (display, 320 - x, 188 - y, 320 + x, 188 + y, TRANS_B5_IN - 1);
		rect (display, 319 - x, 187 - y, 321 + x, 189 + y, TRANS_B5_OUT - 1);


		if (arena.level_finished < 120 && arena.level_finished > 20)
			switch (arena.level)
			{
			case 1:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "STAGE  ONE  COMPLETE");
				break;
			case 2:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "STAGE  TWO  COMPLETE");
				break;
			case 3:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "STAGE  THREE  COMPLETE");
				break;
			case 4:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "FINAL  STAGE  COMPLETE");
				break;
			}
//   textprintf_centre_ex(display, font, 320, 200, -1, -1, "SEE  WWW.SHMUP-DEV.COM  FOR  THE  COMPETITION  VERSION!");
	}

	if (arena.game_over > 0)
	{
//  TRANS_MODE
		int y = 200 - arena.game_over;
		if (y > 17)
			y = 17;
		int x = 200 - arena.game_over;
		x *= 4;
		if (x > 80)
			x = 80;

		rectfill (display, 320 - x, 188 - y, 320 + x, 188 + y,
		          TRANS_YELLOW_IN - 1);
		rect (display, 319 - x, 187 - y, 321 + x, 189 + y, TRANS_YELLOW_OUT - 1);

		rectfill (display, 220, 188 - y, 230, 188 + y, TRANS_ORANGE_IN - 1);
		rect (display, 219, 187 - y, 231, 189 + y, TRANS_ORANGE_OUT - 1);
		rectfill (display, 410, 188 - y, 420, 188 + y, TRANS_ORANGE_IN - 1);
		rect (display, 409, 187 - y, 421, 189 + y, TRANS_ORANGE_OUT - 1);
		rectfill (display, 200, 188 - y, 210, 188 + y, TRANS_SH1_IN - 1);
		rect (display, 199, 187 - y, 211, 189 + y, TRANS_SH1_OUT - 1);
		rectfill (display, 430, 188 - y, 440, 188 + y, TRANS_SH1_IN - 1);
		rect (display, 429, 187 - y, 441, 189 + y, TRANS_SH1_OUT - 1);

		if (arena.game_over < 180)
			textprintf_centre_ex (display, font, 320, 180, -1, -1,
			                      "G A M E      O V E R");
	}

	if (arena.new_level_sign > 0)
	{
		int y = 120 - arena.new_level_sign;
		if (y > 17)
			y = 17;
		if (arena.new_level_sign < 17)
			y = arena.new_level_sign;
		int x = 120 - arena.new_level_sign;
		x *= 4;
		if (x > 80)
			x = 80;
		if (arena.new_level_sign < 20)
		{
			x = arena.new_level_sign * 4;
			if (x > 80)
				x = 80;
		}

		rectfill (display, 278, 188 - y * 3, 292, 188 + y * 3, TRANS_B3_IN - 1);
		rect (display, 277, 187 - y * 3, 293, 189 + y * 3, TRANS_B3_OUT - 1);

		rectfill (display, 348, 188 - y * 3, 362, 188 + y * 3, TRANS_B3_IN - 1);
		rect (display, 347, 187 - y * 3, 363, 189 + y * 3, TRANS_B3_OUT - 1);

		rectfill (display, 250, 188 - y * 2, 260, 188 + y * 2, TRANS_B2_IN - 1);
		rect (display, 249, 187 - y * 2, 261, 189 + y * 2, TRANS_B2_OUT - 1);

		rectfill (display, 380, 188 - y * 2, 390, 188 + y * 2, TRANS_B2_IN - 1);
		rect (display, 379, 187 - y * 2, 391, 189 + y * 2, TRANS_B2_OUT - 1);


		rectfill (display, 320 - x, 188 - y, 320 + x, 188 + y, TRANS_B5_IN - 1);
		rect (display, 319 - x, 187 - y, 321 + x, 189 + y, TRANS_B5_OUT - 1);


		/*
		  rectfill(display, 220, 188 - y, 230, 188 + y, TRANS_B3_IN - 1);
		  rect(display, 219, 187 - y, 231, 189 + y, TRANS_B3_OUT - 1);

		  rectfill(display, 410, 188 - y, 420, 188 + y, TRANS_B3_IN - 1);
		  rect(display, 409, 187 - y, 421, 189 + y, TRANS_B3_OUT - 1);

		  rectfill(display, 200, 188 - y, 210, 188 + y, TRANS_B2_IN - 1);
		  rect(display, 199, 187 - y, 211, 189 + y, TRANS_B2_OUT - 1);

		  rectfill(display, 430, 188 - y, 440, 188 + y, TRANS_B2_IN - 1);
		  rect(display, 429, 187 - y, 441, 189 + y, TRANS_B2_OUT - 1);
		*/

		if (arena.new_level_sign < 100 && arena.new_level_sign > 20)
			switch (arena.level)
			{
			case 1:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "S T A G E   O N E");
				break;
			case 2:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "S T A G E   T W O");
				break;
			case 3:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "S T A G E   T H R E E");
				break;
			case 4:
				textprintf_centre_ex (display, font, 320, 180, -1, -1,
				                      "F I N A L   S T A G E");
				break;
			}
	}

	if (arena.shift_message > 0)
		shift_message ();

	if (arena.change_palette != -1)
	{
//   int newpal = build_new_palette(arena.change_palette, 0, 0, 0);
		int newpal = build_new_palette (arena.level, 0, 0, 0);
//   vsync();
		if (options.run_vsync > 0)
			vsync ();
		if (arena.new_level_sign > 118)
			clear_bitmap (screen);
		else
			blit (display, screen, 0, 0, 0, 0, 640, 480);
//   vsync();
		set_palette (palet[newpal]);
		if (arena.shift_palette == 0)
			arena.change_palette = -1;
	}

	else
	{
		if (options.run_vsync == 2)
			vsync ();

		/*    clear_bitmap(display);

		 textprintf_right_ex(display, font, 600, 200, -1, -1, "LOADING");
		 textprintf_right_ex(display, font, 600, 240, -1, -1, "PLEASE WAIT");

		 textprintf_right_ex(display, font, 600, 260, -1, -1, "WWW.SHMUP-DEV.COM");
		 textprintf_right_ex(display, font, 600, 280, -1, -1, "2007 OPTIONS COMPETITION");
		*/


		blit (display, screen, 0, 0, 0, 0, 640, 480);
	}




}

int detect_player_collision (void)
{

	int x = player.x / GRAIN;
	int y = player.y / GRAIN;

	if (check_pixel (x, y)
	        || check_pixel (x + 1, y + 1)
	        || check_pixel (x + 1, y - 1)
	        || check_pixel (x - 1, y + 1) || check_pixel (x - 1, y - 1))
		return 1;

	if (				//check_pixel(x, y) ||
	    check_pixel (x - 2, y - 2)
	    || check_pixel (x + 2, y - 2)
	    || check_pixel (x - 2, y + 2)
	    || check_pixel (x + 2, y + 2)
	    || check_pixel (x + 2, y + 2) || check_pixel (x, y + 2))
//   || check_pixel(x, y - 3)
//   || check_pixel(x - 1, y - 3)
		//|| check_pixel(x + 1, y - 3))
		return 1;

	return 0;

}

int check_pixel (int x, int y)
{
	int pix = getpixel (display, x, y);

	if ((pix >= TRANS_B1_IN && pix < TRANS_B2_OUT)
	        || (pix >= TRANS_B2_IN && pix < TRANS_B3_OUT)
	        || (pix >= TRANS_B3_IN && pix < TRANS_B4_OUT)
	        || (pix >= TRANS_B4_IN && pix < TRANS_B5_OUT) || (pix >= TRANS_B5_IN))
		return 1;


	return 0;
}



void draw_player (int show)
{

	int x = player.x / GRAIN;
	int y = player.y / GRAIN;
	int xa, ya, xb = 0, yb;

//      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_PLAYER], x - 10, y - 7);

// draw_rle_sprite(display, eRLE_small1 [xa], x - 10, y - 7);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_PLAYER_UP_L],
	                 x - 11 + player.wings1 / 10, y - 8);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_PLAYER_UP_R],
	                 x + 2 + player.wings1 / 10, y - 8);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_PLAYER_DOWN_L],
	                 x - 10 - player.wings2 / 10, y + 2 - player.wings2 / 10);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_PLAYER_DOWN_R],
	                 x + 2 + player.wings2 / 10, y + 2 - player.wings2 / 10);

	xa = S_ENEMY_PLAYER1;
	if (player.recycle_speed > 0 && player.recycle > 0)
	{
//    ya = player.recycle / player.recycle_speed;
		/*    ya /= 8;
		    if (ya < 0)
		     ya = 0;
		    if (ya > 2)
		     ya = 2;
		    xa += ya;*/
		xa = S_ENEMY_PLAYER1;
		ya = 0;
		draw_rle_sprite (display, eRLE_small1[xa], x - 7 - ya, y - 2);
	}

	if (show == 0)
		return;
//  if (grand(3) == 0)
	//rect(display, x-2, y-2, x+2, y+2, COL_COL1 + grand(4));
//      circlefill(display, x, y, 15, COL_WHITE);
//      circle(display, x, y, 15, COL_OUTLINE);
//      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
// rectfill(display, x - 5, y - 4, x + 5, y + 4, COL_WHITE);
// rect(display, x - 6, y - 5, x + 6, y + 5, COL_OUTLINE);
	int i, j, old_x, old_y, old_x2, old_y2, k = 0;


	for (i = 0; i < 3; i++)
	{

		xa = player.wx[i] / GRAIN;
		ya = player.wy[i] / GRAIN;
		k = 0;
		if (player.wstyle[i] == WSTYLE_ORBITAL)
		{
			/*     xb = ANGLE_2;
			     if (i == 2)
			      xb = ANGLE_3;
			     yb = ANGLE_1 - player.orbital_angle;
			     yb *= 2;
			     for (j = 0; j < i + 1; j ++)
			     {
			       if (player.recycle [i] > 0)
			        fcircle(xa + xpart(yb, 9), ya + ypart(yb, 9), 1, TCOL_ORANGE);
			         else
			          fcircle(xa + xpart(yb, 9), ya + ypart(yb, 9), 2, TCOL_YELLOW);
			       yb += xb;
			     }*/
			xb = TCOL_YELLOW;
			yb = 2;
			if (player.recycle[i] > 0)
			{
				xb = TCOL_ORANGE;
				yb = 1;
			}
			switch (i)
			{
			case 0:
				fcircle (xa, ya + 6, yb + 2, xb);
				break;
			case 1:
				fcircle (xa + 5, ya + 5, yb + 1, xb);
				fcircle (xa - 5, ya + 5, yb + 1, xb);
				break;
			case 2:
				fcircle (xa, ya + 6, yb + 1, xb);
				fcircle (xa + 7, ya + 5, yb + 0, xb);
				fcircle (xa - 7, ya + 5, yb + 0, xb);
				break;
			}
		}
		switch (player.weapon[i])
		{
		case WPN_CANNON:
			if (player.wfired == i)
			{
				k = (player.recycle[i] - 500) / 100;
				if (k < 0)
					k = 0;
				if (k > 3)
					k = 3;
			}
			draw_rle_sprite (display, RLE_multi[WPN_CANNON][k].sprite,
			                 xa - RLE_multi[WPN_CANNON][k].x,
			                 ya - RLE_multi[WPN_CANNON][k].y);
			break;
		case WPN_MGUN:
//    if (player.wfired == i)
		{
			k = (player.recycle[i]) / 180;
			if (k < 0)
				k = 0;
			if (k > 4)
				k = 4;
		}
		draw_rle_sprite (display, RLE_multi[WPN_MGUN][k].sprite,
		                 xa - RLE_multi[WPN_MGUN][k].x,
		                 ya - RLE_multi[WPN_MGUN][k].y - 3);
		break;
		case WPN_SCATTER:
//    if (player.wfired == i)
		{
			k = (player.recycle[i]) / 330;
			if (k < 0)
				k = 0;
			if (k > 3)
				k = 3;
		}
		draw_rle_sprite (display, RLE_multi[WPN_SCATTER][k].sprite,
		                 xa - RLE_multi[WPN_SCATTER][k].x,
		                 ya - RLE_multi[WPN_SCATTER][k].y);
		break;
		case WPN_SEEKER:
//    if (player.wfired == i)
		{
			k = (player.recycle[i] - 300) / 60;
			if (k < 0)
				k = 0;
			if (k > 4)
				k = 4;
		}
		draw_rle_sprite (display, RLE_multi[WPN_SEEKER][k].sprite,
		                 xa - RLE_multi[WPN_SEEKER][k].x,
		                 ya - RLE_multi[WPN_SEEKER][k].y);
		break;
		case WPN_RECT:
//    if (player.wfired == i)
		{
			k = (player.recycle[i]) / 60 - 10;
			if (k < 0)
				k = 0;
			if (k > 3)
				k = 3;
		}
		draw_rle_sprite (display, RLE_multi[WPN_RECT][k].sprite,
		                 xa - RLE_multi[WPN_RECT][k].x,
		                 ya - RLE_multi[WPN_RECT][k].y);
		break;
		case WPN_LWBEAM:
			/*    if (player.wfired == i)
			    {
			     k = (player.recycle) / 330;
			     if (k < 0)
			      k = 0;
			     if (k > 3)
			      k = 3;
			    }*/
			k = 0;
			if (player.beam_open[i] > 4)
				k = 1;
			if (player.beam_open[i] > 7)
				k = 2;
			draw_rle_sprite (display, RLE_multi[WPN_LWBEAM][k].sprite,
			                 xa - RLE_multi[WPN_LWBEAM][k].x,
			                 ya - RLE_multi[WPN_LWBEAM][k].y);
			break;
		case WPN_SWBEAM:
//    if (player.wfired == i)
		{
			k = (player.recycle[i]) / 330;
			if (k < 0)
				k = 0;
			if (k > 2)
				k = 2;
		}
		draw_rle_sprite (display, RLE_multi[WPN_SWBEAM][k].sprite,
		                 xa - RLE_multi[WPN_SWBEAM][k].x,
		                 ya - RLE_multi[WPN_SWBEAM][k].y);
		break;
		case WPN_EIGHT:
			draw_rle_sprite (display, RLE_multi[WPN_EIGHT][k].sprite,
			                 xa - RLE_multi[WPN_EIGHT][k].x,
			                 ya - RLE_multi[WPN_EIGHT][k].y);
			break;
		default:
			circlefill (display, xa, ya, 3, COL_LGREY);
			circle (display, xa, ya, 3, COL_OUTLINE);
			break;
		case WPN_BLADE:
			if (player.swing_state > BLADE_RIGHT)
				draw_rle_sprite (display, RLE_multi[WPN_BLADE][1].sprite, xa - 3,
				                 ya - 3);
			else
				draw_rle_sprite (display, RLE_multi[WPN_BLADE][0].sprite, xa - 4,
				                 ya - 4);
			xb =
			    RLE_blade[player.swing_sprite_angle].x -
			    xpart (player.swing_sprite_angle * (ANGLE_1 / SMALL_ROTATIONS), 14);
			yb =
			    RLE_blade[player.swing_sprite_angle].y -
			    ypart (player.swing_sprite_angle * (ANGLE_1 / SMALL_ROTATIONS), 14);
			draw_trans_rle_sprite (display,
			                       RLE_blade[player.swing_sprite_angle].sprite,
			                       xa - xb, ya - yb);
			j = player.swing_pos;
			k = 0;
			TRANS_MODE
			do
			{
				old_x = player.swing_x[j] / GRAIN;
				old_y = player.swing_y[j] / GRAIN;
				old_x2 = player.swing_x2[j] / GRAIN;
				old_y2 = player.swing_y2[j] / GRAIN;
				j--;
				if (j == -1)
					j = 9;
				k++;
				if (j == player.swing_pos || k >= player.swing_length)
					break;
				pline (display, old_x, old_y, player.swing_x[j] / GRAIN,
				       player.swing_y[j] / GRAIN, TRANS_YELLOW_OUT);
				xa = (player.swing_length - k);
				xa /= player.swing_strength[j];
				xb = player.wlevel[player.blade_index];
//    xa *= 10 + xb;
//    xa /= 10;
				fcircle (player.swing_x[j] / GRAIN, player.swing_y[j] / GRAIN, xa,
				         TCOL_YELLOW);
				if (xb > 3)
				{
					xa *= 10 + xb;
					xa /= 10;
					fcircle (player.swing_x[j] / GRAIN, player.swing_y[j] / GRAIN, xa,
					         TCOL_ORANGE);
				}
			}
			while (TRUE);
			END_TRANS_MODE break;	//  end blade
		}


	}

// print_number(100,100,player.swing_hold);

	for (i = 0; i < 3; i++)
	{

		if (player.beam[i] != -1 || player.beam_flicker[i] > 0)
		{
			xa = player.wx[i] / GRAIN;
			ya = player.wy[i] / GRAIN - 4;
			if (player.beam_flicker[i] == 0)
			{
				drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
				if (player.wlevel[i] == 0)
				{
					vline (display, xa, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 1, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 1, ya, 0, TRANS_ORANGE_OUT);
					xb = (player.beam_counter[i] / 3) % 15;
				}
				if (player.wlevel[i] > 0 && player.wlevel[i] <= 3)
				{
					vline (display, xa, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 2, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 2, ya, 0, TRANS_ORANGE_OUT);
					xb = (player.beam_counter[i] / 2) % 15;
				}
				if (player.wlevel[i] > 3 && player.wlevel[i] <= 6)
				{
					vline (display, xa, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 2, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 2, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 3, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 3, ya, 0, TRANS_ORANGE_OUT);
					xb = (player.beam_counter[i]) % 15;
				}
				if (player.wlevel[i] > 6 && player.wlevel[i] <= 8)
				{
					vline (display, xa, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 2, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 2, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 3, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 3, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa - 6, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 6, ya, 0, TRANS_ORANGE_OUT);
					xb = (player.beam_counter[i]) % 15;
				}
				if (player.wlevel[i] > 8)
				{
					vline (display, xa, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 1, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 2, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 2, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 3, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 3, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa - 6, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 6, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa - 7, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa + 7, ya, 0, TRANS_ORANGE_IN);
					vline (display, xa - 8, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 8, ya, 0, TRANS_ORANGE_OUT);
					xb = (player.beam_counter[i]) % 15;
				}
			}
			else
			{
				if (player.wlevel[i] == 0)
					xb = (player.beam_counter[i] / 3) % 15;
				if (player.wlevel[i] > 0 && player.wlevel[i] <= 3)
					xb = (player.beam_counter[i] / 2) % 15;
				if (player.wlevel[i] > 3 && player.wlevel[i] <= 6)
					xb = (player.beam_counter[i]) % 15;
				if (player.wlevel[i] > 6 && player.wlevel[i] <= 8)
					xb = (player.beam_counter[i]) % 15;
				if (player.wlevel[i] > 8)
					xb = (player.beam_counter[i]) % 15;
			}
			fcircle (xa, ya - 3, 9 + grand (5), TCOL_ORANGE);
			fcircle (xa, ya - 2 - grand (3), 3 + grand (3), TCOL_YELLOW);
			if (xb < 1)
				xb = 1;
			if (xb > 15)
				xb = 15;
			yb = RLE_lwbeamshock[xb].x;
			draw_trans_rle_sprite (display, RLE_lwbeamshock[xb].sprite, xa - yb,
			                       ya - yb - 3);

			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		if (player.beam_fade[i] > 0 || player.beam_flicker[i] > 0)
		{
			xa = player.wx[i] / GRAIN;
			ya = player.wy[i] / GRAIN - 4;
			fcircle (xa, ya - 3, player.beam_fade[i] + grand (4), TCOL_ORANGE);
//     if (player.beam_fade [i] > 5
//         || (player.wlevel [i] > 0 && player.beam_flicker [i] > 0))
			if (player.wlevel[i] > 0
			        && (player.beam_fade[i] > 5 || player.beam_flicker[i] > 0))
			{
				TRANS_MODE vline (display, xa, ya, 0, TRANS_ORANGE_IN);
				vline (display, xa - 1, ya, 0, TRANS_ORANGE_OUT);
				vline (display, xa + 1, ya, 0, TRANS_ORANGE_OUT);
				/*      rectfill(display, xa - 55, 0, xa + 55, ya, TRANS_SH1_IN);
				      rect(display, xa - 56, 0, xa + 56, ya + 1, TRANS_SH1_OUT);
				      rectfill(display, 0, 100, 640, 150, TRANS_SH1_IN);
				      rectfill(display, 0, 200, 640, 350, TRANS_SH2_IN);

				      rectfill(display, 100, 0, 150, 480, TRANS_SH1_OUT);
				      rectfill(display, 400, 0, 450, 480, TRANS_SH2_OUT);*/


				if (player.wlevel[i] > 6)
				{
					vline (display, xa - 6, ya, 0, TRANS_ORANGE_OUT);
					vline (display, xa + 6, ya, 0, TRANS_ORANGE_OUT);
				}
				END_TRANS_MODE
			}
			else if (player.beam_fade[i] > 1)
			{
				TRANS_MODE vline (display, xa, ya, 0, TRANS_ORANGE_OUT);
				/*        if (player.wlevel [player.beam_index] > 6)
				        {
				         vline(display, xa - 5, ya, 0, TRANS_ORANGE_OUT);
				         vline(display, xa + 5, ya, 0, TRANS_ORANGE_OUT);
				        }*/
				END_TRANS_MODE
			}
//     if (player.beam_fade > 6)
//      fcircle(xa, ya - 2 - grand(3), player.be + grand(3), 8);
		}


		if (player.swbeam[i] > 0)
		{
			xa = player.swbeam_x1[i] / GRAIN;
			ya = player.swbeam_y1[i] / GRAIN - 4;
			TRANS_MODE xb = (player.swbeam[i] - 4) / 3;
			if (xb > -1)
			{
				rectfill (display, xa - xb, ya, xa + xb, 0, TRANS_YELLOW_IN);
				vline (display, xa - xb - 1, ya, 0, TRANS_YELLOW_OUT);
				vline (display, xa + xb + 1, ya, 0, TRANS_YELLOW_OUT);
			}
			else
				vline (display, xa, ya, 0, TRANS_YELLOW_OUT);
			xb = player.swbeam[i] - 1;
			if (xb > 1)
			{
				rectfill (display, xa - xb, ya, xa + xb, 0, TRANS_ORANGE_IN);
				vline (display, xa - xb - 1, ya, 0, TRANS_ORANGE_OUT);
				vline (display, xa + xb + 1, ya, 0, TRANS_ORANGE_OUT);
			}
			else
				vline (display, xa, ya, 0, TRANS_ORANGE_OUT);
			fcircle (xa, ya - 3, (xb * 2) + 5 + grand (5), TCOL_ORANGE);
			fcircle (xa, ya - 2 - grand (3), (xb * 2) + 3 + grand (3), TCOL_YELLOW);
			END_TRANS_MODE
		}

	}

	for (i = 0; i < 3; i++)
	{
		if (player.wgained[i] > 0)
		{
			xa = player.wx[i] / GRAIN;
			ya = player.wy[i] / GRAIN;
			xb = player.wgained[i] / 3;
			yb = 25 - player.wgained[i];
			TRANS_MODE
			rectfill (display, 0, ya - xb, xa - yb, ya + xb, TRANS_ORANGE_IN);
			hline (display, 0, ya - xb - 1, xa - yb, TRANS_ORANGE_OUT);
			hline (display, 0, ya + xb + 1, xa - yb, TRANS_ORANGE_OUT);
			rectfill (display, xa + yb, ya - xb, 640, ya + xb, TRANS_ORANGE_IN);
			hline (display, xa + yb, ya - xb - 1, 640, TRANS_ORANGE_OUT);
			hline (display, xa + yb, ya + xb + 1, 640, TRANS_ORANGE_OUT);
			END_TRANS_MODE xb = 15 - player.wgained[i];
			if (xb < 1)
				xb = 1;
			if (xb > 14)
				xb = 14;
			yb = RLE_lwbeamshock[xb].x;
			draw_trans_rle_sprite (display, RLE_lwbeamshock[xb].sprite, xa - yb,
			                       ya - yb - 3);
		}
	}

#define SPIN_SPEED 16

	if (player.grace > 0)
	{
		xa = player.x / GRAIN;
		ya = player.y / GRAIN;
		xb = player.grace / 3;
		if (xb > 5)
			xb = 5;
		yb = 25 + xpart (arena.counter * 64, 8);
		fcircle (xa + xpart (arena.counter * SPIN_SPEED, yb),
		         ya + ypart (arena.counter * SPIN_SPEED, yb), xb, TCOL_ORANGE);
		fcircle (xa + xpart (arena.counter * SPIN_SPEED + ANGLE_4, yb),
		         ya + ypart (arena.counter * SPIN_SPEED + ANGLE_4, yb), xb,
		         TCOL_ORANGE);
		fcircle (xa + xpart (arena.counter * SPIN_SPEED - ANGLE_4, yb),
		         ya + ypart (arena.counter * SPIN_SPEED - ANGLE_4, yb), xb,
		         TCOL_ORANGE);
		fcircle (xa + xpart (arena.counter * SPIN_SPEED - ANGLE_2, yb),
		         ya + ypart (arena.counter * SPIN_SPEED - ANGLE_2, yb), xb,
		         TCOL_ORANGE);

//  yb = 25 + xpart(ANGLE_1 - ((arena.counter * 32) & 1023) + ANGLE_6, 13);
		yb = 25 - xpart (arena.counter * 64, 8);
		fcircle (xa + xpart (ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb),
		         ya + ypart (ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb),
		         xb, TCOL_YELLOW);
		fcircle (xa +
		         xpart (ANGLE_4 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023),
		                yb),
		         ya + ypart (ANGLE_4 + ANGLE_1 -
		                     ((arena.counter * SPIN_SPEED) & 1023), yb), xb,
		         TCOL_YELLOW);
		fcircle (xa +
		         xpart (ANGLE_2 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023),
		                yb),
		         ya + ypart (ANGLE_2 + ANGLE_1 -
		                     ((arena.counter * SPIN_SPEED) & 1023), yb), xb,
		         TCOL_YELLOW);
		fcircle (xa +
		         xpart (-ANGLE_4 + ANGLE_1 -
		                ((arena.counter * SPIN_SPEED) & 1023), yb),
		         ya + ypart (-ANGLE_4 + ANGLE_1 -
		                     ((arena.counter * SPIN_SPEED) & 1023), yb), xb,
		         TCOL_YELLOW);
		/*  xb *= 15;
		  xb /= 10;
		  fcircle(xa + xpart(ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), ya + ypart(ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), xb, TCOL_ORANGE);
		  fcircle(xa + xpart(ANGLE_4 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), ya + ypart(ANGLE_4 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), xb, TCOL_ORANGE);
		  fcircle(xa + xpart(ANGLE_2 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), ya + ypart(ANGLE_2 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), xb, TCOL_ORANGE);
		  fcircle(xa + xpart(- ANGLE_4 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), ya + ypart(- ANGLE_4 + ANGLE_1 - ((arena.counter * SPIN_SPEED) & 1023), yb), xb, TCOL_ORANGE);*/
	}

}

void draw_HUD (void)
{

	int i, k, xa, ya;

	textprintf_right_ex (display, font, 635, 10, -1, -1, "%i", player.score);

	textprintf_right_ex (display, font, 635, 45, -1, -1, "%i%%",
	                     arena.phase_bonus);
// textprintf_right_ex(display, font, 635, 80, -1, -1, "%i", arena.max_stage_score);

//  print_number(10, 180, damage_per_time);

	for (i = 0; i < player.lives; i++)
	{
		rectfill (display, 635 - i * 10 - 2, 30, 635 - i * 10 - 10, 38,
		          COL_WHITE);
		rect (display, 635 - i * 10 - 2, 30, 635 - i * 10 - 10, 38, COL_OUTLINE);

	}

	for (i = 0; i < 3; i++)
	{
		xa = 2;
		ya = 15 * i + 1;
		rect (display, 2, ya, 58, ya + 14, COL_OUTLINE);
		rect (display, 3, ya + 1, 57, ya + 13, COL_LGREY);
		rect (display, 4, ya + 2, 56, ya + 12, COL_OUTLINE);
		TRANS_MODE if (player.rpixels[i] > 0)
		{
			if (player.rpixels[i] == 50)
			{
				rect (display, 5, ya + 3, 5 + player.rpixels[i], ya + 11,
				      TRANS_YELLOW_OUT);
				rectfill (display, 6, ya + 4, 4 + player.rpixels[i], ya + 10,
				          TRANS_YELLOW_IN);
			}
			else
			{
				rect (display, 5, ya + 3, 5 + player.rpixels[i], ya + 11,
				      TRANS_ORANGE_OUT);
				rectfill (display, 6, ya + 4, 4 + player.rpixels[i], ya + 10,
				          TRANS_ORANGE_IN);
			}
		}
		END_TRANS_MODE xa = 2;
		ya = 19 * i + 50;
		if (player.wlevel[i] < 9)
		{
			rect (display, 2, ya, 2 + player.wpixels2[i] + 6, ya + 12, COL_OUTLINE);
			rect (display, 3, ya + 1, 2 + player.wpixels2[i] + 5, ya + 11,
			      COL_LGREY);
			rect (display, 4, ya + 2, 2 + player.wpixels2[i] + 4, ya + 10,
			      COL_OUTLINE);
			TRANS_MODE if (player.wpixels[i] > 0)
			{
				rect (display, 5, ya + 3, 5 + player.wpixels[i] + 1, ya + 9,
				      TRANS_ORANGE_OUT);
				rectfill (display, 6, ya + 4, 5 + player.wpixels[i] + 0, ya + 8,
				          TRANS_ORANGE_IN);
			}
			END_TRANS_MODE
		}
		TRANS_MODE for (k = 0; k < player.wlevel[i]; k++)
		{
			rect (display, 4 + k * 5, ya + 13, 7 + k * 5, ya + 18,
			      TRANS_YELLOW_OUT);
			rectfill (display, 5 + k * 5, ya + 14, 6 + k * 5, ya + 17,
			          TRANS_YELLOW_IN);
		}
		END_TRANS_MODE
	}

	if (boss.fight)
	{

		int line_x = (boss.hp * 100) / boss.max_hp;
		if (line_x > boss.hp_bar_size)
			line_x = boss.hp_bar_size;
		TRANS_MODE
//  if (boss.hp > 0)
		if (line_x > 0)
		{
			rectfill (display, 320 - line_x + 1, 10, 320 + line_x - 1, 15,
			          TRANS_YELLOW_IN);
			rect (display, 320 - line_x, 9, 320 + line_x, 16, TRANS_YELLOW_OUT);
		}
		END_TRANS_MODE line_x = boss.hp_bar_size;
		rect (display, 320 - 1 - line_x, 8, 320 + 1 + line_x, 17, COL_OUTLINE);
		rect (display, 320 - 2 - line_x, 7, 320 + 2 + line_x, 18, COL_WHITE);
		rect (display, 320 - 3 - line_x, 6, 320 + 3 + line_x, 19, COL_OUTLINE);

	}

}

void draw_enemies (void)
{

	int e;

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
			continue;
		if (eclass[enemy[e].type].priority == 0)
			draw_an_enemy (e);
	}

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
			continue;
		if (eclass[enemy[e].type].priority == 1)
			draw_an_enemy (e);
	}

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
			continue;
		if (eclass[enemy[e].type].priority == 2)
			draw_an_enemy (e);
	}

}


void draw_an_enemy (int e)
{

	int x = enemy[e].x / GRAIN;
	int y = enemy[e].y / GRAIN;
	int xa, ya, xb, yb;
	int angle;
	int i;

	switch (enemy[e].type)
	{

	case ENEMY_BOSS2:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS2_CORE], x - 27,
		                 y - 14);

		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS2_IN_L],
		                 x - 85 - boss.out_sides, y - 47);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS2_IN_R],
		                 x + 16 + boss.out_sides, y - 47);

		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS2_OUT_L],
		                 x - 104 - boss.out_sides - boss.out_sides2, y - 2);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS2_OUT_R],
		                 x + 73 + boss.out_sides + boss.out_sides2, y - 2);

		circlefill (display, x - 53 - boss.out_sides, y + 40, 10, COL_COL4);
		circle (display, x - 53 - boss.out_sides, y + 40, 10, COL_OUTLINE);
		circlefill (display, x - 53 - boss.out_sides, y + 40, 7, COL_COL1);
		circle (display, x - 53 - boss.out_sides, y + 40, 7, COL_OUTLINE);
		circlefill (display, x - 53 - boss.out_sides + xpart (boss.t_angle[0], 5),
		            y + 40 + ypart (boss.t_angle[0], 5), 2, COL_COL1);
		circle (display, x - 53 - boss.out_sides + xpart (boss.t_angle[0], 5),
		        y + 40 + ypart (boss.t_angle[0], 5), 2, COL_OUTLINE);


		circlefill (display, x - 32 - boss.out_sides, y - 20, 10, COL_COL4);
		circle (display, x - 32 - boss.out_sides, y - 20, 10, COL_OUTLINE);
		circlefill (display, x - 32 - boss.out_sides, y - 20, 7, COL_COL1);
		circle (display, x - 32 - boss.out_sides, y - 20, 7, COL_OUTLINE);
		circlefill (display, x - 32 - boss.out_sides + xpart (boss.t_angle[3], 5),
		            y - 20 + ypart (boss.t_angle[3], 5), 2, COL_COL1);
		circle (display, x - 32 - boss.out_sides + xpart (boss.t_angle[3], 5),
		        y - 20 + ypart (boss.t_angle[3], 5), 2, COL_OUTLINE);

		circlefill (display, x + 53 + boss.out_sides, y + 40, 10, COL_COL4);
		circle (display, x + 53 + boss.out_sides, y + 40, 10, COL_OUTLINE);
		circlefill (display, x + 53 + boss.out_sides, y + 40, 7, COL_COL1);
		circle (display, x + 53 + boss.out_sides, y + 40, 7, COL_OUTLINE);
		circlefill (display, x + 53 + boss.out_sides + xpart (boss.t_angle[2], 5),
		            y + 40 + ypart (boss.t_angle[2], 5), 2, COL_COL1);
		circle (display, x + 53 + boss.out_sides + xpart (boss.t_angle[2], 5),
		        y + 40 + ypart (boss.t_angle[2], 5), 2, COL_OUTLINE);

		circlefill (display, x + 32 + boss.out_sides, y - 20, 10, COL_COL4);
		circle (display, x + 32 + boss.out_sides, y - 20, 10, COL_OUTLINE);
		circlefill (display, x + 32 + boss.out_sides, y - 20, 7, COL_COL1);
		circle (display, x + 32 + boss.out_sides, y - 20, 7, COL_OUTLINE);
		circlefill (display, x + 32 + boss.out_sides + xpart (boss.t_angle[1], 5),
		            y - 20 + ypart (boss.t_angle[1], 5), 2, COL_COL1);
		circle (display, x + 32 + boss.out_sides + xpart (boss.t_angle[1], 5),
		        y - 20 + ypart (boss.t_angle[1], 5), 2, COL_OUTLINE);

		break;

	case ENEMY_BOSS1:

		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_CORE], x - 77,
		                 y - 47);

		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_OUT_L],
		                 x - 103 - boss.out_sides2, y - 32);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_OUT_R],
		                 x + 46 + boss.out_sides2, y - 32);

		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_UL],
		                 x - 101 - boss.out_sides2, y - 76 - boss.out_out_up);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_UR],
		                 x + boss.out_sides2, y - 76 - boss.out_out_up);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_DL],
		                 x - 101 - boss.out_sides2, y + 13 + boss.out_out_up);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_DR],
		                 x + boss.out_sides2, y + 13 + boss.out_out_up);

		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_U], x - 34,
		                 y - 47 - boss.out_up);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_D], x - 34,
		                 y + 17 + boss.out_up);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_L],
		                 x - 48 - boss.out_sides, y - 33);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS1_R],
		                 x + 18 + boss.out_sides, y - 33);

		circlefill (display, x, y, 19, COL_COL1);
		circle (display, x, y, 19, COL_OUTLINE);

		circlefill (display, x - 71 - boss.out_sides2, y, 16, COL_COL1);
		circle (display, x - 71 - boss.out_sides2, y, 16, COL_OUTLINE);
//        circlefill(display, x - 71 - boss.out_sides2 + xpart(boss.angle2, 11), y + ypart(boss.angle2, 11), 3, COL_COL1);
//        circle(display, x - 71 - boss.out_sides2 + xpart(boss.angle2, 11), y + ypart(boss.angle2, 11), 3, COL_OUTLINE);
		circlefill (display,
		            x - 71 - boss.out_sides2 + xpart (boss.t_angle[0], 11),
		            y + ypart (boss.t_angle[0], 11), 3, COL_COL1);
		circle (display, x - 71 - boss.out_sides2 + xpart (boss.t_angle[0], 11),
		        y + ypart (boss.t_angle[0], 11), 3, COL_OUTLINE);

		circlefill (display,
		            x - 71 - boss.out_sides2 - xpart (boss.t_angle[0], 11),
		            y - ypart (boss.t_angle[0], 11), 3, COL_COL1);
		circle (display, x - 71 - boss.out_sides2 - xpart (boss.t_angle[0], 11),
		        y - ypart (boss.t_angle[0], 11), 3, COL_OUTLINE);

		circlefill (display, x + 71 + boss.out_sides2, y, 16, COL_COL1);
		circle (display, x + 71 + boss.out_sides2, y, 16, COL_OUTLINE);
		circlefill (display,
		            x + 71 + boss.out_sides2 + xpart (boss.t_angle[1], 11),
		            y + ypart (boss.t_angle[1], 11), 3, COL_COL1);
		circle (display, x + 71 + boss.out_sides2 + xpart (boss.t_angle[1], 11),
		        y + ypart (boss.t_angle[1], 11), 3, COL_OUTLINE);

		circlefill (display,
		            x + 71 + boss.out_sides2 - xpart (boss.t_angle[1], 11),
		            y - ypart (boss.t_angle[1], 11), 3, COL_COL1);
		circle (display, x + 71 + boss.out_sides2 - xpart (boss.t_angle[1], 11),
		        y - ypart (boss.t_angle[1], 11), 3, COL_OUTLINE);

		/*
		L_ENEMY_BOSS1_CORE,
		L_ENEMY_BOSS1_UL,
		L_ENEMY_BOSS1_UR,
		L_ENEMY_BOSS1_DL,
		L_ENEMY_BOSS1_DR,
		L_ENEMY_BOSS1_U,
		L_ENEMY_BOSS1_D,
		L_ENEMY_BOSS1_L,
		L_ENEMY_BOSS1_R,
		L_ENEMY_BOSS1_OUT_L,
		L_ENEMY_BOSS1_OUT_R,*/

		break;
	case ENEMY_BEAMER:
		xa = 0;
		if (enemy[e].phase == 1)	// opening
		{
			xa = (20 - enemy[e].target_time) / 5;
		}
		if (enemy[e].phase == 2)	// shooting
			xa = 4;
		if (enemy[e].phase == 3)	// closing
		{
			xa = (enemy[e].target_time) / 5;
		}

		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_CORE], x - 19,
		                 y - 18);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_L], x - 39 - xa,
		                 y - 14);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_R], x + 5 + xa,
		                 y - 14);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_U], x - 11,
		                 y - 26 - 4 + xa);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_D], x - 9,
		                 y + 14 + 4 - xa);
		circlefill (display, x, y, 11, COL_COL3);
		circle (display, x, y, 11, COL_OUTLINE);
		circlefill (display, x + xpart (enemy[e].angle1, 10),
		            y + ypart (enemy[e].angle1, 10), 3, COL_COL3);
		circle (display, x + xpart (enemy[e].angle1, 10),
		        y + ypart (enemy[e].angle1, 10), 3, COL_OUTLINE);
		break;

	case ENEMY_SHOOTER:
		xa = 0;
		if (enemy[e].phase == 1)	// opening
		{
			xa = (20 - enemy[e].target_time) / 5;
		}
		if (enemy[e].phase == 2)	// shooting
			xa = 4;
		if (enemy[e].phase == 3)	// closing
		{
			xa = (enemy[e].target_time) / 5;
		}

		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_CORE], x - 19,
		                 y - 18);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SHOOTER_L], x - 39 - xa,
		                 y - 14);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SHOOTER_R], x + 5 + xa,
		                 y - 14);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SHOOTER_U], x - 11,
		                 y - 26 - 4 + xa);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BEAMER_D], x - 9,
		                 y + 14 + 4 - xa);
		circlefill (display, x, y, 11, COL_COL3);
		circle (display, x, y, 11, COL_OUTLINE);
		circlefill (display, x + xpart (enemy[e].angle1, 10),
		            y + ypart (enemy[e].angle1, 10), 3, COL_COL3);
		circle (display, x + xpart (enemy[e].angle1, 10),
		        y + ypart (enemy[e].angle1, 10), 3, COL_OUTLINE);
		break;

	case ENEMY_DRIFTER1:
		xa = enemy[e].status / 4;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DRIFTER1_CORE], x - 19,
		                 y - 15);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DRIFTER1_L], x - 25 - xa,
		                 y - 2 + xa);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DRIFTER1_R], x + 6 + xa,
		                 y - 2 + xa);
		break;
	case ENEMY_DRIFTER2:
		xa = enemy[e].status / 4;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DRIFTER2_CORE], x - 19,
		                 y - 15);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DRIFTER2_L], x - 25 - xa,
		                 y - 2 + xa);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DRIFTER2_R], x + 10 + xa,
		                 y - 2 + xa);
		break;
	case ENEMY_ESCORT:
		xa = enemy[e].status / 4;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_ESCORT_CORE], x - 20,
		                 y - 14);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_ESCORT_L], x - 22 - xa,
		                 y - 14);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_ESCORT_R], x + 7 + xa,
		                 y - 14);
		break;
	case ENEMY_RISER:
		xa = 0;
		if (enemy[e].phase == 1)	// opening
			xa = (eclass[ENEMY_RISER].speed3 - enemy[e].target_time) / 5;
		if (enemy[e].phase == 2)	// shooting
			xa = 4;
		if (enemy[e].phase == 3)	// closing
			xa = enemy[e].target_time / 5;
		xa -= enemy[e].status / 8;
		if (xa < 0)
			xa = 0;
//        xa = enemy[e].status / 4;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_RISER1_L], x - 18 - xa,
		                 y - 8);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_RISER1_R], x + 8 + xa,
		                 y - 8);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_RISER1_CORE], x - 13,
		                 y - 8);
		break;

	case ENEMY_SMALL2:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SMALL_CORE2], x - 7, y - 9);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SMALL_OUTER_L], x - 14 - xa,
		                 y - 16);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SMALL_OUTER_R], x - 0 + xa,
		                 y - 16);
		for (i = 0; i < 3; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			if (i == 1 || i == 2)
			{
				xb = petal2[ya].x - xpart (enemy[e].petal_angle[i], 32);
				yb = petal2[ya].y - ypart (enemy[e].petal_angle[i], 32);
				draw_trans_rle_sprite (display, petal2[ya].sprite, x - xb, y - yb);
			}
			else
			{
				xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 36);
				yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 36);
				draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
			}
		}
		break;

	case ENEMY_SMALL:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SMALL_CORE1], x - 7, y - 9);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SMALL_OUTER_L], x - 14 - xa,
		                 y - 16);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_SMALL_OUTER_R], x - 0 + xa,
		                 y - 16);
//         circle(display, x, y, 3, COL_WHITE);
		for (i = 0; i < 3; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 36);
			yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 36);
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
//            circle(display, x + xb, y + yb, 3, COL_WHITE);
		}
		break;
	case ENEMY_LARGER:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_LARGER_CORE1], x - 10,
		                 y - 10);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_LARGER_OUTER_L],
		                 x - 17 - xa, y - 23);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_LARGER_OUTER_R], x - 0 + xa,
		                 y - 23);
		for (i = 0; i < 4; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 43);
			yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 43);
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
//            circle(display, x + xb, y + yb, 3, COL_WHITE);
		}
//         circle(display, x, y, 1, COL_OUTLINE);
		/*        for (i = 0; i < 3; i ++)
		        {
		            ya = enemy[e].petal_angle [i] / (ANGLE_1 / SMALL_ROTATIONS);
		            ya &= SMALL_ROTATIONS - 1;
		            xb = petal1 [ya].x - xpart(enemy[e].petal_angle [i], 36);
		            yb = petal1 [ya].y - ypart(enemy[e].petal_angle [i], 36);
		            draw_trans_rle_sprite(display, petal1 [ya].sprite, x - xb, y - yb);
		//            circle(display, x + xb, y + yb, 3, COL_WHITE);
		        }*/
//        pline(display, x, y, enemy[e].x_target [0] / 1000, enemy[e].y_target [0] / 1000, COL_WHITE);
//        pline(display, enemy[e].x_target [0] / 1000, enemy[e].y_target [0] / 1000, enemy[e].x_target [1] / 1000, enemy[e].y_target [1] / 1000, COL_COL1);
		break;
	case ENEMY_LARGER2:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_LARGER_CORE2], x - 10,
		                 y - 10);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_LARGER_OUTER_L],
		                 x - 17 - xa, y - 23);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_LARGER_OUTER_R], x - 0 + xa,
		                 y - 23);
		for (i = 0; i < 3; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 41);
			yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 41);
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
		}
		for (i = 3; i < 6; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal2[ya].x - xpart (enemy[e].petal_angle[i], 35);
			yb = petal2[ya].y - ypart (enemy[e].petal_angle[i], 35);
			draw_trans_rle_sprite (display, petal2[ya].sprite, x - xb, y - yb);
		}
		break;
	case ENEMY_WINGS:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_WINGS_CORE1], x - 11,
		                 y - 20);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_WINGS_OUTER_L], x - 21 - xa,
		                 y - 26);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_WINGS_OUTER_R], x - 0 + xa,
		                 y - 26);
		for (i = 0; i < 2; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal3[ya].x - xpart (enemy[e].petal_angle[i], 55);
			yb = petal3[ya].y - ypart (enemy[e].petal_angle[i], 55) + 10;
			draw_trans_rle_sprite (display, petal3[ya].sprite, x - xb, y - yb);
		}
		for (i = 2; i < 4; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 42);
			yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 42);	// + 10;
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
		}
		for (i = 4; i < 6; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal2[ya].x - xpart (enemy[e].petal_angle[i], 35);
			yb = petal2[ya].y - ypart (enemy[e].petal_angle[i], 35);	// + 10;
			draw_trans_rle_sprite (display, petal2[ya].sprite, x - xb, y - yb);
		}
		break;
	case ENEMY_WINGS2:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_WINGS_CORE2], x - 11,
		                 y - 20);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_WINGS_OUTER_L], x - 21 - xa,
		                 y - 26);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_WINGS_OUTER_R], x - 0 + xa,
		                 y - 26);
		for (i = 0; i < 6; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 50);
			yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 50) + 10;
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
		}
		ya = ANGLE_4 / (ANGLE_1 / SMALL_ROTATIONS);
		ya &= SMALL_ROTATIONS - 1;
		xb = petal2[ya].x - xpart (ANGLE_4, 42);
		yb = petal2[ya].y - ypart (ANGLE_4, 42);	// + 10;
		draw_trans_rle_sprite (display, petal2[ya].sprite, x - xb, y - yb);
		break;

	case ENEMY_MEGA:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_MEGA_CORE], x - 21, y - 35);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_MEGA_OUTER_L], x - 38 - xa,
		                 y - 44);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_MEGA_OUTER_R], x - 0 + xa,
		                 y - 44);
		for (i = 0; i < 4; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal3[ya].x - xpart (enemy[e].petal_angle[i], 75);
			yb = petal3[ya].y - ypart (enemy[e].petal_angle[i], 75);
			draw_trans_rle_sprite (display, petal3[ya].sprite, x - xb, y - yb - 6);
		}
		for (i = 4; i < 7; i++)
		{
			ya = enemy[e].petal_angle[i] / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (enemy[e].petal_angle[i], 65);
			yb = petal1[ya].y - ypart (enemy[e].petal_angle[i], 65);
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb - 6);
		}
		break;

	case ENEMY_BOSS3:
		xa = enemy[e].pulse1;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_BOSS3_CORE], x - 21,
		                 y - 35);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_MEGA_OUTER_L], x - 38 - xa,
		                 y - 44);
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_MEGA_OUTER_R], x - 0 + xa,
		                 y - 44);
		for (i = 0; i < 9; i++)
		{
			xa = boss.petal_angle[0] + (ANGLE_1 / 9) * i;
			xa &= 1023;
			ya = xa / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal2[ya].x - xpart (xa, boss.petal_out[0]);
			yb = petal2[ya].y - ypart (xa, boss.petal_out[0]);
			draw_trans_rle_sprite (display, petal2[ya].sprite, x - xb, y - yb - 6);
		}
		for (i = 0; i < 7; i++)
		{
			xa = boss.petal_angle[1] + (ANGLE_1 / 7) * i;
			xa &= 1023;
			ya = xa / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal1[ya].x - xpart (xa, boss.petal_out[1]);
			yb = petal1[ya].y - ypart (xa, boss.petal_out[1]);
			draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb - 6);
		}
		for (i = 0; i < 5; i++)
		{
			xa = boss.petal_angle[2] + (ANGLE_1 / 5) * i;
			xa &= 1023;
			ya = xa / (ANGLE_1 / SMALL_ROTATIONS);
			ya &= SMALL_ROTATIONS - 1;
			xb = petal3[ya].x - xpart (xa, boss.petal_out[2]);
			yb = petal3[ya].y - ypart (xa, boss.petal_out[2]);
			draw_trans_rle_sprite (display, petal3[ya].sprite, x - xb, y - yb - 6);
		}
		break;

	case ENEMY_CRUISER1:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_CRUISER1], x - 49, y - 62);
		circlefill (display, x, y, 14, COL_COL4);
		circle (display, x, y, 14, COL_OUTLINE);
		circlefill (display, x, y, 11, COL_COL3);
		circle (display, x, y, 11, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle, 10),
		            y + ypart (enemy[e].angle, 10), 3, COL_COL3);
		circle (display, x + xpart (enemy[e].angle, 10),
		        y + ypart (enemy[e].angle, 10), 3, COL_OUTLINE);

		circlefill (display, x, y - 41, 10, COL_COL4);
		circle (display, x, y - 41, 10, COL_OUTLINE);
		circlefill (display, x, y - 41, 7, COL_COL3);
		circle (display, x, y - 41, 7, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 6),
		            y - 41 + ypart (enemy[e].angle1, 6), 2, COL_COL3);
		circle (display, x + xpart (enemy[e].angle1, 6),
		        y - 41 + ypart (enemy[e].angle1, 6), 2, COL_OUTLINE);
		break;

	case ENEMY_CRUISER2:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_CRUISER2], x - 55, y - 24);
		circlefill (display, x, y, 11, COL_COL4);
		circle (display, x, y, 11, COL_OUTLINE);
		circlefill (display, x, y, 9, COL_COL3);
		circle (display, x, y, 9, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 8),
		            y + ypart (enemy[e].angle1, 8), 3, COL_COL3);
		circle (display, x + xpart (enemy[e].angle1, 8),
		        y + ypart (enemy[e].angle1, 8), 3, COL_OUTLINE);

		break;

	case ENEMY_CRUISER3:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_CRUISER3], x - 59, y - 60);
		circlefill (display, x, y, 23, COL_COL4);
		circle (display, x, y, 23, COL_OUTLINE);
		circlefill (display, x, y, 20, COL_COL3);
		circle (display, x, y, 20, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 19),
		            y + ypart (enemy[e].angle1, 19), 4, COL_COL3);
		circle (display, x + xpart (enemy[e].angle1, 19),
		        y + ypart (enemy[e].angle1, 19), 4, COL_OUTLINE);

		break;

	case ENEMY_CRUISER4:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_CRUISER4], x - 47, y - 54);
		circlefill (display, x, y, 18, COL_COL3);
		circle (display, x, y, 18, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 17),
		            y + ypart (enemy[e].angle1, 17), 4, COL_COL3);
		circle (display, x + xpart (enemy[e].angle1, 17),
		        y + ypart (enemy[e].angle1, 17), 4, COL_OUTLINE);

		break;

	case ENEMY_POD1:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_POD1_CORE], x - 32, y - 58);

		if (enemy[e].phase == 0 || enemy[e].phase == 4)
			draw_pod1 (x, y, 0, 0, e);

		if (enemy[e].phase == 1)	// opening
			draw_pod1 (x, y, (100 - enemy[e].target_time) / 3,
			           (100 - enemy[e].target_time) / 2, e);

		if (enemy[e].phase == 2)	// shooting
			draw_pod1 (x, y, (100) / 3, (100) / 2, e);

		if (enemy[e].phase == 3)	// closing
			draw_pod1 (x, y, enemy[e].target_time / 3, enemy[e].target_time / 2, e);
		circlefill (display, x, y, 21, COL_COL3);
		circle (display, x, y, 21, COL_OUTLINE);
//       circlefill(display, x + xpart(enemy[e].angle1, 18), y + ypart(enemy[e].angle1, 18), 3, COL_COL3);
//       circle(display, x + xpart(enemy[e].angle1, 18), y + ypart(enemy[e].angle1, 18), 3, COL_OUTLINE);
		circlefill (display, x - xpart (enemy[e].angle1, 18),
		            y - ypart (enemy[e].angle1, 18), 3, COL_COL3);
		circle (display, x - xpart (enemy[e].angle1, 18),
		        y - ypart (enemy[e].angle1, 18), 3, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1 + ANGLE_4 - ANGLE_16, 18),
		            y + ypart (enemy[e].angle1 + ANGLE_4 - ANGLE_16, 18), 2,
		            COL_COL3);
		circle (display, x + xpart (enemy[e].angle1 + ANGLE_4 - ANGLE_16, 18),
		        y + ypart (enemy[e].angle1 + ANGLE_4 - ANGLE_16, 18), 2,
		        COL_OUTLINE);
		circlefill (display, x + xpart (enemy[e].angle1 - ANGLE_4 + ANGLE_16, 18),
		            y + ypart (enemy[e].angle1 - ANGLE_4 + ANGLE_16, 18), 2,
		            COL_COL3);
		circle (display, x + xpart (enemy[e].angle1 - ANGLE_4 + ANGLE_16, 18),
		        y + ypart (enemy[e].angle1 - ANGLE_4 + ANGLE_16, 18), 2,
		        COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1 + ANGLE_4 + ANGLE_8, 18),
		            y + ypart (enemy[e].angle1 + ANGLE_4 + ANGLE_8, 18), 2,
		            COL_COL3);
		circle (display, x + xpart (enemy[e].angle1 + ANGLE_4 + ANGLE_8, 18),
		        y + ypart (enemy[e].angle1 + ANGLE_4 + ANGLE_8, 18), 2,
		        COL_OUTLINE);
		circlefill (display, x + xpart (enemy[e].angle1 - ANGLE_4 - ANGLE_8, 18),
		            y + ypart (enemy[e].angle1 - ANGLE_4 - ANGLE_8, 18), 2,
		            COL_COL3);
		circle (display, x + xpart (enemy[e].angle1 - ANGLE_4 - ANGLE_8, 18),
		        y + ypart (enemy[e].angle1 - ANGLE_4 - ANGLE_8, 18), 2,
		        COL_OUTLINE);
		break;
	case ENEMY_HOOK:
		xa = 2 - enemy[e].pulse1;
		if (xa < 0)
			xa = 0;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_HOOK1 + xa], x - 16,
		                 y - 17);
//        pline(display, x, y, enemy[e].x_target [0] / 1000, enemy[e].y_target [0] / 1000, COL_WHITE);
//        pline(display, enemy[e].x_target [0] / 1000, enemy[e].y_target [0] / 1000, enemy[e].x_target [1] / 1000, enemy[e].y_target [1] / 1000, COL_COL1);
		break;
	case ENEMY_HOOK2:
		xa = 2 - enemy[e].pulse1;
		if (xa < 0)
			xa = 0;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_HOOK2_1 + xa], x - 17,
		                 y - 17);
		break;
	case ENEMY_DROPPER:
		xa = enemy[e].pulse1;
		if (xa > 2)
			xa = 2;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DROPPER3 - xa], x - 14,
		                 y - 23 + xa);
		break;
	case ENEMY_HUNTER:
		xa = 2 - enemy[e].pulse1;
		if (xa < 0)
			xa = 0;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_HUNTER1 + xa], x - 28,
		                 y - 27 + xa);
		break;
	case ENEMY_RING:
		xa = 2 - enemy[e].pulse1;
		if (xa < 0)
			xa = 0;
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_RING1 + xa], x - 24 - xa,
		                 y - 24 - xa);
		break;
	case ENEMY_BOSS4:
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_BOSS4], x - 114, y - 100);
		break;
	case ENEMY_SWOOPER1:
		/*       eRLE_small1 [S_ENEMY_SWOOPER1_1]
		        circlefill(display, x, y, 12, COL_COL2);
		        circle(display, x, y, 12, COL_OUTLINE);*/
		/*        xa = ;//;(arena.counter / 4) % 8;
		        if (xa > 3)
		         xa = 7 - xa;*/
		xa = 3 - enemy[e].pulse1;

		switch (xa)
		{
		case 3:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER1_1], x - 15,
			                 y - 17);
			break;
		case 2:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER1_2], x - 16,
			                 y - 17);
			break;
		case 1:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER1_3], x - 17,
			                 y - 17);
			break;
		case 0:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER1_4], x - 18,
			                 y - 18);
			break;
		}
//        print_number(x, y + 20, enemy[e].pulse1);
//        print_number(x, y + 30, enemy[e].pulse2);

//        pline(display, x, y, enemy[e].x_target [0] / 1000, enemy[e].y_target [0] / 1000, COL_WHITE);
//        pline(display, x, y, enemy[e].x_target [1] / 1000, enemy[e].y_target [1] / 1000, COL_COL1);
//        textprintf_ex(display, font, x, y - 30, COL_WHITE, -1, "%i", enemy[e].target);
//        textprintf_ex(display, font, x, y - 40, COL_WHITE, -1, "%i", enemy[e].angle1);
		break;
	case ENEMY_SWOOPER2:
		xa = 3 - enemy[e].pulse1;

		switch (xa)
		{
		case 3:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER2_1], x - 20,
			                 y - 20);
			break;
		case 2:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER2_2], x - 21,
			                 y - 20);
			break;
		case 1:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER2_3], x - 22,
			                 y - 20);
			break;
		case 0:
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_SWOOPER2_4], x - 23,
			                 y - 20);
			break;
		}
		break;
	case ENEMY_DIVER1:
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_CENTRE], x - 17,
		                 y - 29);
		if (enemy[e].phase == 0)
		{
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_INNER_L], x - 18,
			                 y - 29);
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_OUTER_L], x - 26,
			                 y - 23);
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_INNER_R], x + 9,
			                 y - 29);
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_OUTER_R], x + 18,
			                 y - 23);
			break;
		}
		if (enemy[e].phase == 5)
		{
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_INNER_L], x - 18,
			                 y - 29);
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_OUTER_L], x - 26,
			                 y - 23 + 18);
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_INNER_R], x + 9,
			                 y - 29);
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_OUTER_R], x + 18,
			                 y - 23 + 18);
			break;
		}
		if (enemy[e].phase == 1)
			draw_diver1 (x, y, 4 - (enemy[e].target_time / 5), 0);
		if (enemy[e].phase == 2)
			draw_diver1 (x, y, 4, 19 - (enemy[e].target_time / 3));
		if (enemy[e].phase == 3)
			draw_diver1 (x, y, enemy[e].target_time / 5, 18);
		break;

	case ENEMY_DARTER1:

		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_CENTRE], x - 13,
		                 y - 18);
		circlefill (display, x, y - 1, 10, COL_DGREY);
		circle (display, x, y - 1, 10, COL_OUTLINE);

		circlefill (display, x, y, 7, COL_COL1);
		circle (display, x, y, 7, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 5),
		            y + ypart (enemy[e].angle1, 5), 2, COL_COL1);
		circle (display, x + xpart (enemy[e].angle1, 5),
		        y + ypart (enemy[e].angle1, 5), 2, COL_OUTLINE);

		if (enemy[e].phase == 0 || enemy[e].phase == 4 || enemy[e].phase == 5)
			draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_MAIN], x - 24,
			                 y - 19);

//      textprintf_ex(display, font, x, y, COL_COL1, COL_COL4, "%i ", enemy[e].x);
//        pline(display, x, y, enemy[e].x_target [0] / 1000, enemy[e].y_target [0] / 1000, COL_WHITE);
		if (enemy[e].phase == 1)
			draw_darter1 (x, y, 5 - (enemy[e].target_time / 6),
			              15 - (enemy[e].target_time / 2));
		if (enemy[e].phase == 2)
			draw_darter1 (x, y, 5, 15);
		if (enemy[e].phase == 3)
			draw_darter1 (x, y, (enemy[e].target_time / 6),
			              (enemy[e].target_time / 2));
		/*         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_DARTER1_CENTRE], x - 13, y - 10);
		         xa = 5 - (enemy[e].target_time / 6);
		         ya = 10 - (enemy[e].target_time / 3);
		         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_DARTER1_TOP], x - 13, y - 19 - xa);

		         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_DARTER1_INNER_L], x - 16 - xa, y - 9);
		         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_DARTER1_OUTER_L], x - 24 - xa, y - 6 - ya);

		         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_DARTER1_INNER_R], x + 3 + xa, y - 9);
		         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_DARTER1_OUTER_R], x + 16 + xa, y - 6 - ya);
		*/

		break;
	case ENEMY_WALKER:
		xa = 2 - enemy[e].pulse1;
		if (xa < 0)
			xa = 0;
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_WALKER1 + xa], x - 76,
		                 y - 70);
		break;
	case ENEMY_SPINNER:
		xa = 2 - enemy[e].pulse1;
		if (xa < 0)
			xa = 0;
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_SPINNER1 + xa], x - 52,
		                 y - 86 + xa);
		break;

	case ENEMY_DARTER2:
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_CORE], x - 12,
		                 y - 12);

		circlefill (display, x, y, 6, COL_COL1);
		circle (display, x, y, 6, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 5),
		            y + ypart (enemy[e].angle1, 5), 2, COL_COL1);
		circle (display, x + xpart (enemy[e].angle1, 5),
		        y + ypart (enemy[e].angle1, 5), 2, COL_OUTLINE);

		if (enemy[e].phase == 0 || enemy[e].phase == 4 || enemy[e].phase == 5)
			draw_darter2 (x, y, 0, 0);

		if (enemy[e].phase == 1)
			draw_darter2 (x, y, 5 - (enemy[e].target_time / 6),
			              15 - (enemy[e].target_time / 2));
		if (enemy[e].phase == 2)
			draw_darter2 (x, y, 5, 15);
		if (enemy[e].phase == 3)
			draw_darter2 (x, y, (enemy[e].target_time / 6),
			              (enemy[e].target_time / 2));
		break;
	case ENEMY_CARRIER1_PART:
		draw_rle_sprite (display, eRLE_small1[S_ENEMY_C1P1_CORE], x - 12, y - 10);
		circlefill (display, x, y, 5, COL_COL1);
		circle (display, x, y, 5, COL_OUTLINE);

		circlefill (display, x + xpart (enemy[e].angle1, 4),
		            y + ypart (enemy[e].angle1, 4), 1, COL_COL1);
		circle (display, x + xpart (enemy[e].angle1, 4),
		        y + ypart (enemy[e].angle1, 4), 1, COL_OUTLINE);

		if (enemy[e].phase == 0 || enemy[e].phase == 4 || enemy[e].phase == 5)
			draw_carrier1_part (x, y, 0, 0);

		xa = enemy[e].pulse1 / 3;
		ya = enemy[e].pulse1 / 2;
		if (enemy[e].phase == 1)
			draw_carrier1_part (x, y, 3 - (enemy[e].target_time / 10) + xa,
			                    6 - (enemy[e].target_time / 5) + ya);
		if (enemy[e].phase == 2)
			draw_carrier1_part (x, y, 3 + xa, 6 + ya);
		if (enemy[e].phase == 3)
			draw_carrier1_part (x, y, enemy[e].target_time / 10 + xa,
			                    enemy[e].target_time / 5 + ya);


		break;
	case ENEMY_CARRIER1:
//      circle(display, x, y, 5, COL_WHITE);
//         draw_rle_sprite(display, eRLE_large1 [L_ENEMY_CARRIER1_CORE], x - 68, y - 85);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_CORE], x - 69,
		                 y - 85);

		if (enemy[e].phase == 0 || enemy[e].phase == 4)
			draw_carrier1 (x, y, 0);

		if (enemy[e].phase == 1)	// opening
			draw_carrier1 (x, y,
			               (eclass[ENEMY_CARRIER1].speed3 -
			                enemy[e].target_time) / 3);

		if (enemy[e].phase == 2)	// shooting
			draw_carrier1 (x, y, (eclass[ENEMY_CARRIER1].speed3) / 3);

		if (enemy[e].phase == 3)	// closing
			draw_carrier1 (x, y, enemy[e].target_time / 3);

		circlefill (display, x, y, 21, COL_COL1);
		circle (display, x, y, 21, COL_OUTLINE);
		angle = enemy[e].angle1;
		circlefill (display, x + xpart (angle, 22), y + ypart (angle, 22), 4,
		            COL_COL1);
		circle (display, x + xpart (angle, 22), y + ypart (angle, 22), 4,
		        COL_OUTLINE);
		angle += ANGLE_3;
		circlefill (display, x + xpart (angle, 22), y + ypart (angle, 22), 4,
		            COL_COL1);
		circle (display, x + xpart (angle, 22), y + ypart (angle, 22), 4,
		        COL_OUTLINE);
		angle += ANGLE_3 + 8;
		circlefill (display, x + xpart (angle, 22), y + ypart (angle, 22), 4,
		            COL_COL1);
		circle (display, x + xpart (angle, 22), y + ypart (angle, 22), 4,
		        COL_OUTLINE);

		/*         circlefill(display, x + 55, y - 1, 12, COL_COL1);
		         circle(display, x + 55, y - 1, 12, COL_OUTLINE);
		         circlefill(display, x - 55, y - 1, 12, COL_COL1);
		         circle(display, x - 55, y - 1, 12, COL_OUTLINE);*/
		break;

	case ENEMY_STOPPER1:
//         draw_rle_sprite(display, eRLE_large1 [L_ENEMY_STOPPER1], x - 46, y - 101);
		draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_CENTRE], x - 45,
		                 y - 49);

		if (enemy[e].phase == 0 || enemy[e].phase == 4)
			draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1], x - 46,
			                 y - 101);

		if (enemy[e].phase == 1)
			draw_stopper1 (x, y,
			               5 - (enemy[e].target_time / 12),
			               5 - (enemy[e].target_time / 12),
			               5 - (enemy[e].target_time / 12),
			               10 - (enemy[e].target_time / 6), 0,
			               5 - (enemy[e].target_time / 12),
			               10 - (enemy[e].target_time / 6));

		if (enemy[e].phase == 2)
			draw_stopper1 (x, y, 5, 5, 5, 10, 0, 5, 10);

		if (enemy[e].phase == 3)
			draw_stopper1 (x, y,
			               (enemy[e].target_time / 12),
			               (enemy[e].target_time / 12),
			               (enemy[e].target_time / 12),
			               (enemy[e].target_time / 6), 0,
			               (enemy[e].target_time / 12), (enemy[e].target_time / 6));

		break;

	}

// print_number(x, y + 40, enemy[e].hp);
	/*
	    rect(display, x - eclass[enemy[e].type].xsize / 1000 + eclass[enemy[e].type].xoffset / 1000,
	                  y - eclass[enemy[e].type].ysize / 1000 + eclass[enemy[e].type].yoffset / 1000,
	                  x + eclass[enemy[e].type].xsize / 1000 + eclass[enemy[e].type].xoffset / 1000,
	                  y + eclass[enemy[e].type].ysize / 1000 + eclass[enemy[e].type].yoffset / 1000, COL_WHITE);
	*/

}

void draw_pod1 (int x, int y, int out, int up_out, int e)
{
	int out1 = out;
	if (out1 > 3)
		out1 = 3;

	draw_rle_sprite (display, eRLE_large1[L_ENEMY_POD1_BODYL],
	                 x - 55 - out1 - enemy[e].pulse1, y - 53);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_POD1_BODYR],
	                 x + 3 + out1 + enemy[e].pulse1, y - 53);
	out1 = out - 25;
	if (out1 < 0)
		out1 = 0;
	if (up_out > 45)
		up_out = 45;
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_POD1_TOPL],
	                 x - 20 - out1 + enemy[e].pulse1 / 2, y - 55 - up_out);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_POD1_TOPR],
	                 x - 1 + out1 - enemy[e].pulse1 / 2, y - 55 - up_out);
	out *= 0.7;
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_POD1_TOPC], x - 9,
	                 y - 56 - out);
}

void draw_stopper1 (int x, int y, int front_down, int fx, int fy, int mx,
                    int my, int bx, int by)
{

	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_FRONT], x - 21,
	                 y + 25 + front_down);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_F_L], x - 40 - fx,
	                 y + 8 + fy);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_M_L], x - 46 - mx,
	                 y - 75 + my);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_B_L], x - 21 - bx,
	                 y - 102 - by);

	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_F_R], x + 15 + fx,
	                 y + 8 + fy);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_M_R], x + 15 + mx,
	                 y - 75 + my);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_STOPPER1_B_R], x + 0 + bx,
	                 y - 102 - by);


}

void draw_carrier1 (int x, int y, int out)
{

	int save_out = out;

	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_TOP1], x - 32,
	                 y - 85 - out);
	out *= (float) 0.8;
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_TOP2], x - 52,
	                 y - 74 - out);
	out *= (float) 0.5;
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_TOP3], x - 64,
	                 y - 54 - out);
	out = save_out;
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_BOTTOM2], x - 46,
	                 y + 37 + out);
	out *= (float) 0.8;
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_BOTTOM1], x - 62,
	                 y + 12 + out);

	out = save_out;
	int out2 = out / 3;

	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_MIDL1], x - 68 - out,
	                 y - 31 - out2);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_MIDL2], x - 68 - out,
	                 y - 1 + out2);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_MIDR1], x + 18 + out,
	                 y - 31 - out2);
	draw_rle_sprite (display, eRLE_large1[L_ENEMY_CARRIER1_MIDR2], x + 20 + out,
	                 y - 1 + out2);

}

void draw_carrier1_part (int x, int y, int out, int out2)
{
//         if (out < 3)
	if (out2 > 5)
	{
		rectfill (display, x - 1, y + 15, x + 2, y + 13 + out2, COL_LGREY);
		vline (display, x - 2, y + 13, y + 13 + out2, COL_OUTLINE);
		vline (display, x + 2, y + 13, y + 13 + out2, COL_OUTLINE);
	}


	draw_rle_sprite (display, eRLE_small1[S_ENEMY_C1P1_TOP], x - 8,
	                 y - 10 - out);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_C1P1_BOTTOM1], x - 11,
	                 y + 5 + out);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_C1P1_BOTTOM2], x - 8,
	                 y + 10 + out2);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_C1P1_MIDL], x - 12 - out2,
	                 y - 6);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_C1P1_MIDR], x + out2, y - 6);
}

void draw_darter1 (int x, int y, int xa, int ya)
{
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_TOP], x - 13,
	                 y - 19 - xa);

	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_INNER_L], x - 16 - xa,
	                 y - 9);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_OUTER_L], x - 24 - xa,
	                 y - 6 - ya);

	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_INNER_R], x + 3 + xa,
	                 y - 9);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER1_OUTER_R], x + 16 + xa,
	                 y - 6 - ya);

}

void draw_darter2 (int x, int y, int out2, int out1)
{

	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_TOP], x - 6,
	                 y - 23 - out2);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_BOTTOM], x - 6,
	                 y + 3 + out2);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_OUTER_L],
	                 x - 33 - out2, y - 19 + out1);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_INNER_L],
	                 x - 18 - out2, y - 19 - out1);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_OUTER_R],
	                 x + 18 + out2, y - 19 + out1);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DARTER2_INNER_R],
	                 x + 6 + out2, y - 19 - out1);

}

void draw_diver1 (int x, int y, int xa, int ya)
{

	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_INNER_L], x - 18 - xa,
	                 y - 29);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_OUTER_L], x - 26 - xa,
	                 y - 23 + ya);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_INNER_R], x + 9 + xa,
	                 y - 29);
	draw_rle_sprite (display, eRLE_small1[S_ENEMY_DIVER1_OUTER_R], x + 18 + xa,
	                 y - 23 + ya);

}

void draw_ebullets (void)
{

	int b;

	for (b = 0; b < NO_EBULLETS; b++)
	{
		if (ebullet[b].type == EBULLET_NONE)
			continue;
		draw_an_ebullet (b);
	}

}

void draw_an_ebullet (int b)
{

	int x = ebullet[b].x / GRAIN;
	int y = ebullet[b].y / GRAIN;
	int xb, yb, xa, ya, xc, yc, xd, yd, xe, ye, i;

	switch (ebullet[b].type)
	{

	case EBULLET_SPIN:
		TRANS_MODE
		circle (display, x, y, ebullet[b].status, ebullet[b].cord_colour);
		END_TRANS_MODE xa = ANGLE_1 / ebullet[b].y_speed;
		ya = ebullet[b].angle;
		for (i = 0; i < ebullet[b].y_speed; i++)
		{
			fcircle (x + xpart (ya, ebullet[b].status),
			         y + ypart (ya, ebullet[b].status), ebullet[b].x_speed,
			         ebullet[b].colour);
			fcircle (x + xpart (ya, ebullet[b].status),
			         y + ypart (ya, ebullet[b].status), ebullet[b].x_speed + 4,
			         ebullet[b].accel);
			fcircle (x + xpart (ya + ANGLE_32, ebullet[b].status),
			         y + ypart (ya + ANGLE_32, ebullet[b].status),
			         ebullet[b].x_speed / 2, ebullet[b].accel);
			fcircle (x + xpart (ya - ANGLE_32, ebullet[b].status),
			         y + ypart (ya - ANGLE_32, ebullet[b].status),
			         ebullet[b].x_speed / 2, ebullet[b].accel);
			ya += xa;
		}
		break;
	case EBULLET_BEAM2:
		xd = ebullet[b].status * 45;
		xc = ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS);
		xa = 0;			//ebullet[b].angle2;
		if (ebullet[b].angle2 > 100)
		{
			ya = 120 - ebullet[b].angle2;
			fcircle (ebullet[b].x_speed / GRAIN, ebullet[b].y_speed / GRAIN,
			         ya + grand (4), TCOL_B5);
			fcircle (ebullet[b].x_speed / GRAIN - xpart (xc, 25),
			         ebullet[b].y_speed / GRAIN - ypart (xc, 25),
			         ya / 2 + grand (10), TCOL_B5);
			fcircle (ebullet[b].x_speed / GRAIN - xpart (xc, 35),
			         ebullet[b].y_speed / GRAIN - ypart (xc, 35),
			         ya / 3 + grand (4), TCOL_B5);
			break;
		}

		fcircle (x + xpart (xc, xd), y + ypart (xc, xd), 7 + grand (5), TCOL_B5);
		ya = ebullet[b].angle2;
		if (ya > 15)
			ya = 15;
		fcircle (ebullet[b].x_speed / GRAIN - xpart (xc, 25),
		         ebullet[b].y_speed / GRAIN - ypart (xc, 25), ya / 2 + grand (10),
		         TCOL_B5);
		fcircle (ebullet[b].x_speed / GRAIN - xpart (xc, 35),
		         ebullet[b].y_speed / GRAIN - ypart (xc, 35), ya / 3 + grand (4),
		         TCOL_B5);

		if (ebullet[b].angle2 < 20)
		{
			fcircle (ebullet[b].x_speed / GRAIN, ebullet[b].y_speed / GRAIN,
			         ebullet[b].angle2 + grand (4), TCOL_B5);
			xc = ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS);
			xb = ebullet[b].angle2 / 5 + 1;
			xd = ebullet[b].status * 45;
			TRANS_MODE
			poly4 (display,
			       x + xpart (xc + ANGLE_4, xb), y + ypart (xc + ANGLE_4, xb),
			       x + xpart (xc - ANGLE_4, xb), y + ypart (xc - ANGLE_4, xb),
			       x + xpart (xc, xd) + xpart (xc - ANGLE_4, xb), y + ypart (xc,
			               xd) +
			       ypart (xc - ANGLE_4, xb), x + xpart (xc,
			                                            xd) + xpart (xc + ANGLE_4,
			                                                         xb),
			       y + ypart (xc, xd) + ypart (xc + ANGLE_4, xb), TRANS_B5_OUT);
			END_TRANS_MODE break;
		}

		fcircle (ebullet[b].x_speed / GRAIN, ebullet[b].y_speed / GRAIN,
		         19 + grand (4), TCOL_B5);

		for (i = 0; i < ebullet[b].status; i++)
		{
			xb = RLE_beam[ebullet[b].sprite_angle].x;
			yb = RLE_beam[ebullet[b].sprite_angle].y;
			draw_trans_rle_sprite (display,
			                       RLE_beam[ebullet[b].sprite_angle].sprite,
			                       x +
			                       xpart (ebullet[b].sprite_angle *
			                              (ANGLE_1 / SMALL_ROTATIONS),
			                              23 + 45 * i + xa) - xb,
			                       y +
			                       ypart (ebullet[b].sprite_angle *
			                              (ANGLE_1 / SMALL_ROTATIONS),
			                              23 + 45 * i + xa) - yb);
			fcircle (x +
			         xpart (ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS),
			                45 * i + xa),
			         y +
			         ypart (ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS),
			                45 * i + xa), 5 + grand (2), TCOL_B5);
//       fcircle(x + xpart(ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS), 22 + 45 * i + xa), y + ypart(ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS), 22 + 45 * i + xa), 5, TCOL_B5);
		}
		break;
	case EBULLET_BEAM:
		xa = 0;			//ebullet[b].angle2;


		if (ebullet[b].angle2 < 20)
		{
			fcircle (ebullet[b].x_speed / GRAIN, ebullet[b].y_speed / GRAIN,
			         ebullet[b].angle2 + grand (4), TCOL_B5);
			xc = ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS);
			xb = ebullet[b].angle2 / 5 + 1;
			xd = ebullet[b].status * 45;
			TRANS_MODE
			poly4 (display,
			       x + xpart (xc + ANGLE_4, xb), y + ypart (xc + ANGLE_4, xb),
			       x + xpart (xc - ANGLE_4, xb), y + ypart (xc - ANGLE_4, xb),
			       x + xpart (xc, xd) + xpart (xc - ANGLE_4, xb), y + ypart (xc,
			               xd) +
			       ypart (xc - ANGLE_4, xb), x + xpart (xc,
			                                            xd) + xpart (xc + ANGLE_4,
			                                                         xb),
			       y + ypart (xc, xd) + ypart (xc + ANGLE_4, xb), TRANS_B5_OUT);
			END_TRANS_MODE break;
		}

		fcircle (ebullet[b].x_speed / GRAIN, ebullet[b].y_speed / GRAIN,
		         19 + grand (4), TCOL_B5);

		for (i = 0; i < ebullet[b].status; i++)
		{
			xb = RLE_beam[ebullet[b].sprite_angle].x;
			yb = RLE_beam[ebullet[b].sprite_angle].y;
			draw_trans_rle_sprite (display,
			                       RLE_beam[ebullet[b].sprite_angle].sprite,
			                       x +
			                       xpart (ebullet[b].sprite_angle *
			                              (ANGLE_1 / SMALL_ROTATIONS),
			                              23 + 45 * i + xa) - xb,
			                       y +
			                       ypart (ebullet[b].sprite_angle *
			                              (ANGLE_1 / SMALL_ROTATIONS),
			                              23 + 45 * i + xa) - yb);
			fcircle (x +
			         xpart (ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS),
			                45 * i + xa),
			         y +
			         ypart (ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS),
			                45 * i + xa), 5, TCOL_B5);
			fcircle (x +
			         xpart (ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS),
			                22 + 45 * i + xa),
			         y +
			         ypart (ebullet[b].sprite_angle * (ANGLE_1 / SMALL_ROTATIONS),
			                22 + 45 * i + xa), 5, TCOL_B5);
		}
		break;
	case EBULLET_SHOT:
		xb = dart_bullet[ebullet[b].sprite_angle][0].x;
		yb = dart_bullet[ebullet[b].sprite_angle][0].y;
		if (ebullet[b].time < 20 && ebullet[b].new_ebullet == 1)
		{
			xa = ebullet[b].angle;
			ya = (ebullet[b].time * 12) / 17;
			yc = (ebullet[b].time * 15) / 17;
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			poly4 (display,
			       x + xpart (xa, ya), y + ypart (xa, ya),
			       x + xpart (xa + ANGLE_4 + ANGLE_8, ya),
			       y + ypart (xa + ANGLE_4 + ANGLE_8, ya), x - xpart (xa, yc),
			       y - ypart (xa, yc), x + xpart (xa - ANGLE_4 - ANGLE_8, ya),
			       y + ypart (xa - ANGLE_4 - ANGLE_8, ya), ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
			draw_trans_rle_sprite (display,
			                       dart_bullet[ebullet[b].sprite_angle][ebullet[b].
			                                                            colour].
			                       sprite, x - xb, y - yb);
//  textprintf_ex(display, font, x, y, COL_WHITE, -1, "%i", ebullet[b].sprite_angle);
		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			       x, y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		break;
	case EBULLET_SDART:
		xb = sdart_bullet[ebullet[b].sprite_angle][0].x;
		yb = sdart_bullet[ebullet[b].sprite_angle][0].y;
		if (ebullet[b].time < 20 && ebullet[b].new_ebullet == 1)
		{
			xa = ebullet[b].angle;
			ya = (ebullet[b].time * 10) / 17;
			yc = (ebullet[b].time * 12) / 17;
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			poly4 (display,
			       x + xpart (xa, ya), y + ypart (xa, ya),
			       x + xpart (xa + ANGLE_4 + ANGLE_8, ya),
			       y + ypart (xa + ANGLE_4 + ANGLE_8, ya), x - xpart (xa, yc),
			       y - ypart (xa, yc), x + xpart (xa - ANGLE_4 - ANGLE_8, ya),
			       y + ypart (xa - ANGLE_4 - ANGLE_8, ya), ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
			draw_trans_rle_sprite (display,
			                       sdart_bullet[ebullet[b].sprite_angle][ebullet[b].
			                                                             colour].
			                       sprite, x - xb, y - yb);
		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			       x, y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		break;

	case EBULLET_WING_DIAMOND:
		for (i = 0; i < 3; i++)
		{
			xc =
			    (int) (ebullet[b].angle2 +
			           (i * ANGLE_3)) / (ANGLE_1 / SMALL_ROTATIONS);
			xc &= SMALL_ROTATIONS - 1;
			xb = dart_bullet[xc][0].x;
			yb = dart_bullet[xc][0].y;
			xd = x + xpart (xc * 8, 21);
			yd = y + ypart (xc * 8, 21);
			if (ebullet[b].time < 40)
			{
				xa = xc * 8;		//ebullet[b].angle;
				ya = (ebullet[b].time * 6) / 17;
				yc = (ebullet[b].time * 7) / 17;
				drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
				poly4 (display,
				       xd + xpart (xa, ya), yd + ypart (xa, ya),
				       xd + xpart (xa + ANGLE_4 + ANGLE_8, ya),
				       yd + ypart (xa + ANGLE_4 + ANGLE_8, ya), xd - xpart (xa, yc),
				       yd - ypart (xa, yc), xd + xpart (xa - ANGLE_4 - ANGLE_8, ya),
				       yd + ypart (xa - ANGLE_4 - ANGLE_8, ya),
				       ebullet[b].cord_colour);
				drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
			}
			else
				draw_trans_rle_sprite (display,
				                       dart_bullet[xc][ebullet[b].colour + 1].sprite,
				                       xd - xb, yd - yb);
		}				// fall through...
		if (ebullet[b].time < 30)
		{
			TRANS_MODE
			circlefill (display, x, y, ebullet[b].time / 10,
			            ebullet[b].cord_colour);
			END_TRANS_MODE
		}
		else
			fcircle (x, y, 3, ebullet[b].colour + 4);

		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			       x, y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		break;
		/*     case EBULLET_WING_DIAMOND:
		     for (i = 0; i < 2; i ++)
		     {
		      if (i == 0)
		       xc = (int) (ebullet[b].angle + ANGLE_2 + ebullet[b].angle2) / (ANGLE_1 / SMALL_ROTATIONS);
		        else
		         xc = (int) (ebullet[b].angle + ANGLE_2 - ebullet[b].angle2) / (ANGLE_1 / SMALL_ROTATIONS);
		//      if (xc > SMALL_ROTATIONS)
		//      xc %= SMALL_ROTATIONS;
		      xc &= SMALL_ROTATIONS - 1;
		      xb = dart_bullet [xc] [0].x;
		      yb = dart_bullet [xc] [0].y;
		//      xd = x - xpart(ebullet[b].angle, 6);
		//      yd = y - ypart(ebullet[b].angle, 6);
		      xd = x + xpart(xc * 8, 21);
		      yd = y + ypart(xc * 8, 21);
		      if (ebullet[b].time < 40)
		      {
		         xa = xc * 8; //ebullet[b].angle;
		         ya = (ebullet[b].time * 6) / 17;
		         yc = (ebullet[b].time * 7) / 17;
		       drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		         poly4(display,
		          xd + xpart(xa, ya), yd + ypart(xa, ya),
		          xd + xpart(xa + ANGLE_4 + ANGLE_8, ya), yd + ypart(xa + ANGLE_4 + ANGLE_8, ya),
		          xd - xpart(xa, yc), yd - ypart(xa, yc),
		          xd + xpart(xa - ANGLE_4 - ANGLE_8, ya), yd + ypart(xa - ANGLE_4 - ANGLE_8, ya),
		          ebullet[b].cord_colour);
		       drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		     }
		      else
		       draw_trans_rle_sprite(display, dart_bullet [xc] [ebullet[b].colour - 1].sprite, xd - xb, yd - yb);
		     } // fall through...*/
	case EBULLET_SHOT2:
	case EBULLET_DDART:
	case EBULLET_CURVE:
		xc = (int) ebullet[b].angle / (ANGLE_1 / SMALL_ROTATIONS);
//     if (xc > SMALL_ROTATIONS)
//      xc %= SMALL_ROTATIONS;
		xc &= SMALL_ROTATIONS - 1;
		xb = diamond_bullet[xc][0].x;
		yb = diamond_bullet[xc][0].y;
		if (ebullet[b].time < 40 && ebullet[b].new_ebullet == 1)
		{
			xa = ebullet[b].angle;
			ya = (ebullet[b].time * 9) / 37;
			yc = (ebullet[b].time * 15) / 37;
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			poly4 (display,
			       x + xpart (xa, yc), y + ypart (xa, yc),
			       x + xpart (xa + ANGLE_4, ya), y + ypart (xa + ANGLE_4, ya),
			       x - xpart (xa, yc), y - ypart (xa, yc),
			       x + xpart (xa - ANGLE_4, ya), y + ypart (xa - ANGLE_4, ya),
			       ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
		{
//       print_number(x, y, xc);
//       blit(display, screen, 0, 0, 0, 0, 640, 480);
			draw_trans_rle_sprite (display,
			                       diamond_bullet[xc][ebullet[b].colour].sprite,
			                       x - xb, y - yb);
//       blit(display, screen, 0, 0, 0, 0, 640, 480);
		}
//       draw_trans_rle_sprite(display, diamond_bullet [0] [xc].sprite, x - xb, y - yb);

//       draw_trans_rle_sprite(display, diamond_bullet [ebullet[b].colour] [xc].sprite, x - xb, y - yb);
//  textprintf_ex(display, font, x, y, COL_WHITE, -1, "%i", ebullet[b].sprite_angle);
		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, x,
			       y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		break;
	case EBULLET_TRACKER:
		if (ebullet[b].timeout < ebullet[b].status * 5)
		{
			xa = ebullet[b].timeout / 6;
			fcircle (x, y, xa, ebullet[b].colour + TCOL_B1);
			break;
		}
		xa = ebullet[b].time / 3;
		if (xa > ebullet[b].status)
		{
			xa = ebullet[b].status;
			fcircle (x, y, xa, ebullet[b].colour + TCOL_B1);
			fcircle (x, y, xa + 3, TCOL_B1);
		}
		else
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			circlefill (display, x, y, xa + 3, TRANS_B1_OUT);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		break;
	case EBULLET_DOT:
		if (ebullet[b].timeout < ebullet[b].status * 5)
		{
			xa = ebullet[b].timeout / 5;
			fcircle (x, y, xa, ebullet[b].colour + TCOL_B1);
			xb = xa * 1.8;
			TRANS_MODE circle (display, x, y, xb, ebullet[b].cord_colour);
			END_TRANS_MODE break;
		}
		xa = ebullet[b].time / 3;
		if (xa > ebullet[b].status)
		{
			xa = ebullet[b].status;
			fcircle (x, y, xa, ebullet[b].colour + TCOL_B1);
			xb = xa * 1.8;
			TRANS_MODE circle (display, x, y, xb, ebullet[b].cord_colour);
			END_TRANS_MODE
		}
		else
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			circlefill (display, x, y, xa, ebullet[b].cord_colour);
			xb = xa * 1.8;
			circle (display, x, y, xb, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, x,
			       y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		break;
		/*     case EBULLET_DOT:
		     if (ebullet[b].timeout < ebullet[b].status * 5)
		     {
		      xa = ebullet[b].timeout / 5;
		      fcircle(x, y, xa, ebullet[b].colour + TCOL_B1);
		      xb = xa * 1.3;
		      TRANS_MODE
		      circle(display, x, y, xb, ebullet[b].cord_colour);
		      END_TRANS_MODE
		      break;
		     }
		     xa = ebullet[b].time / 3;
		     if (xa > ebullet[b].status)
		     {
		      xa = ebullet[b].status;
		      fcircle(x, y, xa, ebullet[b].colour + TCOL_B1);
		     }
		       else
		       {
		        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		        circlefill(display, x, y, xa, TRANS_B5_OUT);
		        drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		       }

		      if (ebullet[b].cord_wait > -1)
		      {
		       drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		       pline(display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, x, y, ebullet[b].cord_colour);
		       circle(display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, 1, ebullet[b].cord_colour);
		       drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		      }
		      break;*/
	case EBULLET_SEED:
	case EBULLET_LSEED:
		xb = dart_bullet[ebullet[b].sprite_angle][0].x;
		yb = dart_bullet[ebullet[b].sprite_angle][0].y;
		if (ebullet[b].time < 20)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			circlefill (display, x, y, ebullet[b].time / 5, TRANS_B5_OUT);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
			fcircle (x, y, 4, TCOL_B5);

		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			       x, y, TRANS_B5_OUT);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, TRANS_B5_OUT);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		break;

	case EBULLET_SEED2:
		xb = dart_bullet[ebullet[b].sprite_angle][0].x;
		yb = dart_bullet[ebullet[b].sprite_angle][0].y;
		switch (ebullet[b].colour)
		{
		case 0:
			xa = TRANS_B2_OUT;	// inner circle cord colour
			yc = TCOL_B2;		// inner circle fcircle colour
			xd = EBCOL_B2;		// inner bullet colour
			ya = TRANS_B1_OUT;	// outer cord colour
			xc = EBCOL_B1;		// outer bullet colour
			break;
		case 1:
			xa = TRANS_B3_OUT;	// inner circle cord colour
			yc = TCOL_B3;		// inner circle fcircle colour
			xd = EBCOL_B3;		// inner bullet colour
			ya = TRANS_B2_OUT;	// outer cord colour
			xc = EBCOL_B2;		// outer bullet colour
			break;
		case 2:
			xa = TRANS_B3_OUT;	// inner circle cord colour
			yc = TCOL_B3;		// inner circle fcircle colour
			xd = EBCOL_B3;		// inner bullet colour
			ya = TRANS_B1_OUT;	// outer cord colour
			xc = EBCOL_B1;		// outer bullet colour
			break;
		case 3:
			xa = TRANS_B4_OUT;	// inner circle cord colour
			yc = TCOL_B4;		// inner circle fcircle colour
			xd = EBCOL_B4;		// inner bullet colour
			ya = TRANS_B1_OUT;	// outer cord colour
			xc = EBCOL_B1;		// outer bullet colour
			break;

		}
		if (ebullet[b].timeout < 40)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			circlefill (display, x, y, ebullet[b].timeout / 5, xa);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
			fcircle (x, y, 8, yc);
		ye = ebullet[b].timeout;
		if (ye > 130)
			ye = 160 - ye;

		xe = ye;

		if (xe > 21)
			xe = 21;

		for (i = 0; i < 3; i++)
		{
			draw_dart (x, y, ebullet[b].angle + (i * ANGLE_3) + ANGLE_6, xe, ye, ya,
			           xc);
		}

		if (xe > 17)
			xe = 17;

		for (i = 0; i < 3; i++)
		{
//       draw_dart(x, y, (ANGLE_1 - (ebullet[b].angle + (i * ANGLE_3))) & 1023, xe, ye, xa, xd);
			draw_dart (x, y, ebullet[b].angle + (i * ANGLE_3), xe, ye, xa, xd);
		}
		break;
	case EBULLET_LSEED2:
		xb = dart_bullet[ebullet[b].sprite_angle][0].x;
		yb = dart_bullet[ebullet[b].sprite_angle][0].y;
		switch (ebullet[b].colour)
		{
		case 0:
			xa = TRANS_B2_OUT;	// inner circle cord colour
			yc = TCOL_B2;		// inner circle fcircle colour
			xd = EBCOL_B2;		// inner bullet colour
			ya = TRANS_B1_OUT;	// outer cord colour
			xc = EBCOL_B1;		// outer bullet colour
			break;
		case 1:
			xa = TRANS_B3_OUT;	// inner circle cord colour
			yc = TCOL_B3;		// inner circle fcircle colour
			xd = EBCOL_B3;		// inner bullet colour
			ya = TRANS_B2_OUT;	// outer cord colour
			xc = EBCOL_B2;		// outer bullet colour
			break;
		case 2:
			xa = TRANS_B3_OUT;	// inner circle cord colour
			yc = TCOL_B3;		// inner circle fcircle colour
			xd = EBCOL_B3;		// inner bullet colour
			ya = TRANS_B1_OUT;	// outer cord colour
			xc = EBCOL_B1;		// outer bullet colour
			break;
		case 3:
			xa = TRANS_B4_OUT;	// inner circle cord colour
			yc = TCOL_B4;		// inner circle fcircle colour
			xd = EBCOL_B4;		// inner bullet colour
			ya = TRANS_B1_OUT;	// outer cord colour
			xc = EBCOL_B1;		// outer bullet colour
			break;

		}
		if (ebullet[b].timeout < 40)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			circlefill (display, x, y, ebullet[b].timeout / 5, xa);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
			fcircle (x, y, 8, yc);
		ye = ebullet[b].timeout;
		if (ye > 130)
			ye = 160 - ye;

		xe = ye;

		if (xe > 26)
			xe = 26;

		for (i = 0; i < 5; i++)
		{
			draw_dart (x, y, ebullet[b].angle + (i * ANGLE_5) + ANGLE_8, xe, ye, ya,
			           xc);
		}

		if (xe > 16)
			xe = 16;

		for (i = 0; i < 5; i++)
		{
//       draw_dart(x, y, (ANGLE_1 - (ebullet[b].angle + (i * ANGLE_3))) & 1023, xe, ye, xa, xd);
			draw_dart (x, y, ebullet[b].angle + (i * ANGLE_5), xe, ye, xa, xd);
		}
		break;
	case EBULLET_BURST:
		if (ebullet[b].time < 20)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			circlefill (display, x, y, ebullet[b].time / 5, TRANS_B2_OUT);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}
		else
			fcircle (x, y, 4, TCOL_B2);

		for (i = 0; i < 3; i++)
		{
			xe = x + xpart (ebullet[b].angle2 + i * ANGLE_3, 22);
			ye = y + ypart (ebullet[b].angle2 + i * ANGLE_3, 22);
			xc =
			    (int) (ebullet[b].angle2 + i * ANGLE_3) / (ANGLE_1 / SMALL_ROTATIONS);
			xc &= SMALL_ROTATIONS - 1;
			xb = diamond_bullet[xc][0].x;
			yb = diamond_bullet[xc][0].y;
			if (ebullet[b].time < 40)
			{
//        draw_diamond(xe, ye, ebullet[b].angle2 + i * ANGLE_4, (ebullet[b].time * 12) / 37, (ebullet[b].time * 20) / 37, ebullet[b].cord_colour);
				draw_diamond (xe, ye, ebullet[b].angle2 + i * ANGLE_3,
				              (ebullet[b].time * 9) / 37, (ebullet[b].time * 15) / 37,
				              ebullet[b].cord_colour);
			}
			else
			{
				draw_trans_rle_sprite (display,
				                       diamond_bullet[xc][ebullet[b].colour].sprite,
				                       xe - xb, ye - yb);
			}
		}

		ye = ebullet[b].time;
		xe = ye;
		if (xe > 22)
			xe = 22;

		for (i = 0; i < 3; i++)
		{
			draw_dart (x, y, ebullet[b].angle2 + (i * ANGLE_3) + ANGLE_6, xe, ye,
			           TRANS_B1_OUT, EBCOL_B1);
		}



		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, x,
			       y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		break;
		/*
		     case EBULLET_BURST:
		     if (ebullet[b].time < 20)
		     {
		      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		      circlefill(display, x, y, ebullet[b].time / 5, TRANS_B2_OUT);
		      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		     }
		      else
		       fcircle(x, y, 4, TCOL_B2);

		     for (i = 0; i < 4; i ++)
		     {
		      xe = x + xpart(ebullet[b].angle2 + i * ANGLE_4, 22);
		      ye = y + ypart(ebullet[b].angle2 + i * ANGLE_4, 22);
		      xc = (int) (ebullet[b].angle2 + i * ANGLE_4) / (ANGLE_1 / SMALL_ROTATIONS);
		      xc &= SMALL_ROTATIONS - 1;
		      xb = diamond_bullet [xc] [0].x;
		      yb = diamond_bullet [xc] [0].y;
		      if (ebullet[b].time < 40)
		      {
		//        draw_diamond(xe, ye, ebullet[b].angle2 + i * ANGLE_4, (ebullet[b].time * 12) / 37, (ebullet[b].time * 20) / 37, ebullet[b].cord_colour);
		        draw_diamond(xe, ye, ebullet[b].angle2 + i * ANGLE_4, (ebullet[b].time * 9) / 37, (ebullet[b].time * 15) / 37, ebullet[b].cord_colour);
		      }
		       else
		       {
		         draw_trans_rle_sprite(display, diamond_bullet [xc] [ebullet[b].colour].sprite, xe - xb, ye - yb);
		       }
		     }

		      ye = ebullet[b].time;
		      xe = ye;
		      if (xe > 22)
		       xe = 22;

		      for (i = 0; i < 4; i ++)
		      {
		       draw_dart(x, y, ebullet[b].angle2 + (i * ANGLE_4) + ANGLE_8, xe, ye, TRANS_B1_OUT, EBCOL_B1);
		      }



		     if (ebullet[b].cord_wait > -1)
		     {
		      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		      pline(display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, x, y, ebullet[b].cord_colour);
		      circle(display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, 1, ebullet[b].cord_colour);
		      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		     }

		      break;
		*/

	case EBULLET_STREAM:
		xe = x;			// + xpart(ebullet[b].angle2 + i * ANGLE_4, 22);
		ye = y;			// + ypart(ebullet[b].angle2 + i * ANGLE_4, 22);
		xc = (int) (ebullet[b].angle + ANGLE_4) / (ANGLE_1 / SMALL_ROTATIONS);
		xc &= SMALL_ROTATIONS - 1;
		xb = diamond_bullet[xc][0].x;
		yb = diamond_bullet[xc][0].y;
		if (ebullet[b].time < 40)
		{
//        draw_diamond(xe, ye, ebullet[b].angle2 + i * ANGLE_4, (ebullet[b].time * 12) / 37, (ebullet[b].time * 20) / 37, ebullet[b].cord_colour);
			draw_diamond (xe, ye, ebullet[b].angle + ANGLE_4,
			              (ebullet[b].time * 9) / 37, (ebullet[b].time * 15) / 37,
			              ebullet[b].cord_colour);
		}
		else
		{
			draw_trans_rle_sprite (display,
			                       diamond_bullet[xc][ebullet[b].colour].sprite,
			                       xe - xb, ye - yb);
		}

		if (ebullet[b].cord_wait > -1)
		{
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			pline (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN, x,
			       y, ebullet[b].cord_colour);
			circle (display, ebullet[b].cord_x / GRAIN, ebullet[b].cord_y / GRAIN,
			        1, ebullet[b].cord_colour);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		break;

		/*
		     case EBULLET_SEED2:
		     xb = dart_bullet [ebullet[b].sprite_angle] [0].x;
		     yb = dart_bullet [ebullet[b].sprite_angle] [0].y;
		     if (ebullet[b].timeout < 40)
		     {
		      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		      circlefill(display, x, y, ebullet[b].timeout / 5, TRANS_B3_OUT);
		      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		     }
		      else
		       fcircle(x, y, 8, TCOL_B3);
		      ye = ebullet[b].timeout;
		      if (ye > 130)
		       ye = 160 - ye;

		      xe = ye;
		      if (xe > 30)
		       xe = 30;
		//      if (ebullet[b].timeout > 130)
		//       xe = 160 - ebullet[b].timeout;

		      for (i = 0; i < 5; i ++)
		      {
		       xc = (int) (ebullet[b].angle + (i * ANGLE_5)) / (ANGLE_1 / SMALL_ROTATIONS);
		       xc &= SMALL_ROTATIONS - 1;
		       xb = dart_bullet [xc] [0].x;
		       yb = dart_bullet [xc] [0].y;
		       xd = x + xpart(xc * 8, 1 + xe);
		       yd = y + ypart(xc * 8, 1 + xe);
		//       if (ebullet[b].timeout < 40 || ebullet[b].timeout > 130)
		       if (ye < 30)
		       {
		          xa = xc * 8; //ebullet[b].angle;
		          ya = (ye * 8) / 17;
		          yc = (ye * 9) / 17;
		        drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		          poly4(display,
		           xd + xpart(xa, ya), yd + ypart(xa, ya),
		           xd + xpart(xa + ANGLE_4 + ANGLE_8, ya), yd + ypart(xa + ANGLE_4 + ANGLE_8, ya),
		           xd - xpart(xa, yc), yd - ypart(xa, yc),
		           xd + xpart(xa - ANGLE_4 - ANGLE_8, ya), yd + ypart(xa - ANGLE_4 - ANGLE_8, ya),
		           TRANS_B2_OUT);
		        drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		      }
		       else
		        draw_trans_rle_sprite(display, dart_bullet [xc] [1].sprite, xd - xb, yd - yb);
		      }
		      break;


		*/

	}


}



void draw_dart (int x, int y, int angle, int out, int timeout, int cord_col,
                int col)
{
	int xc = (int) angle / (ANGLE_1 / SMALL_ROTATIONS);
	xc &= SMALL_ROTATIONS - 1;
	int xb = dart_bullet[xc][0].x;
	int yb = dart_bullet[xc][0].y;
	int xd = x + xpart (xc * 8, 1 + out);
	int yd = y + ypart (xc * 8, 1 + out);
	if (timeout < 30)
	{
		int xa = xc * 8;		//ebullet[b].angle;
		int ya = (timeout * 8) / 17;
		int yc = (timeout * 9) / 17;
		drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
		poly4 (display,
		       xd + xpart (xa, ya), yd + ypart (xa, ya),
		       xd + xpart (xa + ANGLE_4 + ANGLE_8, ya),
		       yd + ypart (xa + ANGLE_4 + ANGLE_8, ya), xd - xpart (xa, yc),
		       yd - ypart (xa, yc), xd + xpart (xa - ANGLE_4 - ANGLE_8, ya),
		       yd + ypart (xa - ANGLE_4 - ANGLE_8, ya), cord_col);
		drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
	}
	else
		draw_trans_rle_sprite (display, dart_bullet[xc][col].sprite, xd - xb,
		                       yd - yb);
}


void draw_diamond (int x, int y, int angle, int out1, int out2,
                   int cord_colour)
{
	TRANS_MODE
	poly4 (display,
	       x + xpart (angle, out2), y + ypart (angle, out2),
	       x + xpart (angle + ANGLE_4, out1), y + ypart (angle + ANGLE_4,
	               out1),
	       x - xpart (angle, out2), y - ypart (angle, out2),
	       x + xpart (angle - ANGLE_4, out1), y + ypart (angle - ANGLE_4,
	               out1), cord_colour);
	END_TRANS_MODE
}


void draw_pbullets (void)
{

	int b;

	for (b = 0; b < NO_PBULLETS; b++)
	{
		if (pbullet[b].type == PBULLET_NONE)
			continue;
		draw_a_pbullet (b);
	}

}

void draw_a_pbullet (int b)
{

	int x = pbullet[b].x / GRAIN;
	int y = pbullet[b].y / GRAIN;

	int i, xa, ya;		//, xa, ya;

	switch (pbullet[b].type)
	{
	case PBULLET_RING:
		ya = pbullet[b].status / 10 + xpart (arena.counter * 64, 2) - 2;
		if (ya < 1)
			ya = 1;
		/*        if (xa < 1)
		         xa = 1;
		        if (xa > 50)
		         xa = 50;
		        xb = cloud[c].colour;
		     ya = ;*/
		draw_trans_rle_sprite (display, RLE_ring[ya].sprite, x - RLE_ring[ya].x,
		                       y - RLE_ring[ya].y);
		break;
	case PBULLET_SCATTER:
		xa = grand (pbullet[b].level) / 2;
		fcircle (x, y, 1 + grand (2) + xa, TCOL_YELLOW);
		fcircle (x, y, 4 + grand (2) + xa, TCOL_ORANGE);
//      fcircle(x - xpart(pbullet[b].angle, 4), y - ypart(pbullet[b].angle, 4), 3 + grand(2), 7);
		fcircle (x - xpart (pbullet[b].angle, 6 + xa),
		         y - ypart (pbullet[b].angle, 6 + xa), 3 + grand (2) + xa,
		         TCOL_ORANGE);
		fcircle (x - xpart (pbullet[b].angle, 10 + xa),
		         y - ypart (pbullet[b].angle, 9 + xa), 2 + grand (2) + xa,
		         TCOL_ORANGE);
		break;
	case PBULLET_CANNON:
	case PBULLET_RECT:
		ya = grand (pbullet[b].level) / 2;
		fcircle (x, y, 7 + grand (2) + ya, TCOL_YELLOW);
		fcircle (x, y + 2, 12 + grand (3) + ya, TCOL_ORANGE);
		break;
	case PBULLET_SEEKER:
		fcircle (x, y, 4, TCOL_YELLOW);
		/*      if (pbullet[b].target != -1)
		      {
		         pline(display, x, y, enemy[pbullet[b].target].x / GRAIN, enemy[pbullet[b].target].y / GRAIN, COL_LGREY);
		      }*/
		break;
	case PBULLET_MGUN:
		xa = pbullet[b].level * 2;
		drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
		vline (display, x, y, y + 16 + xa, TRANS_ORANGE_IN);
		vline (display, x - 1, y, y + 16 + xa, TRANS_ORANGE_OUT);
		vline (display, x + 1, y, y + 16 + xa, TRANS_ORANGE_OUT);
		putpixel (display, x, y - 1, TRANS_ORANGE_OUT);
		putpixel (display, x, y + 17 + xa, TRANS_ORANGE_OUT);
		for (i = 0; i < 3 + pbullet[b].level / 2; i++)
		{
			ya = y + grand (14 + xa);
			putpixel (display, x - 1, ya, TRANS_ORANGE_IN);
			putpixel (display, x + 1, ya, TRANS_ORANGE_IN);
			putpixel (display, x - 2, ya, TRANS_ORANGE_OUT);
			putpixel (display, x + 2, ya, TRANS_ORANGE_OUT);

		}
		drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		break;

	}

}



void draw_clouds (void)
{

	int c;

	for (c = 0; c < NO_CLOUDS; c++)
	{
		if (cloud[c].type == CLOUD_NONE)
			continue;
		draw_a_cloud (c);
	}

}

void draw_a_cloud (int c)
{

	int x = cloud[c].x / GRAIN;
	int y = cloud[c].y / GRAIN;
	int xa, ya, xb, i, angle, yb;

	switch (cloud[c].type)
	{
	case CLOUD_SCIRCLE:
		TRANS_MODE circle (display, x, y, cloud[c].timeout, cloud[c].colour);
		END_TRANS_MODE break;
	case CLOUD_DELAY1:
		break;
	case CLOUD_DELAY2:
	case CLOUD_DELAY3:
	case CLOUD_DELAY4:
		fcircle (x, y, cloud[c].x2, cloud[c].colour);
		xa = cloud[c].x2 - 5;
		if (xa > 0)
			fcircle (x, y, xa, cloud[c].colour + 1);
		break;
	case CLOUD_PETAL1_DEBRIS:
		ya = cloud[c].angle / (ANGLE_1 / SMALL_ROTATIONS);
		ya &= SMALL_ROTATIONS - 1;
		xb = petal1[ya].x;		// - xpart(cloud[c].angle, 36);
		yb = petal1[ya].y;		// - ypart(cloud[c].angle, 36);
		draw_trans_rle_sprite (display, petal1[ya].sprite, x - xb, y - yb);
		break;
	case CLOUD_PETAL2_DEBRIS:
		ya = cloud[c].angle / (ANGLE_1 / SMALL_ROTATIONS);
		ya &= SMALL_ROTATIONS - 1;
		xb = petal2[ya].x;		// - xpart(cloud[c].angle, 36);
		yb = petal2[ya].y;		// - ypart(cloud[c].angle, 36);
		draw_trans_rle_sprite (display, petal2[ya].sprite, x - xb, y - yb);
		break;
	case CLOUD_PETAL3_DEBRIS:
		ya = cloud[c].angle / (ANGLE_1 / SMALL_ROTATIONS);
		ya &= SMALL_ROTATIONS - 1;
		xb = petal3[ya].x;		// - xpart(cloud[c].angle, 36);
		yb = petal3[ya].y;		// - ypart(cloud[c].angle, 36);
		draw_trans_rle_sprite (display, petal3[ya].sprite, x - xb, y - yb);
		break;
	case CLOUD_PETAL1:
		TRANS_MODE xa = cloud[c].timeout;
		xb = cloud[c].timeout / 2;
		angle = cloud[c].angle;
		poly4 (display, x, y,
		       x + xpart (angle + ANGLE_8 - ANGLE_32, xb),
		       y + ypart (angle + ANGLE_8 - ANGLE_32, xb), x + xpart (angle, xa),
		       y + ypart (angle, xa), x + xpart (angle - ANGLE_8 + ANGLE_32, xb),
		       y + ypart (angle - ANGLE_8 + ANGLE_32, xb), TRANS_SH2_OUT);
		END_TRANS_MODE break;
	case CLOUD_PRING:
		ya = cloud[c].timeout / 10;
		if (ya < 0)
			ya = 0;
		draw_trans_rle_sprite (display, RLE_ring[ya].sprite, x - RLE_ring[ya].x,
		                       y - RLE_ring[ya].y);
		break;
	case CLOUD_SLINE:
		TRANS_MODE
		pline (display, x, y, cloud[c].x2 / GRAIN, cloud[c].y2 / GRAIN,
		       cloud[c].colour);
		if (cloud[c].timeout <= 1)
			fcircle (x, y, 2, TCOL_ORANGE);
//       circle(display, x, y, 1, TRANS_ORANGE_OUT);
		END_TRANS_MODE break;
	case CLOUD_ESLINE:
		TRANS_MODE
		pline (display, x, y, cloud[c].x2 / GRAIN, cloud[c].y2 / GRAIN,
		       cloud[c].colour);
		if (cloud[c].timeout <= 1)
			circle (display, x, y, 1, cloud[c].colour);
		END_TRANS_MODE break;
	case CLOUD_RSHOCK1:
	case CLOUD_RSHOCK2:
	case CLOUD_RSHOCK3:
	case CLOUD_RSHOCK4:
	case CLOUD_RSHOCK5:
		xa = cloud[c].timeout;
		if (xa < 1)
			xa = 1;
		if (xa > 20)
			xa = 20;
		xb = cloud[c].type - CLOUD_RSHOCK1;
		ya = RLE_rshock[xb][20 - xa].x;
		draw_trans_rle_sprite (display, RLE_rshock[xb][20 - xa].sprite, x - ya,
		                       y - ya);
		break;
	case CLOUD_MSHOCK:
		xa = cloud[c].timeout;
		if (xa < 1)
			xa = 1;
		if (xa > 20)
			xa = 20;
		xb = cloud[c].colour;
		ya = RLE_mshock[xb][20 - xa].x;
		draw_trans_rle_sprite (display, RLE_mshock[xb][20 - xa].sprite, x - ya,
		                       y - ya);
		break;
	case CLOUD_LSHOCK:
		xa = cloud[c].timeout;
		if (xa < 1)
			xa = 1;
		if (xa > 50)
			xa = 50;
		xb = cloud[c].colour;
		ya = RLE_lshock[xb][50 - xa].x;
		draw_trans_rle_sprite (display, RLE_lshock[xb][50 - xa].sprite, x - ya,
		                       y - ya);
		break;
	case CLOUD_HSHOCK:
		xa = cloud[c].timeout;
//        xa = 5 - (xa / 10);
		if (xa < 1)
			xa = 1;
		if (xa > 50)
			xa = 50;
		xb = cloud[c].colour;
		ya = RLE_hshock[xb][50 - xa].x;
		draw_trans_rle_sprite (display, RLE_hshock[xb][50 - xa].sprite, x - ya,
		                       y - ya);
		break;

	case CLOUD_TSHOCK:
		xa = 12 - cloud[c].timeout;
		ya = RLE_mg_shock[xa].x;
		if (xa >= 9)
		{
			TRANS_MODE circle (display, x, y, xa + 2, TRANS_ORANGE_OUT);
			END_TRANS_MODE break;
		}
		draw_trans_rle_sprite (display, RLE_mg_shock[xa].sprite, x - ya, y - ya);
		break;
	case CLOUD_MGSHOCK:
		xa = 12 - cloud[c].timeout;
//        textprintf_ex(display, font, x, y + 20, COL_WHITE, -1, "%i", xa);
		if (cloud[c].timeout > 5)
			fcircle (x, y, cloud[c].timeout - 5, TCOL_YELLOW);
		if (xa >= 9)
		{
			TRANS_MODE circle (display, x, y, xa + 2, TRANS_ORANGE_OUT);
			END_TRANS_MODE break;
		}
		ya = RLE_mg_shock[xa].x;
		draw_trans_rle_sprite (display, RLE_mg_shock[xa].sprite, x - ya, y - ya);
		if (xa < 10)
		{
			angle = cloud[c].x2;
			for (i = 0; i < 4; i++)
			{
				fcircle (x + xpart (angle, ya + 2), y + ypart (angle, ya + 2),
				         (10 - xa) / 1, TCOL_ORANGE);
				angle += ANGLE_4;
			}


		}

		/*if (xa < 10)
		   {
		   fcircle(x - ya + 2, y - ya + 2, (10 - xa) / 1, 7);
		   fcircle(x + ya - 2, y - ya + 2, (10 - xa) / 1, 7);
		   fcircle(x - ya + 2, y + ya - 2, (10 - xa) / 1, 7);
		   fcircle(x + ya - 2, y + ya - 2, (10 - xa) / 1, 7);
		   } */
//       circle(display, x, y, 10, COL_WHITE);
		break;
	case CLOUD_CIRCLE10:
		fcircle (x, y, cloud[c].timeout / 10, cloud[c].colour);
		break;
	case CLOUD_CIRCLE5:
		fcircle (x, y, cloud[c].timeout / 5, cloud[c].colour);
		break;
	case CLOUD_CIRCLE3:
		fcircle (x, y, cloud[c].timeout / 3, cloud[c].colour);
		break;
	case CLOUD_CIRCLE2:
		fcircle (x, y, cloud[c].timeout / 2, cloud[c].colour);
		break;
	case CLOUD_CIRCLE1:
//     case CLOUD_SCIRCLE:
		fcircle (x, y, cloud[c].timeout, cloud[c].colour);
		break;
	case CLOUD_CARRIER1_BANG:
		xa = 100 - cloud[c].timeout;
		ya = (cloud[c].timeout);
		angle = cloud[c].x2;
		for (i = 0; i < 6; i++)
		{
			fcircle (x + xpart (angle, xa), y + ypart (angle, xa), ya, TCOL_SH1);
			fcircle (x + xpart (angle, xa - 4), y + ypart (angle, xa - 4), ya / 2,
			         TCOL_SH2);
			angle += ANGLE_6;
			angle &= 1023;
		}
		angle = ANGLE_1 - cloud[c].x2;
		angle &= 1023;
		for (i = 0; i < 6; i++)
		{
			fcircle (x + xpart (angle, xa), y + ypart (angle, xa), ya, TCOL_SH1);
			fcircle (x + xpart (angle, xa - 4), y + ypart (angle, xa - 4), ya / 2,
			         TCOL_SH2);
			angle += ANGLE_6;
			angle &= 1023;
		}
		break;
	}


}





void draw_background (void)
{
	int i, j;
	int x, y, xb;			//, xa, xb;

	int start = platform_step;
	int across = 0;

// blit(underlay, display, 0, arena.underlay_position, 0, 0, 640, 480);
	clear_to_color (display, COL_OUTLINE);

	for (i = 0; i < 20; i++)
	{
		xb = arena.block2_y[i] / 100;
		rectfill (display, arena.block2_x[i] - arena.block2_size[i],
		          xb - arena.block2_size[i],
		          arena.block2_x[i] + arena.block2_size[i],
		          xb + arena.block2_size[i], COL_BACK1);
		/*   drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		       rect(display, arena.block2_x [i] - arena.block2_size [i] - 1, xb - arena.block2_size [i] - 1,
		         arena.block2_x [i] + arena.block2_size [i] + 1, xb + arena.block2_size [i] + 1, TRANS_BACK2_OUTLINE);
		     drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);*/
//     circlefill(display, arena.block2_x [i], arena.block2_y [i], arena.block2_size [i], COL_BACK1);
//     circle(display, arena.block2_x [i], arena.block2_y [i], arena.block2_size [i], COL_OUTLINE);
	}

	for (i = 0; i < 20; i++)
	{
//     circlefill(display, arena.block2_x [i], arena.block1_y [i], arena.block1_size [i], COL_BACK2);
//     circle(display, arena.block1_x [i], arena.block1_y [i], arena.block1_size [i], COL_OUTLINE);
		xb = arena.block1_y[i] / 100;
		rectfill (display, arena.block1_x[i] - arena.block1_size[i],
		          xb - arena.block1_size[i],
		          arena.block1_x[i] + arena.block1_size[i],
		          xb + arena.block1_size[i], COL_BACK2);
		drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
		rect (display, arena.block1_x[i] - arena.block1_size[i] - 1,
		      xb - arena.block1_size[i] - 1,
		      arena.block1_x[i] + arena.block1_size[i] + 1,
		      xb + arena.block1_size[i] + 1, TRANS_BACK3_OUTLINE);
		drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
	}

	for (j = 0; j < 7; j++)
	{
		across = start - j;
		if (across < 0)
			across += PLAT_Y;
		if (across >= PLAT_Y)
			across -= PLAT_Y;

		for (i = 0; i < 9; i++)
		{
			if (platform_tile[i][across] != PLATFORM_EMPTY)
			{
				x = i * 91 - 82;
				y = j * 91 - 95 + platform_position;
				if (platform_tile[i][across] >= PLATFORM_NODE2)
				{
					xb = arena.flower_turn1 / 10;
					if ((i + across) % 3 == 0)
						xb = arena.flower_turn2 / 10;
					if ((i + across) % 2 == 0)
						xb = 39 - xb;
					/*           if (xb < 0)
					            xb = 0;
					           if (xb > 19)
					            xb = 19;*/
					draw_rle_sprite (display, eRLE_flower[0][xb].sprite,
					                 x - eRLE_flower[0][xb].x + 42 + 3,
					                 y - eRLE_flower[0][xb].y + 47);
				}
			}

		}
	}

	for (j = 0; j < 7; j++)
	{
		across = start - j;
		if (across < 0)
			across += PLAT_Y;
		if (across >= PLAT_Y)
			across -= PLAT_Y;

		for (i = 0; i < 9; i++)
		{
			if (platform_tile[i][across] != PLATFORM_EMPTY)
			{
				x = i * 91 - 82;
				y = j * 91 - 95 + platform_position;
				draw_rle_sprite (display, platform_RLE[platform_tile[i][across]], x,
				                 y);
			}
		}
	}


}



void draw_background2 (void)
{
	int i, j;
	int x, y, xb;			//, xa, xb;

	int start = island1_step;
	int across = 0;

	clear_to_color (display, COL_OUTLINE);

	for (i = 0; i < NO_STARS; i++)
	{
		for (j = 0; j < 3; j++)
		{
			xb = star_y[i][j] / GRAIN;
			y = (star_y[i][j] - star_y_speed[i][j]) / GRAIN;
			if (xb == y)
			{
				putpixel (display, star_x[i][j], xb, COL_BACK3 - j);
				putpixel (display, star_x[i][j] + 1, xb, COL_OUTLINE);
				putpixel (display, star_x[i][j] - 1, xb, COL_OUTLINE);
				putpixel (display, star_x[i][j], xb + 1, COL_OUTLINE);
				putpixel (display, star_x[i][j], xb - 1, COL_OUTLINE);
			}
			else
			{
				vline (display, star_x[i][j], xb, y, COL_BACK3 - j);
				vline (display, star_x[i][j] - 1, xb, y, COL_OUTLINE);
				vline (display, star_x[i][j] + 1, xb, y, COL_OUTLINE);
				putpixel (display, star_x[i][j], xb + 1, COL_OUTLINE);
				putpixel (display, star_x[i][j], y - 1, COL_OUTLINE);
			}
			/*   xb = star_y [i] [j] / GRAIN;
			   putpixel(display, star_x [i] [j], xb, COL_BACK3 - j);
			   putpixel(display, star_x [i] [j] + 1, xb, COL_OUTLINE);
			   putpixel(display, star_x [i] [j] - 1, xb, COL_OUTLINE);
			   putpixel(display, star_x [i] [j], xb + 1, COL_OUTLINE);
			   putpixel(display, star_x [i] [j], xb - 1, COL_OUTLINE);
			*/
		}
	}


	for (j = 0; j < 7; j++)
	{
		across = start - j;
		if (across < 0)
			across += ISLAND1_Y;
		if (across >= ISLAND1_Y)
			across -= ISLAND1_Y;

		for (i = 0; i < 9; i++)
		{
			if (island1_tile[i][across] > ISLAND1_EMPTY
			        && island1_tile[i][across] < ISLAND1_RLES)
			{
				x = i * 91 - 82;
				y = j * 91 - 95 + island1_position;
				draw_rle_sprite (display, island1_RLE[island1_tile[i][across]], x, y);
//         rect(display, x + 20, y + 20, x + 70, y + 70, COL_WHITE);
//         rect(display, x, y, x + 90, y + 90, COL_WHITE);
			}
		}
	}


}


void draw_background3 (void)
{
	int i, j, out, col, waved, cy;
	int p_rank = arena.part_rank;

	END_TRANS_MODE clear_to_color (display, COL_BACK1);

	int i_dir;

	for (j = 0; j < 14; j++)
	{
		if (p_rank % 2 == 0)
		{
			i = 15;
			i_dir = -1;
		}
		else
		{
			i = 0;
			i_dir = 1;
		}
//  for (i = 0; i < 16; i ++)
		do
		{
			col = COL_BACK2;
			out = 0;
			cy =
			    (-40 + (j * 40) + arena.part_cy[i][p_rank] + arena.part_flow) * 1000;
			if (arena.wave_y > cy - 90000 && arena.wave_y < cy + 90000)
			{
				waved = arena.wave_y - cy;
				if (waved < 0)
					waved *= -1;
				out = (90000 - waved) / 18000;
				col += (90000 - waved) / 23000;
				if (col > COL_BACK5)
					col = COL_BACK5;
			}
			if ((i + p_rank) % 2 == 0)
				out += arena.part_pulse1 / 2;
			else
				out += arena.part_pulse2 / 2;

			rectfill (display, 20 + (i * 40) + arena.part_x1[i][p_rank] - out,
			          -40 + (j * 40) + arena.part_y1[i][p_rank] + arena.part_flow -
			          out, 20 + (i * 40) + arena.part_x2[i][p_rank] + out,
			          -40 + (j * 40) + arena.part_y2[i][p_rank] + arena.part_flow +
			          out, col);
			rect (display, 20 + (i * 40) + arena.part_x1[i][p_rank] - out,
			      -40 + (j * 40) + arena.part_y1[i][p_rank] + arena.part_flow - out,
			      20 + (i * 40) + arena.part_x2[i][p_rank] + out,
			      -40 + (j * 40) + arena.part_y2[i][p_rank] + arena.part_flow + out,
			      COL_OUTLINE);

			i += i_dir;
			if (i < 0 || i > 15)
				break;
		}
		while (TRUE);
		p_rank--;
		if (p_rank < 0)
			p_rank = 13;
	}

}


void draw_background4 (void)
{

	clear_to_color (display, COL_BACK1);

	int i;

	for (i = 0; i < NO_BONES; i++)
	{
//  circle(display, arena.bone_x [0] [i],    arena.bone_y [0] [i], 10, COL_WHITE);
		if (arena.bone_y[0][i] > -50)
		{
			switch (arena.bone_type[0][i])
			{
			case 0:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_B_HOOK1 + arena.bone_subtype[0][i]],
				                 arena.bone_x[0][i] - 4, arena.bone_y[0][i] - 4);
				break;
			case 1:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_B_DROP1 + arena.bone_subtype[0][i]],
				                 arena.bone_x[0][i] - 5, arena.bone_y[0][i] - 4);
				break;
			case 2:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_B_RING1 + arena.bone_subtype[0][i]],
				                 arena.bone_x[0][i] - 11, arena.bone_y[0][i] - 11);
				break;
			case 3:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_B_HUNTER1 + arena.bone_subtype[0][i]],
				                 arena.bone_x[0][i] - 13, arena.bone_y[0][i] - 11);
				break;
			case 4:
				draw_rle_sprite (display, bone_RLE[BONE_B_SPINNER],
				                 arena.bone_x[0][i] - 32, arena.bone_y[0][i] - 53);
				break;
			case 5:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_B_WALKER1 + arena.bone_subtype[0][i]],
				                 arena.bone_x[0][i] - 44, arena.bone_y[0][i] - 37);
				break;
			}


		}

	}


	for (i = 0; i < 20; i++)
	{
//  circle(display, arena.bone_x [0] [i],    arena.bone_y [0] [i], 10, COL_WHITE);
		if (arena.bone_y[1][i] > -30)
		{
			switch (arena.bone_type[1][i])
			{
			case 0:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_HOOK1 + arena.bone_subtype[1][i]],
				                 arena.bone_x[1][i] - 11, arena.bone_y[1][i] - 10);
				break;
			case 1:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_DROP1 + arena.bone_subtype[1][i]],
				                 arena.bone_x[1][i] - 12, arena.bone_y[1][i] - 17);
				break;
			case 2:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_RING1 + arena.bone_subtype[1][i]],
				                 arena.bone_x[1][i] - 19, arena.bone_y[1][i] - 19);
				break;
			case 3:
				draw_rle_sprite (display,
				                 bone_RLE[BONE_HUNTER1 + arena.bone_subtype[1][i]],
				                 arena.bone_x[1][i] - 22, arena.bone_y[1][i] - 20);
				break;

			}


		}

	}

}


void shift_message (void)
{
	int x, y, angle, xa, ya, i;

	switch (arena.level)
	{
	case 1:
		x = (arena.shift_message * 2) + 40;
		if (x < 350)
			x = 350;
		y = 430;
		if (arena.shift_message < 50)
			y = 480 - arena.shift_message;

		rectfill (display, x, y, 640, y + 23, COL_BACK3);
		rect (display, x, y, 640, y + 23, COL_OUTLINE);
		x += 25;
		y += 5;
		switch (arena.phase)
		{
		case 0:
			textprintf_ex (display, font, x, y, -1, -1, "YOU ARE BORN INTO SPRING");
			break;
		case 1:
			textprintf_ex (display, font, x, y, -1, -1, "THE SUN IS HIGH ABOVE");
			break;
		case 2:
			textprintf_ex (display, font, x, y, -1, -1,
			               "AUTUMN IS THE WINDING DOWN");
			break;
		case 3:
			textprintf_ex (display, font, x, y, -1, -1,
			               "NO NEW LIFE WITHOUT DEATH");
			break;
		case 4:
			textprintf_ex (display, font, x, y, -1, -1, "THE CYCLE IS RENEWED");
			break;
		}
		if (arena.shift_message < 150)
		{
			x = 50 / 3;
			if (arena.shift_message > 100)
			{
				x = 150 - arena.shift_message;
				x /= 3;
			}
			if (arena.shift_message < 50)
			{
				x = arena.shift_message;
				x /= 3;
			}
			angle = arena.phase * ANGLE_4 - ANGLE_4 - ANGLE_4;
			if (arena.shift_message <= 100)
			{
				y = ((100 - arena.shift_message)) * 5;
				if (y > ANGLE_4)
					y = ANGLE_4;
				if (y < 0)
					y = 0;
				angle += y;
			}
			angle &= 1023;
			if (arena.phase == 0)
				angle = ANGLE_4 + ANGLE_2;
			else if (arena.shift_message < 50)
				angle = arena.phase * ANGLE_4 - ANGLE_4;
			if (x > 0)
			{
				circlefill (display, 350, 441, x, TRANS_YELLOW_IN + 2);
				circle (display, 350, 441, x, TRANS_YELLOW_OUT + 2);
//       circle(display, 350, 441, x * 1.2, TRANS_YELLOW_OUT + 2);
//       circle(display, 350, 441, x, COL_OUTLINE);
			}
			x /= 3;
			if (x > 0)
			{
				circlefill (display, 350 + xpart (angle, 28), 441 + ypart (angle, 28),
				            x, COL_COL4);
				circle (display, 350 + xpart (angle, 28), 441 + ypart (angle, 28), x,
				        COL_OUTLINE);
			}
		}

		break;
	case 2:
		x = (arena.shift_message * 2) + 40;
		if (x < 340)
			x = 340;
		y = 430;
		if (arena.shift_message < 50)
			y = 480 - arena.shift_message;

		rectfill (display, x, y, 640, y + 23, COL_BACK3);
		rect (display, x, y, 640, y + 23, COL_OUTLINE);
		x += 25;
		y += 5;
		switch (arena.phase)
		{
		case 0:
			textprintf_ex (display, font, x, y, -1, -1, "IT IS COLD IN THE VOID");
			break;
		case 1:
			textprintf_ex (display, font, x, y, -1, -1,
			               "DISTANT STARS WARM THE DUST");
			break;
		case 2:
			textprintf_ex (display, font, x, y, -1, -1,
			               "DESTRUCTION BLOWS A HOT WIND");
			break;
		case 3:
			textprintf_ex (display, font, x, y, -1, -1,
			               "ORDER TURNS TO BOILING CHAOS");
			break;
		case 4:
			textprintf_ex (display, font, x, y, -1, -1,
			               "THE EQUILIBRIUM OF HEAT DEATH");
			break;
		}
		if (arena.shift_message < 150)
		{
			x = 50 / 3;
			if (arena.shift_message > 100)
			{
				x = 150 - arena.shift_message;
				x /= 3;
			}
			if (arena.shift_message < 50)
			{
				x = arena.shift_message;
				x /= 3;
			}
			TRANS_MODE switch (arena.phase)
			{
			case 0:
				if (x > 7)
					x = 7;
				fcircle (340, 441, x, TCOL_B3);
				break;
			case 1:
				if (x > 9)
					x = 9;
				fcircle (340, 441, x, TCOL_SH1);
				break;
			case 2:
				fcircle (340, 441, x, TCOL_SH2);
				y = (arena.shift_message / 2) % 6;
				circle (display, 340, 441, x - y + 5, TRANS_SH1_OUT);
				break;
			case 3:
				fcircle (340 + grand (3) - grand (3), 441 + grand (3) - grand (3), x,
				         TCOL_ORANGE);
				fcircle (340 + grand (4) - grand (4), 441 + grand (4) - grand (4),
				         x + 5 + grand (3), TCOL_SH2);
				y = (arena.shift_message / 2) % 8;
				circle (display, 340, 441, x - y + 8, TRANS_SH2_OUT);
				break;
			case 4:
				fcircle (340 + grand (4) - grand (4), 441 + grand (4) - grand (4), x,
				         TCOL_YELLOW);
				fcircle (340 + grand (4) - grand (4), 441 + grand (4) - grand (4),
				         x + 6 + grand (3), TCOL_ORANGE);
				y = (arena.shift_message / 1) % 9;
				circle (display, 340, 441, x - y + 10, TRANS_YELLOW_OUT);
				break;
			}
			END_TRANS_MODE
		}
		break;

	case 3:
		x = (arena.shift_message * 2) + 40;
		if (x < 330)
			x = 330;
		y = 430;
		if (arena.shift_message < 50)
			y = 480 - arena.shift_message;

		rectfill (display, x, y, 640, y + 23, COL_BACK3);
		rect (display, x, y, 640, y + 23, COL_OUTLINE);
		x += 8;
		y += 5;
		switch (arena.phase)
		{
		case 0:
			textprintf_ex (display, font, x, y, -1, -1,
			               "MOTION BENDS THE FABRIC OF SPACE");
			break;
		case 1:
			textprintf_ex (display, font, x, y, -1, -1,
			               "THE WAVES SEEM TO ACCELERATE");
			break;
		case 2:
			textprintf_ex (display, font, x, y, -1, -1,
			               "THE OUTER REGIONS CALL TO YOU");
			break;
		case 3:
			textprintf_ex (display, font, x, y, -1, -1,
			               "SPEED SHIFTS LIGHT TO BLUE");
			break;
		case 4:
			textprintf_ex (display, font, x, y, -1, -1,
			               "THERE IS NO PATH BACK FROM HERE");
			break;
		}
		if (arena.shift_message < 200)
		{
			y = 10;
			if (arena.shift_message > 160)
			{
				y = (200 - arena.shift_message) / 4;
			}
			if (arena.shift_message < 40)
			{
				y = arena.shift_message / 4;
			}
			if (y <= 0)
				break;
			TRANS_MODE angle = arena.shift_message * ((1 + arena.phase * 9) * 4);
			angle &= 1023;
			int old_angle = (arena.shift_message + 1) * ((1 + arena.phase * 9) * 4);
			for (i = 0; i < 25; i++)
			{
//      angle += 30 + (arena.phase * 60);
//      old_angle = angle - y * (10 + (arena.phase * 8));
				angle += 30 + (arena.phase * 20);
				old_angle = angle - y * (10 + (arena.phase * 4));
				x = 215 + i * 5;
				xa = xpart (angle, 10);
				ya = xpart (old_angle, 10);
//      if (abs(xa - ya) > )
//       ya = y;
				vline (display, x, 441 + xa, 441 + ya, TRANS_ORANGE_OUT);

			}

			/*     angle = arena.shift_message * (5 + arena.phase);
			     angle &= 1023;
			     int old_angle;
			     for (i = 0; i < 15; i ++)
			     {
			      old_angle = angle;
			      angle += (5 + arena.phase) * 29;
			      x = 250 + i * 4;
			      xa = xpart(angle, 7);
			      if (angle < old_angle)
			       ya = old_angle - angle;
			        else
			         ya = angle - old_angle;
			      if (ya > y)
			       ya = y;
			      rectfill(display, x - 1, 441 + xa, x + 4, 445 + xa + ya, TRANS_ORANGE_IN);
			      rect(display, x - 2, 440 + xa, x + 5, 446 + xa + ya, TRANS_ORANGE_OUT);

			     }*/
			END_TRANS_MODE
		}
		break;

	case 4:
		x = (arena.shift_message * 2) + 40;
		if (x < 330)
			x = 330;
		y = 430;
		if (arena.shift_message < 50)
			y = 480 - arena.shift_message;

		rectfill (display, x, y, 640, y + 23, COL_BACK3);
		rect (display, x, y, 640, y + 23, COL_OUTLINE);
		x += 8;
		y += 5;
		switch (arena.phase)
		{
		case 0:
			textprintf_ex (display, font, x, y, -1, -1, "THINGS COME HERE TO DIE");
			break;
		case 1:
			textprintf_ex (display, font, x, y, -1, -1,
			               "BONES FADE INTO THE GLOOM");
			break;
		case 2:
			textprintf_ex (display, font, x, y, -1, -1, "CRUMBLING IN SILENCE");
			break;
		case 3:
			textprintf_ex (display, font, x, y, -1, -1, "GONE THEN FORGOTTEN");
			break;
		case 4:
			textprintf_ex (display, font, x, y, -1, -1, "DUST TO DUST");
			break;
		}
		break;


	}




}

void display_pause (int pc)
{
	vsync ();
	rectfill (screen, 200, 100, 440, 200, TRANS_ORANGE_IN - 1);
	rect (screen, 200, 100, 440, 200, TRANS_ORANGE_OUT - 1);

	rect (screen, 190, 90, 450, 210, TRANS_YELLOW_IN - 1);
	rect (screen, 191, 91, 449, 209, TRANS_YELLOW_OUT - 1);
	rect (screen, 189, 89, 451, 211, TRANS_YELLOW_OUT - 1);

	textprintf_centre_ex (screen, font, 320, 125, -1, -1, "QUIT?");


	textprintf_centre_ex (screen, font, 320, 150, -1, -1,
	                      "PRESS  'Y'  TO  QUIT");
	textprintf_centre_ex (screen, font, 320, 165, -1, -1,
	                      "OR  'N'  TO  CONTINUE");

}

/*
void run_underlay(void)
{

 arena.underlay_position -= 2;
 if (arena.underlay_position <= 0)
  arena.underlay_position += 480;


}

*/

void fcircle (int x, int y, int size, int col)
{
	if (size < 0)
		size = 0;
	if (size >= RLE_CIRCLES)
		size = RLE_CIRCLES - 1;

	draw_trans_rle_sprite (display, RLE_circle[size][col], x - size - 1,
	                       y - size - 1);

}





/*

Use this instead of Allegro's line function - it has better bounds checking.
Sometimes for some reason I get an extremely long line which slows things down.

*/
void pline (BITMAP * bmp, int x1, int y1, int x2, int y2, int colour)
{
	if (x1 < -500 || x1 > 900
	        || x2 < -500 || x2 > 900
	        || y2 < -500 || y2 > 900 || y2 < -500 || y2 > 900)
		return;

	line (bmp, x1, y1, x2, y2, colour);

}



void poly4 (BITMAP * target, int x1, int y1, int x2, int y2, int x3, int y3,
            int x4, int y4, int col)
{

	points4[0] = x1;
	points4[1] = y1;
	points4[2] = x2;
	points4[3] = y2;
	points4[4] = x3;
	points4[5] = y3;
	points4[6] = x4;
	points4[7] = y4;

	polygon (target, 4, points4, col);

}


void print_number (int x, int y, int n)
{
	textprintf_ex (display, font, x, y, -1, -1, "%i", n);
}
