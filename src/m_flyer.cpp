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

flyer

==============================================================================
*/

#include "q_shared.h"
#include "m_flyer.h"

static soundindex_t	sound_idle;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_die;


void flyer_stand (edict_t *self);

void flyer_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

mframe_t flyer_frames_stand [] =
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
	ai_stand, 0, nullptr
};
mmove_t	flyer_move_stand = {FRAME_stand01, FRAME_stand45, flyer_frames_stand, nullptr};


mframe_t flyer_frames_walk [] =
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
	ai_walk, 5, nullptr
};
mmove_t	flyer_move_walk = {FRAME_stand01, FRAME_stand45, flyer_frames_walk, nullptr};

mframe_t flyer_frames_run [] =
{
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr,
	ai_run, 10, nullptr
};
mmove_t	flyer_move_run = {FRAME_stand01, FRAME_stand45, flyer_frames_run, nullptr};

void flyer_run (edict_t *self)
{
	self->monsterinfo.currentmove = &flyer_move_run;
}

void flyer_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &flyer_move_walk;
}

void flyer_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &flyer_move_stand;
}

void flyer_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	int32_t		n;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	n = irandom(2);
	if (n == 0)
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else if (n == 1)
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
}


void flyer_die(edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point)
{
	gi.sound (self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	BecomeExplosion1(self);
}
	

/*QUAKED monster_flyer (1 .5 0) (-16 -16 -24) (16 16 32) Ambush Trigger_Spawn Sight
*/
void SP_monster_flyer (edict_t *self)
{
	// fix a map bug in jail5.bsp
	if (!stricmp(level.mapname, "jail5") && (self->s.origin[2] == -104))
	{
		self->targetname = self->target;
		self->target = nullptr;
	}

	sound_idle = gi.soundindex ("flyer/flysrch1.wav");
	sound_pain1 = gi.soundindex ("flyer/flypain1.wav");
	sound_pain2 = gi.soundindex ("flyer/flypain2.wav");
	sound_die = gi.soundindex ("flyer/flydeth1.wav");

	self->s.modelindex = gi.modelindex ("models/monsters/flyer/tris.md2");
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, 32);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	self->s.sound = gi.soundindex ("flyer/flyidle1.wav");

	self->health = 50;
	self->mass = 50;

	self->pain = flyer_pain;
	self->die = flyer_die;

	self->monsterinfo.stand = flyer_stand;
	self->monsterinfo.walk = flyer_walk;
	self->monsterinfo.run = flyer_run;
	self->monsterinfo.idle = flyer_idle;

	gi.linkentity (self);

	self->monsterinfo.currentmove = &flyer_move_stand;	
	self->monsterinfo.scale = MODEL_SCALE;
	self->monsterinfo.damaged_skin = 1;

	flymonster_start (self);
}
