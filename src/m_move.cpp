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
==================
PM_ClipVelocity

Slide off of the impacting object
returns the blocked flags (1 = floor, 2 = step / wall)
==================
*/
#define	STOP_EPSILON	0.1f

static void M_ClipVelocity (const vec3_t in, const vec3_t normal, vec3_t out, vec_t overbounce)
{
	vec_t	backoff, change;
	int32_t		i;
	
	backoff = DotProduct (in, normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i]*backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0;
	}
}

/*
==================
PM_StepSlideMove

Each intersection will try to step over the obstruction instead of
sliding along it.

Returns a new origin, velocity, and contact entity
Does not modify any world state?
==================
*/
const vec_t	MIN_STEP_NORMAL	= 0.7f;		// can't step up onto very steep slopes
const size_t MAX_CLIP_PLANES	= 5;
static void M_StepSlideMove_ (edict_t *ent, vec3_t velocity)
{
	int32_t			bumpcount, numbumps = 4;
	vec3_t		dir;
	vec_t		d;
	int32_t			numplanes = 0;
	vec3_t		planes[MAX_CLIP_PLANES];
	vec3_t		primal_velocity;
	int32_t			i, j;
	trace_t	trace;
	vec3_t		end;
	vec_t		time_left;
	
	
	VectorCopy (velocity, primal_velocity);
	time_left = 1.0;

	for (bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		end[0] = ent->s.origin[0] + time_left * velocity[0];
		end[1] = ent->s.origin[1] + time_left * velocity[1];
		end[2] = ent->s.origin[2] + time_left * velocity[2];

		trace = gi.trace (ent->s.origin, ent->mins, ent->maxs, end, ent, MASK_MONSTERSOLID);

		if (trace.allsolid)
		{	// entity is trapped in another solid
			velocity[2] = 0;	// don't build up falling damage
			return;
		}

		if (trace.fraction > 0)
		{	// actually covered some distance
			VectorCopy (trace.endpos, ent->s.origin);
			numplanes = 0;
		}

		if (trace.fraction == 1)
			 break;		// moved the entire distance

		// save entity for contact
		/*if (pm->numtouch < MAXTOUCH && trace.ent)
		{
			pm->touchents[pm->numtouch] = trace.ent;
			pm->numtouch++;
		}*/
		
		time_left -= time_left * trace.fraction;

		// slide along this plane
		if (numplanes >= MAX_CLIP_PLANES)
		{	// this shouldn't really happen
			VectorClear(velocity);
			break;
		}

		VectorCopy (trace.plane.normal, planes[numplanes]);
		numplanes++;


//
// modify original_velocity so it parallels all of the clip planes
//
		for (i = 0; i < numplanes; i++)
		{
			M_ClipVelocity (velocity, planes[i], velocity, 1.01f);
			for (j=0 ; j<numplanes ; j++) {
				if (j != i) {
					if (DotProduct(velocity, planes[j]) < 0)
						break;	// not ok
				}
			}
			if (j == numplanes)
				break;
		}
		
		if (i != numplanes)
		{	// go along this plane
		}
		else
		{	// go along the crease
			if (numplanes != 2)
			{
//				Con_Printf ("clip velocity, numplanes == %i\n",numplanes);
				VectorClear(velocity);
				break;
			}
			CrossProduct (planes[0], planes[1], dir);
			d = DotProduct (dir, velocity);
			VectorScale (dir, d, velocity);
		}

		// if velocity is against the original velocity, stop dead
		// to avoid tiny occilations in sloping corners
		//
		if (DotProduct (velocity, primal_velocity) <= 0)
		{
			VectorClear(velocity);
			break;
		}
	}

	VectorCopy (primal_velocity, velocity);
}

/*
==================
PM_StepSlideMove

==================
*/
static bool M_StepSlideMove (edict_t *ent, vec3_t velocity)
{
	vec3_t		start_o, start_v;
	vec3_t		down_o, down_v;
	trace_t		trace;
	vec_t		down_dist, up_dist;
	vec3_t		up, down;

	VectorCopy (ent->s.origin, start_o);
	VectorCopy (velocity, start_v);

	M_StepSlideMove_ (ent, velocity);

	VectorCopy (ent->s.origin, down_o);
	VectorCopy (velocity, down_v);

	VectorCopy (start_o, up);
	up[2] += STEPSIZE;

	trace = gi.trace (up, ent->mins, ent->maxs, up, ent, MASK_MONSTERSOLID);
	if (trace.allsolid)
		return false;		// can't step up

	// try sliding above
	VectorCopy (up, ent->s.origin);
	VectorCopy (start_v, velocity);

	M_StepSlideMove_ (ent, velocity);

	// push down the final amount
	VectorCopy (ent->s.origin, down);
	down[2] -= STEPSIZE;
	trace = gi.trace (ent->s.origin, ent->mins, ent->maxs, down, ent, MASK_MONSTERSOLID);
	if (!trace.allsolid)
	{
		VectorCopy (trace.endpos, ent->s.origin);
	}

	VectorCopy(ent->s.origin, up);

	// decide which one went farther
    down_dist = (down_o[0] - start_o[0])*(down_o[0] - start_o[0]) + (down_o[1] - start_o[1])*(down_o[1] - start_o[1]);
    up_dist = (up[0] - start_o[0])*(up[0] - start_o[0]) + (up[1] - start_o[1])*(up[1] - start_o[1]);

	if (down_dist > up_dist || trace.plane.normal[2] < MIN_STEP_NORMAL)
	{
		VectorCopy (down_o, ent->s.origin);
		VectorCopy (down_v, velocity);
		return true;
	}
	//!! Special case
	// if we were walking along a plane, then we need to copy the Z over
	velocity[2] = down_v[2];
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
	vec3_t move_vel = { move[0], move[1], ent->velocity[2] };

	if (M_StepSlideMove(ent, move_vel))
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

void M_MoveToController (edict_t *self, vec_t dist, bool turn)
{
	edict_t *ent = self->control;

	if (!ent)
		return;

	const vec3_t view_angles = {
		0, ent->client->resp.cmd_angles[YAW] + SHORT2ANGLE(ent->client->ps.pmove.delta_angles[YAW]), 0
	};

	if (turn && dist >= 0)
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

		if (ent->client->control_waitjump)
			pm.cmd.upmove = 0;

		pm.forwardmove_f = pm.cmd.forwardmove;
		pm.sidemove_f = pm.cmd.sidemove;

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

	if (ent->client->control_pmove)
		return;

	if (dist > 0)
	{
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
