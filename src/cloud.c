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

#include <math.h>

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"


#include "palette.h"


void run_clouds (void);
void destroy_cloud (int b);
int quick_cloud (int type, int x, int y, int x_speed, int y_speed,
		 int timeout, int colour, int drag);


void init_clouds (void)
{

  int c;

  for (c = 0; c < NO_CLOUDS; c++)
  {
    cloud[c].type = CLOUD_NONE;
  }

}

int create_cloud (int type)
{
  int c;

  for (c = 0; c < NO_CLOUDS; c++)
  {
    if (cloud[c].type == CLOUD_NONE)
      break;
    if (c == NO_CLOUDS - 1)
      return -1;

  }

  cloud[c].type = type;
  cloud[c].timeout = 0;
  cloud[c].x_speed = 0;
  cloud[c].y_speed = 0;
  cloud[c].drag = 0;
  return c;

}

void run_clouds (void)
{

  int c;

  for (c = 0; c < NO_CLOUDS; c++)
  {
    if (cloud[c].type == CLOUD_NONE)
      continue;
//    if (arena.counter % 8 == 0)
    cloud[c].timeout--;
    if (cloud[c].timeout <= 0)
    {
      destroy_cloud (c);
      continue;
    }
    if (cloud[c].drag != 0)
    {
      cloud[c].x_speed *= cloud[c].drag;
      cloud[c].x_speed /= 1000;
      cloud[c].y_speed *= cloud[c].drag;
      cloud[c].y_speed /= 1000;
    }
    switch (cloud[c].type)
    {
    case CLOUD_SCIRCLE:
      cloud[c].timeout -= 2;
      if (cloud[c].timeout <= 0)
	destroy_cloud (c);
      break;
    case CLOUD_DELAY1:
      cloud[c].angle--;
      if (cloud[c].angle == 0)
      {
	cloud[c].type = CLOUD_DELAY2;
	cloud[c].angle = cloud[c].y2;
	cloud[c].x2 = 1;
      }
      break;
    case CLOUD_DELAY2:
      cloud[c].angle--;
      if (cloud[c].angle & 3)
	cloud[c].x2++;
      if (cloud[c].angle == 0)
      {
	cloud[c].type = CLOUD_DELAY3;
//        cloud[c].angle = 15;
      }
      break;
    case CLOUD_DELAY3:
//       cloud[c].angle --;
      if (cloud[c].timeout & 3)
	cloud[c].x2--;
      if (cloud[c].x2 <= 0)
      {
	destroy_cloud (c);
      }
      break;
    case CLOUD_BURN:
      cloud[c].x += cloud[c].x_speed;
      cloud[c].y += cloud[c].y_speed;
      cloud[c].y_speed += 200;
      if (cloud[c].y > 490000 || cloud[c].x < -5000 || cloud[c].x > 645000)
      {
	destroy_cloud (c);
	break;
      }
      if (cloud[c].timeout & 1)
	quick_cloud (CLOUD_CIRCLE5, cloud[c].x + grand (8000) - grand (8000),
		     cloud[c].y + grand (8000) - grand (8000),
		     cloud[c].x_speed, cloud[c].y_speed,
		     cloud[c].timeout / 3 + 18 + grand (15), TCOL_SH2, 900);
      else
	quick_cloud (CLOUD_CIRCLE3, cloud[c].x + grand (8000) - grand (8000),
		     cloud[c].y + grand (8000) - grand (8000),
		     cloud[c].x_speed, cloud[c].y_speed,
		     cloud[c].timeout / 8 + 10 + grand (10), TCOL_ORANGE,
		     900);
      cloud[c].timeout -= cloud[c].colour;
      break;
    case CLOUD_SPAWNER:
      cloud[c].x += cloud[c].x_speed;
      cloud[c].y += cloud[c].y_speed;
//         if (grand(3) == 0)
      quick_cloud (CLOUD_CIRCLE2, cloud[c].x + grand (18000) - grand (18000),
		   cloud[c].y + grand (18000) - grand (18000), 0, 0,
		   20 + grand (20), cloud[c].colour, 950);
      break;
    case CLOUD_MSHOCK:
//         if (arena.counter % 8 != 0)
//              cloud[c].timeout ++;
      break;
    case CLOUD_LSHOCK:
    case CLOUD_HSHOCK:
      if (arena.level == 2)
	cloud[c].timeout--;
      break;
    case CLOUD_MGSHOCK:
//         cloud[c].x += cloud[c].x_speed;
//         cloud[c].y += cloud[c].y_speed;
      if (arena.counter % 2 == 0)
	cloud[c].timeout++;
      break;
    case CLOUD_PETAL1:
      cloud[c].x += cloud[c].x_speed;
      cloud[c].y += cloud[c].y_speed;
      cloud[c].timeout--;
      break;
    case CLOUD_PETAL1_DEBRIS:
    case CLOUD_PETAL2_DEBRIS:
    case CLOUD_PETAL3_DEBRIS:
      cloud[c].x += cloud[c].x_speed;
      cloud[c].y += cloud[c].y_speed;
      cloud[c].x_speed *= 1.01;
      cloud[c].y_speed *= 1.01;
      cloud[c].angle += cloud[c].x2;
      if (cloud[c].x < -20000
	  || cloud[c].x > 660000
	  || cloud[c].y < -20000 || cloud[c].y > 500000)
	destroy_cloud (c);
      break;
    case CLOUD_TSHOCK:
      if (arena.counter % 2 == 0)
	cloud[c].timeout++;
      break;
    case CLOUD_SLINE:
//         cloud[c].timeout = 1;
//         destroy_cloud(c);
      break;
    case CLOUD_PRING:
      cloud[c].timeout -= 10;
      break;
    case CLOUD_CARRIER1_BANG:
      cloud[c].x2 += 8;
      cloud[c].x2 &= 1023;
      break;
    default:
      cloud[c].x += cloud[c].x_speed;
      cloud[c].y += cloud[c].y_speed;
      break;
    }				// cloud may be destroyed here
  }

}



int quick_cloud (int type, int x, int y, int x_speed, int y_speed,
		 int timeout, int colour, int drag)
{

  int c;

  c = create_cloud (type);

  if (c != -1)
  {
    cloud[c].x = x;
    cloud[c].y = y;
    cloud[c].timeout = timeout;
    cloud[c].colour = colour;
    cloud[c].drag = drag;
    cloud[c].x_speed = x_speed;
    cloud[c].y_speed = y_speed;
  }

  return c;

}




void destroy_cloud (int c)
{
  cloud[c].type = CLOUD_NONE;

}
