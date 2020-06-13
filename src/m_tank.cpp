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

TANK

==============================================================================
*/

#include "q_shared.h"
#include "m_tank.h"

static soundindex_t	sound_thud;
static soundindex_t	sound_pain;
static soundindex_t	sound_idle;
static soundindex_t	sound_die;
static soundindex_t	sound_step;

//
// misc
//

void tank_footstep (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_step, 1, ATTN_NORM, 0);
}

void tank_thud (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_thud, 1, ATTN_NORM, 0);
}

void tank_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}


//
// stand
//

mframe_t tank_frames_stand []=
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
	ai_stand, 0, nullptr
};
mmove_t	tank_move_stand = {FRAME_stand01, FRAME_stand30, tank_frames_stand, nullptr};
	
void tank_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &tank_move_stand;
}


//
// walk
//

void tank_walk (edict_t *self);

mframe_t tank_frames_start_walk [] =
{
	ai_walk,  0, nullptr,
	ai_walk,  6, nullptr,
	ai_walk,  6, nullptr,
	ai_walk, 11, tank_footstep
};
mmove_t	tank_move_start_walk = {FRAME_walk01, FRAME_walk04, tank_frames_start_walk, tank_walk};

mframe_t tank_frames_walk [] =
{
	ai_walk, 4,	nullptr,
	ai_walk, 5,	nullptr,
	ai_walk, 3,	nullptr,
	ai_walk, 2,	nullptr,
	ai_walk, 5,	nullptr,
	ai_walk, 5,	nullptr,
	ai_walk, 4,	nullptr,
	ai_walk, 4,	tank_footstep,
	ai_walk, 3,	nullptr,
	ai_walk, 5,	nullptr,
	ai_walk, 4,	nullptr,
	ai_walk, 5,	nullptr,
	ai_walk, 7,	nullptr,
	ai_walk, 7,	nullptr,
	ai_walk, 6,	nullptr,
	ai_walk, 6,	tank_footstep
};
mmove_t	tank_move_walk = {FRAME_walk05, FRAME_walk20, tank_frames_walk, nullptr};

mframe_t tank_frames_stop_walk [] =
{
	ai_walk,  3, nullptr,
	ai_walk,  3, nullptr,
	ai_walk,  2, nullptr,
	ai_walk,  2, nullptr,
	ai_walk,  4, tank_footstep
};
mmove_t	tank_move_stop_walk = {FRAME_walk21, FRAME_walk25, tank_frames_stop_walk, tank_stand};

void tank_walk (edict_t *self)
{
		self->monsterinfo.currentmove = &tank_move_walk;
}


//
// run
//

void tank_run (edict_t *self);

mframe_t tank_frames_start_run [] =
{
	ai_run,  0, nullptr,
	ai_run,  6, nullptr,
	ai_run,  6, nullptr,
	ai_run, 11, tank_footstep
};
mmove_t	tank_move_start_run = {FRAME_walk01, FRAME_walk04, tank_frames_start_run, tank_run};

mframe_t tank_frames_run [] =
{
	ai_run, 4,	nullptr,
	ai_run, 5,	nullptr,
	ai_run, 3,	nullptr,
	ai_run, 2,	nullptr,
	ai_run, 5,	nullptr,
	ai_run, 5,	nullptr,
	ai_run, 4,	nullptr,
	ai_run, 4,	tank_footstep,
	ai_run, 3,	nullptr,
	ai_run, 5,	nullptr,
	ai_run, 4,	nullptr,
	ai_run, 5,	nullptr,
	ai_run, 7,	nullptr,
	ai_run, 7,	nullptr,
	ai_run, 6,	nullptr,
	ai_run, 6,	tank_footstep
};
mmove_t	tank_move_run = {FRAME_walk05, FRAME_walk20, tank_frames_run, nullptr};

mframe_t tank_frames_stop_run [] =
{
	ai_run,  3, nullptr,
	ai_run,  3, nullptr,
	ai_run,  2, nullptr,
	ai_run,  2, nullptr,
	ai_run,  4, tank_footstep
};
mmove_t	tank_move_stop_run = {FRAME_walk21, FRAME_walk25, tank_frames_stop_run, tank_walk};

void tank_run (edict_t *self)
{
	if (self->monsterinfo.currentmove == &tank_move_walk ||
		self->monsterinfo.currentmove == &tank_move_start_run)
	{
		self->monsterinfo.currentmove = &tank_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &tank_move_start_run;
	}
}

void tank_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;
	gi.sound (self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);
};

//
// death
//

void tank_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -16);
	VectorSet (self->maxs, 16, 16, -0);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t tank_frames_death1 [] =
{
	ai_move, -7,  nullptr,
	ai_move, -2,  nullptr,
	ai_move, -2,  nullptr,
	ai_move, 1,   nullptr,
	ai_move, 3,   nullptr,
	ai_move, 6,   nullptr,
	ai_move, 1,   nullptr,
	ai_move, 1,   nullptr,
	ai_move, 2,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, -2,  nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, -3,  nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, -4,  nullptr,
	ai_move, -6,  nullptr,
	ai_move, -4,  nullptr,
	ai_move, -5,  nullptr,
	ai_move, -7,  nullptr,
	ai_move, -15, tank_thud,
	ai_move, -5,  nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr
};
mmove_t	tank_move_death = {FRAME_death101, FRAME_death132, tank_frames_death1, tank_dead};

void tank_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

// check for gib
	if (self->health <= self->gib_health)
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 1 /*4*/; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		for (n= 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/sm_metal/tris.md2", damage, GIB_METALLIC);
		ThrowGib (self, "models/objects/gibs/chest/tris.md2", damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/gear/tris.md2", damage, GIB_METALLIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

// regular death
	gi.sound (self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	self->monsterinfo.currentmove = &tank_move_death;
}


//
// monster_tank
//

/*QUAKED monster_tank (1 .5 0) (-32 -32 -16) (32 32 72) Ambush Trigger_Spawn Sight
*/
/*QUAKED monster_tank_commander (1 .5 0) (-32 -32 -16) (32 32 72) Ambush Trigger_Spawn Sight
*/
void SP_monster_tank (edict_t *self)
{
	self->s.modelindex = gi.modelindex ("models/monsters/tank/tris.md2");
	VectorSet (self->mins, -32, -32, -16);
	VectorSet (self->maxs, 32, 32, 72);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	sound_pain = gi.soundindex ("tank/tnkpain2.wav");
	sound_thud = gi.soundindex ("tank/tnkdeth2.wav");
	sound_idle = gi.soundindex ("tank/tnkidle1.wav");
	sound_die = gi.soundindex ("tank/death.wav");
	sound_step = gi.soundindex ("tank/step.wav");

	if (prandom(50))//(strcmp(self->classname, "monster_tank_commander") == 0)
	{
		self->health = 1000;
		self->gib_health = -225;
		self->s.skinnum = 2;
		self->monsterinfo.undamaged_skin = 2;
		self->monsterinfo.damaged_skin = 3;
	}
	else
	{
		self->health = 750;
		self->gib_health = -200;
		self->monsterinfo.undamaged_skin = 0;
		self->monsterinfo.damaged_skin = 1;
	}

	self->mass = 500;

	self->pain = tank_pain;
	self->die = tank_die;
	self->monsterinfo.stand = tank_stand;
	self->monsterinfo.walk = tank_walk;
	self->monsterinfo.run = tank_run;
	self->monsterinfo.idle = tank_idle;

	gi.linkentity (self);
	
	self->monsterinfo.currentmove = &tank_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);
}
