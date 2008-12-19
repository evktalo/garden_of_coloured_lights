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

#include "stuff.h"
#include "enemy.h"
#include "pbullet.h"
#include "ebullet.h"
#include "display.h"
#include "cloud.h"

#include "palette.h"

#include <math.h>

#include "sound.h"

#define NO_TRACKS 3
#define TRACK_SIZE 10


enum
{
	TRACK_WANDER,
	TRACK_DARTER,
	TRACK_STOPPER,
	TRACK_SIDES,
	TRACK_DIVER,
	TRACK_DART_STOPPER,
	TRACK_DRIFTER,
	TRACK_CRUISER,
	TRACK_BOSS,
	TRACK_SLIDER,
	TRACK_WALK,

	TRACK_GLIDER,
	TRACK_MARCHER,
	TRACK_ATTACKER,
	TRACK_BEAMER,
	TRACK_BOSS1,
	TRACK_BOSS2,

	TRACK_MB1
};

enum
{
	DIST_RANDOM,
	DIST_ALL_W,
	DIST_ALL_B,
	DIST_ALTERNATE_WB,
	DIST_ALTERNATE_BW,
	DIST_PART_WB,
	DIST_PART_BW
};


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

int next_track;

struct trackstruct track[NO_TRACKS];


/*
struct overtrackstruct
{
  int subtrack [NO_TRACKS];
  int position;
};

struct overtrackstruct overtrack [OVERTRACK_SIZE];
*/


enum
{
	T_EMPTY,
	T_OVERTRACK_END,
	T_CHANGE_MUSIC,
	T_END_LEVEL,
	T_ESCORTS_0,
	T_ESCORTS_1,
	T_ESCORTS_2,
	T_ESCORTS_3,
	T_SWOOPER1,
	T_DARTER1,
	T_STOPPER1,
	T_DIVER1,
	T_CARRIER1,
	T_POD1,
	T_BOSS1,

	T_SWOOPER2,
	T_DARTER2,
	T_SMALL,
	T_LARGER,
	T_SMALL2,
	T_LARGER2,
	T_WINGS,
	T_WINGS2,
	T_MEGA,
	T_BOSS3,

	T_DRIFTER1,
	T_BEAMER,
	T_RISER,
	T_CRUISER1,
	T_CRUISER2,
	T_CRUISER3,
	T_CRUISER4,
	T_CRUISER5,
	T_DRIFTER2,
	T_SHOOTER,
	T_BOSS2,

	T_HOOK,
	T_WALK,
	T_DROPPER,
	T_RING,
	T_HUNTER,
	T_SPINNER,
	T_HOOK2,
	T_BOSS4,

// remember to add to track_score!

	T_END
};


int track_score[T_END][2] = {
	{0, 0},			//T_EMPTY,
	{0, 0},			//T_OVERTRACK_END,
	{0, 0},			//T_CHANGE_MUSIC,
	{0, 0},			//T_END_LEVEL,
	{0, 0},			//T_ESCORTS_0,
	{0, 0},			//T_ESCORTS_1,
	{0, 0},			//T_ESCORTS_2,
	{0, 0},			//T_ESCORTS_3,
	{4, ENEMY_SWOOPER1},		//T_SWOOPER1,
	{4, ENEMY_DARTER1},		//T_DARTER1,
	{1, ENEMY_STOPPER1},		//T_STOPPER1,
	{3, ENEMY_DIVER1},		//T_DIVER1,
	{1, ENEMY_CARRIER1},		//T_CARRIER1,
	{2, ENEMY_POD1},		//T_POD1,
	{0, 0},			//T_BOSS1,
	{4, ENEMY_SWOOPER2},		//T_SWOOPER2,
	{3, ENEMY_DARTER2},		//T_DARTER2,
	{3, ENEMY_SMALL},		//T_SMALL,
	{2, ENEMY_LARGER},		//T_LARGER,
	{3, ENEMY_SMALL2},		//T_SMALL2,
	{2, ENEMY_LARGER2},		//T_LARGER2,
	{2, ENEMY_WINGS},		//T_WINGS,
	{2, ENEMY_WINGS2},		//T_WINGS2,
	{1, ENEMY_MEGA},		//T_MEGA,
	{0, 0},			//T_BOSS3,
	{3, ENEMY_DRIFTER1},		//T_DRIFTER1,
	{3, ENEMY_BEAMER},		//T_BEAMER,
	{6, ENEMY_RISER},		//T_RISER,
	{1, ENEMY_CRUISER1},		//T_CRUISER1,
	{1, ENEMY_CRUISER2},		//T_CRUISER2,
	{1, ENEMY_CRUISER3},		//T_CRUISER3,
	{1, ENEMY_CRUISER4},		//T_CRUISER4,
	{1, ENEMY_CRUISER5},		//T_CRUISER5,
	{1, ENEMY_DRIFTER2},		//T_DRIFTER2,
	{2, ENEMY_SHOOTER},		//T_SHOOTER,
	{4, ENEMY_HOOK},		//T_SHOOTER,
	{1, ENEMY_WALKER},		//T_SHOOTER,
	{4, ENEMY_DROPPER},		//T_SHOOTER,
	{1, ENEMY_RING},		//T_SHOOTER,
	{2, ENEMY_HUNTER},		//T_SHOOTER,
	{1, ENEMY_SPINNER},		//T_SHOOTER,
	{4, ENEMY_HOOK2},		//T_SHOOTER,
	{0, 0},			//T_BOSS4,

//T_END

};


int overtrack_pos;
int overtrack_pos2;

#define OT_NEXT 3
#define OT_WAIT 4

int overtrack[4][71][5] = {
	{
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 3, 2},	// doesn't appear
		/*{T_SWOOPER1, T_SWOOPER1, T_SWOOPER1, 160, 80},
		{T_SWOOPER1, T_SWOOPER1, T_SWOOPER1, 160, 80},
		{T_SWOOPER1, T_SWOOPER1, T_SWOOPER1, 160, 80},*/
//{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},

//{T_DIVER1, T_DIVER1, T_EMPTY, 160, 80},
		{T_CHANGE_MUSIC, OVERMUSIC_L1_2, 0, 0, 0},
		/*{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},

		*/
//{T_DARTER2, T_EMPTY, T_EMPTY, 160, 80},
//{T_DARTER2, T_EMPTY, T_EMPTY, 160, 80},

//{T_BOSS1, T_EMPTY, T_EMPTY, 150, 50},



		{T_SWOOPER1, T_EMPTY, T_EMPTY, 150, 10},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},

		{T_SWOOPER1, T_SWOOPER1, T_EMPTY, 160, 80},
//{T_END_LEVEL, T_EMPTY, T_EMPTY, 460, 80},

		{T_SWOOPER1, T_DARTER1, T_EMPTY, 260, 80},
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_SWOOPER1, T_DARTER1, T_EMPTY, 360, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_SWOOPER1, T_SWOOPER1, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_SWOOPER1, T_DARTER1, T_EMPTY, 160, 80},
		{T_SWOOPER1, T_SWOOPER1, T_EMPTY, 260, 80},
		{T_STOPPER1, T_DARTER1, T_EMPTY, 260, 80},
		{T_SWOOPER1, T_SWOOPER1, T_EMPTY, 260, 80},
		{T_DIVER1, T_SWOOPER1, T_EMPTY, 260, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_DIVER1, T_SWOOPER1, T_EMPTY, 160, 80},
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 160, 80},

		{T_POD1, T_EMPTY, T_EMPTY, 260, 80},
		{T_SWOOPER1, T_DARTER1, T_EMPTY, 260, 80},
		{T_DIVER1, T_STOPPER1, T_EMPTY, 260, 80},
		{T_SWOOPER1, T_DARTER1, T_EMPTY, 260, 80},

		{T_POD1, T_EMPTY, T_EMPTY, 260, 80},
		{T_DIVER1, T_SWOOPER1, T_EMPTY, 360, 80},

		{T_CARRIER1, T_EMPTY, T_EMPTY, 460, 80},

		{T_SWOOPER1, T_SWOOPER1, T_EMPTY, 260, 80},
		{T_STOPPER1, T_DARTER1, T_EMPTY, 260, 80},
		{T_SWOOPER1, T_STOPPER1, T_EMPTY, 260, 80},
		{T_DIVER1, T_SWOOPER1, T_EMPTY, 260, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_DIVER1, T_SWOOPER1, T_EMPTY, 160, 80},

		{T_CARRIER1, T_EMPTY, T_EMPTY, 260, 80},
		{T_SWOOPER1, T_SWOOPER1, T_EMPTY, 260, 80},

