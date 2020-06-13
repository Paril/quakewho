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

brain

==============================================================================
*/

#include "q_shared.h"
#include "m_brain.h"


static soundindex_t	sound_death;
static soundindex_t	sound_idle1;
static soundindex_t	sound_idle2;
static soundindex_t	sound_idle3;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_search;


void brain_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}


void brain_run (edict_t *self);
void brain_dead (edict_t *self);


//
// STAND
//

mframe_t brain_frames_stand [] =
{
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,

	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,

	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr
};
mmove_t brain_move_stand = {FRAME_stand01, FRAME_stand30, brain_frames_stand, nullptr};

void brain_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &brain_move_stand;
}


//
// IDLE
//

mframe_t brain_frames_idle [] =
{
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,

	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,

	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr,
	ai_stand,	0,	nullptr
};
mmove_t brain_move_idle = {FRAME_stand31, FRAME_stand60, brain_frames_idle, brain_stand};

void brain_idle (edict_t *self)
{
	gi.sound (self, CHAN_AUTO, sound_idle3, 1, ATTN_IDLE, 0);
	self->monsterinfo.currentmove = &brain_move_idle;
}


//
// WALK
//
mframe_t brain_frames_walk1 [] =
{
	ai_walk,	7,	nullptr,
	ai_walk,	2,	nullptr,
	ai_walk,	3,	nullptr,
	ai_walk,	3,	nullptr,
	ai_walk,	1,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	9,	nullptr,
	ai_walk,	-4,	nullptr,
	ai_walk,	-1,	nullptr,
	ai_walk,	2,	nullptr
};
mmove_t brain_move_walk1 = {FRAME_walk101, FRAME_walk111, brain_frames_walk1, nullptr};

// walk2 is FUBAR, do not use
#if 0
void brain_walk2_cycle (edict_t *self)
{
	if (random() > 0.1)
		self->monsterinfo.nextframe = FRAME_walk220;
}

mframe_t brain_frames_walk2 [] =
{
	ai_walk,	3,	nullptr,
	ai_walk,	-2,	nullptr,
	ai_walk,	-4,	nullptr,
	ai_walk,	-3,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	1,	nullptr,
	ai_walk,	12,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	-3,	nullptr,
	ai_walk,	0,	nullptr,

	ai_walk,	-2,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	1,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	10,	nullptr,		// Cycle Start

	ai_walk,	-1,	nullptr,
	ai_walk,	7,	nullptr,
	ai_walk,	0,	nullptr,
	ai_walk,	3,	nullptr,
	ai_walk,	-3,	nullptr,
	ai_walk,	2,	nullptr,
	ai_walk,	4,	nullptr,
	ai_walk,	-3,	nullptr,
	ai_walk,	2,	nullptr,
	ai_walk,	0,	nullptr,

	ai_walk,	4,	brain_walk2_cycle,
	ai_walk,	-1,	nullptr,
	ai_walk,	-1,	nullptr,
	ai_walk,	-8,	nullptr,		
	ai_walk,	0,	nullptr,
	ai_walk,	1,	nullptr,
	ai_walk,	5,	nullptr,
	ai_walk,	2,	nullptr,
	ai_walk,	-1,	nullptr,
	ai_walk,	-5,	nullptr
};
mmove_t brain_move_walk2 = {FRAME_walk201, FRAME_walk240, brain_frames_walk2, nullptr};
#endif

void brain_walk (edict_t *self)
{
//	if (random() <= 0.5)
		self->monsterinfo.currentmove = &brain_move_walk1;
//	else
//		self->monsterinfo.currentmove = &brain_move_walk2;
}

mframe_t brain_frames_death2 [] =
{
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	9,	nullptr,
	ai_move,	0,	nullptr
};
mmove_t brain_move_death2 = {FRAME_death201, FRAME_death205, brain_frames_death2, brain_dead};

mframe_t brain_frames_death1 [] =
{
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	-2,	nullptr,
	ai_move,	9,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	0,	nullptr
};
mmove_t brain_move_death1 = {FRAME_death101, FRAME_death118, brain_frames_death1, brain_dead};

//
// RUN
//

mframe_t brain_frames_run [] =
{
	ai_run,	9,	nullptr,
	ai_run,	2,	nullptr,
	ai_run,	3,	nullptr,
	ai_run,	3,	nullptr,
	ai_run,	1,	nullptr,
	ai_run,	0,	nullptr,
	ai_run,	0,	nullptr,
	ai_run,	10,	nullptr,
	ai_run,	-4,	nullptr,
	ai_run,	-1,	nullptr,
	ai_run,	2,	nullptr
};
mmove_t brain_move_run = {FRAME_walk101, FRAME_walk111, brain_frames_run, nullptr};

void brain_run (edict_t *self)
{
	self->monsterinfo.currentmove = &brain_move_run;
}


void brain_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
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
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
}

void brain_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}



void brain_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	int32_t		n;

	self->s.effects = EF_NONE;

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
	gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;
	if (prandom(50))
		self->monsterinfo.currentmove = &brain_move_death1;
	else
		self->monsterinfo.currentmove = &brain_move_death2;
}

/*QUAKED monster_brain (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_brain (edict_t *self)
{
	sound_death = gi.soundindex ("brain/brndeth1.wav");
	sound_idle1 = gi.soundindex ("brain/brnidle1.wav");
	sound_idle2 = gi.soundindex ("brain/brnidle2.wav");
	sound_idle3 = gi.soundindex ("brain/brnlens1.wav");
	sound_pain1 = gi.soundindex ("brain/brnpain1.wav");
	sound_pain2 = gi.soundindex ("brain/brnpain2.wav");
	sound_search = gi.soundindex ("brain/brnsrch1.wav");

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/brain/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 32);

	self->health = 300;
	self->gib_health = -150;
	self->mass = 400;

	self->pain = brain_pain;
	self->die = brain_die;

	self->monsterinfo.stand = brain_stand;
	self->monsterinfo.walk = brain_walk;
	self->monsterinfo.run = brain_run;
	self->monsterinfo.search = brain_search;
	self->monsterinfo.idle = brain_idle;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &brain_move_stand;	
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
