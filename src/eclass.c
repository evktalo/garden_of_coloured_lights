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


struct eclassstruct eclass[NO_ENEMY_TYPES] = {
	{
		0,				// int max_hp
		0,				// int xsize
		0,				// int ysize
		0,				// int xoffset
		0,				// int yoffset
		0,				// ai
		0,				// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0},				// NONE
	{
		50,				// int max_hp
		15000,			// int xsize
		15000,			// int ysize
		0,
		0,
		AI_SWOOPER,
		4000,			// speed1
		10,				// speed2
		0,				// speed3
		2,				// priority
		0,				// score
		10				// xp
	},				// SWOOPER1
	{
		300,				// int max_hp
		25000,			// int xsize
		25000,			// int ysize
		0,
		4000,
		AI_DARTER,
		6000,			// speed1
		10,				// speed2
		0,				// speed3
		1,				// priority
		0,
		20				// xp
	},				// DARTER1
	{
		3800,			// int max_hp
		45000,			// int xsize
		75000,			// int ysize
		0,
		-30000,
		AI_STOPPER,
		2000,			// speed1
		150,				// speed2
		60,				// speed3
		0,				// priority
		0,
		80				// xp
	},				// STOPPER1
	{
		200,				// int max_hp
		25000,			// int xsize
		21000,			// int ysize
		0,
		-7000,
		AI_DIVER,
		6000,			// speed1
		10,				// speed2
		0,				// speed3
		1,				// priority
		0,
		20				// xp
	},				// DIVER1
	{
		7000,			// int max_hp
		68000,			// int xsize
		80000,			// int ysize
		0,
		-5000,
		AI_STOPPER,
		1700,			// speed1
		520,				// speed2
		40,				// speed3
		0,				// priority
		0,
		100				// xp
	},				// CARRIER1
	{
		800,				// int max_hp
		16000,			// int xsize
		16000,			// int ysize
		0,
		3000,
		AI_DARTER,
		8000,			// speed1
		10,				// speed2
		0,				// speed3
		0,				// priority
		0,
		30				// xp
	},				// CARRIER1_PART
	{
		4000,			// int max_hp
		57000,			// int xsize
		52000,			// int ysize
		0,
		-13000,
		AI_STOPPER,
		3500,			// speed1
		580,				// speed2
		100,				// speed3
		0,				// priority
		0,
		50				// xp
	},				// POD1
	{
		50,				// int max_hp
		20000,			// int xsize
		20000,			// int ysize
		0,
		0,
		AI_SWOOPER,
		6000,			// speed1
		14,				// speed2
		0,				// speed3
		2,				// priority
		0,				// score
		15				// xp
	},				// SWOOPER2
	{
		400,				// int max_hp
		30000,			// int xsize
		28000,			// int ysize
		0,
		0,
		AI_DARTER,
		8000,			// speed1
		15,				// speed2
		0,				// speed3
		1,				// priority
		0,
		30				// xp
	},				// DARTER2

	{
		5000,			// int max_hp
		110000,			// int xsize
		75000,			// int ysize
		0,
		0,
		AI_BOSS,
		0,				// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		300				// xp
	},				// BOSS1

	{
		2500,			// int max_hp
		20000,			// int xsize
		20000,			// int ysize
		0,
		0,
		AI_BOSS,
		0,				// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		0				// xp
	},				// BOSS1_PART


	{
		50,				// int max_hp
		22000,			// int xsize
		16000,			// int ysize
		0,
		0,
		AI_DRIFTER,
		170,				// speed1 - move
		960,				// speed2 - drag
		600,				// speed3 - before retreats
		2,				// priority
		0,
		10				// xp
	},				// L2 - DRIFTER1

	{
		1500,			// int max_hp
		38000,			// int xsize
		25000,			// int ysize
		0,
		-4000,
		AI_STOPPER,
		-6000,			// speed1
		410,				// speed2
		20,				// speed3
		1,				// priority
		0,
		50				// xp
	},				// L2 - BEAMER

	{
		80,				// int max_hp
		21000,			// int xsize
		20000,			// int ysize
		0,
		5000,
		AI_STOPPER,
		-3000,			// speed1
		260,				// speed2
		20,				// speed3
		2,				// priority
		0,
		20				// xp
	},				// L2 - RISER

	{
		4800,			// int max_hp
		50000,			// int xsize
		62500,			// int ysize
		0,
		0,
		AI_CRUISER,
		1200,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		80				// xp
	},				// L2 - CRUISER1

	{
		4000,			// int max_hp
		50000,			// int xsize
		32500,			// int ysize
		0,
		0,
		AI_CRUISER,
		1200,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		50				// xp
	},				// L2 - CRUISER2

	{
		6000,			// int max_hp
		50000,			// int xsize
		62500,			// int ysize
		0,
		0,
		AI_CRUISER,
		1200,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		100				// xp
	},				// L2 - CRUISER3

	{
		4500,			// int max_hp
		45000,			// int xsize
		42500,			// int ysize
		0,
		-5000,
		AI_CRUISER,
		1200,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		80				// xp
	},				// L2 - CRUISER4

	{
		3500,			// int max_hp
		50000,			// int xsize
		62500,			// int ysize
		0,
		0,
		AI_CRUISER,
		1200,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		150				// xp
	},				// L2 - CRUISER5 - not implemented