//{T_CARRIER1, T_EMPTY, T_EMPTY, 260, 80},
//{T_CARRIER1, T_EMPTY, T_EMPTY, 260, 80},
//{T_CARRIER1, T_EMPTY, T_EMPTY, 260, 80},


		{T_SWOOPER2, T_DARTER2, T_EMPTY, 160, 80},
		{T_POD1, T_EMPTY, T_EMPTY, 260, 80},

		{T_SWOOPER2, T_EMPTY, T_EMPTY, 210, 80},
		{T_SWOOPER2, T_SWOOPER1, T_EMPTY, 210, 80},
		{T_SWOOPER2, T_EMPTY, T_EMPTY, 210, 80},
		{T_SWOOPER2, T_DARTER2, T_EMPTY, 210, 80},
		{T_SWOOPER2, T_DIVER1, T_EMPTY, 210, 80},
		{T_SWOOPER2, T_SWOOPER1, T_EMPTY, 210, 80},


		{T_CARRIER1, T_EMPTY, T_EMPTY, 860, 80},

		{T_BOSS1, T_EMPTY, T_EMPTY, 5, 2},


		{T_END_LEVEL, T_EMPTY, T_EMPTY, 460, 80},

		/*{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},
		{T_STOPPER1, T_EMPTY, T_EMPTY, 160, 80},*/

// need an end?
	},

// STAGE 2!
	{
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 10, 5},	// doesn't appear

		{T_CHANGE_MUSIC, OVERMUSIC_L2_3, 0, 0, 0},


		{T_DRIFTER1, T_EMPTY, T_EMPTY, 150, 5},

		{T_DRIFTER1, T_DRIFTER1, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_DRIFTER1, T_EMPTY, 150, 5},
		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_DRIFTER1, T_EMPTY, 150, 5},

		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},

		{T_DRIFTER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_RISER, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_DRIFTER1, T_EMPTY, 150, 5},
		{T_RISER, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_DRIFTER1, T_EMPTY, 150, 5},

		{T_BEAMER, T_EMPTY, T_EMPTY, 150, 5},

		{T_ESCORTS_1, T_EMPTY, T_EMPTY, 150, 5},

		{T_DRIFTER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_RISER, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_DRIFTER1, T_EMPTY, 150, 5},
		{T_RISER, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_EMPTY, T_EMPTY, 150, 5},

		{T_BEAMER, T_EMPTY, T_EMPTY, 150, 5},

		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER3, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_DRIFTER1, T_EMPTY, 150, 5},
		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER3, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_EMPTY, T_EMPTY, 150, 5},

		{T_BEAMER, T_EMPTY, T_EMPTY, 150, 5},

		{T_DRIFTER2, T_EMPTY, T_EMPTY, 150, 5},

		{T_ESCORTS_2, T_EMPTY, T_EMPTY, 150, 5},

		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER3, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER1, T_DRIFTER1, T_EMPTY, 150, 5},
		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER3, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER1, T_EMPTY, T_EMPTY, 150, 5},

		{T_RISER, T_EMPTY, T_EMPTY, 150, 5},
		{T_SHOOTER, T_EMPTY, T_EMPTY, 150, 5},

		{T_DRIFTER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER2, T_EMPTY, T_EMPTY, 150, 5},

		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER4, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER4, T_EMPTY, T_EMPTY, 150, 5},
		{T_DRIFTER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER2, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER4, T_EMPTY, T_EMPTY, 150, 5},
		{T_CRUISER4, T_EMPTY, T_EMPTY, 150, 5},
		{T_SHOOTER, T_EMPTY, T_EMPTY, 250, 5},

		{T_BOSS2, T_EMPTY, T_EMPTY, 250, 5},


		{T_END_LEVEL, T_EMPTY, T_EMPTY, 460, 80},

	},

// STAGE 3!
	{
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 10, 5},	// doesn't appear


//{T_CHANGE_MUSIC, OVERMUSIC_L3_4, 0, 0, 0},

//{T_BOSS3, T_EMPTY, T_EMPTY, 5, 2},

		{T_SMALL, T_EMPTY, T_EMPTY, 190, 50},

		{T_SMALL, T_EMPTY, T_EMPTY, 190, 50},
		{T_CHANGE_MUSIC, OVERMUSIC_L3_3, 0, 0, 0},

		{T_SMALL, T_SMALL, T_SMALL, 190, 50},

		{T_LARGER, T_SMALL, T_EMPTY, 190, 50},
		{T_LARGER, T_SMALL, T_EMPTY, 190, 50},

		{T_WINGS, T_EMPTY, T_EMPTY, 290, 50},

		{T_SMALL, T_EMPTY, T_EMPTY, 190, 50},

		{T_LARGER, T_SMALL, T_EMPTY, 190, 50},
		{T_LARGER, T_SMALL, T_EMPTY, 190, 50},
		{T_WINGS, T_EMPTY, T_EMPTY, 290, 50},
		{T_LARGER, T_SMALL, T_EMPTY, 190, 50},
		{T_MEGA, T_EMPTY, T_EMPTY, 400, 50},

		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},
		{T_SMALL, T_SMALL2, T_SMALL2, 190, 50},
		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},

		{T_WINGS, T_EMPTY, T_EMPTY, 290, 50},
		{T_LARGER, T_SMALL2, T_EMPTY, 190, 50},
		{T_SMALL, T_SMALL2, T_SMALL, 190, 50},
		{T_SMALL, T_SMALL2, T_SMALL2, 190, 50},
		{T_LARGER2, T_SMALL2, T_EMPTY, 190, 50},

		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},
		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},
		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},

		{T_MEGA, T_SMALL, T_EMPTY, 310, 50},
		{T_MEGA, T_SMALL2, T_EMPTY, 310, 50},
		{T_CHANGE_MUSIC, OVERMUSIC_L3_4, 0, 0, 0},

		{T_WINGS2, T_EMPTY, T_EMPTY, 150, 50},
		{T_WINGS, T_EMPTY, T_EMPTY, 150, 50},
		{T_LARGER, T_EMPTY, T_EMPTY, 150, 50},
		{T_LARGER2, T_EMPTY, T_EMPTY, 150, 50},
		{T_SMALL, T_SMALL2, T_EMPTY, 150, 50},
		{T_SMALL2, T_SMALL, T_SMALL, 150, 50},
		{T_LARGER, T_EMPTY, T_EMPTY, 150, 50},
		{T_SMALL, T_SMALL2, T_EMPTY, 150, 50},
		{T_LARGER, T_LARGER2, T_EMPTY, 150, 50},
		{T_MEGA, T_MEGA, T_EMPTY, 310, 50},

		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},
		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},
		{T_SMALL, T_SMALL, T_SMALL2, 190, 50},

		{T_WINGS2, T_EMPTY, T_EMPTY, 210, 50},
		{T_WINGS, T_SMALL, T_EMPTY, 210, 50},
		{T_LARGER, T_SMALL2, T_EMPTY, 210, 50},
		{T_LARGER2, T_SMALL, T_EMPTY, 210, 50},
		{T_SMALL, T_SMALL, T_EMPTY, 210, 50},
		{T_SMALL2, T_SMALL, T_EMPTY, 210, 50},
		/*
		{T_LARGER, T_EMPTY, T_EMPTY, 210, 50},
		{T_SMALL, T_SMALL2, T_EMPTY, 210, 50},
		{T_LARGER, T_LARGER2, T_EMPTY, 210, 50},
		{T_SMALL2, T_SMALL, T_EMPTY, 210, 50},
		{T_LARGER, T_SMALL, T_EMPTY, 210, 50},
		{T_LARGER2, T_SMALL2, T_EMPTY, 210, 50},
		{T_LARGER, T_SMALL, T_EMPTY, 210, 50},
		{T_LARGER2, T_LARGER2, T_EMPTY, 210, 50},
		{T_SMALL2, T_SMALL, T_EMPTY, 250, 50},*/

		{T_BOSS3, T_EMPTY, T_EMPTY, 250, 50},

		{T_END_LEVEL, T_EMPTY, T_EMPTY, 460, 80},

	},

