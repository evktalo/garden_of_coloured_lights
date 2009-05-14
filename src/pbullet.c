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

void run_pbullets (void);
void destroy_pbullet (int b);
int check_pbullet_collision (int b, int survive);
void pbullet_explode (int b, int hit);
void run_seeker (int b);
void find_new_seeker_target (int b);
void explosion (int x, int y, int r, int d, int b);

void init_pbullets (void)
{

	int b;

	for (b = 0; b < NO_PBULLETS; b++)
	{
		pbullet[b].type = PBULLET_NONE;
	}

}

int create_pbullet (int type)
{
	int b;

	for (b = 0; b < NO_PBULLETS; b++)
	{
		if (pbullet[b].type == PBULLET_NONE)
			break;
		if (b == NO_PBULLETS - 1)
			return -1;

	}

	pbullet[b].type = type;
	pbullet[b].persist = 0;
	pbullet[b].time = 0;
	pbullet[b].timeout = -1;
	return b;

}

void run_pbullets (void)
{

	int b, c, c2;

	for (b = 0; b < NO_PBULLETS; b++)
	{
		if (pbullet[b].type == PBULLET_NONE)
			continue;
		pbullet[b].time++;
		switch (pbullet[b].type)
		{
		case PBULLET_BLADE:
			check_pbullet_collision (b, 1);
			destroy_pbullet (b);
			continue;
//         break;
		case PBULLET_SEEKER:
			run_seeker (b);
			check_pbullet_collision (b, 0);
			break;
		case PBULLET_CANNON:
			pbullet[b].y += pbullet[b].y_speed;
			c = create_cloud (CLOUD_CIRCLE5);
			if (c != -1)
			{
				cloud[c].x = pbullet[b].x;
				cloud[c].y = pbullet[b].y;
//             cloud[c].x_speed = xpart((arena.counter * 64) % ANGLE_1, 700 + pbullet[b].level * 70);
				cloud[c].x_speed =
				    xpart (arena.counter * 64, 700 + pbullet[b].level * 70);
				cloud[c].y_speed = -900 - grand (500);
				cloud[c].colour = TCOL_ORANGE;
				cloud[c].timeout = 30 + grand (20) + pbullet[b].level;
			}
			check_pbullet_collision (b, 0);
			break;

		case PBULLET_RECT:		// eye of rectification
			pbullet[b].y += pbullet[b].y_speed;
			c = create_cloud (CLOUD_CIRCLE3);
			if (c != -1)
			{
				cloud[c].x = pbullet[b].x + grand (9000) - grand (9000);
				cloud[c].y = pbullet[b].y + grand (9000) - grand (9000);
				cloud[c].x_speed = grand (400) - grand (400);
				cloud[c].y_speed = grand (400) - grand (400);
				cloud[c].colour = TCOL_ORANGE;
				cloud[c].timeout = 12 + grand (40) + pbullet[b].level * 2;

				c2 = create_cloud (CLOUD_CIRCLE2);
				if (c2 != -1)
				{
					cloud[c2].x = cloud[c].x + grand (3000) - grand (3000);
					cloud[c2].y = cloud[c].y + grand (3000) - grand (3000);
					cloud[c2].x_speed = cloud[c].x_speed + grand (100) - grand (100);
					cloud[c2].y_speed = cloud[c].y_speed + grand (100) - grand (100);
					cloud[c2].colour = TCOL_YELLOW;
					cloud[c2].timeout = cloud[c].timeout / 3;
				}
			}

			check_pbullet_collision (b, 0);
			break;
		default:
		case PBULLET_SCATTER:
			pbullet[b].x += pbullet[b].x_speed;
			pbullet[b].y += pbullet[b].y_speed;
			check_pbullet_collision (b, 0);
			break;
		case PBULLET_RING:
			pbullet[b].x += pbullet[b].x_speed;
			pbullet[b].y += pbullet[b].y_speed;
			check_pbullet_collision (b, 0);
			pbullet[b].status += 2 + pbullet[b].level / 3;
			if (pbullet[b].status > 140 + pbullet[b].level * 5)
				pbullet[b].status = 140 + pbullet[b].level * 5;
			pbullet[b].xsize = pbullet[b].status * 100;
			pbullet[b].ysize = pbullet[b].status * 100;
			break;
		case PBULLET_MGUN:
//            pbullet[b].x += pbullet[b].x_speed;
			pbullet[b].y += pbullet[b].y_speed;
			check_pbullet_collision (b, 0);
			break;
		}

		if (pbullet[b].timeout > -1)
		{
			pbullet[b].timeout--;
			if (pbullet[b].timeout <= 0)
			{
				pbullet_explode (b, 0);
				continue;
			}
		}
		if (pbullet[b].persist == 0)
		{
			if (pbullet[b].x <= X_MIN - 20000
			        || pbullet[b].x >= X_MAX + 20000
			        || pbullet[b].y <= Y_MIN - 20000 || pbullet[b].y >= Y_MAX + 20000)
			{
				destroy_pbullet (b);
				continue;
			}
		}
	}

}

