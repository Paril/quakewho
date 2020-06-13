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

GLADIATOR

==============================================================================
*/

#include "q_shared.h"
#include "m_gladiator.h"


static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_die;
static soundindex_t	sound_idle;
static soundindex_t	sound_search;


void gladiator_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

void gladiator_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}

mframe_t gladiator_frames_stand [] =
{
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t gladiator_move_stand = {FRAME_stand1, FRAME_stand7, gladiator_frames_stand, nullptr};

void gladiator_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &gladiator_move_stand;
}


mframe_t gladiator_frames_walk [] =
{
	ai_walk, 15, nullptr,
	ai_walk, 7,  nullptr,
	ai_walk, 6,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 8,  nullptr,
	ai_walk, 12, nullptr,
	ai_walk, 8,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 5,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 2,  nullptr,
	ai_walk, 1,  nullptr,
	ai_walk, 8,  nullptr
};
mmove_t gladiator_move_walk = {FRAME_walk1, FRAME_walk16, gladiator_frames_walk, nullptr};

void gladiator_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &gladiator_move_walk;
}


mframe_t gladiator_frames_run [] =
{
	ai_run, 23,	nullptr,
	ai_run, 14,	nullptr,
	ai_run, 14,	nullptr,
	ai_run, 21,	nullptr,
	ai_run, 12,	nullptr,
	ai_run, 13,	nullptr
};
mmove_t gladiator_move_run = {FRAME_run1, FRAME_run6, gladiator_frames_run, nullptr};

void gladiator_run (edict_t *self)
{
	self->monsterinfo.currentmove = &gladiator_move_run;
}

void gladiator_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	if (prandom(50))
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
}


void gladiator_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t gladiator_frames_death [] =
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
mmove_t gladiator_move_death = {FRAME_death1, FRAME_death22, gladiator_frames_death, gladiator_dead};

void gladiator_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
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
	gi.sound (self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	self->monsterinfo.currentmove = &gladiator_move_death;
}


/*QUAKED monster_gladiator (1 .5 0) (-32 -32 -24) (32 32 64) Ambush Trigger_Spawn Sight
*/
void SP_monster_gladiator (edict_t *self)
{
	sound_pain1 = gi.soundindex ("gladiator/pain.wav");	
	sound_pain2 = gi.soundindex ("gladiator/gldpain2.wav");	
	sound_die = gi.soundindex ("gladiator/glddeth2.wav");
	sound_idle = gi.soundindex ("gladiator/gldidle1.wav");
	sound_search = gi.soundindex ("gladiator/gldsrch1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/gladiatr/tris.md2");
	VectorSet (self->mins, -32, -32, -24);
	VectorSet (self->maxs, 32, 32, 64);

	self->health = 400;
	self->gib_health = -175;
	self->mass = 400;

	self->pain = gladiator_pain;
	self->die = gladiator_die;

	self->monsterinfo.stand = gladiator_stand;
	self->monsterinfo.walk = gladiator_walk;
	self->monsterinfo.run = gladiator_run;
	self->monsterinfo.idle = gladiator_idle;
	self->monsterinfo.search = gladiator_search;

	gi.linkentity (self);
	self->monsterinfo.currentmove = &gladiator_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