// STAGE 4!
	{
		{T_SWOOPER1, T_EMPTY, T_EMPTY, 150, 5},	// doesn't appear

// droppers need a low random number!

//{T_BOSS4, T_EMPTY, T_EMPTY, 150, 50},

		{T_RING, T_EMPTY, T_EMPTY, 250, 50},
		{T_RING, T_EMPTY, T_EMPTY, 250, 50},
		{T_HOOK, T_EMPTY, T_EMPTY, 250, 50},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_SPINNER, T_HOOK, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_RING, T_RING, T_EMPTY, 150, 50},
		{T_HUNTER, T_EMPTY, T_EMPTY, 150, 50},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_SPINNER, T_EMPTY, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 250, 0},
		{T_SPINNER, T_HOOK, T_EMPTY, 150, 50},
		{T_DROPPER, T_HOOK, T_EMPTY, 150, 0},
		{T_HUNTER, T_EMPTY, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},

		{T_CHANGE_MUSIC, OVERMUSIC_L4_2, 0, 0, 0},

		{T_WALK, T_HOOK, T_EMPTY, 150, 50},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_RING, T_RING, T_EMPTY, 150, 50},
		{T_WALK, T_EMPTY, T_EMPTY, 150, 50},
		{T_HOOK, T_EMPTY, T_EMPTY, 150, 50},
		{T_HUNTER, T_EMPTY, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_HOOK2, T_EMPTY, T_EMPTY, 150, 50},
		{T_SPINNER, T_RING, T_EMPTY, 150, 50},
		{T_DROPPER, T_HOOK, T_EMPTY, 150, 0},
		{T_HOOK2, T_EMPTY, T_EMPTY, 150, 50},
		{T_RING, T_RING, T_EMPTY, 150, 50},
		{T_WALK, T_HOOK, T_EMPTY, 150, 50},
		{T_DROPPER, T_EMPTY, T_EMPTY, 150, 0},
		{T_SPINNER, T_EMPTY, T_EMPTY, 150, 50},
		{T_HOOK2, T_EMPTY, T_EMPTY, 150, 50},
		{T_WALK, T_HOOK, T_EMPTY, 150, 50},
		{T_HUNTER, T_EMPTY, T_EMPTY, 350, 50},

		{T_BOSS4, T_EMPTY, T_EMPTY, 350, 50},


		{T_END_LEVEL, T_EMPTY, T_EMPTY, 460, 80},

	}
};



void spawn_enemy (int type, int pole);
void make_track (int t);
void run_tracks (void);

void make_group_track (int t, int type);
void set_dancer (int e, int t);
void set_hook (int e, int t);
void set_darter (int e, int t);
void set_stopper (int e, int t);
void set_diver (int e, int t);
void set_glider (int e, int t);
void set_attacker (int e, int t);
void set_dart_stopper (int e, int t, int m);
void set_drifter (int e, int t);
void set_cruiser (int e, int t);
void set_walker (int e, int t);
void give_escort (int e, int etype, int x1, int y1, int x, int y);

void set_beamer (int e, int t);
void set_pauser (int e, int t);
void place_marchers (int t);

int create_part (int pe, int index, int type, int x, int y);
void set_boss (int e, int t);


void init_level (void)
{
	init_pbullets ();
	init_ebullets ();
	init_enemies ();
	init_clouds ();

	int i;

	for (i = 0; i < NO_TRACKS; i++)
	{
		track[i].type = ENEMY_NONE;

	}

	overtrack_pos = 0;
	overtrack_pos2 = 0;
// boss.fight = 0;

	arena.new_level_sign = 100;
	arena.beam = 0;
	arena.old_beam = 0;
	arena.level_finished = 0;

	arena.underlay_position = 0;

	next_track = 50;

	arena.cruiser_escorts = 0;

// init_mtracks(); sound!!! replace

// level_display_init();

}

void run_level (void)
{

	run_tracks ();


}

void run_tracks (void)
{
	int t;
	int e;

//if (boss.fight)
// return;

	if (arena.level_finished > 0 || arena.game_over > 0)
		return;

	if (boss.fight == 0)
	{
		next_track--;

		if (next_track <= 0)
		{
			for (t = 0; t < 3; t++)
			{
				track[t].type = ENEMY_NONE;
				make_track (t);
			}
			next_track = overtrack[arena.level - 1][overtrack_pos][OT_NEXT];	// see also above
		}
	}

	for (t = 0; t < 3; t++)
	{
		if (track[t].type == ENEMY_NONE || track[t].multiple == 0)
		{
			continue;
		}
		if (track[t].count <= 0)
		{
			if (track[t].multiple == 0)
			{
				track[t].delay--;
				continue;
			}
			while (track[t].count <= 0 && track[t].multiple > 0)
			{
				/*       if (track[t].style == TRACK_MARCHER)
				   {
				   place_marchers(t);
				   track[t].multiple = 0;
				   track[t].count = 0;
				   track[t].type = ENEMY_NONE;
				   continue;
				   } */
//       if (track[t].style == TRACK_BOSS1)
//        continue;
				e = create_enemy (track[t].type);
				if (e != -1)
				{
					enemy[e].x = track[t].x[track[t].multiple];
					enemy[e].y = track[t].y[track[t].multiple];
					enemy[e].angle = ANGLE_4;
					switch (eclass[enemy[e].type].ai)
					{
					default:
					case AI_DANCER:
						set_dancer (e, t);
						break;
					case AI_DARTER:
						set_darter (e, t);
						break;
					case AI_DART_STOPPER:
						set_dart_stopper (e, t, track[t].multiple);
						break;
					case AI_STOPPER:
						set_stopper (e, t);
						break;
					case AI_DIVER:
						set_diver (e, t);
						break;
					case AI_DRIFTER:
						set_drifter (e, t);
						break;
					case AI_CRUISER:
						set_cruiser (e, t);
						break;
					case AI_WALK:
						set_walker (e, t);
						break;
					case AI_BOSS:
						set_boss (e, t);
						break;
					case AI_HOOK:
						set_hook (e, t);
						break;
						/*          case AI_DIVER:
						           set_diver(e, t);
						           break;
						          case AI_GLIDER:
						           set_glider(e, t);
						           break;
						          case AI_PAUSER:
						           set_pauser(e, t);
						           break;
						          case AI_ATTACKER:
						           set_attacker(e, t);
						           break;
						          case AI_BEAMER:
						           set_beamer(e, t);
						           break;*/
					}
				}
				track[t].count = track[t].count2;
				track[t].multiple--;
			}
		}				// count > 0. so -- it
		track[t].count--;

	}



}

