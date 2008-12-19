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

void run_display (int show);

void run_underlay (void);

void display_pause (int pc);


struct RLE_STRUCT
{
	RLE_SPRITE *sprite;
	int x;
	int y;
};



#define PLAT_Y 300

#define ISLAND1_Y 300

#define PLATFORM_SIZE 92
#define ISLAND1_SIZE 92
// includes one border pixel - only used in loading

enum
{
	FILE_PLATFORM_LR,
	FILE_PLATFORM_UD,
//FILE_PLATFORM_LR_TLR,
//FILE_PLATFORM_UD_TUD,
//FILE_PLATFORM_LR_TL,
//FILE_PLATFORM_UD_TU,
	FILE_PLATFORM_L,
	FILE_PLATFORM_U,
	FILE_PLATFORM_UL,
	FILE_PLATFORM_UDL,
	FILE_PLATFORM_ULR,
	FILE_PLATFORM_NODE,
	FILE_PLATFORM_NODE2,
	FILE_PLATFORM_NODE3,
	FILE_PLATFORM_RLES
};

enum
{
	PLATFORM_EMPTY = -1,
	PLATFORM_LR,
	PLATFORM_UD,
	PLATFORM_UL,
	PLATFORM_DL,
	PLATFORM_UR,
	PLATFORM_DR,

//PLATFORM_LR_TLR, // terminates L/R
//PLATFORM_LR_TL,
//PLATFORM_LR_TR,
//PLATFORM_UD_TUD,
//PLATFORM_UD_TU,
//PLATFORM_UD_TD,

	PLATFORM_UDL,
	PLATFORM_UDR,
	PLATFORM_ULR,
	PLATFORM_DLR,
	PLATFORM_L,
	PLATFORM_R,
	PLATFORM_U,
	PLATFORM_D,
	PLATFORM_NODE,
	PLATFORM_NODE2,
	PLATFORM_NODE3,
	/*PLATFORM1_,
	PLATFORM1_,
	PLATFORM1_,
	PLATFORM1_,*/
	PLATFORM_RLES
};


enum
{
	FILE_ISLAND1_D,
	FILE_ISLAND1_R,
	FILE_ISLAND1_DR,
	FILE_ISLAND1_NOT_UL,
	FILE_ISLAND1_UL_DR,
	FILE_ISLAND1_FULL,
	FILE_ISLAND1_FULL2,
	FILE_ISLAND1_FULL3,
	FILE_ISLAND1_FULL4,
	FILE_ISLAND1_FULL5,
	FILE_ISLAND1_RLES
};


enum
{
	ISLAND1_EMPTY = -1,
	ISLAND1_D,
	ISLAND1_U,

	ISLAND1_L,
	ISLAND1_R,

	ISLAND1_DR,
	ISLAND1_DL,
	ISLAND1_UR,
	ISLAND1_UL,

	ISLAND1_NOT_UL,
	ISLAND1_NOT_UR,
	ISLAND1_NOT_DL,
	ISLAND1_NOT_DR,

	ISLAND1_UL_DR,
	ISLAND1_UR_DL,

	ISLAND1_FULL,
	ISLAND1_FULL2,
	ISLAND1_FULL3,
	ISLAND1_FULL4,
	ISLAND1_FULL5,

	ISLAND1_D2,
	ISLAND1_U2,

	ISLAND1_L2,
	ISLAND1_R2,

	ISLAND1_DR2,
	ISLAND1_DL2,
	ISLAND1_UR2,
	ISLAND1_UL2,

	ISLAND1_NOT_UL2,
	ISLAND1_NOT_UR2,
	ISLAND1_NOT_DL2,
	ISLAND1_NOT_DR2,

	ISLAND1_UL_DR2,
	ISLAND1_UR_DL2,

	ISLAND1_D3,
	ISLAND1_U3,

	ISLAND1_L3,
	ISLAND1_R3,

	ISLAND1_DR3,
	ISLAND1_DL3,
	ISLAND1_UR3,
	ISLAND1_UL3,

	ISLAND1_NOT_UL3,
	ISLAND1_NOT_UR3,
	ISLAND1_NOT_DL3,
	ISLAND1_NOT_DR3,

	ISLAND1_UL_DR3,
	ISLAND1_UR_DL3,

	ISLAND1_RLES
};

