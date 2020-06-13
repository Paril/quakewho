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

BERSERK

==============================================================================
*/

#include "q_shared.h"
#include "m_berserk.h"


static soundindex_t sound_pain;
static soundindex_t sound_die;
static soundindex_t sound_idle;
static soundindex_t sound_search;

void berserk_fidget (edict_t *self);
mframe_t berserk_frames_stand [] =
{
	ai_stand, 0, berserk_fidget,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t berserk_move_stand = {FRAME_stand1, FRAME_stand5, berserk_frames_stand, nullptr};

void berserk_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &berserk_move_stand;
}

void berserk_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}

mframe_t berserk_frames_stand_fidget [] =
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
	ai_stand, 0, nullptr
};
mmove_t berserk_move_stand_fidget = {FRAME_standb1, FRAME_standb20, berserk_frames_stand_fidget, berserk_stand};

void berserk_fidget (edict_t *self)
{
	if (!M_FidgetCheck(self, 15))
		return;

	self->monsterinfo.currentmove = &berserk_move_stand_fidget;
	gi.sound (self, CHAN_WEAPON, sound_idle, 1, ATTN_IDLE, 0);
}


mframe_t berserk_frames_walk [] =
{
	ai_walk, 9.1, nullptr,
	ai_walk, 6.3, nullptr,
	ai_walk, 4.9, nullptr,
	ai_walk, 6.7, nullptr,
	ai_walk, 6.0, nullptr,
	ai_walk, 8.2, nullptr,
	ai_walk, 7.2, nullptr,
	ai_walk, 6.1, nullptr,
	ai_walk, 4.9, nullptr,
	ai_walk, 4.7, nullptr,
	ai_walk, 4.7, nullptr,
	ai_walk, 4.8, nullptr
};
mmove_t berserk_move_walk = {FRAME_walkc1, FRAME_walkc11, berserk_frames_walk, nullptr};

void berserk_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &berserk_move_walk;
}

mframe_t berserk_frames_run1 [] =
{
	ai_run, 21, nullptr,
	ai_run, 11, nullptr,
	ai_run, 21, nullptr,
	ai_run, 25, nullptr,
	ai_run, 18, nullptr,
	ai_run, 19, nullptr
};
mmove_t berserk_move_run1 = {FRAME_run1, FRAME_run6, berserk_frames_run1, nullptr};

void berserk_run (edict_t *self)
{
	self->monsterinfo.currentmove = &berserk_move_run1;
}
void berserk_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;
	gi.sound (self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);
}


void berserk_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}


mframe_t berserk_frames_death1 [] =
{
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr
	
};
mmove_t berserk_move_death1 = {FRAME_death1, FRAME_death13, berserk_frames_death1, berserk_dead};


mframe_t berserk_frames_death2 [] =
{
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr,
	ai_move, 0, nullptr
};
mmove_t berserk_move_death2 = {FRAME_deathc1, FRAME_deathc8, berserk_frames_death2, berserk_dead};


void berserk_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

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

	gi.sound (self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	if (damage >= 50)
		self->monsterinfo.currentmove = &berserk_move_death1;
	else
		self->monsterinfo.currentmove = &berserk_move_death2;
}


/*QUAKED monster_berserk (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_berserk (edict_t *self)
{
	// pre-caches
	sound_pain  = gi.soundindex ("berserk/berpain2.wav");
	sound_die   = gi.soundindex ("berserk/berdeth2.wav");
	sound_idle  = gi.soundindex ("berserk/beridle1.wav");
	sound_search = gi.soundindex ("berserk/bersrch1.wav");

	self->s.modelindex = gi.modelindex("models/monsters/berserk/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 32);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	self->health = 240;
	self->gib_health = -60;
	self->mass = 250;

	self->pain = berserk_pain;
	self->die = berserk_die;

	self->monsterinfo.stand = berserk_stand;
	self->monsterinfo.walk = berserk_walk;
	self->monsterinfo.run = berserk_run;
	self->monsterinfo.search = berserk_search;

	self->monsterinfo.currentmove = &berserk_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	gi.linkentity (self);

	walkmonster_start (self);
}