void make_track (int t)
{

	int etype;

	track[t].total = 0;

	track[t].reverse = -1;

	int track_number = grand (3);

	if (t == 0 && grand (2) == 0)
		track_number = 3;

	int s_etype = 3;

	overtrack_pos2++;
	if (overtrack_pos2 > 2)
	{
		overtrack_pos2 = 0;
		overtrack_pos++;
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_CHANGE_MUSIC)
		{
			change_music (overtrack[arena.level - 1][overtrack_pos][1]);
			overtrack_pos++;
			overtrack_pos2 = -1;	// so with the increment above it'll be 0
			return;
//    overtrack_pos2 = 0; // must reset this for every non-enemy overtrack entry!!
		}
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_ESCORTS_1)
		{
			arena.cruiser_escorts = 1;
			overtrack_pos++;
			overtrack_pos2 = -1;	// so with the increment above it'll be 0
			return;
//    overtrack_pos2 = 0; // must reset this for every non-enemy overtrack entry!!
		}
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_ESCORTS_2)
		{
			arena.cruiser_escorts = 2;
			overtrack_pos++;
			overtrack_pos2 = -1;	// so with the increment above it'll be 0
			return;
//    overtrack_pos2 = 0; // must reset this for every non-enemy overtrack entry!!
		}
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_ESCORTS_3)
		{
			arena.cruiser_escorts = 3;
			overtrack_pos++;
			overtrack_pos2 = -1;	// so with the increment above it'll be 0
			return;
//    overtrack_pos2 = 0; // must reset this for every non-enemy overtrack entry!!
		}
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_ESCORTS_0)
		{
			arena.cruiser_escorts = 0;
			overtrack_pos++;
			overtrack_pos2 = -1;	// so with the increment above it'll be 0
			return;
//    overtrack_pos2 = 0; // must reset this for every non-enemy overtrack entry!!
		}
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_END_LEVEL)
		{
			arena.level_finished = 300;
			return;
		}
		if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] ==
		        T_OVERTRACK_END)
			overtrack_pos = 0;
	}
	if (overtrack[arena.level - 1][overtrack_pos][overtrack_pos2] == T_EMPTY)
	{
		track[t].multiple = 0;
		return;
	}

	s_etype = overtrack[arena.level - 1][overtrack_pos][overtrack_pos2];


	switch (s_etype)
	{
	case T_BOSS1:
		etype = ENEMY_BOSS1;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_BOSS;
		make_group_track (t, etype);
		break;
	case T_BOSS2:
		etype = ENEMY_BOSS2;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_BOSS;
		make_group_track (t, etype);
		break;
	case T_BOSS3:
		etype = ENEMY_BOSS3;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_BOSS;
		make_group_track (t, etype);
		break;
	case T_BOSS4:
		etype = ENEMY_BOSS4;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_BOSS;
		make_group_track (t, etype);
		break;
	case T_CRUISER1:
		etype = ENEMY_CRUISER1;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	case T_CRUISER2:
		etype = ENEMY_CRUISER2;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	case T_CRUISER3:
		etype = ENEMY_CRUISER3;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	case T_CRUISER4:
		etype = ENEMY_CRUISER4;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	case T_CRUISER5:
		etype = ENEMY_CRUISER5;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	case T_RISER:
		etype = ENEMY_RISER;
		track[t].type = etype;
		track[t].multiple = 6;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_BEAMER:
		etype = ENEMY_BEAMER;
		track[t].type = etype;
		track[t].multiple = 3;
		track[t].count = 2;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_SHOOTER:
		etype = ENEMY_SHOOTER;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 2;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_DRIFTER1:
		etype = ENEMY_DRIFTER1;
		track[t].type = etype;
		track[t].multiple = 3;
		track[t].count = 25;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DRIFTER;
		make_group_track (t, etype);
		break;
	case T_DRIFTER2:
		etype = ENEMY_DRIFTER2;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 2;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DRIFTER;
		make_group_track (t, etype);
		break;
	case T_MEGA:
		etype = ENEMY_MEGA;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 2;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_WINGS:
		etype = ENEMY_WINGS;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 2;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_WINGS2:
		etype = ENEMY_WINGS2;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 2;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_SMALL:
		etype = ENEMY_SMALL;
		track[t].type = etype;
		track[t].multiple = 3;
		track[t].count = 30;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_WANDER;
		make_group_track (t, etype);
		break;
	case T_SMALL2:
		etype = ENEMY_SMALL2;
		track[t].type = etype;
		track[t].multiple = 3;
		track[t].count = 25;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_WANDER;
		make_group_track (t, etype);
		break;
	case T_LARGER:
		etype = ENEMY_LARGER;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 5;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DART_STOPPER;
		make_group_track (t, etype);
		break;
	case T_LARGER2:
		etype = ENEMY_LARGER2;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 5;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DART_STOPPER;
		make_group_track (t, etype);
		break;
	case T_SWOOPER1:
		etype = ENEMY_SWOOPER1;
		track[t].type = etype;
		track[t].multiple = 4;
		track[t].count = 20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_WANDER;
		make_group_track (t, etype);
		break;
	case T_SWOOPER2:
		etype = ENEMY_SWOOPER2;
		track[t].type = etype;
		track[t].multiple = 4;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_WANDER;
		make_group_track (t, etype);
		break;
	case T_DARTER1:
		etype = ENEMY_DARTER1;
		track[t].type = etype;
		track[t].multiple = 4;
		track[t].count = 0;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DARTER;
		make_group_track (t, etype);
		break;
	case T_DARTER2:
		etype = ENEMY_DARTER2;
		track[t].type = etype;
		track[t].multiple = 3;
		track[t].count = 0;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DARTER;
		make_group_track (t, etype);
		break;
	case T_STOPPER1:
		etype = ENEMY_STOPPER1;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 0;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_CARRIER1:
		etype = ENEMY_CARRIER1;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 0;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_DIVER1:
		etype = ENEMY_DIVER1;
		track[t].type = etype;
		track[t].multiple = 3;
		track[t].count = 0;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DIVER;
		make_group_track (t, etype);
		break;
	case T_POD1:
		etype = ENEMY_POD1;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 1;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_HOOK:
		etype = ENEMY_HOOK;
		track[t].type = etype;
		track[t].multiple = 4;
		track[t].count = 10;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_SLIDER;
		make_group_track (t, etype);
		break;
	case T_HOOK2:
		etype = ENEMY_HOOK2;
		track[t].type = etype;
		track[t].multiple = 4;
		track[t].count = 15;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_SLIDER;
		make_group_track (t, etype);
		break;
	case T_WALK:
		etype = ENEMY_WALKER;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 0;		//20;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_WALK;
		make_group_track (t, etype);
		break;
	case T_DROPPER:
		etype = ENEMY_DROPPER;
		track[t].type = etype;
		track[t].multiple = 4;
		track[t].count = 45;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	case T_RING:
		etype = ENEMY_RING;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 1;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_DRIFTER;
		make_group_track (t, etype);
		break;
	case T_HUNTER:
		etype = ENEMY_HUNTER;
		track[t].type = etype;
		track[t].multiple = 2;
		track[t].count = 1 + grand (30);
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_STOPPER;
		make_group_track (t, etype);
		break;
	case T_SPINNER:
		etype = ENEMY_SPINNER;
		track[t].type = etype;
		track[t].multiple = 1;
		track[t].count = 1;
		track[t].count2 = track[t].count;
		track[t].delay = 80;
		track[t].style = TRACK_CRUISER;
		make_group_track (t, etype);
		break;
	}

	track[t].count = grand (overtrack[arena.level - 1][overtrack_pos][OT_WAIT]);

}

