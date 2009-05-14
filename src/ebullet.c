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

#include "data.h"

#include <math.h>

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"
#include "enemy.h"
#include "sound.h"
#include "cloud.h"

#include "palette.h"

void run_ebullets (void);
void destroy_ebullet (int b);
//int check_ebullet_collision(int b);
void run_standard_cord (int b);
void drag_ebullet (int b, int drag);

/*
Initialises the enemy bullets. Must be called at start of each game/level.
*/
void init_ebullets (void)
{

	int b;

	for (b = 0; b < NO_EBULLETS; b++)
	{
		ebullet[b].type = EBULLET_NONE;
	}

}

/*
Creates an enemy bullet of type type and returns the index in the ebullet struct array.
*/
int create_ebullet (int type)
{
	int b;

	for (b = 0; b < NO_EBULLETS; b++)
	{
		if (ebullet[b].type == EBULLET_NONE)
			break;
		if (b == NO_EBULLETS - 1)
			return -1;

	}

	ebullet[b].type = type;
	ebullet[b].time = 0;
	ebullet[b].new_ebullet = 1;
	return b;

}

/*
Call this from the game loop to make the ebullets work.
*/
void run_ebullets (void)
{

	int b, i, angle, k, c;
	char no_more_bullet = 0;

	for (b = 0; b < NO_EBULLETS; b++)
	{
		if (ebullet[b].type == EBULLET_NONE)
			continue;
		no_more_bullet = 0;
		ebullet[b].time++;
		switch (ebullet[b].type)
		{
		case EBULLET_TRACKER:
			ebullet[b].x_speed += xpart (ebullet[b].angle, 200);
			ebullet[b].y_speed += ypart (ebullet[b].angle, 200);
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;

			c = create_cloud (CLOUD_ESLINE);
			if (c != -1)
			{
				cloud[c].x2 = ebullet[b].x;
				cloud[c].y2 = ebullet[b].y;
				cloud[c].x = ebullet[b].x - ebullet[b].x_speed;
				cloud[c].y = ebullet[b].y - ebullet[b].y_speed;
				cloud[c].timeout = 9;
				cloud[c].colour = TRANS_B1_OUT;
			}


			ebullet[b].x_speed *= 98;
			ebullet[b].x_speed /= 100;
			ebullet[b].y_speed *= 98;
			ebullet[b].y_speed /= 100;
			if (ebullet[b].timeout > 430 - (arena.difficulty * 40)
			        && ebullet[b].timeout < 490)
				ebullet[b].angle =
				    turn_towards_xy (ebullet[b].x, ebullet[b].y, player.x, player.y,
				                     ebullet[b].angle, 10 + arena.difficulty * 4);

			break;
		case EBULLET_SPIN:
		case EBULLET_BEAM:
		case EBULLET_BEAM2:
			break;
		case EBULLET_SEED:
			if (ebullet[b].cord_wait == -1)
			{
				drag_ebullet (b, 910);
				if (ebullet[b].x_speed < 500 && ebullet[b].x_speed > -500
				        && ebullet[b].y_speed < 500 && ebullet[b].y_speed > -500)
				{
					ebullet[b].type = EBULLET_SEED2;
					ebullet[b].x_speed = 0;
					ebullet[b].y_speed = 0;
					ebullet[b].timeout = 160;
					ebullet[b].angle2 = pos_or_neg (10 + grand (12));
					quick_cloud (CLOUD_CIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 15,
					             TCOL_B5, 0);
				}
			}
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			break;
		case EBULLET_LSEED:
			if (ebullet[b].cord_wait == -1)
			{
				drag_ebullet (b, 910);
				if (ebullet[b].x_speed < 500 && ebullet[b].x_speed > -500
				        && ebullet[b].y_speed < 500 && ebullet[b].y_speed > -500)
				{
					ebullet[b].type = EBULLET_LSEED2;
					ebullet[b].x_speed = 0;
					ebullet[b].y_speed = 0;
					ebullet[b].timeout = 160;
					ebullet[b].angle2 = pos_or_neg (10 + grand (12));
					quick_cloud (CLOUD_CIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 15,
					             TCOL_B5, 0);
				}
			}
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			break;
		case EBULLET_SEED2:
		case EBULLET_LSEED2:
			ebullet[b].angle += ebullet[b].angle2;
			break;
		case EBULLET_STREAM:
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			if (ebullet[b].time % 20 == 0)
			{
				angle = ebullet[b].angle + ANGLE_4;
				angle &= 1023;
				k =
				    fire_dart (ebullet[b].x + xpart (angle, 10000),
				               ebullet[b].y + ypart (angle, 10000), angle, EBCOL_B1, 60,
				               80);
				if (k != -1)
				{
//               ebullet[k].new_ebullet = 0;
					ebullet[k].cord_x = ebullet[b].x;
					ebullet[k].cord_y = ebullet[b].y;
				}
				angle = ebullet[b].angle - ANGLE_4;
				angle &= 1023;
				k =
				    fire_dart (ebullet[b].x + xpart (angle, 10000),
				               ebullet[b].y + ypart (angle, 10000), angle, EBCOL_B1, 60,
				               80);
				if (k != -1)
				{
//               ebullet[k].new_ebullet = 0;
					ebullet[k].cord_x = ebullet[b].x;
					ebullet[k].cord_y = ebullet[b].y;
				}
			}

			break;
		case EBULLET_BURST:
			if (ebullet[b].time < 120)
			{
				ebullet[b].x_speed += xpart (ebullet[b].angle, ebullet[b].accel);
				ebullet[b].y_speed += ypart (ebullet[b].angle, ebullet[b].accel);
			}
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			ebullet[b].angle2 += ebullet[b].status;
			if (ebullet[b].timeout == 1)
			{
				for (i = 0; i < 3; i++)
				{
					angle = ebullet[b].angle2 + i * ANGLE_3;
					angle &= 1023;
//              k = fire_ddart(ebullet[b].x + xpart(angle, 22000), ebullet[b].y + ypart(angle, 22000), angle, EBCOL_B2, 60, 60);
					k =
					    fire_diamond (ebullet[b].x + xpart (angle, 25000),
					                  ebullet[b].y + ypart (angle, 25000), angle,
					                  EBCOL_B2, 2000, ebullet[b].status / 2, 30);
					if (k != -1)
					{
						ebullet[k].new_ebullet = 0;
						ebullet[k].cord_x = ebullet[b].x;
						ebullet[k].cord_y = ebullet[b].y;
					}
					angle += ANGLE_6;
					angle &= 1023;
					k =
					    fire_dart (ebullet[b].x + xpart (angle, 20000),
					               ebullet[b].y + ypart (angle, 20000), angle, EBCOL_B1,
					               60, 30);
					if (k != -1)
					{
						ebullet[k].new_ebullet = 0;
						ebullet[k].cord_x = ebullet[b].x;
						ebullet[k].cord_y = ebullet[b].y;
					}
				}
				quick_cloud (CLOUD_CIRCLE3, ebullet[b].x, ebullet[b].y, 0, 0, 15,
				             TCOL_B2, 0);
				destroy_ebullet (b);
				no_more_bullet = 1;
			}
			/*            if (ebullet[b].timeout == 1)
			            {
			             for (i = 0; i < 4; i ++)
			             {
			              angle = ebullet[b].angle2 + i * ANGLE_4;
			              angle &= 1023;
			//              k = fire_ddart(ebullet[b].x + xpart(angle, 22000), ebullet[b].y + ypart(angle, 22000), angle, EBCOL_B2, 60, 60);
			              k = fire_diamond(ebullet[b].x + xpart(angle, 25000), ebullet[b].y + ypart(angle, 25000), angle, EBCOL_B2, 2000, ebullet[b].status / 2, 60);
			              if (k != -1)
			              {
			               ebullet[k].new_ebullet = 0;
			               ebullet[k].cord_x = ebullet[b].x;
			               ebullet[k].cord_y = ebullet[b].y;
			              }
			              angle += ANGLE_8;
			              angle &= 1023;
			              k = fire_dart(ebullet[b].x + xpart(angle, 20000), ebullet[b].y + ypart(angle, 20000), angle, EBCOL_B1, 60, 50);
			              if (k != -1)
			              {
			               ebullet[k].new_ebullet = 0;
			               ebullet[k].cord_x = ebullet[b].x;
			               ebullet[k].cord_y = ebullet[b].y;
			              }
			             }
			             quick_cloud(CLOUD_CIRCLE3, ebullet[b].x, ebullet[b].y, 0, 0, 15, TCOL_B2, 0);
			             destroy_ebullet(b);
			             no_more_bullet = 1;
			            }*/

			break;
		case EBULLET_WING_DIAMOND:
			if (ebullet[b].time < 80)
			{
				ebullet[b].x_speed += xpart (ebullet[b].angle, ebullet[b].accel);
				ebullet[b].y_speed += ypart (ebullet[b].angle, ebullet[b].accel);
			}
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
//            if (ebullet[b].angle2 < ANGLE_8)
			ebullet[b].angle2 += ebullet[b].status;
			break;
		case EBULLET_SDART:
		case EBULLET_SHOT:
			if (ebullet[b].time < 60)
			{
				ebullet[b].x_speed += xpart (ebullet[b].angle, ebullet[b].accel);
				ebullet[b].y_speed += ypart (ebullet[b].angle, ebullet[b].accel);
			}
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			break;
		case EBULLET_SHOT2:
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			ebullet[b].angle += ebullet[b].angle2;
			ebullet[b].angle &= 1023;
			run_standard_cord (b);
			break;
		case EBULLET_CURVE:
			ebullet[b].x += xpart (ebullet[b].status, ebullet[b].x_speed);	//ebullet[b].x_speed;
			ebullet[b].y += ypart (ebullet[b].status, ebullet[b].x_speed);	//ebullet[b].x_speed;
			ebullet[b].angle += ebullet[b].angle2;
			ebullet[b].angle &= 1023;
			if (ebullet[b].cord_wait == -1 && ebullet[b].timeout > 300)
				ebullet[b].status += ebullet[b].y_speed;
			run_standard_cord (b);
			break;
		case EBULLET_DDART:
			if (ebullet[b].time < 60)
			{
				ebullet[b].x_speed += xpart (ebullet[b].angle, ebullet[b].accel);
				ebullet[b].y_speed += ypart (ebullet[b].angle, ebullet[b].accel);
			}
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			break;
		case EBULLET_DOT:
			ebullet[b].x += ebullet[b].x_speed;
			ebullet[b].y += ebullet[b].y_speed;
			run_standard_cord (b);
			break;
		}

//    if (check_ebullet_collision(b) == 1)
//     continue; // ebullet no longer exists

		if (no_more_bullet == 1)
			continue;

		ebullet[b].timeout--;

		if (ebullet[b].cord_wait == -1 &&
		        (ebullet[b].x <= -10000 || ebullet[b].y <= -10000
		         || ebullet[b].x >= 650000 || ebullet[b].y >= 490000))
			destroy_ebullet (b);
		else
		{
			if (ebullet[b].timeout <= 0)
				destroy_ebullet (b);
		}

	}

}

