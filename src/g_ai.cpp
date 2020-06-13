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
// g_ai.c

#include "q_shared.h"

extern cvar_t	*maxclients;

//============================================================================

/*
=============
ai_move

Move the specified distance at current facing.
This replaces the QC functions: ai_forward, ai_back, ai_pain, and ai_painforward
==============
*/
void ai_move (edict_t *self, vec_t dist)
{
	if (self->control)
	{
		M_MoveToController (self, dist, true);
		return;
	}

	M_walkmove (self, self->s.angles[YAW], dist);
}


/*
=============
ai_stand

Used for standing around and looking for players
Distance is for slight position adjustments needed by the animations
==============
*/
void ai_stand (edict_t *self, vec_t dist)
{
	if (self->control)
	{
		M_MoveToController (self, dist, false);

		if (self->monsterinfo.idle && level.time > self->monsterinfo.idle_time)
		{
			bool ready_to_idle = ((self->control->client->latched_buttons | self->control->client->buttons) & BUTTON_ATTACK) && (level.time > self->monsterinfo.idle_time);
		
			if (ready_to_idle)
			{
				if (self->monsterinfo.idle_time)
				{
					self->monsterinfo.idle (self);
					self->monsterinfo.idle_time = level.time + 1;
				}
				else
					self->monsterinfo.idle_time = level.time + 1;
			}
		}

		return;
	}

	if (dist)
		M_walkmove (self, self->s.angles[YAW], dist);

	if (level.time > self->monsterinfo.pausetime)
	{
		self->monsterinfo.walk (self);
		return;
	}

	if (!(self->spawnflags & 1) && self->monsterinfo.idle)
	{		
		if (level.time > self->monsterinfo.idle_time)
		{
			if (self->monsterinfo.idle_time)
			{
				self->monsterinfo.idle (self);
				self->monsterinfo.idle_time = level.time + irandom(15, 30);
			}
			else
				self->monsterinfo.idle_time = level.time + irandom(15);
		}
	}
}


/*
=============
ai_walk

The monster is walking it's beat
=============
*/
void ai_walk (edict_t *self, vec_t dist)
{
	if (self->control)
	{
		M_MoveToController (self, dist, true);

		if (self->monsterinfo.search && level.time > self->monsterinfo.idle_time)
		{
			bool ready_to_idle = ((self->control->client->latched_buttons | self->control->client->buttons) & BUTTON_ATTACK) && (level.time > self->monsterinfo.idle_time);
		
			if (ready_to_idle)
			{
				if (self->monsterinfo.idle_time)
				{
					self->monsterinfo.search (self);
					self->monsterinfo.idle_time = level.time + 1;
				}
				else
					self->monsterinfo.idle_time = level.time + 1;
			}
		}

		return;
	}

	M_MoveToGoal (self, dist);

	if ((self->monsterinfo.search) && (level.time > self->monsterinfo.idle_time))
	{
		if (self->monsterinfo.idle_time)
		{
			self->monsterinfo.search (self);
			self->monsterinfo.idle_time = level.time + irandom(15, 30);
		}
		else
		{
			self->monsterinfo.idle_time = level.time + irandom(15);
		}
	}
}


/*
=============
ai_charge

Turns towards target and advances
Use this call with a distnace of 0 to replace ai_face
==============
*/
void ai_charge (edict_t *self, vec_t dist)
{
	if (self->control)
	{
		M_MoveToController (self, dist, true);
		return;
	}

	vec3_t	v;

	VectorSubtract (self->enemy->s.origin, self->s.origin, v);
	self->ideal_yaw = vectoyaw(v);
	M_ChangeYaw (self);

	if (dist)
		M_walkmove (self, self->s.angles[YAW], dist);
}

/*
=============
range

returns the range catagorization of an entity reletive to self
0	melee range, will become hostile even if back is turned
1	visibility and infront, or visibility and show hostile
2	infront and show hostile
3	only triggered by damage
=============
*/
range_t range (edict_t *self, edict_t *other)
{
	vec3_t	v;
	vec_t	len;

	VectorSubtract (self->s.origin, other->s.origin, v);
	len = VectorLength (v);
	if (len < MELEE_DISTANCE)
		return RANGE_MELEE;
	if (len < 500)
		return RANGE_NEAR;
	if (len < 1000)
		return RANGE_MID;
	return RANGE_FAR;
}

/*
=============
visible

returns 1 if the entity is visible to self, even if not infront ()
=============
*/
bool visible (edict_t *self, edict_t *other)
{
	vec3_t	spot1;
	vec3_t	spot2;
	trace_t	trace;

	VectorCopy (self->s.origin, spot1);
	spot1[2] += self->viewheight;
	VectorCopy (other->s.origin, spot2);
	spot2[2] += other->viewheight;
	trace = gi.trace (spot1, vec3_origin, vec3_origin, spot2, self, MASK_OPAQUE);
	
	if (trace.fraction == 1.0)
		return true;
	return false;
}


/*
=============
infront

returns 1 if the entity is in front (in sight) of self
=============
*/
bool infront (edict_t *self, edict_t *other)
{
	vec3_t	vec;
	vec_t	dot;
	vec3_t	forward;
	
	AngleVectors (self->s.angles, forward, nullptr, nullptr);
	VectorSubtract (other->s.origin, self->s.origin, vec);
	VectorNormalize (vec);
	dot = DotProduct (vec, forward);
	
	if (dot > 0.3)
		return true;
	return false;
}

//=============================================================================

/*
============
FacingIdeal

============
*/
bool FacingIdeal(edict_t *self)
{
	vec_t	delta;

	delta = anglemod(self->s.angles[YAW] - self->ideal_yaw);
	if (delta > 45 && delta < 315)
		return false;
	return true;
}


/*
=============
ai_run

The monster has an enemy it is trying to kill
=============
*/
void ai_run (edict_t *self, vec_t dist)
{
	if (self->control)
		M_MoveToController (self, dist, true);
	else
		M_MoveToGoal (self, dist);
}