void make_group_track (int t, int type)
{

	int a;			//, b;

	int i;

	switch (track[t].style)
	{
	case TRACK_WALK:
		track[t].x[1] = 200000 + grand (240000);
		track[t].y[1] = -70000;
		break;
	case TRACK_CRUISER:
		track[t].x[1] = RAND_X_IN;
		track[t].y[1] = -70000;
		break;
	case TRACK_DRIFTER:
		for (i = 1; i < track[t].multiple + 1; i++)
		{
			switch (grand (5))
			{
			default:
				track[t].x[i] = RAND_X;
				track[t].y[i] = -30000;
				break;
			case 0:
				track[t].x[i] = -30000;
				track[t].y[i] = RAND_Y_IN - 100000;
				break;
			case 1:
				track[t].x[i] = 670000;
				track[t].y[i] = RAND_Y_IN - 100000;
				break;
			}
		}
		break;
	case TRACK_STOPPER:
		if (type == ENEMY_POD1 || type == ENEMY_HUNTER)
		{
//    track[t].x [0] = 100000;
			track[t].x[1] = 100000 + grand (150000);
			track[t].x[2] = 540000 - grand (150000);
			track[t].y[1] = -40000;
			track[t].y[2] = -40000;
			break;
		}
		if (type == ENEMY_WINGS || type == ENEMY_WINGS2)
		{
			track[t].x[1] = 100000 + grand (150000);
			track[t].x[2] = 540000 - grand (150000);
			track[t].y[1] = 520000;
			track[t].y[2] = 520000;
			break;
		}
		if (type == ENEMY_BEAMER || type == ENEMY_SHOOTER)
		{
			track[t].x[1] = 100000 + grand (100000);
			track[t].x[2] = 540000 - grand (100000);
			track[t].x[3] = 270000 + grand (100000);
			track[t].y[1] = 520000;
			track[t].y[2] = 520000;
			track[t].y[3] = 520000;
			break;
		}
		if (type == ENEMY_RISER)
		{
			track[t].x[1] = 40000 + grand (50000);
			track[t].x[2] = 140000 + grand (50000);
			track[t].x[3] = 240000 + grand (50000);
			track[t].x[4] = 340000 + grand (50000);
			track[t].x[5] = 440000 + grand (50000);
			track[t].x[6] = 540000 + grand (50000);
			track[t].y[1] = 520000;
			track[t].y[2] = 520000;
			track[t].y[3] = 520000;
			track[t].y[4] = 520000;
			track[t].y[5] = 520000;
			track[t].y[6] = 520000;
			break;
		}
		track[t].x[1] = RAND_X_IN;
		track[t].y[1] = -40000;
		if (type == ENEMY_MEGA)
			track[t].y[1] = -70000;
		break;
	default:
	case TRACK_WANDER:
		track[t].x[0] = RAND_X;
		track[t].y[0] = -30000;
		if (type == ENEMY_SMALL || type == ENEMY_SMALL2)
			track[t].y[0] = -60000;
// int tw = grand(2);
		for (i = 1; i < TRACK_SIZE; i++)
		{
			track[t].x[i] = track[t].x[0];
			track[t].y[i] = track[t].y[0];
		}

		switch (grand (3))
		{
		case 0:			// R-L-R
		default:
			track[t].x[0] = RAND_X;
			track[t].x[0] = (RAND_X / 3) + X_MAX / 3 * 2;
//    track[t].y [0] = 150000;

			track[t].x_target[0] = RAND_X / 5 + X_MAX / 8;
//    track[t].x_target [0] = RAND_X / 3;
//    track[t].y_target [0] = (RAND_Y / 3) * 2;
			track[t].y_target[0] = (RAND_Y / 2) + Y_MAX / 5;

			track[t].x_target[1] = (RAND_X / 3) + (X_MAX / 3) * 2;
			track[t].y_target[1] = (RAND_Y / 2) + Y_MAX / 5;
//    track[t].y_target [1] = (RAND_Y / 3) * 2;

			track[t].x_target[2] = -1;
			track[t].y_target[2] = -1;
			if (grand (2) == 0)
			{
				track[t].x[0] = X_MAX - track[t].x[0];
				track[t].x_target[0] = X_MAX - track[t].x_target[0];
				track[t].x_target[1] = X_MAX - track[t].x_target[1];
			}
			break;
		}

		track[t].target_time = 250;
		break;

	case TRACK_SLIDER:
		track[t].x[0] = -30000;
		track[t].y[0] = RAND_Y;
		for (i = 1; i < TRACK_SIZE; i++)
		{
			track[t].x[i] = track[t].x[0];
			track[t].y[i] = track[t].y[0];
		}

		track[t].x_target[0] = 400000 + grand (200000);
		track[t].y_target[0] = RAND_Y;

		track[t].x_target[1] = 40000 + grand (200000);
		track[t].y_target[1] = RAND_Y;

		track[t].x_target[2] = 400000 + grand (200000);
		track[t].y_target[2] = RAND_Y;

		track[t].x_target[3] = -1;
		track[t].y_target[3] = -1;
		track[t].target_time = 250;
		break;

	case TRACK_DARTER:
		track[t].x[0] = grand (50000) * -1;
		track[t].y[0] = -30000 - grand (140000);

		for (i = 1; i < track[t].multiple + 1; i++)
		{
			track[t].x[i] = track[t].x[i - 1] + 120000;
			track[t].y[i] = -30000 - grand (140000);	//track[t].y [0];
		}



		break;

	case TRACK_DIVER:
		track[t].x[0] = grand (50000) * -1;
		track[t].y[0] = -30000 - grand (140000);

		for (i = 1; i < track[t].multiple + 1; i++)
		{
			track[t].x[i] = track[t].x[i - 1] + 120000 + grand (35000);
			track[t].y[i] = -30000 - 60000 * i;	//grand(140000);//track[t].y [0];
		}



		break;

	case TRACK_DART_STOPPER:
//   track[t].x [0] = grand(50000) * -1;
//   track[t].y [0] = -30000 - grand(140000);

		/*    for (i = 1; i < track[t].multiple + 1; i ++)
		    {
		     track[t].y [i] = RAND_Y;
		     track[t].x [i] = -50000;
		     if (i % 2 == 0)
		      track[t].x [i] = 690000;
		     track[t].x_target [0] = RAND_X_IN;
		     track[t].y_target [0] = RAND_Y_IN;
		     track[t].x_target [1] = RAND_X_IN;
		     track[t].y_target [1] = RAND_Y_IN;
		    }*/
		break;
	case TRACK_SIDES:
		track[t].reverse = grand (2);
		a = grand (2);
		track[t].x[0] = X_MIN + 40000 + grand (30000);
//   track[t].x [0] = 100000;
		if (a == 1)
			track[t].x[0] = X_MAX - 40000 - grand (30000);

		track[t].y[0] = -30000;

		for (i = 1; i < track[t].multiple + 1; i++)
		{
			if (i % 2 == 1)
				track[t].x[i] = X_MAX - track[t].x[0];
			else
				track[t].x[i] = track[t].x[0];
			track[t].y[i] = track[t].y[0];

		}


		track[t].x_target[0] = X_MIN + RAND_X / 4;
		if (a == 1)
			track[t].x_target[0] = X_MAX - RAND_X / 4;
		track[t].y_target[0] = Y_MIN + RAND_Y / 3;

		track[t].x_target[1] = X_MIDDLE;
		track[t].y_target[1] = Y_MIDDLE;	// rand?

		track[t].x_target[2] = X_MAX - RAND_X / 4;
		if (a == 1)
			track[t].x_target[2] = X_MIN + RAND_X / 4;
		track[t].y_target[2] = 500000;

		track[t].x_target[3] = -1;
		track[t].y_target[3] = -1;

		track[t].target_time = 280;
		break;
		/*   case TRACK_DIVER:
		//        exit(1);
		   track[t].reverse = grand(2);
		   a = 0;

		   track[t].x [0] = X_MIN + 40000 + grand(30000);
		   if (a == 1)
		    track[t].x [0] = X_MAX - 50000;

		   track[t].y [0] = -30000;

		   for (i = 1; i < track[t].multiple + 1; i ++)
		   {
		    track[t].x [i] = track[t].x [0] + i * 75000;
		    if (a == 1)
		     track[t].x [i] = track[t].x [0] - i * 75000;
		    track[t].y [i] = track[t].y [0];
		   }

		   track[t].target_time = grand(30) + 20;
		    break;
		   case TRACK_GLIDER:
		   track[t].x [0] = RAND_X;
		   track[t].y [0] = -70000 + grand(23000);

		   track[t].x_target [0] = track[t].x [0];
		   track[t].y_target [0] = 500000;

		   for (i = 1; i < track[t].multiple + 1; i ++)
		   {
		    track[t].x [i] = track[t].x [0];
		    track[t].y [i] = track[t].y [0];
		    track[t].x_target [0] = track[t].x [0];
		    track[t].y_target [0] = 500000;
		   }
		        break;
		   case TRACK_ATTACKER:

		// note i = 0
		   t = t;
		   int str_x;
		   int str_y = -20000 - grand(55000);
		   if (grand(2) == 0)
		   {
		//    str_x = 30000 + grand(50000);
		    str_x = -30000 + grand(75000);
		    for (i = 0; i < track[t].multiple + 1; i ++)
		    {
		     track[t].x [i] = str_x + i * 90000;
		     track[t].y [i] = str_y;
		    }
		   } else
		   {
		//    str_x = X_MAX - (30000 + grand(50000));
		    str_x = X_MAX - (-40000 + grand(45000));
		    for (i = 0; i < track[t].multiple + 1; i ++)
		    {
		     track[t].x [i] = str_x - (i * 90000);
		     track[t].y [i] = str_y;
		    }
		   }


		//   track[t].target_time = grand(30) + 30;
		    break;
		   case TRACK_BEAMER:
		   for (i = 0; i < track[t].multiple + 1; i ++)
		   {
		    track[t].x [i] = RAND_X;
		    track[t].y [i] = -70000;
		   }
		    break;*/

	}

}

int get_max_stage_score (void)
{
	int i, j;
	int s = 0;
	int ttype, etype;

	for (i = 0; i < 71; i++)
	{
		for (j = 0; j < 3; j++)
		{
			ttype = overtrack[arena.level - 1][i][j];
			if (ttype == T_END_LEVEL)
			{
				i = 80;
				break;
			}
			etype = track_score[ttype][1];
			if (track_score[ttype][0] == 0)
				continue;
			s += eclass[etype].xp * track_score[ttype][0];
//         s += track_score [ttype] [0];
		}

	}

	return s;

}


void give_escort (int e, int etype, int x1, int y1, int x, int y)
{


	int e2 = create_enemy (etype);
	if (e2 != -1)
	{
		enemy[e2].x = x;
		enemy[e2].y = y;
		enemy[e2].angle = ANGLE_4;

		enemy[e2].persist = 1;

		enemy[e2].target = 0;
		enemy[e2].escorting = e;
		enemy[e2].pulse1 = x1;
		enemy[e2].pulse2 = y1;

		enemy[e2].target_time = eclass[etype].speed3;
		enemy[e2].phase = 0;

		enemy[e2].angle1 = ANGLE_4;
		enemy[e2].angle = ANGLE_4;

		enemy[e2].x_target[0] = enemy[e].x;
		enemy[e2].y_target[0] = enemy[e].y;
//        enemy[e2].angle = radians_to_angle(atan2(enemy [e].y_target [0] - enemy [e].y, enemy [e].x_target [0] - enemy [e].x));
//        enemy[e2].angle = radians_to_angle(atan2(enemy [e].y_target [0] - enemy [e].y, enemy [e].x_target [0] - enemy [e].x));
		enemy[e2].angle = grand (ANGLE_1);
		enemy[e2].x_speed = 0;	//xpart(enemy[e2].angle, 3000);
		enemy[e2].y_speed = 0;	//ypart(enemy[e2].angle, 3000);
	}





}