enum
{
	BONE_HOOK1,
	BONE_HOOK2,
	BONE_HOOK3,
	BONE_HOOK4,
	BONE_HOOK5,
	BONE_HOOK2F,
	BONE_HOOK3F,
	BONE_HOOK4F,
	BONE_HOOK5F,
	BONE_B_HOOK1,
	BONE_B_HOOK2,
	BONE_B_HOOK3,
	BONE_B_HOOK4,
	BONE_B_HOOK5,
	BONE_B_HOOK2F,
	BONE_B_HOOK3F,
	BONE_B_HOOK4F,
	BONE_B_HOOK5F,
	BONE_DROP1,
	BONE_DROP2,
	BONE_DROP3,
	BONE_DROP4,
	BONE_DROP2F,
	BONE_DROP3F,
	BONE_DROP4F,
	BONE_B_DROP1,
	BONE_B_DROP2,
	BONE_B_DROP3,
	BONE_B_DROP4,
	BONE_B_DROP2F,
	BONE_B_DROP3F,
	BONE_B_DROP4F,
	BONE_RING1,
	BONE_RING2,
	BONE_RING3,
	BONE_RING4,
	BONE_RING5,
	BONE_RING2FH,
	BONE_RING3FH,
	BONE_RING4FH,
	BONE_RING5FH,
	BONE_RING2FV,
	BONE_RING3FV,
	BONE_RING4FV,
	BONE_RING5FV,
	BONE_RING2FVH,
	BONE_RING3FVH,
	BONE_RING4FVH,
	BONE_RING5FVH,

	BONE_B_RING1,
	BONE_B_RING2,
	BONE_B_RING3,
	BONE_B_RING4,
	BONE_B_RING5,
	BONE_B_RING2FH,
	BONE_B_RING3FH,
	BONE_B_RING4FH,
	BONE_B_RING5FH,
	BONE_B_RING2FV,
	BONE_B_RING3FV,
	BONE_B_RING4FV,
	BONE_B_RING5FV,
	BONE_B_RING2FVH,
	BONE_B_RING3FVH,
	BONE_B_RING4FVH,
	BONE_B_RING5FVH,

	BONE_HUNTER1,
	BONE_HUNTER2,
	BONE_HUNTER3,
	BONE_HUNTER4,
	BONE_HUNTER5,
	BONE_HUNTER2F,
	BONE_HUNTER3F,
	BONE_HUNTER4F,
	BONE_HUNTER5F,

	BONE_B_HUNTER1,
	BONE_B_HUNTER2,
	BONE_B_HUNTER3,
	BONE_B_HUNTER4,
	BONE_B_HUNTER5,
	BONE_B_HUNTER2F,
	BONE_B_HUNTER3F,
	BONE_B_HUNTER4F,
	BONE_B_HUNTER5F,

	BONE_B_SPINNER,
	BONE_B_WALKER1,
	BONE_B_WALKER2,
	BONE_B_WALKER2F,

	BONE_RLES
};

enum
{
	S_ENEMY_SWOOPER1_1,
	S_ENEMY_SWOOPER1_2,
	S_ENEMY_SWOOPER1_3,
	S_ENEMY_SWOOPER1_4,
	S_ENEMY_DARTER1_MAIN,
	S_ENEMY_DARTER1_TOP,
	S_ENEMY_DARTER1_CENTRE,
	S_ENEMY_DARTER1_INNER_L,
	S_ENEMY_DARTER1_OUTER_L,
	S_ENEMY_DARTER1_INNER_R,
	S_ENEMY_DARTER1_OUTER_R,
	S_ENEMY_PLAYER,
	S_ENEMY_PLAYER1,
	S_ENEMY_PLAYER2,
	S_ENEMY_PLAYER3,
	S_ENEMY_PLAYER_UP_L,
	S_ENEMY_PLAYER_UP_R,
	S_ENEMY_PLAYER_DOWN_L,
	S_ENEMY_PLAYER_DOWN_R,
	S_ENEMY_DIVER1_CENTRE,
	S_ENEMY_DIVER1_INNER_L,
	S_ENEMY_DIVER1_INNER_R,
	S_ENEMY_DIVER1_OUTER_L,
	S_ENEMY_DIVER1_OUTER_R,
	S_ENEMY_C1P1_CORE,
	S_ENEMY_C1P1_TOP,
	S_ENEMY_C1P1_BOTTOM1,
	S_ENEMY_C1P1_BOTTOM2,
	S_ENEMY_C1P1_MIDL,
	S_ENEMY_C1P1_MIDR,
	S_ENEMY_SWOOPER2_1,
	S_ENEMY_SWOOPER2_2,
	S_ENEMY_SWOOPER2_3,
	S_ENEMY_SWOOPER2_4,
	S_ENEMY_DARTER2_TOP,
	S_ENEMY_DARTER2_BOTTOM,
	S_ENEMY_DARTER2_CORE,
	S_ENEMY_DARTER2_INNER_L,
	S_ENEMY_DARTER2_OUTER_L,
	S_ENEMY_DARTER2_INNER_R,
	S_ENEMY_DARTER2_OUTER_R,
	S_ENEMY_SMALL_OUTER_L,
	S_ENEMY_SMALL_OUTER_R,
	S_ENEMY_SMALL_CORE1,
	S_ENEMY_SMALL_CORE2,
	S_ENEMY_LARGER_OUTER_L,
	S_ENEMY_LARGER_OUTER_R,
	S_ENEMY_LARGER_CORE1,
	S_ENEMY_LARGER_CORE2,
	S_ENEMY_WINGS_OUTER_L,
	S_ENEMY_WINGS_OUTER_R,
	S_ENEMY_WINGS_CORE1,
	S_ENEMY_WINGS_CORE2,
	S_ENEMY_MEGA_OUTER_L,
	S_ENEMY_MEGA_OUTER_R,
	S_ENEMY_MEGA_CORE,
	S_ENEMY_BOSS3_CORE,
	S_ENEMY_DRIFTER1_CORE,
	S_ENEMY_DRIFTER1_L,
	S_ENEMY_DRIFTER1_R,
	S_ENEMY_BEAMER_CORE,
	S_ENEMY_BEAMER_L,
	S_ENEMY_BEAMER_R,
	S_ENEMY_BEAMER_U,
	S_ENEMY_BEAMER_D,
	S_ENEMY_RISER1_CORE,
	S_ENEMY_RISER1_L,
	S_ENEMY_RISER1_R,
	S_ENEMY_DRIFTER2_CORE,
	S_ENEMY_DRIFTER2_L,
	S_ENEMY_DRIFTER2_R,
	S_ENEMY_ESCORT_CORE,
	S_ENEMY_ESCORT_L,
	S_ENEMY_ESCORT_R,
	S_ENEMY_SHOOTER_L,
	S_ENEMY_SHOOTER_R,
	S_ENEMY_SHOOTER_U,
	S_ENEMY_HOOK1,
	S_ENEMY_HOOK2,
	S_ENEMY_HOOK3,
	S_ENEMY_DROPPER1,
	S_ENEMY_DROPPER2,
	S_ENEMY_DROPPER3,
	S_ENEMY_RING1,
	S_ENEMY_RING2,
	S_ENEMY_RING3,
	S_ENEMY_HUNTER1,
	S_ENEMY_HUNTER2,
	S_ENEMY_HUNTER3,
	S_ENEMY_HOOK2_1,
	S_ENEMY_HOOK2_2,
	S_ENEMY_HOOK2_3,
	S_ENEMY_RLES
};

