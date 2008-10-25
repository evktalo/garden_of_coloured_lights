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


int create_enemy (int type);

void init_enemies (void);

void run_enemies (void);

void pbullet_hits_enemy (int b, int e);

int hurt_enemy (int e, int dam, int source);;
void set_wpixels (void);

int fire_dart (int x, int y, int angle, int colour, int wait, int accel);
int fire_ddart (int x, int y, int angle, int colour, int wait, int accel);
int fire_diamond (int x, int y, int angle, int colour, int speed, int rot,
		  int wait);
