/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
==============================================================================

SOLDIER

==============================================================================
*/

#include "q_shared.h"
#include "m_soldier.h"


static soundindex_t	sound_idle;
static soundindex_t	sound_pain_light;
static soundindex_t	sound_pain;
static soundindex_t	sound_pain_ss;
static soundindex_t	sound_death_light;
static soundindex_t	sound_death;
static soundindex_t	sound_death_ss;
static soundindex_t	sound_cock;

void soldier_idle (edict_t *self)
{
	if (M_FidgetCheck(self, 20))
		gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

void soldier_cock (edict_t *self)
{
	if (self->s.frame == FRAME_stand322)
		gi.sound (self, CHAN_WEAPON, sound_cock, 1, ATTN_IDLE, 0);
	else
		gi.sound (self, CHAN_WEAPON, sound_cock, 1, ATTN_NORM, 0);
}


// STAND

void soldier_stand (edict_t *self);

mframe_t soldier_frames_stand1 [] =
{
	ai_stand, 0, soldier_idle,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t soldier_move_stand1 = {FRAME_stand101, FRAME_stand130, soldier_frames_stand1, soldier_stand};

mframe_t soldier_frames_stand3 [] =
{
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, soldier_cock,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t soldier_move_stand3 = {FRAME_stand301, FRAME_stand339, soldier_frames_stand3, soldier_stand};

#if 0
mframe_t soldier_frames_stand4 [] =
{
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 4, nullptr,
	ai_stand, 1, nullptr,
	ai_stand, -1, nullptr,
	ai_stand, -2, nullptr,

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t soldier_move_stand4 = {FRAME_stand401, FRAME_stand452, soldier_frames_stand4, nullptr};
#endif

void soldier_stand (edict_t *self)
{
	if ((self->monsterinfo.currentmove == &soldier_move_stand3) || (prandom(80)))
		self->monsterinfo.currentmove = &soldier_move_stand1;
	else
		self->monsterinfo.currentmove = &soldier_move_stand3;
}


//
// WALK
//

void soldier_walk1_random (edict_t *self)
{
	if (prandom(90))
		self->monsterinfo.nextframe = FRAME_walk101;
}

mframe_t soldier_frames_walk1 [] =
{
	ai_walk, 3,  nullptr,
	ai_walk, 6,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 1,  nullptr,
	ai_walk, 6,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 3,  nullptr,
	ai_walk, -1, soldier_walk1_random,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr
};
mmove_t soldier_move_walk1 = {FRAME_walk101, FRAME_walk133, soldier_frames_walk1, nullptr};

mframe_t soldier_frames_walk2 [] =
{
	ai_walk, 4,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 9,  nullptr,
	ai_walk, 8,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 1,  nullptr,
	ai_walk, 3,  nullptr,
	ai_walk, 7,  nullptr,
	ai_walk, 6,  nullptr,
	ai_walk, 7,  nullptr
};
mmove_t soldier_move_walk2 = {FRAME_walk209, FRAME_walk218, soldier_frames_walk2, nullptr};

void soldier_walk (edict_t *self)
{
	if (prandom(50))
		self->monsterinfo.currentmove = &soldier_move_walk1;
	else
		self->monsterinfo.currentmove = &soldier_move_walk2;
}


//
// RUN
//

void soldier_run (edict_t *self);

mframe_t soldier_frames_start_run [] =
{
	ai_run, 7,  nullptr,
	ai_run, 5,  nullptr
};
mmove_t soldier_move_start_run = {FRAME_run01, FRAME_run02, soldier_frames_start_run, soldier_run};

mframe_t soldier_frames_run [] =
{
	ai_run, 10, nullptr,
	ai_run, 11, nullptr,
	ai_run, 11, nullptr,
	ai_run, 16, nullptr,
	ai_run, 10, nullptr,
	ai_run, 15, nullptr
};
mmove_t soldier_move_run = {FRAME_run03, FRAME_run08, soldier_frames_run, nullptr};

void soldier_run (edict_t *self)
{
	if (self->monsterinfo.currentmove == &soldier_move_walk1 ||
		self->monsterinfo.currentmove == &soldier_move_walk2 ||
		self->monsterinfo.currentmove == &soldier_move_start_run)
	{
		self->monsterinfo.currentmove = &soldier_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &soldier_move_start_run;
	}
}


//
// PAIN
//

void soldier_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	int32_t		n;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	n = self->s.skinnum | 1;
	if (n == 1)
		gi.sound (self, CHAN_VOICE, sound_pain_light, 1, ATTN_NORM, 0);
	else if (n == 3)
		gi.sound (self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain_ss, 1, ATTN_NORM, 0);
}

//
// DEATH
//

void soldier_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t soldier_frames_death2 [] =
{
	ai_move, -5,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr
};
mmove_t soldier_move_death2 = {FRAME_death201, FRAME_death235, soldier_frames_death2, soldier_dead};

mframe_t soldier_frames_death3 [] =
{
	ai_move, -5,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
};
mmove_t soldier_move_death3 = {FRAME_death301, FRAME_death345, soldier_frames_death3, soldier_dead};

mframe_t soldier_frames_death4 [] =
{
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr
};
mmove_t soldier_move_death4 = {FRAME_death401, FRAME_death453, soldier_frames_death4, soldier_dead};

mframe_t soldier_frames_death5 [] =
{
	ai_move, -5,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,

	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr
};
mmove_t soldier_move_death5 = {FRAME_death501, FRAME_death524, soldier_frames_death5, soldier_dead};

mframe_t soldier_frames_death6 [] =
{
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr
};
mmove_t soldier_move_death6 = {FRAME_death601, FRAME_death610, soldier_frames_death6, soldier_dead};

void soldier_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

// check for gib
	if (self->health <= self->gib_health)
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 3; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowGib (self, "models/objects/gibs/chest/tris.md2", damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

// regular death
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	self->s.skinnum |= 1;

	if (self->s.skinnum == 1)
		gi.sound (self, CHAN_VOICE, sound_death_light, 1, ATTN_NORM, 0);
	else if (self->s.skinnum == 3)
		gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	else // (self->s.skinnum == 5)
		gi.sound (self, CHAN_VOICE, sound_death_ss, 1, ATTN_NORM, 0);

	if (fabs((self->s.origin[2] + self->viewheight) - point[2]) <= 4)
	{
		// head shot
		self->monsterinfo.currentmove = &soldier_move_death3;
		return;
	}

	n = irandom(3);
	if (n == 0)
		self->monsterinfo.currentmove = &soldier_move_death2;
	else if (n == 1)
		self->monsterinfo.currentmove = &soldier_move_death4;
	else if (n == 2)
		self->monsterinfo.currentmove = &soldier_move_death5;
	else
		self->monsterinfo.currentmove = &soldier_move_death6;
}


//
// SPAWN
//

void SP_monster_soldier_x (edict_t *self)
{

	self->s.modelindex = gi.modelindex ("models/monsters/soldier/tris.md2");
	self->monsterinfo.scale = MODEL_SCALE;
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 32);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	sound_idle =	gi.soundindex ("soldier/solidle1.wav");
	sound_cock =	gi.soundindex ("infantry/infatck3.wav");

	self->mass = 100;

	self->pain = soldier_pain;
	self->die = soldier_die;

	self->monsterinfo.stand = soldier_stand;
	self->monsterinfo.walk = soldier_walk;
	self->monsterinfo.run = soldier_run;

	gi.linkentity (self);

	self->monsterinfo.stand (self);

	walkmonster_start (self);
}


/*QUAKED monster_soldier_light (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_soldier_light (edict_t *self)
{
	sound_pain_light = gi.soundindex ("soldier/solpain2.wav");
	sound_death_light =	gi.soundindex ("soldier/soldeth2.wav");
	gi.soundindex ("misc/lasfly.wav");

	self->s.skinnum = 0;
	self->health = 20;
	self->gib_health = -30;
	self->monsterinfo.undamaged_skin = 0;
	self->monsterinfo.damaged_skin = 1;

	SP_monster_soldier_x (self);
}

/*QUAKED monster_soldier (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_soldier (edict_t *self)
{
	sound_pain = gi.soundindex ("soldier/solpain1.wav");
	sound_death = gi.soundindex ("soldier/soldeth1.wav");

	self->s.skinnum = 2;
	self->health = 30;
	self->gib_health = -30;
	self->monsterinfo.undamaged_skin = 2;
	self->monsterinfo.damaged_skin = 3;

	SP_monster_soldier_x (self);
}

/*QUAKED monster_soldier_ss (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_soldier_ss (edict_t *self)
{
	sound_pain_ss = gi.soundindex ("soldier/solpain3.wav");
	sound_death_ss = gi.soundindex ("soldier/soldeth3.wav");

	self->s.skinnum = 4;
	self->health = 40;
	self->gib_health = -30;
	self->monsterinfo.undamaged_skin = 4;
	self->monsterinfo.damaged_skin = 5;

	SP_monster_soldier_x (self);
}