void run_seeker (int b)
{

//    int old_x = pbullet[b].x;
//    int old_y = pbullet[b].y;
	int target_x = -1, target_y = -1;

	if (pbullet[b].target == -1 && pbullet[b].timeout % 5 == 0)
		find_new_seeker_target (b);

	if (pbullet[b].target != -1)
	{
		target_x = enemy[pbullet[b].target].x;
		target_y = enemy[pbullet[b].target].y;
	}
	else
	{
		target_x = player.x;
		target_y = player.y;
	}

//    pbullet[b].acceleration += 80;
	/*    pbullet[b].acceleration += 300;
	    if (pbullet[b].acceleration > 1900)
	     pbullet[b].acceleration = 1900;*/

	pbullet[b].x_speed += xpart (pbullet[b].angle, 600);	// 700
	pbullet[b].y_speed += ypart (pbullet[b].angle, 600);	// 700
	pbullet[b].x += pbullet[b].x_speed;
	pbullet[b].y += pbullet[b].y_speed;

	int c = create_cloud (CLOUD_SLINE);
	if (c != -1)
	{
		cloud[c].x2 = pbullet[b].x;
		cloud[c].y2 = pbullet[b].y;
		cloud[c].x = pbullet[b].x - pbullet[b].x_speed;
		cloud[c].y = pbullet[b].y - pbullet[b].y_speed;
		cloud[c].timeout = 5 + pbullet[b].level / 3;
		cloud[c].colour = TRANS_ORANGE_OUT;
	}

	/*    pbullet[b].drag -= 50;
	    if (pbullet[b].drag < 900)
	     pbullet[b].drag = 900;*/

	pbullet[b].x_speed *= 96;
	pbullet[b].x_speed /= 100;
	pbullet[b].y_speed *= 96;
	pbullet[b].y_speed /= 100;

	/*    int c = create_cloud(w, CLOUD_SEEKER_TRAIL, 0, pbullet[b].x, pbullet[b].y, 0, 0, 11);
	    if (c != -1)
	    {
	     cloud[w][c].x2 = old_x;
	     cloud[w][c].y2 = old_y;
	     cloud[w][c].angle = pbullet[b].angle;
	    }*/

//    pbullet[b].turn += 185;
//    if (pbullet[b].turn > 1300)
//     pbullet[b].turn = 1300;
	/* if (pbullet[b].timeout < 130)
	 {
	    if (target_x != -1 || target_y != -1)
	     pbullet[b].angle = radians_to_angle(atan2((target_x - pbullet[b].y), (target_y - pbullet[b].x))) % ANGLE_1;

	 }
	  else*/
	{
		if (target_x != -1 || target_y != -1)
			pbullet[b].angle = turn_towards_xy (pbullet[b].x, pbullet[b].y, target_x, target_y, pbullet[b].angle, 28);	// 22
	}
//     seeker[s].angle = turn_towards_xy(seeker[s].x, seeker[s].y, target_x, target_y, seeker[s].angle, 32);

	pbullet[b].timeout--;

//    if (seeker_collision(w, s))
	return;

	if (pbullet[b].timeout <= 0)
	{
//     create_cloud(w, CLOUD_DRAG_EXPLODE, 0, pbullet[b].x, pbullet[b].y, 0, 0, 30 + grand(5)); // also in collision
		destroy_pbullet (b);
	}

}

