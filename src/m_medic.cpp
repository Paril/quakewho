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

MEDIC

==============================================================================
*/

#include "q_shared.h"
#include "m_medic.h"

static soundindex_t	sound_idle1;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_die;
static soundindex_t	sound_search;

void medic_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle1, 1, ATTN_IDLE, 0);
}

void medic_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_IDLE, 0);
}

mframe_t medic_frames_stand [] =
{
	ai_stand, 0, medic_idle,
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

};
mmove_t medic_move_stand = {FRAME_wait1, FRAME_wait90, medic_frames_stand, nullptr};

void medic_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &medic_move_stand;
}


mframe_t medic_frames_walk [] =
{
	ai_walk, 6.2,	nullptr,
	ai_walk, 18.1,  nullptr,
	ai_walk, 1,		nullptr,
	ai_walk, 9,		nullptr,
	ai_walk, 10,	nullptr,
	ai_walk, 9,		nullptr,
	ai_walk, 11,	nullptr,
	ai_walk, 11.6,  nullptr,
	ai_walk, 2,		nullptr,
	ai_walk, 9.9,	nullptr,
	ai_walk, 14,	nullptr,
	ai_walk, 9.3,	nullptr
};
mmove_t medic_move_walk = {FRAME_walk1, FRAME_walk12, medic_frames_walk, nullptr};

void medic_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &medic_move_walk;
}


mframe_t medic_frames_run [] =
{
	ai_run, 18,		nullptr,
	ai_run, 22.5,	nullptr,
	ai_run, 25.4,	nullptr,
	ai_run, 23.4,	nullptr,
	ai_run, 24,		nullptr,
	ai_run, 35.6,	nullptr
	
};
mmove_t medic_move_run = {FRAME_run1, FRAME_run6, medic_frames_run, nullptr};

void medic_run (edict_t *self)
{
	self->monsterinfo.currentmove = &medic_move_run;
}


void medic_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	if (prandom(50))
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
}


void medic_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t medic_frames_death [] =
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
mmove_t medic_move_death = {FRAME_death1, FRAME_death30, medic_frames_death, medic_dead};

void medic_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

	// if we had a pending patient, free him up for another medic
	if ((self->enemy) && (self->enemy->owner == self))
		self->enemy->owner = nullptr;

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

	self->monsterinfo.currentmove = &medic_move_death;
}

/*QUAKED monster_medic (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_medic (edict_t *self)
{
	sound_idle1 = gi.soundindex ("medic/idle.wav");
	sound_pain1 = gi.soundindex ("medic/medpain1.wav");
	sound_pain2 = gi.soundindex ("medic/medpain2.wav");
	sound_die = gi.soundindex ("medic/meddeth1.wav");
	sound_search = gi.soundindex ("medic/medsrch1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/medic/tris.md2");
	VectorSet (self->mins, -24, -24, -24);
	VectorSet (self->maxs, 24, 24, 32);

	self->health = 300;
	self->gib_health = -130;
	self->mass = 400;

	self->pain = medic_pain;
	self->die = medic_die;

	self->monsterinfo.stand = medic_stand;
	self->monsterinfo.walk = medic_walk;
	self->monsterinfo.run = medic_run;
	self->monsterinfo.idle = medic_idle;
	self->monsterinfo.search = medic_search;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &medic_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