void set_boss (int e, int t)
{
	int i;
// enemy [e].target_time = track[t].target_time;
	enemy[e].persist = 1;

// enemy [e].target = 0;

// enemy [e].target_time = 60 + grand(60); // may be overridden below
// enemy [e].max_target_time = track[t].target_time;
// enemy [e].target = 0;
// enemy[e].phase = 0;
// enemy[e].phase2 = 0;

	enemy[e].x = 320000;
	enemy[e].y = -70000;
	enemy[e].x_speed = 0;
	enemy[e].y_speed = 2000;
	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;

	boss.fight = 1;
	boss.index = e;
	boss.phase = 0;
	boss.phase_count = 0;
	boss.recycle = 50;
	boss.status = 0;

	switch (enemy[e].type)
	{
	case ENEMY_BOSS1:
		boss.hp = 50000;		// + arena.difficulty * 15000;
//      boss.hp = 1000 + arena.difficulty * 100;
		boss.max_hp = boss.hp;
		boss.hp_bar_size = 0;

		boss.phase_count = 100;
		boss.out_sides = 0;
		boss.out_sides2 = 0;
		boss.out_up = 0;
		boss.out_out_up = 0;
		boss.t_angle[0] = ANGLE_2;
		boss.t_angle_turning[0] = 0;
		boss.t_angle[1] = 0;
		boss.t_angle_turning[1] = 0;
		boss.t_status = 0;
		boss.t_recycle[0] = 10;
		break;
	case ENEMY_BOSS2:
		boss.hp = 60000;		// + arena.difficulty * 16000;
//      boss.hp = 3000;
		boss.max_hp = boss.hp;
		boss.hp_bar_size = 0;

		enemy[e].y = 550000;
		enemy[e].x_speed = 0;
		enemy[e].y_speed = -7000;

		boss.phase_count = 60;
		boss.out_sides = 0;
		boss.out_sides2 = 0;
		boss.t_status = grand (4);
		boss.t_status_change = 1;

		for (i = 0; i < 4; i++)
		{
			boss.t_angle[i] = -ANGLE_4;
			boss.t_angle_turning[i] = 0;
			boss.t_recycle[i] = 10;
			boss.t_beam[i] = 0;
		}
		break;
	case ENEMY_BOSS3:
		boss.hp = 50000;		// + arena.difficulty * 15000;
		//boss.hp = 3000;
		boss.max_hp = boss.hp;
		boss.hp_bar_size = 0;

		enemy[e].y = -250000;
		enemy[e].x_speed = 0;
		enemy[e].y_speed = 4000;

		boss.phase_count = 75;
		boss.phase2 = 3;


		boss.petal_angle[0] = grand (ANGLE_1);
		boss.petal_angle[1] = grand (ANGLE_1);
		boss.petal_angle[2] = grand (ANGLE_1);
		boss.petal_speed[0] = 4;
		boss.petal_speed[1] = 8;
		boss.petal_speed[2] = -6;
		boss.petal_out[0] = 90;
		boss.petal_out[1] = 140;
		boss.petal_out[2] = 190;

		boss.next_small = 50;

//      boss.out_sides = 0;
//      boss.out_sides2 = 0;
//      boss.t_status = grand(4);
//      boss.t_status_change = 1;
		break;
	case ENEMY_BOSS4:
		boss.hp = 70000;		// + arena.difficulty * 18000;
//    boss.hp = 3000;
		boss.max_hp = boss.hp;
		boss.hp_bar_size = 0;
		enemy[e].y = -250000;
		boss.phase_count = 5000;
		boss.spin_size = 0;
		boss.spin_rad = 0;
		boss.spin_angle = 0;
		boss.spin_size2 = 0;
		boss.spin_rad2 = 0;
		boss.spin_angle2 = 0;
		boss.spin_size3 = 0;
		boss.spin_rad3 = 0;
		boss.spin_counter++;
		boss.sides_recycle = 30;
		boss.outer_fire_type = 1;
		boss.outer_fire_count = 10;
		boss.recycle = 200;
		break;

	}

}

void create_boss3_dancer (int type)
{
	int i;

	int e = create_enemy (type);

	if (e == -1)
		return;

	enemy[e].persist = 1;
	enemy[e].angle1 = ANGLE_4;


	int t = 0;


	enemy[e].x = RAND_X;
	enemy[e].y = -60000;

	track[t].x[0] = RAND_X;
	track[t].x[0] = (RAND_X / 3) + X_MAX / 3 * 2;

	track[t].x_target[0] = RAND_X / 5 + X_MAX / 8;
	track[t].y_target[0] = (RAND_Y / 2) + Y_MAX / 5;

	track[t].x_target[1] = (RAND_X / 3) + (X_MAX / 3) * 2;
	track[t].y_target[1] = (RAND_Y / 2) + Y_MAX / 5;

	track[t].x_target[2] = -1;
	track[t].y_target[2] = -1;
	if (grand (2) == 0)
	{
		track[t].x[0] = X_MAX - track[t].x[0];
		track[t].x_target[0] = X_MAX - track[t].x_target[0];
		track[t].x_target[1] = X_MAX - track[t].x_target[1];
	}

	track[t].target_time = 250;


	for (i = 0; i < MAX_TARGETS; i++)
	{
		enemy[e].x_target[i] = track[t].x_target[i];	//X_MIN + track[t].x_target [i];
		if (track[t].x_target[i] == -1)
			enemy[e].x_target[i] = -1;
		enemy[e].y_target[i] = track[t].y_target[i];	//Y_MIN + track[t].y_target [i];
	}

	/*
	 if (reverse != 0)
	 {
	  for (i = 0; i < MAX_TARGETS; i ++)
	  {
	    enemy [e].x_target [i] = X_MAX - track[t].x_target [i];
	    if (track[t].x_target [i] == -1)
	     enemy [e].x_target [i] = -1;
	    enemy [e].y_target [i] = Y_MIN + track[t].y_target [i];
	  }
	 }
	  else
	  {
	   for (i = 0; i < MAX_TARGETS; i ++)
	   {
	      enemy [e].x_target [i] = X_MIN + track[t].x_target [i];
	      if (track[t].x_target [i] == -1)
	       enemy [e].x_target [i] = -1;
	      enemy [e].y_target [i] = Y_MIN + track[t].y_target [i];
	   }
	  }
	*/
	enemy[e].target_time = track[t].target_time;
	enemy[e].max_target_time = track[t].target_time;
	enemy[e].target = 0;

	enemy[e].angle1_turning = 0;
	enemy[e].angle1_turning_count = 0;
	enemy[e].recycle = 5 + grand (10);

	switch (enemy[e].type)
	{
	case ENEMY_SMALL:
	case ENEMY_SMALL2:
		enemy[e].angle1 =
		    radians_to_angle (atan2
		                      (enemy[e].y_target[0] - enemy[e].y,
		                       enemy[e].x_target[0] - enemy[e].x));
		enemy[e].petal_angle[0] = enemy[e].angle1 + ANGLE_2;
		enemy[e].petal_angle[1] = enemy[e].angle1 + ANGLE_2 - ANGLE_6;
		enemy[e].petal_angle[2] = enemy[e].angle1 + ANGLE_2 + ANGLE_6;
		enemy[e].max_target_time = 250;
		break;

	}

}


void set_hook (int e, int t)
{
	int i;

	enemy[e].persist = 1;
	enemy[e].angle1 = 0;

	if (track[t].multiple & 1)
	{
		enemy[e].angle1 = ANGLE_2;
		enemy[e].x = 670000;
		for (i = 0; i < MAX_TARGETS; i++)
		{
			enemy[e].x_target[i] = X_MAX - track[t].x_target[i];
			if (track[t].x_target[i] == -1)
				enemy[e].x_target[i] = -1;
			enemy[e].y_target[i] = track[t].y_target[i];
		}
	}
	else
	{
		for (i = 0; i < MAX_TARGETS; i++)
		{
			enemy[e].x_target[i] = track[t].x_target[i];
			if (track[t].x_target[i] == -1)
				enemy[e].x_target[i] = -1;
			enemy[e].y_target[i] = track[t].y_target[i];
		}
	}

	enemy[e].target_time = track[t].target_time;
	enemy[e].max_target_time = track[t].target_time;
	enemy[e].target = 0;
	enemy[e].angle1_turning = 0;
	enemy[e].angle1_turning_count = 0;
	enemy[e].recycle = 5 + grand (10);

}


