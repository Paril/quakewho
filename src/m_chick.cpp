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

chick

==============================================================================
*/

#include "q_shared.h"
#include "m_chick.h"

void chick_stand (edict_t *self);
void chick_run (edict_t *self);

static soundindex_t	sound_death1;
static soundindex_t	sound_death2;
static soundindex_t	sound_idle1;
static soundindex_t	sound_idle2;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_pain3;
static soundindex_t	sound_search;


void ChickMoan (edict_t *self)
{
	if (prandom(50))
		gi.sound (self, CHAN_VOICE, sound_idle1, 1, ATTN_IDLE, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_idle2, 1, ATTN_IDLE, 0);
}

mframe_t chick_frames_fidget [] =
{
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  ChickMoan,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr,
	ai_stand, 0,  nullptr
};
mmove_t chick_move_fidget = {FRAME_stand201, FRAME_stand230, chick_frames_fidget, chick_stand};

void chick_fidget (edict_t *self)
{
	if (M_FidgetCheck(self, 30))
		self->monsterinfo.currentmove = &chick_move_fidget;
}

mframe_t chick_frames_stand [] =
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
	ai_stand, 0, chick_fidget,

};
mmove_t chick_move_stand = {FRAME_stand101, FRAME_stand130, chick_frames_stand, nullptr};

void chick_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &chick_move_stand;
}

mframe_t chick_frames_start_run [] =
{
	ai_run, 1,  nullptr,
	ai_run, 0,  nullptr,
	ai_run, 0,	 nullptr,
	ai_run, -1, nullptr, 
	ai_run, -1, nullptr, 
	ai_run, 0,  nullptr,
	ai_run, 1,  nullptr,
	ai_run, 3,  nullptr,
	ai_run, 6,	 nullptr,
	ai_run, 3,	 nullptr
};
mmove_t chick_move_start_run = {FRAME_walk01, FRAME_walk10, chick_frames_start_run, chick_run};

mframe_t chick_frames_run [] =
{
	ai_run, 6,	nullptr,
	ai_run, 8,  nullptr,
	ai_run, 13, nullptr,
	ai_run, 5,  nullptr,
	ai_run, 7,  nullptr,
	ai_run, 4,  nullptr,
	ai_run, 11, nullptr,
	ai_run, 5,  nullptr,
	ai_run, 9,  nullptr,
	ai_run, 7,  nullptr

};

mmove_t chick_move_run = {FRAME_walk11, FRAME_walk20, chick_frames_run, nullptr};

mframe_t chick_frames_walk [] =
{
	ai_walk, 6,	 nullptr,
	ai_walk, 8,  nullptr,
	ai_walk, 13, nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 7,  nullptr,
	ai_walk, 4,  nullptr,
	ai_walk, 11, nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 9,  nullptr,
	ai_walk, 7,  nullptr
};

mmove_t chick_move_walk = {FRAME_walk11, FRAME_walk20, chick_frames_walk, nullptr};

void chick_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &chick_move_walk;
}

void chick_run (edict_t *self)
{
	if (self->monsterinfo.currentmove == &chick_move_walk ||
		self->monsterinfo.currentmove == &chick_move_start_run)
	{
		self->monsterinfo.currentmove = &chick_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &chick_move_start_run;
	}
}

void chick_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	int32_t r = irandom(2);
	if (r == 0)
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else if (r == 1)
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain3, 1, ATTN_NORM, 0);
}

void chick_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, 0);
	VectorSet (self->maxs, 16, 16, 16);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t chick_frames_death2 [] =
{
	ai_move, -6, nullptr,
	ai_move, 0,  nullptr,
	ai_move, -1,  nullptr,
	ai_move, -5, nullptr,
	ai_move, 0, nullptr,
	ai_move, -1,  nullptr,
	ai_move, -2,  nullptr,
	ai_move, 1,  nullptr,
	ai_move, 10, nullptr,
	ai_move, 2,  nullptr,
	ai_move, 3,  nullptr,
	ai_move, 1,  nullptr,
	ai_move, 2, nullptr,
	ai_move, 0,  nullptr,
	ai_move, 3,  nullptr,
	ai_move, 3,  nullptr,
	ai_move, 1,  nullptr,
	ai_move, -3,  nullptr,
	ai_move, -5, nullptr,
	ai_move, 4, nullptr,
	ai_move, 15, nullptr,
	ai_move, 14, nullptr,
	ai_move, 1, nullptr
};
mmove_t chick_move_death2 = {FRAME_death201, FRAME_death223, chick_frames_death2, chick_dead};

mframe_t chick_frames_death1 [] =
{
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, -7, nullptr,
	ai_move, 4,  nullptr,
	ai_move, 11, nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr,
	ai_move, 0,  nullptr
	
};
mmove_t chick_move_death1 = {FRAME_death101, FRAME_death112, chick_frames_death1, chick_dead};

void chick_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
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

	n = prandom(50);
	if (n == 0)
	{
		self->monsterinfo.currentmove = &chick_move_death1;
		gi.sound (self, CHAN_VOICE, sound_death1, 1, ATTN_NORM, 0);
	}
	else
	{
		self->monsterinfo.currentmove = &chick_move_death2;
		gi.sound (self, CHAN_VOICE, sound_death2, 1, ATTN_NORM, 0);
	}
}


/*QUAKED monster_chick (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_chick (edict_t *self)
{
	sound_death1			= gi.soundindex ("chick/chkdeth1.wav");
	sound_death2			= gi.soundindex ("chick/chkdeth2.wav");
	sound_idle1				= gi.soundindex ("chick/chkidle1.wav");
	sound_idle2				= gi.soundindex ("chick/chkidle2.wav");
	sound_pain1				= gi.soundindex ("chick/chkpain1.wav");
	sound_pain2				= gi.soundindex ("chick/chkpain2.wav");
	sound_pain3				= gi.soundindex ("chick/chkpain3.wav");
	sound_search			= gi.soundindex ("chick/chksrch1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/bitch/tris.md2");
	VectorSet (self->mins, -16, -16, 0);
	VectorSet (self->maxs, 16, 16, 56);

	self->health = 175;
	self->gib_health = -70;
	self->mass = 200;

	self->pain = chick_pain;
	self->die = chick_die;

	self->monsterinfo.stand = chick_stand;
	self->monsterinfo.walk = chick_walk;
	self->monsterinfo.run = chick_run;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &chick_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