	{
		250,				// int max_hp
		25000,			// int xsize
		25000,			// int ysize
		0,
		8000,
		AI_DRIFTER,
		130,				// speed1 - move
		970,				// speed2 - drag
		700,				// speed3 - before retreats
		1,				// priority
		0,
		20				// xp
	},				// L2 - DRIFTER2

	{
		50,				// int max_hp
		23000,			// int xsize
		16000,			// int ysize
		0,
		0,
		AI_DRIFTER,
		290,				// speed1 - move
		930,				// speed2 - drag
		700,				// speed3 - before retreats
		2,				// priority
		0,
		10				// xp
	},				// L2 - ESCORT


	{
		1500,			// int max_hp
		38000,			// int xsize
		25000,			// int ysize
		0,
		-4000,
		AI_STOPPER,
		-6000,			// speed1
		210,				// speed2
		20,				// speed3
		1,				// priority
		0,
		50				// xp
	},				// L2 - SHOOTER

	{
		5000,			// int max_hp
		105000,			// int xsize
		55000,			// int ysize
		0,
		5000,
		AI_BOSS,
		0,				// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		400				// xp
	},				// BOSS2




	{
		50,				// int max_hp
		15000,			// int xsize
		15000,			// int ysize
		0,
		0,
		AI_SWOOPER,
		4500,			// speed1
		10,				// speed2
		0,				// speed3
		2,				// priority
		0,				// score
		10				// xp
	},				// L3 - SMALL

	{
		500,				// int max_hp
		19000,			// int xsize
		19000,			// int ysize
		0,
		-4000,
		AI_DART_STOPPER,
		400,				// speed1
		15,				// speed2
		150,				// speed3
		1,				// priority
		0,
		25				// xp
	},				// L3 - LARGER

	{
		50,				// int max_hp
		15000,			// int xsize
		15000,			// int ysize
		0,
		0,
		AI_SWOOPER,
		5500,			// speed1
		14,				// speed2
		0,				// speed3
		2,				// priority
		0,				// score
		15				// xp
	},				// L3 - SMALL2

	{
		700,				// int max_hp
		19000,			// int xsize
		19000,			// int ysize
		0,
		-4000,
		AI_DART_STOPPER,
		500,				// speed1
		18,				// speed2
		250,				// speed3
		1,				// priority
		0,
		35				// xp
	},				// L3 - LARGER2


	{
		2900,			// int max_hp
		22000,			// int xsize
		21000,			// int ysize
		0,
		-4000,
		AI_STOPPER,
		-6000,			// speed1
		160,				// speed2
		40,				// speed3
		0,				// priority
		0,
		70				// xp
	},				// L3 - WINGS

	{
		3300,			// int max_hp
		22000,			// int xsize
		21000,			// int ysize
		0,
		-4000,
		AI_STOPPER,
		-7000,			// speed1
		160,				// speed2
		40,				// speed3
		0,				// priority
		0,
		80				// xp
	},				// L3 - WINGS2

	{
		9000,			// int max_hp
		40000,			// int xsize
		38000,			// int ysize
		0,
		-6000,
		AI_STOPPER,
		4000,			// speed1
		540,				// speed2
		40,				// speed3
		0,				// priority
		0,
		120				// xp
	},				// L3 - MEGA

	{
		5000,			// int max_hp
		40000,			// int xsize
		38000,			// int ysize
		0,
		0,
		AI_BOSS,
		0,				// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		500				// xp
	},				// BOSS3

	{
		80,				// int max_hp
		16000,			// int xsize
		17000,			// int ysize
		0,
		0,
		AI_HOOK,
		4500,			// speed1
		15,				// speed2
		0,				// speed3
		2,				// priority
		0,				// score
		10				// xp
	},				// HOOK
	{
		5800,			// int max_hp
		76000,			// int xsize
		61000,			// int ysize
		0,
		-5000,
		AI_WALK,
		1500,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		90				// xp
	},				// WALKER

	{
		250,				// int max_hp
		14000,			// int xsize
		21000,			// int ysize
		0,
		0,
		AI_CRUISER,
		1500,			// speed1
		0,				// speed2
		0,				// speed3
		1,				// priority
		0,
		20				// xp
	},				// DROPPER

	{
		450,				// int max_hp
		24000,			// int xsize
		24000,			// int ysize
		0,
		0,
		AI_DRIFTER,
		130,				// speed1 - move
		930,				// speed2 - drag
		600,				// speed3 - before retreats
		1,				// priority
		0,
		30				// xp
	},				// RING

	{
		1800,			// int max_hp
		28000,			// int xsize
		25000,			// int ysize
		0,
		0,
		AI_STOPPER,
		4000,			// speed1
		150,				// speed2
		60,				// speed3
		1,				// priority
		0,
		50				// xp
	},				// HUNTER

	{
		7250,			// int max_hp
		52000,			// int xsize
		84000,			// int ysize
		0,
		0,
		AI_CRUISER,
		1500,			// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		100				// xp
	},				// SPINNER
	{
		120,				// int max_hp
		17000,			// int xsize
		17000,			// int ysize
		0,
		0,
		AI_HOOK,
		4200,			// speed1
		18,				// speed2
		0,				// speed3
		2,				// priority
		0,				// score
		15				// xp
	},				// HOOK2


	{
		5000,			// int max_hp
		114000,			// int xsize
		100000,			// int ysize
		0,
		0,
		AI_BOSS,
		0,				// speed1
		0,				// speed2
		0,				// speed3
		0,				// priority
		0,
		600				// xp
	},				// BOSS4


};
