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
// m_move.c -- monster movement

#include "q_shared.h"

const vec_t STEPSIZE	= 18;

/*
=============
M_CheckBottom

Returns false if any part of the bottom of the entity is off an edge that
is not a staircase.

=============
*/
int32_t c_yes, c_no;

bool M_CheckBottom (edict_t *ent)
{
	vec3_t	mins, maxs, start, stop;
	trace_t	trace;
	int32_t		x, y;
	vec_t	mid, bottom;
	
	VectorAdd (ent->s.origin, ent->mins, mins);
	VectorAdd (ent->s.origin, ent->maxs, maxs);

// if all of the points under the corners are solid world, don't bother
// with the tougher checks
// the corners must be within 16 of the midpoint
	start[2] = mins[2] - 1;
	for	(x=0 ; x<=1 ; x++)
		for	(y=0 ; y<=1 ; y++)
		{
			start[0] = x ? maxs[0] : mins[0];
			start[1] = y ? maxs[1] : mins[1];
			if (gi.pointcontents (start) != CONTENTS_SOLID)
				goto realcheck;
		}

	c_yes++;
	return true;		// we got out easy

realcheck:
	c_no++;
//
// check it for real...
//
	start[2] = mins[2];
	
// the midpoint must be within 16 of the bottom
	start[0] = stop[0] = (mins[0] + maxs[0])*0.5;
	start[1] = stop[1] = (mins[1] + maxs[1])*0.5;
	stop[2] = start[2] - 2*STEPSIZE;
	trace = gi.trace (start, vec3_origin, vec3_origin, stop, ent, MASK_MONSTERSOLID);

	if (trace.fraction == 1.0)
		return false;
	mid = bottom = trace.endpos[2];
	
// the corners must be within 16 of the midpoint	
	for	(x=0 ; x<=1 ; x++)
		for	(y=0 ; y<=1 ; y++)
		{
			start[0] = stop[0] = x ? maxs[0] : mins[0];
			start[1] = stop[1] = y ? maxs[1] : mins[1];
			
			trace = gi.trace (start, vec3_origin, vec3_origin, stop, ent, MASK_MONSTERSOLID);
			
			if (trace.fraction != 1.0 && trace.endpos[2] > bottom)
				bottom = trace.endpos[2];
			if (trace.fraction == 1.0 || mid - trace.endpos[2] > STEPSIZE)
				return false;
		}

	c_yes++;
	return true;
}


