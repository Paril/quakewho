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


static soundindex_t	sound_swing;
static soundindex_t	sound_hit;
static soundindex_t	sound_hit2;
static soundindex_t	sound_death;
static soundindex_t	sound_idle;
static soundindex_t	sound_pain1;
static soundindex_t	sound_pain2;
static soundindex_t	sound_sight;
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

void mutant_sight (edict_t *self, edict_t *other)
{
	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void mutant_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}

void mutant_swing (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_swing, 1, ATTN_NORM, 0);
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
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &mutant_move_stand;
	else
		self->monsterinfo.currentmove = &mutant_move_run;
}


//
// MELEE
//

void mutant_hit_left (edict_t *self)
{
	vec3_t	aim;

	VectorSet (aim, MELEE_DISTANCE, self->mins[0], 8);
	if (fire_hit (self, aim, irandom(10, 14), 100))
		gi.sound (self, CHAN_WEAPON, sound_hit, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_WEAPON, sound_swing, 1, ATTN_NORM, 0);
}

void mutant_hit_right (edict_t *self)
{
	vec3_t	aim;

	VectorSet (aim, MELEE_DISTANCE, self->maxs[0], 8);
	if (fire_hit (self, aim, irandom(10, 14), 100))
		gi.sound (self, CHAN_WEAPON, sound_hit2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_WEAPON, sound_swing, 1, ATTN_NORM, 0);
}

void mutant_check_refire (edict_t *self)
{
	if (!self->enemy || !self->enemy->inuse || self->enemy->health <= 0)
		return;

	if ( ((skill->value == 3) && prandom(50)) || (range(self, self->enemy) == RANGE_MELEE) )
		self->monsterinfo.nextframe = FRAME_attack09;
}

mframe_t mutant_frames_attack [] =
{
	ai_charge,	0,	nullptr,
	ai_charge,	0,	nullptr,
	ai_charge,	0,	mutant_hit_left,
	ai_charge,	0,	nullptr,
	ai_charge,	0,	nullptr,
	ai_charge,	0,	mutant_hit_right,
	ai_charge,	0,	mutant_check_refire
};
mmove_t mutant_move_attack = {FRAME_attack09, FRAME_attack15, mutant_frames_attack, mutant_run};

void mutant_melee (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_attack;
}


//
// ATTACK
//

void mutant_jump_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (self->health <= 0)
	{
		self->touch = nullptr;
		return;
	}

	if (other->takedamage)
	{
		if (VectorLength(self->velocity) > 400)
		{
			vec3_t	point;
			vec3_t	normal;
			int32_t		damage;

			VectorCopy (self->velocity, normal);
			VectorNormalize(normal);
			VectorMA (self->s.origin, self->maxs[0], normal, point);
			damage = irandom(40, 50);
			T_Damage (other, self, self, self->velocity, point, normal, damage, damage, DAMAGE_NONE, MOD_UNKNOWN);
		}
	}

	if (!M_CheckBottom (self))
	{
		if (self->groundentity)
		{
			self->monsterinfo.nextframe = FRAME_attack02;
			self->touch = nullptr;
		}
		return;
	}

	self->touch = nullptr;
}

void mutant_jump_takeoff (edict_t *self)
{
	vec3_t	forward;

	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
	AngleVectors (self->s.angles, forward, nullptr, nullptr);
	self->s.origin[2] += 1;
	VectorScale (forward, 600, self->velocity);
	self->velocity[2] = 250;
	self->groundentity = nullptr;
	self->monsterinfo.aiflags |= AI_DUCKED;
	self->monsterinfo.attack_finished = level.time + 3;
	self->touch = mutant_jump_touch;
}

void mutant_check_landing (edict_t *self)
{
	if (self->groundentity)
	{
		gi.sound (self, CHAN_WEAPON, sound_thud, 1, ATTN_NORM, 0);
		self->monsterinfo.attack_finished = 0;
		self->monsterinfo.aiflags &= ~AI_DUCKED;
		return;
	}

	if (level.time > self->monsterinfo.attack_finished)
		self->monsterinfo.nextframe = FRAME_attack02;
	else
		self->monsterinfo.nextframe = FRAME_attack05;
}

mframe_t mutant_frames_jump [] =
{
	ai_charge,	 0,	nullptr,
	ai_charge,	17,	nullptr,
	ai_charge,	15,	mutant_jump_takeoff,
	ai_charge,	15,	nullptr,
	ai_charge,	15,	mutant_check_landing,
	ai_charge,	 0,	nullptr,
	ai_charge,	 3,	nullptr,
	ai_charge,	 0,	nullptr
};
mmove_t mutant_move_jump = {FRAME_attack01, FRAME_attack08, mutant_frames_jump, mutant_run};

