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

// Global variables:
//  at the moment these are all defined in main.c.
//  but they have to be externed here so all modules with #include globvars.h can access them.

extern struct playerstruct player;

extern struct ebulletstruct ebullet[NO_EBULLETS];

extern struct pbulletstruct pbullet[NO_PBULLETS];

extern struct enemystruct enemy[NO_ENEMIES];

extern struct cloudstruct cloud[NO_CLOUDS];

extern unsigned char counter;

extern struct arenastruct arena;

extern struct eclassstruct eclass[NO_ENEMY_TYPES];

extern struct bossstruct boss;

extern struct optionstruct options;