void find_new_seeker_target (int b)
{

	int enemies_found = 0;

	int e;
	int x = pbullet[b].x;
	int y = pbullet[b].y;

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
		{
			enemy[e].distance = 900000;
			continue;
		}

		enemy[e].distance = hypot (enemy[e].y - y, enemy[e].x - x) / 100;
		enemies_found++;

	}

	if (enemies_found == 0)
		return;

	int closest = -1;
	int smallest_distance = 900000;

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].distance < smallest_distance)
		{
			/*    for (j = 0; j < NO_SEEKERS; j ++)
			    {
			     if (seeker[w][j].target == e) // should allow multiple seekers to lock on to big targets
			      continue;
			    }*/
			closest = e;
			smallest_distance = enemy[e].distance;
		}
	}

	if (smallest_distance == 900000 || closest == -1)
	{
		return;
	}

	pbullet[b].target = closest;

}

void beam_collision (int m)
{

	int e;

	int x, y, dam;
	int max_y;

	/* switch(player.power [0])
	 {
	  case 1: dam = 12; which_explode = 0; break;
	  case 2: dam = 15; which_explode = 0; break;
	  case 3: dam = 18; which_explode = 1; break;
	  case 4: dam = 21; which_explode = 2; break;
	  case 5: dam = 24; which_explode = 2; break;
	/ *  case 1: dam = 5; which_explode = 0; break;
	  case 2: dam = 7; which_explode = 0; break;
	  case 3: dam = 9; which_explode = 1; break;
	  case 4: dam = 11; which_explode = 2; break;
	  case 5: dam = 13; which_explode = 2; break;* /
	 }*/
	if (arena.counter & 1)
		dam = 7 + (player.wlevel[m] * 2);	// for show damage see input.c
	else
		dam = 7 + (player.wlevel[m]);	// for show damage see input.c

// so it's effectively player.wlevel [m] * 1.5

	x = player.wx[m];

	y = player.wy[m] - 3000;

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
			continue;
		if (x >=
		        enemy[e].x - eclass[enemy[e].type].xsize +
		        eclass[enemy[e].type].xoffset
		        && x <=
		        enemy[e].x + eclass[enemy[e].type].xsize +
		        eclass[enemy[e].type].xoffset
		        && y >=
		        enemy[e].y - eclass[enemy[e].type].ysize +
		        eclass[enemy[e].type].yoffset)
		{
			max_y =
			    enemy[e].y + eclass[enemy[e].type].ysize +
			    eclass[enemy[e].type].yoffset;
			if (max_y > y)
				max_y = y;
			max_y -= grand (16000);
			quick_cloud (CLOUD_CIRCLE1, x, max_y, grand (2000) - grand (2000),
			             grand (2000) - grand (2000) - 500 - player.wlevel[m] * 250,
			             3 + grand (3) + grand (player.wlevel[m]), TCOL_YELLOW, 0);
			max_y -= grand (8000);
			quick_cloud (CLOUD_CIRCLE1, x, max_y, grand (2000) - grand (2000),
			             grand (2000) - grand (2000) - 500 - player.wlevel[m] * 250,
			             7 + grand (5) + player.wlevel[m], TCOL_ORANGE, 0);

			/*         if (grand(2) == 0)
			         {
			         max_y = y;
			         if (enemy[w][e].y + eclass[enemy[w][e].type].size - 10000 < max_y)
			          max_y = enemy[w][e].y + eclass[enemy[w][e].type].size - 10000;

			          for (j = enemy[w][e].y - eclass[enemy[w][e].type].size + 10000 + grand(10000); j < max_y; j += 5000 + grand(15000))
			          {
			           create_cloud(w, CLOUD_EXPLODE, which_explode, x, j, 0, 0, 3 + grand(3));

			          }
			         }*/
			hurt_enemy (e, dam, m);
			// remember, enemy might be destroyed here!!
			// so we can't assume its values are still good (actually we prob can, but let's not)
		}
	}

}