/*
=============
SV_movestep

Called by monster program code.
The move will be adjusted for slopes and stairs, but if the move isn't
possible, no move is done, false is returned, and
pr_global_struct->trace_normal is set to the normal of the blocking wall
=============
*/
//FIXME since we need to test end position contents here, can we avoid doing
//it again later in catagorize position?
bool SV_movestep (edict_t *ent, const vec3_t move, bool relink)
{
	vec_t		dz;
	vec3_t		oldorg, neworg, end;
	trace_t		trace;
	int32_t			i;
	vec_t		stepsize;
	vec3_t		test;
	brushcontents_t contents;

// try the move	
	VectorCopy (ent->s.origin, oldorg);
	VectorAdd (ent->s.origin, move, neworg);

// flying monsters don't step up
	if ( ent->flags & (FL_SWIM | FL_FLY) )
	{
	// try one move with vertical motion, then one without
		for (i=0 ; i<2 ; i++)
		{
			VectorAdd (ent->s.origin, move, neworg);
			if (i == 0 && ent->enemy)
			{
				if (!ent->goalentity)
					ent->goalentity = ent->enemy;
				dz = ent->s.origin[2] - ent->goalentity->s.origin[2];
				if (ent->goalentity->client)
				{
					if (dz > 40)
						neworg[2] -= 8;
					if (!((ent->flags & FL_SWIM) && (ent->waterlevel < WATER_WAIST)))
						if (dz < 30)
							neworg[2] += 8;
				}
				else
				{
					if (dz > 8)
						neworg[2] -= 8;
					else if (dz > 0)
						neworg[2] -= dz;
					else if (dz < -8)
						neworg[2] += 8;
					else
						neworg[2] += dz;
				}
			}
			trace = gi.trace (ent->s.origin, ent->mins, ent->maxs, neworg, ent, MASK_MONSTERSOLID);
	
			/*// fly monsters don't enter water voluntarily
			if (ent->flags & FL_FLY)
			{
				if (!ent->waterlevel)
				{
					test[0] = trace.endpos[0];
					test[1] = trace.endpos[1];
					test[2] = trace.endpos[2] + ent->mins[2] + 1;
					contents = gi.pointcontents(test);
					if (contents & MASK_WATER)
						return false;
				}
			}

			// swim monsters don't exit water voluntarily
			if (ent->flags & FL_SWIM)
			{
				if (ent->waterlevel < WATER_WAIST)
				{
					test[0] = trace.endpos[0];
					test[1] = trace.endpos[1];
					test[2] = trace.endpos[2] + ent->mins[2] + 1;
					contents = gi.pointcontents(test);
					if (!(contents & MASK_WATER))
						return false;
				}
			}*/

			if (trace.fraction == 1)
			{
				VectorCopy (trace.endpos, ent->s.origin);
				if (relink)
				{
					gi.linkentity (ent);
					G_TouchTriggers (ent);
				}
				return true;
			}
			
			if (!ent->enemy)
				break;
		}
		
		return false;
	}

// push down from a step height above the wished position
	if (!(ent->monsterinfo.aiflags & AI_NOSTEP))
		stepsize = STEPSIZE;
	else
		stepsize = 1;

	neworg[2] += stepsize;
	VectorCopy (neworg, end);
	end[2] -= stepsize*2;

	trace = gi.trace (neworg, ent->mins, ent->maxs, end, ent, MASK_MONSTERSOLID);

	if (trace.allsolid)
		return false;

	if (trace.startsolid)
	{
		neworg[2] -= stepsize;
		trace = gi.trace (neworg, ent->mins, ent->maxs, end, ent, MASK_MONSTERSOLID);
		if (trace.allsolid || trace.startsolid)
			return false;
	}


	// don't go in to water
	/*if (ent->waterlevel == WATER_NONE)
	{
		test[0] = trace.endpos[0];
		test[1] = trace.endpos[1];
		test[2] = trace.endpos[2] + ent->mins[2] + 1;	
		contents = gi.pointcontents(test);

		if (contents & MASK_WATER)
			return false;
	}*/

	/*if (trace.fraction == 1)
	{
	// if monster had the ground pulled out, go ahead and fall
		if ( ent->flags & FL_PARTIALGROUND )
		{
			VectorAdd (ent->s.origin, move, ent->s.origin);
			if (relink)
			{
				gi.linkentity (ent);
				G_TouchTriggers (ent);
			}
			ent->groundentity = nullptr;
			return true;
		}
	
		return false;		// walked off an edge
	}*/

// check point traces down for dangling corners
	VectorCopy (trace.endpos, ent->s.origin);
	
	/*if (!M_CheckBottom (ent))
	{
		if ( ent->flags & FL_PARTIALGROUND )
		{	// entity had floor mostly pulled out from underneath it
			// and is trying to correct
			if (relink)
			{
				gi.linkentity (ent);
				G_TouchTriggers (ent);
			}
			return true;
		}
		VectorCopy (oldorg, ent->s.origin);
		return false;
	}

	if ( ent->flags & FL_PARTIALGROUND )
	{
		ent->flags &= ~FL_PARTIALGROUND;
	}*/
	ent->groundentity = trace.ent;
	ent->groundentity_linkcount = trace.ent->linkcount;

// the move is ok
	if (relink)
	{
		gi.linkentity (ent);
		G_TouchTriggers (ent);
	}
	return true;
}


//============================================================================

/*
===============
M_ChangeYaw

===============
*/
void M_ChangeYaw (edict_t *ent)
{
	vec_t	ideal;
	vec_t	current;
	vec_t	move;
	vec_t	speed;
	
	current = anglemod(ent->s.angles[YAW]);
	ideal = ent->ideal_yaw;

	if (current == ideal)
		return;

	move = ideal - current;
	speed = ent->yaw_speed;
	if (ideal > current)
	{
		if (move >= 180)
			move = move - 360;
	}
	else
	{
		if (move <= -180)
			move = move + 360;
	}
	if (move > 0)
	{
		if (move > speed)
			move = speed;
	}
	else
	{
		if (move < -speed)
			move = -speed;
	}
	
	ent->s.angles[YAW] = anglemod (current + move);
}


/*
======================
SV_StepDirection

Turns to the movement direction, and walks the current distance if
facing it.

======================
*/
bool SV_StepDirection (edict_t *ent, vec_t yaw, vec_t dist)
{
	vec3_t		move, oldorigin;
	vec_t		delta;
	
	ent->ideal_yaw = yaw;
	M_ChangeYaw (ent);
	
	yaw = yaw*M_PI*2 / 360;
	move[0] = cos(yaw)*dist;
	move[1] = sin(yaw)*dist;
	move[2] = 0;

	VectorCopy (ent->s.origin, oldorigin);
	if (SV_movestep (ent, move, false))
	{
		delta = ent->s.angles[YAW] - ent->ideal_yaw;
		if (delta > 45 && delta < 315)
		{		// not turned far enough, so don't take the step
			VectorCopy (oldorigin, ent->s.origin);
		}
		gi.linkentity (ent);
		G_TouchTriggers (ent);
		return true;
	}
	gi.linkentity (ent);
	G_TouchTriggers (ent);
	return false;
}