void mutant_jump (edict_t *self)
{
	self->monsterinfo.currentmove = &mutant_move_jump;
}


//
// CHECKATTACK
//

bool mutant_check_melee (edict_t *self)
{
	if (range (self, self->enemy) == RANGE_MELEE)
		return true;
	return false;
}

bool mutant_check_jump (edict_t *self)
{
	vec3_t	v;
	vec_t	distance;

	if (self->absmin[2] > (self->enemy->absmin[2] + 0.75 * self->enemy->size[2]))
		return false;

	if (self->absmax[2] < (self->enemy->absmin[2] + 0.25 * self->enemy->size[2]))
		return false;

	v[0] = self->s.origin[0] - self->enemy->s.origin[0];
	v[1] = self->s.origin[1] - self->enemy->s.origin[1];
	v[2] = 0;
	distance = VectorLength(v);

	if (distance < 100)
		return false;
	if (distance > 100)
	{
		if (prandom(90))
			return false;
	}

	return true;
}

bool mutant_checkattack (edict_t *self)
{
	if (!self->enemy || self->enemy->health <= 0)
		return false;

	if (mutant_check_melee(self))
	{
		self->monsterinfo.attack_state = AS_MELEE;
		return true;
	}

	if (mutant_check_jump(self))
	{
		self->monsterinfo.attack_state = AS_MISSILE;
		// FIXME play a jump sound here
		return true;
	}

	return false;
}


//
// PAIN
//

mframe_t mutant_frames_pain1 [] =
{
	ai_move,	4,	nullptr,
	ai_move,	-3,	nullptr,
	ai_move,	-8,	nullptr,
	ai_move,	2,	nullptr,
	ai_move,	5,	nullptr
};
mmove_t mutant_move_pain1 = {FRAME_pain101, FRAME_pain105, mutant_frames_pain1, mutant_run};

mframe_t mutant_frames_pain2 [] =
{
	ai_move,	-24,nullptr,
	ai_move,	11,	nullptr,
	ai_move,	5,	nullptr,
	ai_move,	-2,	nullptr,
	ai_move,	6,	nullptr,
	ai_move,	4,	nullptr
};
mmove_t mutant_move_pain2 = {FRAME_pain201, FRAME_pain206, mutant_frames_pain2, mutant_run};

mframe_t mutant_frames_pain3 [] =
{
	ai_move,	-22,nullptr,
	ai_move,	3,	nullptr,
	ai_move,	3,	nullptr,
	ai_move,	2,	nullptr,
	ai_move,	1,	nullptr,
	ai_move,	1,	nullptr,
	ai_move,	6,	nullptr,
	ai_move,	3,	nullptr,
	ai_move,	2,	nullptr,
	ai_move,	0,	nullptr,
	ai_move,	1,	nullptr
};
mmove_t mutant_move_pain3 = {FRAME_pain301, FRAME_pain311, mutant_frames_pain3, mutant_run};

void mutant_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage)
{
	if (self->health < (self->max_health / 2))
		self->s.skinnum = 1;

	if (level.time < self->pain_debounce_time)
		return;

	self->pain_debounce_time = level.time + 3;

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	int32_t r = irandom(2);
	if (r == 0)
	{
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &mutant_move_pain1;
	}
	else if (r == 1)
	{
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &mutant_move_pain2;
	}
	else
	{
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
		self->monsterinfo.currentmove = &mutant_move_pain3;
	}
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

	M_FlyCheck (self);
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

void mutant_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, vec3_t point)
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
	self->s.skinnum = 1;

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
	if (deathmatch->value)
	{
		G_FreeEdict (self);
		return;
	}

	sound_swing = gi.soundindex ("mutant/mutatck1.wav");
	sound_hit = gi.soundindex ("mutant/mutatck2.wav");
	sound_hit2 = gi.soundindex ("mutant/mutatck3.wav");
	sound_death = gi.soundindex ("mutant/mutdeth1.wav");
	sound_idle = gi.soundindex ("mutant/mutidle1.wav");
	sound_pain1 = gi.soundindex ("mutant/mutpain1.wav");
	sound_pain2 = gi.soundindex ("mutant/mutpain2.wav");
	sound_sight = gi.soundindex ("mutant/mutsght1.wav");
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
	self->monsterinfo.dodge = nullptr;
	self->monsterinfo.attack = mutant_jump;
	self->monsterinfo.melee = mutant_melee;
	self->monsterinfo.sight = mutant_sight;
	self->monsterinfo.search = mutant_search;
	self->monsterinfo.idle = mutant_idle;
	self->monsterinfo.checkattack = mutant_checkattack;

	gi.linkentity (self);
	
	self->monsterinfo.currentmove = &mutant_move_stand;

	self->monsterinfo.scale = MODEL_SCALE;
	walkmonster_start (self);
}