void set_dancer (int e, int t)
{
	int i;

	enemy[e].persist = 1;
	enemy[e].angle1 = ANGLE_4;

	if (track[t].reverse != -1 && track[t].multiple % 2 == track[t].reverse)
	{
		for (i = 0; i < MAX_TARGETS; i++)
		{
			enemy[e].x_target[i] = X_MAX - track[t].x_target[i];
			if (track[t].x_target[i] == -1)
				enemy[e].x_target[i] = -1;
			enemy[e].y_target[i] = Y_MIN + track[t].y_target[i];
		}
	}
	else
	{
		for (i = 0; i < MAX_TARGETS; i++)
		{
			enemy[e].x_target[i] = X_MIN + track[t].x_target[i];
			if (track[t].x_target[i] == -1)
				enemy[e].x_target[i] = -1;
			enemy[e].y_target[i] = Y_MIN + track[t].y_target[i];
		}
	}

	enemy[e].target_time = track[t].target_time;
	enemy[e].max_target_time = track[t].target_time;
	enemy[e].target = 0;
	enemy[e].angle1_turning = 0;
	enemy[e].angle1_turning_count = 0;
	enemy[e].recycle = 5 + grand (10);

	switch (enemy[e].type)
	{
	case ENEMY_SMALL:
	case ENEMY_SMALL2:
		enemy[e].angle1 =
		    radians_to_angle (atan2
		                      (enemy[e].y_target[0] - enemy[e].y,
		                       enemy[e].x_target[0] - enemy[e].x));
		enemy[e].petal_angle[0] = enemy[e].angle1 + ANGLE_2;
		enemy[e].petal_angle[1] = enemy[e].angle1 + ANGLE_2 - ANGLE_6;
		enemy[e].petal_angle[2] = enemy[e].angle1 + ANGLE_2 + ANGLE_6;
		enemy[e].max_target_time = 250;
		/*   for (i = 0; i < 3; i ++)
		   {
		    enemy[e].petal_angle [i] = i * ANGLE_3;
		   }*/
		break;

	}

}

void set_darter (int e, int t)
{
// int i;
// enemy [e].target_time = track[t].target_time;
	enemy[e].persist = 1;

	enemy[e].target = 0;

	enemy[e].target_time = 30 + grand (60);
// enemy [e].max_target_time = track[t].target_time;
// enemy [e].target = 0;
	enemy[e].phase = 0;
	enemy[e].phase2 = 0;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = 4000;
	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;
}

void set_dart_stopper (int e, int t, int m)
{

	enemy[e].persist = 1;

	enemy[e].target = 0;

	enemy[e].target_time = 30 + grand (60);
	enemy[e].phase = 0;
	enemy[e].phase2 = 0;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = 0;



	enemy[e].x = -50000;
	switch (enemy[e].type)
	{
	default:
	case ENEMY_LARGER:
		enemy[e].status = 16;
		break;
	case ENEMY_LARGER2:
		enemy[e].status = 12;
		break;
	}
	if (m % 2 == 0)
	{
		enemy[e].x = 690000;
		enemy[e].status *= -1;
	}

	enemy[e].y = RAND_Y;

	enemy[e].x_target[0] = RAND_X;
	enemy[e].y_target[0] = RAND_Y;
	enemy[e].x_target[1] = RAND_X_IN;
	enemy[e].y_target[1] = RAND_Y_IN;

// enemy [e].angle1 = ANGLE_4;

	enemy[e].angle =
	    radians_to_angle (atan2
	                      (enemy[e].y_target[0] - enemy[e].y,
	                       enemy[e].x_target[0] - enemy[e].x));

	switch (enemy[e].type)
	{

	case ENEMY_LARGER:
		enemy[e].petal_angle[0] = enemy[e].angle + ANGLE_2 - ANGLE_4;
		enemy[e].petal_angle[1] = enemy[e].angle + ANGLE_2 - ANGLE_8;
		enemy[e].petal_angle[2] = enemy[e].angle + ANGLE_2 + ANGLE_8;
		enemy[e].petal_angle[3] = enemy[e].angle + ANGLE_2 + ANGLE_4;
		break;
	case ENEMY_LARGER2:
		enemy[e].petal_angle[0] = enemy[e].angle + ANGLE_2 - ANGLE_8;
		enemy[e].petal_angle[1] = enemy[e].angle + ANGLE_2;
		enemy[e].petal_angle[2] = enemy[e].angle + ANGLE_2 + ANGLE_8;

		enemy[e].petal_angle[3] = enemy[e].angle + ANGLE_2 - ANGLE_16;
		enemy[e].petal_angle[4] = enemy[e].angle + ANGLE_2;
		enemy[e].petal_angle[5] = enemy[e].angle + ANGLE_2 + ANGLE_16;
		break;
	}

	/* if (enemy[e].x < 0)
	  enemy [e].angle = 0;
	   else
	  enemy [e].angle = ANGLE_2;*/
}

void set_drifter (int e, int t)
{
// int i;
// enemy [e].target_time = track[t].target_time;
	enemy[e].persist = 1;

	enemy[e].target = 0;

	enemy[e].target_time = eclass[enemy[e].type].speed3;
	enemy[e].phase = 0;
// enemy[e].phase2 = 0;

	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;

	enemy[e].x_target[0] = RAND_X_IN;
	enemy[e].y_target[0] = RAND_Y_IN;
	enemy[e].angle =
	    radians_to_angle (atan2
	                      (enemy[e].y_target[0] - enemy[e].y,
	                       enemy[e].x_target[0] - enemy[e].x));
	enemy[e].x_speed = xpart (enemy[e].angle, 3000);
	enemy[e].y_speed = ypart (enemy[e].angle, 3000);

// may need to mirror some changes in give_escort

	switch (enemy[e].type)
	{
	case ENEMY_DRIFTER2:
		give_escort (e, ENEMY_ESCORT, -50000, 0, enemy[e].x, enemy[e].y);
		give_escort (e, ENEMY_ESCORT, 50000, 0, enemy[e].x, enemy[e].y);
		break;
	case ENEMY_RING:
		enemy[e].angle2 = pos_or_neg (7 + grand (5));
		break;


	}

}



void set_stopper (int e, int t)
{
	int se, i;
// enemy [e].target_time = track[t].target_time;
	enemy[e].persist = 1;

	enemy[e].target = 0;

	enemy[e].target_time = 60 + grand (60);	// may be overridden below
// enemy [e].max_target_time = track[t].target_time;
// enemy [e].target = 0;
	enemy[e].phase = 0;
	enemy[e].phase2 = 0;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = eclass[enemy[e].type].speed1;
	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;

	switch (enemy[e].type)
	{
	case ENEMY_HUNTER:
		enemy[e].target_time = 20 + grand (30);
		break;
	case ENEMY_POD1:
		enemy[e].target_time = 30 + grand (40);
		break;
	case ENEMY_BEAMER:
		enemy[e].target_time = 50 + grand (10);
		enemy[e].angle = ANGLE_4;
		break;
	case ENEMY_SHOOTER:
		enemy[e].target_time = 40 + grand (20);
		enemy[e].angle = ANGLE_4;
		enemy[e].status = 9 + arena.difficulty * 3;	// alse in enemy.c
		break;
	case ENEMY_RISER:
		enemy[e].target_time = 20 + grand (25);
		enemy[e].angle = ANGLE_4 + ANGLE_2;
		break;
	case ENEMY_WINGS:
		enemy[e].target_time = 50 + grand (10);
		enemy[e].petal_angle[0] = ANGLE_4 + ANGLE_16;
		enemy[e].petal_angle[1] = ANGLE_4 - ANGLE_16;
		enemy[e].petal_angle[2] = ANGLE_4 + ANGLE_32;
		enemy[e].petal_angle[3] = ANGLE_4 - ANGLE_32;
		enemy[e].petal_angle[4] = ANGLE_4 + ANGLE_64;
		enemy[e].petal_angle[5] = ANGLE_4 - ANGLE_64;
		break;
	case ENEMY_WINGS2:
		enemy[e].target_time = 50 + grand (10);
		enemy[e].petal_angle[0] = ANGLE_4 + ANGLE_16;
		enemy[e].petal_angle[1] = ANGLE_4 - ANGLE_16;
		enemy[e].petal_angle[2] = ANGLE_4 + ANGLE_32;
		enemy[e].petal_angle[3] = ANGLE_4 - ANGLE_32;
		enemy[e].petal_angle[4] = ANGLE_4 + ANGLE_64;
		enemy[e].petal_angle[5] = ANGLE_4 - ANGLE_64;
		enemy[e].petal_angle[6] = ANGLE_4;
		break;
	case ENEMY_MEGA:
		enemy[e].target_time = 50 + grand (10);
		enemy[e].petal_angle[0] = 0;
		enemy[e].petal_angle[1] = ANGLE_4;
		enemy[e].petal_angle[2] = ANGLE_2;
		enemy[e].petal_angle[3] = -ANGLE_4;
		enemy[e].petal_angle[4] = ANGLE_6;
		enemy[e].petal_angle[5] = ANGLE_6 + ANGLE_3;
		enemy[e].petal_angle[6] = ANGLE_6 + ANGLE_3 + ANGLE_3;
		se = pos_or_neg (7);
		for (i = 0; i < 4; i++)
		{
			enemy[e].petal_angle_speed[i] = se;
		}
		se *= -1.5;
		for (i = 4; i < 7; i++)
		{
			enemy[e].petal_angle_speed[i] = se;
		}
		break;
	case ENEMY_CARRIER1:
		enemy[e].y = -70000;
		enemy[e].x = RAND_X / 2 + X_MAX / 4;
		enemy[e].target_time = 80 + grand (60);
		enemy[e].angle1_turning = 0;
		enemy[e].angle1 = -ANGLE_4;
		se =
		    create_part (e, 0, ENEMY_CARRIER1_PART, enemy[e].x - 55000,
		                 enemy[e].y - 1000);
		if (se != -1)
		{
			enemy[se].persist = 1;
			enemy[se].target = 0;
			enemy[se].target_time = 9000;
			enemy[se].phase = 0;
			enemy[se].phase2 = 0;
			enemy[se].angle1 = ANGLE_4;
			enemy[se].angle = ANGLE_4;
			enemy[se].y_speed = 0;	//enemy[e].y_speed;
		}
		se =
		    create_part (e, 1, ENEMY_CARRIER1_PART, enemy[e].x + 55000,
		                 enemy[e].y - 1000);
		if (se != -1)
		{
			enemy[se].persist = 1;
			enemy[se].target = 0;
			enemy[se].target_time = 9000;
			enemy[se].phase = 0;
			enemy[se].phase2 = 0;
			enemy[se].angle1 = ANGLE_4;
			enemy[se].angle = ANGLE_4;
			enemy[se].y_speed = 0;	//enemy[e].y_speed;
		}
		break;

	}
}

