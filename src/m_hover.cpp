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

hover

==============================================================================
*/

#include "q_shared.h"
#include "m_hover.h"

static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_death1;
static soundindex_t	sound_death2;
static soundindex_t	sound_search1;
static soundindex_t	sound_search2;

void hover_search (edict_t *self)
{
	if (prandom(50))
		gi.sound (self, CHAN_VOICE, sound_search1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_search2, 1, ATTN_NORM, 0);
}


void hover_run (edict_t *self);
void hover_stand (edict_t *self);
void hover_dead (edict_t *self);
void hover_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, vec3_t point);

mframe_t hover_frames_stand [] =
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
mmove_t	hover_move_stand = {FRAME_stand01, FRAME_stand30, hover_frames_stand, nullptr};

mframe_t hover_frames_walk [] =
{
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	4,	nullptr
};
mmove_t hover_move_walk = {FRAME_forwrd01, FRAME_forwrd35, hover_frames_walk, nullptr};

mframe_t hover_frames_run [] =
{
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	10,	nullptr
};
mmove_t hover_move_run = {FRAME_forwrd01, FRAME_forwrd35, hover_frames_run, nullptr};

mframe_t hover_frames_death1 [] =
{
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	-10,nullptr,
	ai_move,	3,	nullptr,
	ai_move,	5,	nullptr,
	ai_move,	4,	nullptr,
	ai_move,	7,	nullptr
};
mmove_t hover_move_death1 = {FRAME_death101, FRAME_death111, hover_frames_death1, hover_dead};


void hover_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &hover_move_stand;
}

void hover_run (edict_t *self)
{
	self->monsterinfo.currentmove = &hover_move_run;
}

void hover_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &hover_move_walk;
}

void hover_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	if (damage <= 25)
	{
		if (prandom(50))
		{
			gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		}
		else
		{
			gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
		}
	}
	else
	{
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	}
}

void hover_deadthink (edict_t *self)
{
	if (!self->groundentity && level.time < self->timestamp)
	{
		self->nextthink = level.time + FRAMETIME;
		return;
	}
	BecomeExplosion1(self);
}

void hover_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->think = hover_deadthink;
	self->nextthink = level.time + FRAMETIME;
	self->timestamp = level.time + 15;
	gi.linkentity (self);
}

void hover_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

// check for gib
	if (self->health <= self->gib_health)
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 2; n++)
			ThrowGib (self, "models/objects/gibs/bone/tris.md2", damage, GIB_ORGANIC);
		for (n= 0; n < 2; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

// regular death
	if (prandom(50))
		gi.sound (self, CHAN_VOICE, sound_death1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_death2, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	self->monsterinfo.currentmove = &hover_move_death1;
}

/*QUAKED monster_hover (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_hover (edict_t *self)
{
	sound_pain1 = gi.soundindex ("hover/hovpain1.wav");	
	sound_pain2 = gi.soundindex ("hover/hovpain2.wav");	
	sound_death1 = gi.soundindex ("hover/hovdeth1.wav");	
	sound_death2 = gi.soundindex ("hover/hovdeth2.wav");
	sound_search1 = gi.soundindex ("hover/hovsrch1.wav");	
	sound_search2 = gi.soundindex ("hover/hovsrch2.wav");

	self->s.sound = gi.soundindex ("hover/hovidle1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex("models/monsters/hover/tris.md2");
	VectorSet (self->mins, -24, -24, -24);
	VectorSet (self->maxs, 24, 24, 32);

	self->health = 240;
	self->gib_health = -100;
	self->mass = 150;

	self->pain = hover_pain;
	self->die = hover_die;

	self->monsterinfo.stand = hover_stand;
	self->monsterinfo.walk = hover_walk;
	self->monsterinfo.run = hover_run;
	self->monsterinfo.search = hover_search;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &hover_move_stand;	
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	flymonster_start (self);
}