/*
======================
SV_FixCheckBottom

======================
*/
void SV_FixCheckBottom (edict_t *ent)
{
	ent->flags |= FL_PARTIALGROUND;
}



/*
================
SV_NewChaseDir

================
*/

const int32_t DI_NODIR	= -1;

void SV_NewChaseDir (edict_t *actor, edict_t *enemy, vec_t dist)
{
	vec_t	deltax,deltay;
	vec_t	d[3];
	vec_t	tdir, olddir, turnaround;

	//FIXME: how did we get here with no enemy
	if (!enemy)
		return;

	olddir = anglemod( (int32_t)(actor->ideal_yaw/45)*45 );
	turnaround = anglemod(olddir - 180);

	deltax = enemy->s.origin[0] - actor->s.origin[0];
	deltay = enemy->s.origin[1] - actor->s.origin[1];
	if (deltax>10)
		d[1]= 0;
	else if (deltax<-10)
		d[1]= 180;
	else
		d[1]= DI_NODIR;
	if (deltay<-10)
		d[2]= 270;
	else if (deltay>10)
		d[2]= 90;
	else
		d[2]= DI_NODIR;

// try direct route
	if (d[1] != DI_NODIR && d[2] != DI_NODIR)
	{
		if (d[1] == 0)
			tdir = d[2] == 90 ? 45 : 315;
		else
			tdir = d[2] == 90 ? 135 : 215;
			
		if (tdir != turnaround && SV_StepDirection(actor, tdir, dist))
			return;
	}

// try other directions
	if (prandom(50) || abs(deltay) > abs(deltax))
	{
		tdir=d[1];
		d[1]=d[2];
		d[2]=tdir;
	}

	if (d[1]!=DI_NODIR && d[1]!=turnaround 
	&& SV_StepDirection(actor, d[1], dist))
			return;

	if (d[2]!=DI_NODIR && d[2]!=turnaround
	&& SV_StepDirection(actor, d[2], dist))
			return;

/* there is no direct path to the player, so pick another direction */

	if (olddir!=DI_NODIR && SV_StepDirection(actor, olddir, dist))
			return;

	if (prandom(50)) 	/*randomly determine direction of search*/
	{
		for (tdir=0 ; tdir<=315 ; tdir += 45)
			if (tdir!=turnaround && SV_StepDirection(actor, tdir, dist) )
					return;
	}
	else
	{
		for (tdir=315 ; tdir >=0 ; tdir -= 45)
			if (tdir!=turnaround && SV_StepDirection(actor, tdir, dist) )
					return;
	}

	if (turnaround != DI_NODIR && SV_StepDirection(actor, turnaround, dist) )
			return;

	actor->ideal_yaw = olddir;		// can't move

// if a bridge was pulled out from underneath a monster, it may not have
// a valid standing position at all

	if (!M_CheckBottom (actor))
		SV_FixCheckBottom (actor);
}

/*
======================
SV_CloseEnough

======================
*/
bool SV_CloseEnough (edict_t *ent, edict_t *goal, vec_t dist)
{
	int32_t		i;
	
	for (i=0 ; i<3 ; i++)
	{
		if (goal->absmin[i] > ent->absmax[i] + dist)
			return false;
		if (goal->absmax[i] < ent->absmin[i] - dist)
			return false;
	}
	return true;
}


/*
======================
M_MoveToGoal
======================
*/
void M_MoveToGoal (edict_t *ent, vec_t dist)
{
	edict_t		*goal;
	
	goal = ent->goalentity;

	if (!ent->groundentity && !(ent->flags & (FL_FLY|FL_SWIM)))
		return;

// if the next step hits the enemy, return immediately
	if (ent->enemy &&  SV_CloseEnough (ent, ent->enemy, dist) )
		return;

// bump around...
	if (prandom(25) || !SV_StepDirection (ent, ent->ideal_yaw, dist))
	{
		if (ent->inuse)
			SV_NewChaseDir (ent, goal, dist);
	}
}

void Pmove (m_pmove_t *pmove);

edict_t	*pm_m_passent;

