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

parasite

==============================================================================
*/

#include "q_shared.h"
#include "m_parasite.h"


static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_die;
static soundindex_t	sound_tap;
static soundindex_t	sound_scratch;


void parasite_stand (edict_t *self);
void parasite_start_run (edict_t *self);
void parasite_run (edict_t *self);
void parasite_walk (edict_t *self);
void parasite_start_walk (edict_t *self);
void parasite_end_fidget (edict_t *self);
void parasite_do_fidget (edict_t *self);
void parasite_refidget (edict_t *self);

void parasite_tap (edict_t *self)
{
	gi.sound (self, CHAN_WEAPON, sound_tap, 1, ATTN_IDLE, 0);
}

void parasite_scratch (edict_t *self)
{
	gi.sound (self, CHAN_WEAPON, sound_scratch, 1, ATTN_IDLE, 0);
}

mframe_t parasite_frames_start_fidget [] =
{
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t parasite_move_start_fidget = {FRAME_stand18, FRAME_stand21, parasite_frames_start_fidget, parasite_do_fidget};

mframe_t parasite_frames_fidget [] =
{	
	ai_stand, 0, parasite_scratch,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_scratch,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t parasite_move_fidget = {FRAME_stand22, FRAME_stand27, parasite_frames_fidget, parasite_refidget};

mframe_t parasite_frames_end_fidget [] =
{
	ai_stand, 0, parasite_scratch,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr
};
mmove_t parasite_move_end_fidget = {FRAME_stand28, FRAME_stand35, parasite_frames_end_fidget, parasite_stand};

void parasite_end_fidget (edict_t *self)
{
	self->monsterinfo.currentmove = &parasite_move_end_fidget;
}

void parasite_do_fidget (edict_t *self)
{
	self->monsterinfo.currentmove = &parasite_move_fidget;
}

void parasite_refidget (edict_t *self)
{ 
	if (prandom(80))
		self->monsterinfo.currentmove = &parasite_move_fidget;
	else
		self->monsterinfo.currentmove = &parasite_move_end_fidget;
}

void parasite_idle (edict_t *self)
{ 
	self->monsterinfo.currentmove = &parasite_move_start_fidget;
}


mframe_t parasite_frames_stand [] =
{
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_tap,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_tap,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_tap,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_tap,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_tap,
	ai_stand, 0, nullptr,
	ai_stand, 0, parasite_tap
};
mmove_t	parasite_move_stand = {FRAME_stand01, FRAME_stand17, parasite_frames_stand, parasite_stand};

void parasite_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &parasite_move_stand;
}


mframe_t parasite_frames_run [] =
{
	ai_run, 30, nullptr,
	ai_run, 30, nullptr,
	ai_run, 22, nullptr,
	ai_run, 19, nullptr,
	ai_run, 24, nullptr,
	ai_run, 28, nullptr,
	ai_run, 25, nullptr
};
mmove_t parasite_move_run = {FRAME_run03, FRAME_run09, parasite_frames_run, nullptr};

mframe_t parasite_frames_start_run [] =
{
	ai_run, 0,	nullptr,
	ai_run, 30, nullptr,
};
mmove_t parasite_move_start_run = {FRAME_run01, FRAME_run02, parasite_frames_start_run, parasite_run};

mframe_t parasite_frames_stop_run [] =
{	
	ai_run, 20, nullptr,
	ai_run, 20,	nullptr,
	ai_run, 12, nullptr,
	ai_run, 10, nullptr,
	ai_run, 0,  nullptr,
	ai_run, 0,  nullptr
};
mmove_t parasite_move_stop_run = {FRAME_run10, FRAME_run15, parasite_frames_stop_run, nullptr};

void parasite_start_run (edict_t *self)
{	
	self->monsterinfo.currentmove = &parasite_move_start_run;
}

void parasite_run (edict_t *self)
{
	self->monsterinfo.currentmove = &parasite_move_run;
}


mframe_t parasite_frames_walk [] =
{
	ai_walk, 30, nullptr,
	ai_walk, 30, nullptr,
	ai_walk, 22, nullptr,
	ai_walk, 19, nullptr,
	ai_walk, 24, nullptr,
	ai_walk, 28, nullptr,
	ai_walk, 25, nullptr
};
mmove_t parasite_move_walk = {FRAME_run03, FRAME_run09, parasite_frames_walk, parasite_walk};

mframe_t parasite_frames_start_walk [] =
{
	ai_walk, 0,	nullptr,
	ai_walk, 30, parasite_walk
};
mmove_t parasite_move_start_walk = {FRAME_run01, FRAME_run02, parasite_frames_start_walk, nullptr};

mframe_t parasite_frames_stop_walk [] =
{	
	ai_walk, 20, nullptr,
	ai_walk, 20,	nullptr,
	ai_walk, 12, nullptr,
	ai_walk, 10, nullptr,
	ai_walk, 0,  nullptr,
	ai_walk, 0,  nullptr
};
mmove_t parasite_move_stop_walk = {FRAME_run10, FRAME_run15, parasite_frames_stop_walk, nullptr};

void parasite_start_walk (edict_t *self)
{	
	self->monsterinfo.currentmove = &parasite_move_start_walk;
}

void parasite_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &parasite_move_walk;
}

void parasite_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	if (prandom(50))
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
}


void parasite_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
}

mframe_t parasite_frames_death [] =
{
	ai_move, 0,	 nullptr,
	ai_move, 0,	 nullptr,
	ai_move, 0,	 nullptr,
	ai_move, 0,	 nullptr,
	ai_move, 0,	 nullptr,
	ai_move, 0,	 nullptr,
	ai_move, 0,	 nullptr
};
mmove_t parasite_move_death = {FRAME_death101, FRAME_death107, parasite_frames_death, parasite_dead};

void parasite_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
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
	self->monsterinfo.currentmove = &parasite_move_death;
}

/*
===
End Death Stuff
===
*/

/*QUAKED monster_parasite (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_parasite (edict_t *self)
{
	sound_pain1 = gi.soundindex ("parasite/parpain1.wav");	
	sound_pain2 = gi.soundindex ("parasite/parpain2.wav");	
	sound_die = gi.soundindex ("parasite/pardeth1.wav");	
	sound_tap = gi.soundindex("parasite/paridle1.wav");
	sound_scratch = gi.soundindex("parasite/paridle2.wav");

	self->s.modelindex = gi.modelindex ("models/monsters/parasite/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 24);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	self->health = 175;
	self->gib_health = -50;
	self->mass = 250;

	self->pain = parasite_pain;
	self->die = parasite_die;

	self->monsterinfo.stand = parasite_stand;
	self->monsterinfo.walk = parasite_start_walk;
	self->monsterinfo.run = parasite_start_run;
	self->monsterinfo.idle = parasite_idle;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &parasite_move_stand;	
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	walkmonster_start (self);
}