void set_walker (int e, int t)
{

	enemy[e].persist = 1;

	enemy[e].target = 0;

	enemy[e].phase = 0;
	enemy[e].phase2 = 0;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = eclass[enemy[e].type].speed1;
	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;

	enemy[e].recycle = 20;

}



void set_cruiser (int e, int t)
{
// enemy [e].target_time = track[t].target_time;
	enemy[e].persist = 1;

	enemy[e].target = 0;

// enemy [e].target_time = 60 + grand(60); // may be overridden below
// enemy [e].max_target_time = track[t].target_time;
// enemy [e].target = 0;
	enemy[e].phase = 0;
	enemy[e].phase2 = 0;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = eclass[enemy[e].type].speed1;
	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;

	switch (enemy[e].type)
	{
	case ENEMY_CRUISER1:
	case ENEMY_CRUISER2:
	case ENEMY_CRUISER3:
		enemy[e].recycle = 120;
		enemy[e].status = 90;
		break;
	case ENEMY_CRUISER4:
		enemy[e].recycle = 120;
		enemy[e].status = 0;
		break;
	case ENEMY_DROPPER:
		enemy[e].x = RAND_X;
		enemy[e].y = -20000 - grand (10000);
		enemy[e].angle2 = pos_or_neg (4 + grand (4));
		break;
	case ENEMY_SPINNER:
		enemy[e].angle2 = pos_or_neg (2 + grand (2));
		break;

	}

	if (arena.level == 2)
	{

		if (arena.cruiser_escorts == 2)
		{
			give_escort (e, ENEMY_ESCORT, -40000, 30000, enemy[e].x, enemy[e].y);
			give_escort (e, ENEMY_ESCORT, 40000, 30000, enemy[e].x, enemy[e].y);
		}
		if (arena.cruiser_escorts == 3)
		{
			give_escort (e, ENEMY_ESCORT, -50000, 40000, enemy[e].x, enemy[e].y);
			give_escort (e, ENEMY_ESCORT, 50000, 40000, enemy[e].x, enemy[e].y);
			give_escort (e, ENEMY_ESCORT, 0, -60000, enemy[e].x, enemy[e].y);
		}
		if (arena.cruiser_escorts == 1)
		{
			give_escort (e, ENEMY_ESCORT, 0, 0, enemy[e].x, enemy[e].y);
		}
	}


}


int create_part (int pe, int index, int type, int x, int y)
{
	int e = create_enemy (type);
	if (e == -1)
		return -1;

	enemy[e].parent = pe;
	enemy[e].part_index = index;
	enemy[pe].part[index] = e;
	enemy[e].x = x;
	enemy[e].y = y;
	enemy[e].x_speed = 0;
	enemy[e].y_speed = 0;

	return e;
}


void set_diver (int e, int t)
{

	enemy[e].persist = 1;

	enemy[e].target = 0;

	enemy[e].target_time = 65;	// + grand(10);
// enemy [e].max_target_time = track[t].target_time;
// enemy [e].target = 0;
	enemy[e].phase = 0;
	enemy[e].phase2 = 0;

	enemy[e].x_speed = 0;
	if (enemy[e].x < 180000 + grand (280000))
		enemy[e].x_speed = 900 + grand (200);
	else
		enemy[e].x_speed = -900 - grand (200);
	enemy[e].y_speed = 6000;
	enemy[e].angle1 = ANGLE_4;
	enemy[e].angle = ANGLE_4;
}





void set_glider (int e, int t)
{
// int i;
	enemy[e].target = 0;
	enemy[e].x_target[0] = track[t].x_target[0];
	enemy[e].y_target[0] = track[t].y_target[0];

	enemy[e].angle1 =
	    radians_to_angle (atan2
	                      (enemy[e].y_target[0] - enemy[e].y,
	                       enemy[e].x_target[0] - enemy[e].x));

	int speed = 5000;		//eclass[enemy [e].type].speed1;

	/* if (enemy[e].type >= ENEMY_FLOWER1 && enemy[e].type <= ENEMY_FLOWER5)
	  speed += grand(1000);*/

	enemy[e].x_speed = xpart (enemy[e].angle1, speed);
	enemy[e].y_speed = ypart (enemy[e].angle1, speed);

}

void set_attacker (int e, int t)
{
// int i;

	enemy[e].angle1 = ANGLE_4;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = 3000;
	enemy[e].target_time = grand (30) + 10;
//   track[t].target_time = grand(30) + 30;

}

void set_pauser (int e, int t)
{

	enemy[e].angle1 = ANGLE_4;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = 3000;
	enemy[e].target_time = 30;	//grand(30) + 10;

}


void set_beamer (int e, int t)
{

	enemy[e].angle1 = ANGLE_4;

	enemy[e].x_speed = 0;
	enemy[e].y_speed = 2000;
	enemy[e].target_time = grand (30) + 50;
	enemy[e].target = 0;

}


void place_marchers (int t)
{


	/* int i, e;

	 int x = X_MIN + grand(40000);
	 int y = -30000;

	 for (i = 0; i < track[t].multiple; i ++)
	// for (i = 0; i < 2; i ++)
	 {

	//  e = create_enemy(track[t].type, 0, x + i * 40000, y, ANGLE_4, 0, 2200 + grand(500), ATT3, ATT4, ATT5, ATT6, ATT7);

	 }*/

}

/*

void make_mb1_track(void)
{

 boss.side1 = grand(2);
 if (boss.side1 == 0)
  boss.side2 = 1;
   else
    boss.side2 = 0;

 int e1 = create_enemy(boss.side1, ENEMY_MB1, 0, 160000, -80000, ANGLE_4, 0, 0, ATT3, ATT4, ATT5, ATT6, ATT7);
// I guess I should check these values, but I don't think it's actually possible for these functions to fail here.
 boss.e1 = e1;

  boss.hp = 60000 + arena.difficulty * 6000;
  boss.max_hp = boss.hp;
  boss.moving = 0;
  boss.angle_1 = 0;
  boss.angle_2 = 0;
  boss.angle_3 = 0;
  boss.bflower_recycle = 100;
  boss.trecycle = 250;
  boss.bpattern = 0;
  boss.bpattern2 = 450;
  boss.btime = 200;
  boss.brecycle = 150;

 boss.fight = 1;

 enemy[boss.side1][e1].y_speed = 2000;
 enemy[boss.side1][e1].x_speed = 0;


}


*/