// pmove doesn't need to know about passent and contentmask
trace_t	PM_m_trace (vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end)
{
	if (pm_m_passent->health > 0)
		return gi.trace (start, mins, maxs, end, pm_m_passent, MASK_MONSTERSOLID);
	else
		return gi.trace (start, mins, maxs, end, pm_m_passent, MASK_DEADSOLID);
}

void M_MoveToController (edict_t *self, vec_t dist)
{
	edict_t *ent = self->control;

	const vec3_t view_angles = {
		0, ent->client->resp.cmd_angles[YAW] + SHORT2ANGLE(ent->client->ps.pmove.delta_angles[YAW]), 0
	};

	if (dist >= 0)
	{
		self->control_dist = dist;
		self->ideal_yaw = view_angles[YAW];
		M_ChangeYaw (self);
	}

	if (dist == -1 && ent->client->control_pmove)
	{
		pm_m_passent = self;

		m_pmove_t pm;

		// set up for pmove
		memset (&pm, 0, sizeof(pm));

		ent->pmove_state.pm_type = PM_NORMAL;
		ent->pmove_state.gravity = sv_gravity->value;

		pm.s = ent->pmove_state;
		
		VectorCopy(self->s.origin, pm.origin_f);
		VectorCopy(self->velocity, pm.velocity_f);
		
		//pm.snapinitial = true;

		VectorCopy(self->mins, pm.mins);
		VectorCopy(self->maxs, pm.maxs);

		//if (memcmp(&self->old_pmove_state, &pm.s, sizeof(pm.s)))
		//{
		//	pm.snapinitial = true;
			//gi.dprintf ("pmove changed!\n");
		//}

		pm.cmd = ent->client->cmd;
		pm.cmd.msec = 100;

		vec_t len = max(abs(pm.cmd.forwardmove), abs(pm.cmd.sidemove));

		if (len)
		{
			if (pm.cmd.forwardmove)
				pm.forwardmove_f = self->control_dist * (pm.cmd.forwardmove / len);
			if (pm.cmd.sidemove)
				pm.sidemove_f = self->control_dist * (pm.cmd.sidemove / len);
		}
		
		for (int32_t i = 0; i < 3; i++)
			pm.cmd.angles[i] = ANGLE2SHORT(view_angles[i]);

		VectorClear(pm.s.delta_angles);

		pm.trace = PM_m_trace;	// adds default parms
		pm.pointcontents = gi.pointcontents;

		// perform a pmove
		Pmove (&pm);

		// save results of pmove
		self->pmove_state = pm.s;
		self->old_pmove_state = pm.s;
		
		VectorCopy(pm.origin_f, self->s.origin);
		VectorCopy(pm.velocity_f, self->velocity);

		self->viewheight = pm.viewheight;
		self->waterlevel = pm.waterlevel;
		self->watertype = pm.watertype;
		self->groundentity = pm.groundentity;
		if (pm.groundentity)
			self->groundentity_linkcount = pm.groundentity->linkcount;

		gi.linkentity (self);

		G_TouchTriggers (self);

		// touch other objects
		for (int32_t i = 0; i < pm.numtouch; i++)
		{
			int32_t j = 0;
			edict_t *other = pm.touchents[i];
			
			for (j = 0; j < i; j++)
			{
				if (pm.touchents[j] == other)
					break;
			}

			if (j != i)
				continue;	// duplicated

			if (!other->touch)
				continue;

			other->touch (other, self, nullptr, nullptr);
		}

		return;
	}

	if (dist < 0 || ent->client->control_pmove)
		return;

	pm_m_passent = self;

	vec3_t forward, right;

	AngleVectors(view_angles, forward, right, NULL);

	vec3_t wanted = { ent->client->cmd.forwardmove, ent->client->cmd.sidemove, 0 };
	vec_t wanted_len = VectorNormalize(wanted);

	if (wanted_len)
	{
		vec3_t move;

		for (int32_t i = 0; i < 3; i++)
			move[i] = forward[i] * wanted[0] * dist + right[i] * wanted[1] * dist;
	
		vec3_t oldorigin;
		VectorCopy (self->s.origin, oldorigin);

		SV_movestep (self, move, false);

		gi.linkentity (self);
		G_TouchTriggers (self);
	}
}

/*
===============
M_walkmove
===============
*/
bool M_walkmove (edict_t *ent, vec_t yaw, vec_t dist)
{
	vec3_t	move;
	
	if (!ent->groundentity && !(ent->flags & (FL_FLY|FL_SWIM)))
		return false;

	yaw = yaw*M_PI*2 / 360;
	
	move[0] = cos(yaw)*dist;
	move[1] = sin(yaw)*dist;
	move[2] = 0;

	return SV_movestep(ent, move, true);
}