void swbeam_collision (int m)
{

	int e;

	int x, y, dam;
	int max_y, angle, speed;
	int i;

	dam = 1400 + player.swbeam_level[m] * 240;

	x = player.wx[m];

	y = player.wy[m] - 3000;

	max_y = y - 90000;

	if (player.swbeam_level[m] > 2)
	{
		while (max_y > -90000)
		{
			quick_cloud (CLOUD_CIRCLE5, x, max_y, grand (500) - grand (500),
			             3000 + grand (1000),
			             20 + grand (8) + player.swbeam_level[m] * 2, TCOL_ORANGE,
			             970);
			max_y -= 20000 + grand (15000) - player.swbeam_level[m] * 1000;

		};
	}

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
			continue;
		if (x >=
		        enemy[e].x - eclass[enemy[e].type].xsize +
		        eclass[enemy[e].type].xoffset - 5000 - player.swbeam_level[m] * 1500
		        && x <=
		        enemy[e].x + eclass[enemy[e].type].xsize +
		        eclass[enemy[e].type].xoffset + 5000 + player.swbeam_level[m] * 1500
		        && y >=
		        enemy[e].y - eclass[enemy[e].type].ysize +
		        eclass[enemy[e].type].yoffset)
		{
			max_y =
			    enemy[e].y + eclass[enemy[e].type].ysize +
			    eclass[enemy[e].type].yoffset;
			if (max_y > y)
				max_y = y;
			max_y -= grand (16000);
			angle = grand (ANGLE_8);
			for (i = 0; i < 8 + player.swbeam_level[m]; i++)
			{
				angle = (i * ANGLE_8) + grand (ANGLE_8);
				speed = grand (3000) + 1000 + player.swbeam_level[m] * 200;
				quick_cloud (CLOUD_CIRCLE5, x, max_y, xpart (angle, speed),
				             ypart (angle, speed) - grand (2000),
				             45 + grand (28) + player.swbeam_level[m] * 2,
				             TCOL_ORANGE, 940);
			}
			quick_cloud (CLOUD_CIRCLE2, x, max_y, 0, 0 - grand (1000),
			             25 + grand (8) + player.swbeam_level[m], TCOL_YELLOW, 950);
			max_y -= grand (8000);
			quick_cloud (CLOUD_CIRCLE2, x, max_y, 0, 0 - grand (1000),
			             45 + grand (15) + player.swbeam_level[m] * 2, TCOL_ORANGE,
			             950);
			if (hurt_enemy (e, dam, m) == 1)
				quick_cloud (CLOUD_MSHOCK, x, max_y, 0, 0, 20, 1, 0);
			// remember, enemy might be destroyed here!!
			// so we can't assume its values are still good (actually we prob can, but let's not)
		}
	}

}

