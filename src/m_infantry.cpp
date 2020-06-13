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

INFANTRY

==============================================================================
*/

#include "q_shared.h"
#include "m_infantry.h"

static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_die1;
static soundindex_t	sound_die2;
static soundindex_t	sound_search;
static soundindex_t	sound_idle;


mframe_t infantry_frames_stand [] =
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
	ai_stand, 0, nullptr
};
mmove_t infantry_move_stand = {FRAME_stand50, FRAME_stand71, infantry_frames_stand, nullptr};

void infantry_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &infantry_move_stand;
}


mframe_t infantry_frames_fidget [] =
{
	ai_stand, 1,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 3,  nullptr,
	ai_stand, 6,  nullptr,
	ai_stand, 3,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, -1, nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, -2, nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, -1, nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, -1, nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, -1, nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 1,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, -1, nullptr,
	ai_stand, -1, nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, -3, nullptr,
	ai_stand, -2, nullptr,
	ai_stand, -3, nullptr,
	ai_stand, -3, nullptr,
	ai_stand, -2, nullptr
};
mmove_t infantry_move_fidget = {FRAME_stand01, FRAME_stand49, infantry_frames_fidget, infantry_stand};

void infantry_fidget (edict_t *self)
{
	self->monsterinfo.currentmove = &infantry_move_fidget;
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

mframe_t infantry_frames_walk [] =
{
	ai_walk, 5,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 6,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 5,  nullptr
};
mmove_t infantry_move_walk = {FRAME_walk03, FRAME_walk14, infantry_frames_walk, nullptr};

void infantry_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &infantry_move_walk;
}

mframe_t infantry_frames_run [] =
{
	ai_run, 10, nullptr,
	ai_run, 20, nullptr,
	ai_run, 5,  nullptr,
	ai_run, 7,  nullptr,
	ai_run, 30, nullptr,
	ai_run, 35, nullptr,
	ai_run, 2,  nullptr,
	ai_run, 6,  nullptr
};
mmove_t infantry_move_run = {FRAME_run01, FRAME_run08, infantry_frames_run, nullptr};

void infantry_run (edict_t *self)
{
	self->monsterinfo.currentmove = &infantry_move_run;
}

void infantry_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	gi.linkentity (self);
}

mframe_t infantry_frames_death1 [] =
{
	ai_move, -4, nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, -1, nullptr,
	ai_move, -4, nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, -1, nullptr,
	ai_move, 3,  nullptr,
	ai_move, 1,  nullptr,
	ai_move, 1,  nullptr,
	ai_move, -2, nullptr,
	ai_move, 2,  nullptr,
	ai_move, 2,  nullptr,
	ai_move, 9,  nullptr,
	ai_move, 9,  nullptr,
	ai_move, 5,  nullptr,
	ai_move, -3, nullptr,
	ai_move, -3, nullptr
};
mmove_t infantry_move_death1 = {FRAME_death101, FRAME_death120, infantry_frames_death1, infantry_dead};

mframe_t infantry_frames_death3 [] =
{
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr,
	ai_move, -6,  nullptr,
	ai_move, -11, nullptr,
	ai_move, -3,  nullptr,
	ai_move, -11, nullptr,
	ai_move, 0,   nullptr,
	ai_move, 0,   nullptr
};
mmove_t infantry_move_death3 = {FRAME_death301, FRAME_death309, infantry_frames_death3, infantry_dead};


void infantry_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

// check for gib
	if (self->health <= self->gib_health)
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 2; n++)
			ThrowGib (self, "models/objects/gibs/bone/tris.md2", damage, GIB_ORGANIC);
		for (n= 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

// regular death
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	n = irandom(2);
	if (n == 0)
	{
		self->monsterinfo.currentmove = &infantry_move_death1;
		gi.sound (self, CHAN_VOICE, sound_die2, 1, ATTN_NORM, 0);
	}
	else if (n == 1)
	{
		self->monsterinfo.currentmove = &infantry_move_death3;
		gi.sound (self, CHAN_VOICE, sound_die1, 1, ATTN_NORM, 0);
	}
	else
	{
		self->monsterinfo.currentmove = &infantry_move_death3;
		gi.sound (self, CHAN_VOICE, sound_die2, 1, ATTN_NORM, 0);
	}
}

void infantry_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	int32_t		n;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	n = prandom(50);
	if (n == 0)
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
}

/*QUAKED monster_infantry (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_infantry (edict_t *self)
{
	sound_pain1 = gi.soundindex ("infantry/infpain1.wav");
	sound_pain2 = gi.soundindex ("infantry/infpain2.wav");
	sound_die1 = gi.soundindex ("infantry/infdeth1.wav");
	sound_die2 = gi.soundindex ("infantry/infdeth2.wav");

	sound_search = gi.soundindex ("infantry/infsrch1.wav");
	sound_idle = gi.soundindex ("infantry/infidle1.wav");
	

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/infantry/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 32);

	self->health = 100;
	self->gib_health = -40;
	self->mass = 200;

	self->pain = infantry_pain;
	self->die = infantry_die;

	self->monsterinfo.stand = infantry_stand;
	self->monsterinfo.walk = infantry_walk;
	self->monsterinfo.run = infantry_run;
	self->monsterinfo.idle = infantry_fidget;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &infantry_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