void run_standard_cord (int b)
{
	if (ebullet[b].cord_wait == -1)
		return;

	if (ebullet[b].cord_wait > 0)
	{
		ebullet[b].cord_wait--;
		return;
	}

	ebullet[b].cord_x_speed += xpart (ebullet[b].cord_angle, 200);
	ebullet[b].cord_y_speed += ypart (ebullet[b].cord_angle, 200);
	ebullet[b].cord_x += ebullet[b].cord_x_speed;
	ebullet[b].cord_y += ebullet[b].cord_y_speed;

// was 5000

	if (ebullet[b].cord_x >= ebullet[b].x - 9000
	        && ebullet[b].cord_x <= ebullet[b].x + 9000
	        && ebullet[b].cord_y >= ebullet[b].y - 9000
	        && ebullet[b].cord_y <= ebullet[b].y + 9000)
		ebullet[b].cord_wait = -1;

}

/*
int check_ebullet_collision(int b)
{
 if (ebullet[b].x >= player.x - 9000
  && ebullet[b].x <= player.x + 9000
  && ebullet[b].y >= player.y - 9000
  && ebullet[b].y <= player.y + 9000)
  {
//   play_effectwfvx(WAV_BLOP, 500 + grand(1000), 200, ebullet[b].x);
   destroy_ebullet(b);
   return 1;
  }

 return 0; // no collision
}
*/

void drag_ebullet (int b, int drag)
{
	ebullet[b].x_speed *= drag;
	ebullet[b].x_speed /= 1000;
	ebullet[b].y_speed *= drag;
	ebullet[b].y_speed /= 1000;

}



void destroy_ebullet (int b)
{
	ebullet[b].type = EBULLET_NONE;
}