int check_pbullet_collision (int b, int survive)
{

	{
		int e;
// int destroy_type = 0;

		for (e = 0; e < NO_ENEMIES; e++)
		{
			if (enemy[e].type == ENEMY_NONE)
				continue;
			if (pbullet[b].x >=
			        enemy[e].x - eclass[enemy[e].type].xsize +
			        eclass[enemy[e].type].xoffset - pbullet[b].xsize
			        && pbullet[b].x <=
			        enemy[e].x + eclass[enemy[e].type].xsize +
			        eclass[enemy[e].type].xoffset + pbullet[b].xsize
			        && pbullet[b].y >=
			        enemy[e].y - eclass[enemy[e].type].ysize +
			        eclass[enemy[e].type].yoffset - pbullet[b].ysize
			        && pbullet[b].y <=
			        enemy[e].y + eclass[enemy[e].type].ysize +
			        eclass[enemy[e].type].yoffset + pbullet[b].ysize)
			{
				{
					pbullet_hits_enemy (b, e);
//        if (survive == 2)
//         return 1; // green2_circle
					if (survive == 0)
					{
						pbullet_explode (b, 1);
						destroy_pbullet (b);
						return 1;		// bullet destroyed
					}
					else
					{
						pbullet_explode (b, 1);
//             return 0; // bullet not destroyed
					}
				}
			}

		}
		return 0;			// bullet not destroyed
	}

}

void explosion (int x, int y, int r, int d, int b)
{
	int e;
// int destroy_type = 0;

	for (e = 0; e < NO_ENEMIES; e++)
	{
		if (enemy[e].type == ENEMY_NONE)
			continue;
		if (x >=
		        enemy[e].x - eclass[enemy[e].type].xsize +
		        eclass[enemy[e].type].xoffset - r
		        && x <=
		        enemy[e].x + eclass[enemy[e].type].xsize +
		        eclass[enemy[e].type].xoffset + r
		        && y >=
		        enemy[e].y - eclass[enemy[e].type].ysize +
		        eclass[enemy[e].type].yoffset - r
		        && y <=
		        enemy[e].y + eclass[enemy[e].type].ysize +
		        eclass[enemy[e].type].yoffset + r)
		{
			hurt_enemy (e, d, pbullet[b].source);
		}
	}

}