enum
{
	L_ENEMY_STOPPER1,
	L_ENEMY_STOPPER1_FRONT,
	L_ENEMY_STOPPER1_CENTRE,
	L_ENEMY_STOPPER1_F_L,
	L_ENEMY_STOPPER1_F_R,
	L_ENEMY_STOPPER1_M_L,
	L_ENEMY_STOPPER1_M_R,
	L_ENEMY_STOPPER1_B_L,
	L_ENEMY_STOPPER1_B_R,
	L_ENEMY_CARRIER1,
	L_ENEMY_CARRIER1_CORE,
	L_ENEMY_CARRIER1_TOP1,
	L_ENEMY_CARRIER1_TOP2,
	L_ENEMY_CARRIER1_TOP3,
	L_ENEMY_CARRIER1_MIDL1,
	L_ENEMY_CARRIER1_MIDL2,
	L_ENEMY_CARRIER1_MIDR1,
	L_ENEMY_CARRIER1_MIDR2,
	L_ENEMY_CARRIER1_BOTTOM1,
	L_ENEMY_CARRIER1_BOTTOM2,
	L_ENEMY_POD1_CORE,
	L_ENEMY_POD1_BODYL,
	L_ENEMY_POD1_BODYR,
	L_ENEMY_POD1_TOPC,
	L_ENEMY_POD1_TOPL,
	L_ENEMY_POD1_TOPR,
	L_ENEMY_CRUISER1,
	L_ENEMY_CRUISER2,
	L_ENEMY_CRUISER3,
	L_ENEMY_CRUISER4,
	L_ENEMY_CRUISER5,

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
	L_ENEMY_BOSS1_OUT_R,

	L_ENEMY_BOSS2_CORE,
	L_ENEMY_BOSS2_IN_L,
	L_ENEMY_BOSS2_IN_R,
	L_ENEMY_BOSS2_OUT_L,
	L_ENEMY_BOSS2_OUT_R,

	L_ENEMY_WALKER1,
	L_ENEMY_WALKER2,
	L_ENEMY_WALKER3,
	L_ENEMY_SPINNER1,
	L_ENEMY_SPINNER2,
	L_ENEMY_SPINNER3,

	L_ENEMY_BOSS4,

	L_ENEMY_RLES
};

enum
{
	RLE_TRANS_MGBULLET1,
	RLE_TRANS_MGBULLET2,
	RLE_TRANS_MGBULLET3,
	RLE_TRANS_MGBULLET4,
	TRANS_RLES
};
