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

floater

==============================================================================
*/

#include "q_shared.h"
#include "m_float.h"


static soundindex_t	sound_death1;
static soundindex_t	sound_idle;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_sight;


void floater_sight (edict_t *self, edict_t *other)
{
	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void floater_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}


//void floater_stand1 (edict_t *self);
void floater_dead (edict_t *self);
void floater_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, vec3_t point);
void floater_run (edict_t *self);

mframe_t floater_frames_stand1 [] =
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
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t	floater_move_stand1 = {FRAME_stand101, FRAME_stand152, floater_frames_stand1, nullptr};

mframe_t floater_frames_stand2 [] =
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
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t	floater_move_stand2 = {FRAME_stand201, FRAME_stand252, floater_frames_stand2, nullptr};

void floater_stand (edict_t *self)
{
	if (prandom(50))		
		self->monsterinfo.currentmove = &floater_move_stand1;
	else
		self->monsterinfo.currentmove = &floater_move_stand2;
}

mframe_t floater_frames_walk [] =
{
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr,
	ai_walk, 5, nullptr
};
mmove_t	floater_move_walk = {FRAME_stand101, FRAME_stand152, floater_frames_walk, nullptr};

mframe_t floater_frames_run [] =
{
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr,
	ai_run, 13, nullptr
};
mmove_t	floater_move_run = {FRAME_stand101, FRAME_stand152, floater_frames_run, nullptr};

void floater_run (edict_t *self)
{
	self->monsterinfo.currentmove = &floater_move_run;
}

void floater_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &floater_move_walk;
}

void floater_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	int32_t		n;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	n = irandom(2);
	if (n == 0)
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
}

void floater_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

void floater_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	gi.sound (self, CHAN_VOICE, sound_death1, 1, ATTN_NORM, 0);
	BecomeExplosion1(self);
}

/*QUAKED monster_floater (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_floater (edict_t *self)
{
	sound_death1 = gi.soundindex ("floater/fltdeth1.wav");
	sound_idle = gi.soundindex ("floater/fltidle1.wav");
	sound_pain1 = gi.soundindex ("floater/fltpain1.wav");
	sound_pain2 = gi.soundindex ("floater/fltpain2.wav");
	sound_sight = gi.soundindex ("floater/fltsght1.wav");

	self->s.sound = gi.soundindex ("floater/fltsrch1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/float/tris.md2");
	VectorSet (self->mins, -24, -24, -24);
	VectorSet (self->maxs, 24, 24, 32);

	self->health = 200;
	self->gib_health = -80;
	self->mass = 300;

	self->pain = floater_pain;
	self->die = floater_die;

	self->monsterinfo.stand = floater_stand;
	self->monsterinfo.walk = floater_walk;
	self->monsterinfo.run = floater_run;
	self->monsterinfo.idle = floater_idle;

	gi.linkentity (self);

	if (prandom(50))		
		self->monsterinfo.currentmove = &floater_move_stand1;	
	else
		self->monsterinfo.currentmove = &floater_move_stand2;	
	
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	flymonster_start (self);
}
