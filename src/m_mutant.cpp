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

mutant

==============================================================================
*/

#include "q_shared.h"
#include "m_mutant.h"


static soundindex_t	sound_death;
static soundindex_t	sound_idle;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_search;
static soundindex_t	sound_step1;
static soundindex_t	sound_step2;
static soundindex_t	sound_step3;
static soundindex_t	sound_thud;

//
// SOUNDS
//

void mutant_step (edict_t *self)
{
	int32_t		n;
	n = irandom(2);
	if (n == 0)
		gi.sound (self, CHAN_VOICE, sound_step1, 1, ATTN_NORM, 0);		
	else if (n == 1)
		gi.sound (self, CHAN_VOICE, sound_step2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_step3, 1, ATTN_NORM, 0);
}

void mutant_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}

//
// STAND
//

mframe_t mutant_frames_stand [] =
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
	ai_stand, 0, nullptr,		// 10

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,		// 20

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,		// 30

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,		// 40

	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,		// 50

	ai_stand, 0, nullptr
};
mmove_t mutant_move_stand = {FRAME_stand101, FRAME_stand151, mutant_frames_stand, nullptr};

void mutant_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_stand;
}


//
// IDLE
//

void mutant_idle_loop (edict_t *self)
{
	if (prandom(75))
		self->monsterinfo.nextframe = FRAME_stand155;
}

mframe_t mutant_frames_idle [] =
{
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,					// scratch loop start
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, mutant_idle_loop,		// scratch loop end
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t mutant_move_idle = {FRAME_stand152, FRAME_stand164, mutant_frames_idle, mutant_stand};

void mutant_idle (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_idle;
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}


//
// WALK
//

void mutant_walk (edict_t *self);

mframe_t mutant_frames_walk [] =
{
	ai_walk,	3,		nullptr,
	ai_walk,	1,		nullptr,
	ai_walk,	5,		nullptr,
	ai_walk,	10,		nullptr,
	ai_walk,	13,		nullptr,
	ai_walk,	10,		nullptr,
	ai_walk,	0,		nullptr,
	ai_walk,	5,		nullptr,
	ai_walk,	6,		nullptr,
	ai_walk,	16,		nullptr,
	ai_walk,	15,		nullptr,
	ai_walk,	6,		nullptr
};
mmove_t mutant_move_walk = {FRAME_walk05, FRAME_walk16, mutant_frames_walk, nullptr};

void mutant_walk_loop (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_walk;
}

mframe_t mutant_frames_start_walk [] =
{
	ai_walk,	5,		nullptr,
	ai_walk,	5,		nullptr,
	ai_walk,	-2,		nullptr,
	ai_walk,	1,		nullptr
};
mmove_t mutant_move_start_walk = {FRAME_walk01, FRAME_walk04, mutant_frames_start_walk, mutant_walk_loop};

void mutant_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_start_walk;
}


//
// RUN
//

mframe_t mutant_frames_run [] =
{
	ai_run,	40,		nullptr,
	ai_run,	40,		mutant_step,
	ai_run,	24,		nullptr,
	ai_run,	5,		mutant_step,
	ai_run,	17,		nullptr,
	ai_run,	10,		nullptr
};
mmove_t mutant_move_run = {FRAME_run03, FRAME_run08, mutant_frames_run, nullptr};

void mutant_run (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_run;
}

//
// PAIN
//

void mutant_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
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


//
// DEATH
//

void mutant_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	gi.linkentity (self);
}

mframe_t mutant_frames_death1 [] =
{
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
mmove_t mutant_move_death1 = {FRAME_death101, FRAME_death109, mutant_frames_death1, mutant_dead};

mframe_t mutant_frames_death2 [] =
{
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
mmove_t mutant_move_death2 = {FRAME_death201, FRAME_death210, mutant_frames_death2, mutant_dead};

void mutant_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
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

	gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	if (prandom(50))
		self->monsterinfo.currentmove = &mutant_move_death1;
	else
		self->monsterinfo.currentmove = &mutant_move_death2;
}


//
// SPAWN
//

/*QUAKED monster_mutant (1 .5 0) (-32 -32 -24) (32 32 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_mutant (edict_t *self)
{
	sound_death = gi.soundindex ("mutant/mutdeth1.wav");
	sound_idle = gi.soundindex ("mutant/mutidle1.wav");
	sound_pain1 = gi.soundindex ("mutant/mutpain1.wav");
	sound_pain2 = gi.soundindex ("mutant/mutpain2.wav");
	sound_search = gi.soundindex ("mutant/mutsrch1.wav");
	sound_step1 = gi.soundindex ("mutant/step1.wav");
	sound_step2 = gi.soundindex ("mutant/step2.wav");
	sound_step3 = gi.soundindex ("mutant/step3.wav");
	sound_thud = gi.soundindex ("mutant/thud1.wav");
	
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;
	self->s.modelindex = gi.modelindex ("models/monsters/mutant/tris.md2");
	VectorSet (self->mins, -32, -32, -24);
	VectorSet (self->maxs, 32, 32, 48);

	self->health = 300;
	self->gib_health = -120;
	self->mass = 300;

	self->pain = mutant_pain;
	self->die = mutant_die;

	self->monsterinfo.stand = mutant_stand;
	self->monsterinfo.walk = mutant_walk;
	self->monsterinfo.run = mutant_run;
	self->monsterinfo.search = mutant_search;
	self->monsterinfo.idle = mutant_idle;

	gi.linkentity (self);
	
	self->monsterinfo.currentmove = &mutant_move_stand;
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