void pbullet_explode (int b, int hit)
{
	int c, xa, ya, xb, yb, xc, i, angle, pieces, pieces2;

	switch (pbullet[b].type)
	{
	case PBULLET_BLADE:
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x + grand (5000) - grand (5000),
		             pbullet[b].y + grand (5000) - grand (5000), 0, 0, 12,
		             TCOL_YELLOW, 0);
		quick_cloud (CLOUD_CIRCLE1, pbullet[b].x + grand (5000) - grand (5000),
		             pbullet[b].y + grand (5000) - grand (5000), 0, 0, 12,
		             TCOL_ORANGE, 0);
		angle = grand (ANGLE_1);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x + xpart (angle, 9000),
		             pbullet[b].y + ypart (angle, 9000), xpart (angle, 3000),
		             ypart (angle, 3000), 20, TCOL_ORANGE, 970);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x - xpart (angle, 9000),
		             pbullet[b].y - ypart (angle, 9000), xpart (angle, -3000),
		             ypart (angle, -3000), 20, TCOL_ORANGE, 970);
		angle = grand (ANGLE_1);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x + xpart (angle, 9000),
		             pbullet[b].y + ypart (angle, 9000), xpart (angle, 3000),
		             ypart (angle, 3000), 20, TCOL_ORANGE, 970);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x - xpart (angle, 9000),
		             pbullet[b].y - ypart (angle, 9000), xpart (angle, -3000),
		             ypart (angle, -3000), 20, TCOL_ORANGE, 970);
		break;
	case PBULLET_SCATTER:
		xa = pbullet[b].level;
		ya = 9000 + pbullet[b].level * 500;
		quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, 0, 0, 30 + xa,
		             TCOL_YELLOW, 0);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, 0, 0, 30 + xa,
		             TCOL_ORANGE, 0);
		angle = grand (ANGLE_1);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x + xpart (angle, ya),
		             pbullet[b].y + ypart (angle, ya), 0, 0, 20 + xa, TCOL_ORANGE,
		             0);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x - xpart (angle, ya),
		             pbullet[b].y - ypart (angle, ya), 0, 0, 20 + xa, TCOL_ORANGE,
		             0);
		break;
	case PBULLET_RECT:
		ya = pbullet[b].y - grand (14000);
		quick_cloud (CLOUD_RSHOCK1 + pbullet[b].level / 2, pbullet[b].x, ya, 0, 0,
		             20, 0, 0);
		quick_cloud (CLOUD_CIRCLE1, pbullet[b].x, ya, 0, 0, 20 + pbullet[b].level,
		             TCOL_YELLOW, 0);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x, ya, 0, 0,
		             50 + pbullet[b].level * 2, TCOL_ORANGE, 0);

		angle = grand (ANGLE_1);
		pieces = 3 + pbullet[b].level;
		pieces2 = ANGLE_1 / pieces;
		xc = 40 + grand (5);
		for (i = 0; i < pieces; i++)
		{
			xb = pbullet[b].x + xpart (angle, 25000);
			yb = ya + ypart (angle, 25000);
			quick_cloud (CLOUD_CIRCLE1, xb, yb,
			             xpart (angle, 1000 + pbullet[b].level * 40), ypart (angle,
			                     1000 +
			                     pbullet
			                     [b].
			                     level *
			                     40),
			             20 + pbullet[b].level, TCOL_YELLOW, 0);
			quick_cloud (CLOUD_CIRCLE2, xb, yb,
			             xpart (angle, 1000 + pbullet[b].level * 40), ypart (angle,
			                     1000 +
			                     pbullet
			                     [b].
			                     level *
			                     40),
			             50 + pbullet[b].level * 2, TCOL_ORANGE, 0);
			angle += pieces2;
		}
		explosion (pbullet[b].x, pbullet[b].y, 60000 + pbullet[b].level * 5000,
		           1400 + pbullet[b].level * 255, b);
		play_effectwfvx (WAV_RECT, 250, 120, pbullet[b].x);
		break;
	case PBULLET_RING:
		ya = pbullet[b].y - grand (14000);
		quick_cloud (CLOUD_CIRCLE1, pbullet[b].x, ya, 0, 0, 6, TCOL_YELLOW, 0);
		quick_cloud (CLOUD_CIRCLE2, pbullet[b].x, ya, 0, 0, 15, TCOL_ORANGE, 0);
		quick_cloud (CLOUD_PRING, pbullet[b].x, ya, 0, 0, pbullet[b].status, 0,
		             0);
		break;
	case PBULLET_MGUN:
		c = create_cloud (CLOUD_MGSHOCK);
		if (c != -1)
		{
			cloud[c].x = pbullet[b].x;
			cloud[c].y = pbullet[b].y - grand (14000);
			cloud[c].x_speed = grand (1000) - grand (1000);
			cloud[c].y_speed = grand (1000) - grand (1000);
			cloud[c].timeout = 11;
			cloud[c].x2 = grand (ANGLE_1);
		}
		break;
	case PBULLET_SEEKER:
//   quick_cloud(CLOUD_MGSHOCK, pbullet[b].x, pbullet[b].y, 0, 0, 11, 0, 0);
		quick_cloud (CLOUD_CIRCLE5, pbullet[b].x, pbullet[b].y, 0, 0, 30,
		             TCOL_YELLOW, 0);
		quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, 0, 0, 30,
		             TCOL_ORANGE, 0);
		if (hit == 1)
		{
			xa = xpart (pbullet[b].angle - ANGLE_16 + ANGLE_2, 500);
			ya = ypart (pbullet[b].angle - ANGLE_16 + ANGLE_2, 500);
			quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa, ya, 30,
			             TCOL_ORANGE, 980);
			quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 2, ya * 2,
			             35, TCOL_ORANGE, 980);
			quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 3, ya * 3,
			             40, TCOL_ORANGE, 980);
			xa = xpart (pbullet[b].angle + ANGLE_16 + ANGLE_2, 500);
			ya = ypart (pbullet[b].angle + ANGLE_16 + ANGLE_2, 500);
			quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa, ya, 30,
			             TCOL_ORANGE, 980);
			quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 2, ya * 2,
			             35, TCOL_ORANGE, 980);
			quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 3, ya * 3,
			             40, TCOL_ORANGE, 980);
		}
		else
		{
			angle = pbullet[b].angle;
			for (i = 0; i < 3; i++)
			{
				quick_cloud (CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y,
				             xpart (angle, 800), ypart (angle, 800), 40, TCOL_ORANGE,
				             980);
				angle += ANGLE_3;
			}
			angle += ANGLE_6;
			for (i = 0; i < 3; i++)
			{
				quick_cloud (CLOUD_CIRCLE5, pbullet[b].x, pbullet[b].y,
				             xpart (angle, 900), ypart (angle, 900), 40, TCOL_ORANGE,
				             990);
				angle += ANGLE_3;
			}
		}
		break;
	case PBULLET_CANNON:
		xa = pbullet[b].x;
		play_effectwfvx (WAV_CANNON, 2200, 180, xa);
		ya = pbullet[b].y - grand (14000);
		c = create_cloud (CLOUD_CIRCLE1);
		if (c != -1)
		{
			cloud[c].x = xa;
			cloud[c].y = ya;
			cloud[c].timeout = 25 + pbullet[b].level;
			cloud[c].colour = TCOL_YELLOW;
		}
		c = create_cloud (CLOUD_CIRCLE2);
		if (c != -1)
		{
			cloud[c].x = xa;
			cloud[c].y = ya;
			cloud[c].timeout = 60 + pbullet[b].level * 2;
			cloud[c].colour = TCOL_ORANGE;
		}
		angle = grand (ANGLE_1);
		pieces = 4 + pbullet[b].level / 2;
		pieces2 = ANGLE_1 / pieces;
		xc = 40 + grand (5);
		for (i = 0; i < pieces; i++)
		{
			xb = pbullet[b].x + xpart (angle, 20000);
			yb = ya + ypart (angle, 20000);
			c = create_cloud (CLOUD_CIRCLE3);
			if (c != -1)
			{
				cloud[c].x = xb;
				cloud[c].y = yb;
				cloud[c].x_speed = xpart (angle, 2000);
				cloud[c].y_speed = ypart (angle, 2000);
				cloud[c].timeout = xc;
				cloud[c].colour = TCOL_ORANGE;
				cloud[c].drag = 950;
			}
			angle += pieces2;

		}

		angle = grand (ANGLE_1);
		xc = 40 + grand (5);
		for (i = 0; i < pieces; i++)
		{
			xb = pbullet[b].x + xpart (angle, 20000);
			yb = ya + ypart (angle, 20000);
			c = create_cloud (CLOUD_CIRCLE3);
			if (c != -1)
			{
				cloud[c].x = xb;
				cloud[c].y = yb;
				cloud[c].x_speed = xpart (angle, 1200);
				cloud[c].y_speed = ypart (angle, 1200);
				cloud[c].timeout = xc;
				cloud[c].colour = TCOL_ORANGE;
				cloud[c].drag = 950;
			}
			angle += pieces2;

		}

		angle = grand (ANGLE_1);
		xc = 40 + grand (5);
		for (i = 0; i < pieces; i++)
		{
			xb = pbullet[b].x + xpart (angle, 20000);
			yb = ya + ypart (angle, 20000);
			c = create_cloud (CLOUD_CIRCLE3);
			if (c != -1)
			{
				cloud[c].x = xb;
				cloud[c].y = yb;
				cloud[c].x_speed = xpart (angle, 700);
				cloud[c].y_speed = ypart (angle, 700);
				cloud[c].timeout = xc;
				cloud[c].colour = TCOL_ORANGE;
				cloud[c].drag = 950;
			}
			angle += pieces2;

		}
		break;

	}
	destroy_pbullet (b);

}

void destroy_pbullet (int b)
{

	pbullet[b].type = PBULLET_NONE;

}
