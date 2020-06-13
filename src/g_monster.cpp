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

#include "q_shared.h"

bool M_FidgetCheck (edict_t *ent, int percent)
{
	if (ent->control && ((ent->control->client->latched_buttons | ent->control->client->buttons) & BUTTON_ATTACK))
		return true;

	return prandom(percent);
}

void M_CheckGround (edict_t *ent)
{
	vec3_t		point;
	trace_t		trace;

	if (ent->flags & (FL_SWIM|FL_FLY))
		return;

	if (ent->velocity[2] > 100)
	{
		ent->groundentity = nullptr;
		return;
	}

// if the hull point one-quarter unit down is solid the entity is on ground
	point[0] = ent->s.origin[0];
	point[1] = ent->s.origin[1];
	point[2] = ent->s.origin[2] - 0.25;

	trace = gi.trace (ent->s.origin, ent->mins, ent->maxs, point, ent, MASK_MONSTERSOLID);

	// check steepness
	if ( trace.plane.normal[2] < 0.7 && !trace.startsolid)
	{
		ent->groundentity = nullptr;
		return;
	}

//	ent->groundentity = trace.ent;
//	ent->groundentity_linkcount = trace.ent->linkcount;
//	if (!trace.startsolid && !trace.allsolid)
//		VectorCopy (trace.endpos, ent->s.origin);
	if (!trace.startsolid && !trace.allsolid)
	{
		VectorCopy (trace.endpos, ent->s.origin);
		ent->groundentity = trace.ent;
		ent->groundentity_linkcount = trace.ent->linkcount;
		ent->velocity[2] = 0;
	}
}


void M_CatagorizePosition (edict_t *ent)
{
	vec3_t		point;
	brushcontents_t	cont;

//
// get waterlevel
//
	point[0] = ent->s.origin[0];
	point[1] = ent->s.origin[1];
	point[2] = ent->s.origin[2] + ent->mins[2] + 1;	
	cont = gi.pointcontents (point);

	if (!(cont & MASK_WATER))
	{
		ent->waterlevel = WATER_NONE;
		ent->watertype = CONTENTS_NONE;
		return;
	}

	ent->watertype = cont;
	ent->waterlevel = WATER_FEET;
	point[2] += 26;
	cont = gi.pointcontents (point);
	if (!(cont & MASK_WATER))
		return;

	ent->waterlevel = WATER_WAIST;
	point[2] += 22;
	cont = gi.pointcontents (point);
	if (cont & MASK_WATER)
		ent->waterlevel = WATER_UNDER;
}


void M_WorldEffects (edict_t *ent)
{
	int32_t		dmg;

	if (ent->health > 0)
	{
		if (!(ent->flags & FL_SWIM))
		{
			if (ent->waterlevel < WATER_UNDER)
			{
				ent->air_finished = level.time + 12;
			}
			else if (ent->air_finished < level.time)
			{	// drown!
				if (ent->pain_debounce_time < level.time)
				{
					dmg = 2 + 2 * floor(level.time - ent->air_finished);
					if (dmg > 15)
						dmg = 15;
					T_Damage (ent, world, world, vec3_origin, ent->s.origin, vec3_origin, dmg, 0, DAMAGE_NO_ARMOR, MOD_WATER);
					ent->pain_debounce_time = level.time + 1;
				}
			}
		}
		else
		{
			if (ent->waterlevel > WATER_NONE)
			{
				ent->air_finished = level.time + 9;
			}
			else if (ent->air_finished < level.time)
			{	// suffocate!
				if (ent->pain_debounce_time < level.time)
				{
					dmg = 2 + 2 * floor(level.time - ent->air_finished);
					if (dmg > 15)
						dmg = 15;
					T_Damage (ent, world, world, vec3_origin, ent->s.origin, vec3_origin, dmg, 0, DAMAGE_NO_ARMOR, MOD_WATER);
					ent->pain_debounce_time = level.time + 1;
				}
			}
		}
	}
	
	if (ent->waterlevel == WATER_NONE)
	{
		if (ent->flags & FL_INWATER)
		{	
			gi.sound (ent, CHAN_BODY, gi.soundindex("player/watr_out.wav"), 1, ATTN_NORM, 0);
			ent->flags &= ~FL_INWATER;
		}
		return;
	}

	if ((ent->watertype & CONTENTS_LAVA) && !(ent->flags & FL_IMMUNE_LAVA))
	{
		if (ent->damage_debounce_time < level.time)
		{
			ent->damage_debounce_time = level.time + 0.2;
			T_Damage (ent, world, world, vec3_origin, ent->s.origin, vec3_origin, 10*ent->waterlevel, 0, DAMAGE_NONE, MOD_LAVA);
		}
	}
	if ((ent->watertype & CONTENTS_SLIME) && !(ent->flags & FL_IMMUNE_SLIME))
	{
		if (ent->damage_debounce_time < level.time)
		{
			ent->damage_debounce_time = level.time + 1;
			T_Damage (ent, world, world, vec3_origin, ent->s.origin, vec3_origin, 4*ent->waterlevel, 0, DAMAGE_NONE, MOD_SLIME);
		}
	}
	
	if ( !(ent->flags & FL_INWATER) )
	{	
		if (!(ent->svflags & SVF_DEADMONSTER))
		{
			if (ent->watertype & CONTENTS_LAVA)
				if (prandom(50))
					gi.sound (ent, CHAN_BODY, gi.soundindex("player/lava1.wav"), 1, ATTN_NORM, 0);
				else
					gi.sound (ent, CHAN_BODY, gi.soundindex("player/lava2.wav"), 1, ATTN_NORM, 0);
			else if (ent->watertype & CONTENTS_SLIME)
				gi.sound (ent, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
			else if (ent->watertype & CONTENTS_WATER)
				gi.sound (ent, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		}

		ent->flags |= FL_INWATER;
		ent->damage_debounce_time = 0;
	}
}


void M_droptofloor (edict_t *ent)
{
	vec3_t		end;
	trace_t		trace;

	ent->s.origin[2] += 1;
	VectorCopy (ent->s.origin, end);
	end[2] -= 256;
	
	trace = gi.trace (ent->s.origin, ent->mins, ent->maxs, end, ent, MASK_MONSTERSOLID);

	if (trace.fraction == 1 || trace.allsolid)
		return;

	VectorCopy (trace.endpos, ent->s.origin);

	gi.linkentity (ent);
	M_CheckGround (ent);
	M_CatagorizePosition (ent);
}


void M_SetEffects (edict_t *ent)
{
	ent->s.effects &= ~(EF_COLOR_SHELL|EF_POWERSCREEN);
	ent->s.renderfx &= ~(RF_SHELL_RED|RF_SHELL_GREEN|RF_SHELL_BLUE);
}


void M_MoveFrame (edict_t *self)
{
	mmove_t	*move;
	int32_t		index;

	move = self->monsterinfo.currentmove;
	self->nextthink = level.time + FRAMETIME;

	if ((self->monsterinfo.nextframe) && (self->monsterinfo.nextframe >= move->firstframe) && (self->monsterinfo.nextframe <= move->lastframe))
	{
		self->s.frame = self->monsterinfo.nextframe;
		self->monsterinfo.nextframe = 0;
	}
	else
	{
		if (self->s.frame == move->lastframe)
		{
			if (move->endfunc)
			{
				move->endfunc (self);

				// regrab move, endfunc is very likely to change it
				move = self->monsterinfo.currentmove;

				// check for death
				if (self->svflags & SVF_DEADMONSTER)
					return;
			}
		}

		if (self->s.frame < move->firstframe || self->s.frame > move->lastframe)
			self->s.frame = move->firstframe;
		else
		{
			self->s.frame++;
			if (self->s.frame > move->lastframe)
				self->s.frame = move->firstframe;
		}
	}

	index = self->s.frame - move->firstframe;
	if (move->frame[index].aifunc)
		move->frame[index].aifunc (self, move->frame[index].dist * self->monsterinfo.scale);

	if (move->frame[index].thinkfunc)
		move->frame[index].thinkfunc (self);
}

void EnsureGoodPosition(edict_t *whomst);

void Unpossess(edict_t *player)
{
	player->movetype = MOVETYPE_WALK;
	player->solid = SOLID_BBOX;
	player->svflags &= ~SVF_NOCLIENT;
	player->client->ps.pmove.pm_type = PM_NORMAL;
	player->viewheight = 22;
	player->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
	VectorCopy(player->control->velocity, player->velocity);
	VectorCopy(player->control->s.origin, player->s.origin);
	player->s.origin[2] += player->control->mins[2];
	player->s.origin[2] -= player->mins[2];
	
	player->health = player->control->health;
	G_FreeEdict(player->control);
	player->control = nullptr;

	EnsureGoodPosition(player);
	gi.linkentity (player);

	player->s.event = EV_PLAYER_TELEPORT;
	player->client->jump_sound_debounce = level.time + 1;
}

void monster_think (edict_t *self)
{
	if (self->control && (self->control->client->cmd.upmove > 0 || self->waterlevel >= WATER_UNDER))
	{
		Unpossess(self->control);
		return;
	}
	else if (self->control && (self->control->client->cmd.forwardmove ||
			self->control->client->cmd.sidemove))
	{
		vec_t len = sqrt((self->control->client->cmd.forwardmove * self->control->client->cmd.forwardmove) + (self->control->client->cmd.sidemove * self->control->client->cmd.sidemove));

		if (len <= 200)
		{
			if (self->monsterinfo.currentmove->frame->aifunc != ai_walk)
				self->monsterinfo.walk(self);
		}
		else
		{
			if (self->monsterinfo.currentmove->frame->aifunc != ai_run)
				self->monsterinfo.run(self);
		}
	}
	else if (!self->control)
	{
		if (self->monsterinfo.currentmove->frame->aifunc == ai_stand)
		{
			if (level.time > self->monsterinfo.next_runwalk_check)
			{
				if (prandom(50))
				{
					self->ideal_yaw = random(360);

					if (random() < 0.5)
						self->monsterinfo.run(self);
					else
						self->monsterinfo.walk(self);

					self->monsterinfo.should_stand_check = level.time + irandom(1, 24);
				}

				self->monsterinfo.next_runwalk_check = level.time + irandom(1, 24);
			}
		}
	}

	M_MoveFrame (self);
	if (self->linkcount != self->monsterinfo.linkcount)
	{
		self->monsterinfo.linkcount = self->linkcount;
		M_CheckGround (self);
	}
	M_CatagorizePosition (self);
	M_WorldEffects (self);
	M_SetEffects (self);
}

/*
================
monster_death_use

When a monster dies, it fires all of its targets with the current
enemy as activator.
================
*/
void monster_death_use (edict_t *self)
{
	self->flags &= ~(FL_FLY|FL_SWIM);
}


//============================================================================

bool monster_start (edict_t *self)
{
	if (self->spawnflags & 4)
	{
		self->spawnflags &= ~4;
		self->spawnflags |= 1;
//		gi.dprintf("fixed spawnflags on %s at %s\n", self->classname, vtos(self->s.origin));
	}

	self->mins[0] = self->mins[1] = -16;
	self->maxs[0] = self->maxs[1] = 16;

	gi.linkentity(self);

	self->nextthink = level.time + FRAMETIME;
	self->svflags |= SVF_MONSTER;
	self->takedamage = DAMAGE_AIM;
	self->air_finished = level.time + 12;
	self->max_health = self->health;
	self->clipmask = MASK_MONSTERSOLID;

	self->deadflag = DEAD_NO;
	self->svflags &= ~SVF_DEADMONSTER;

	VectorCopy (self->s.origin, self->s.old_origin);

	// randomize what frame they start on
	if (self->monsterinfo.currentmove)
		self->s.frame = irandom(self->monsterinfo.currentmove->firstframe, self->monsterinfo.currentmove->lastframe);

	bool damaged = prandom(50);

	self->max_health = 200;
	self->health = damaged ? 100 : 200;

	if (damaged)
		self->s.skinnum = self->monsterinfo.damaged_skin;

	return true;
}

void monster_start_go (edict_t *self)
{
	vec3_t	v;

	if (self->health <= 0)
		return;

	// check for target to combat_point and change to combattarget
	if (self->target)
	{
		bool		notcombat;
		bool		fixup;
		edict_t		*target;

		target = nullptr;
		notcombat = false;
		fixup = false;
		while ((target = G_Find (target, FOFS(targetname), self->target)) != nullptr)
		{
			if (strcmp(target->classname, "point_combat") == 0)
			{
				self->combattarget = self->target;
				fixup = true;
			}
			else
			{
				notcombat = true;
			}
		}
		if (notcombat && self->combattarget)
			gi.dprintf("%s at %s has target with mixed types\n", self->classname, vtos(self->s.origin));
		if (fixup)
			self->target = nullptr;
	}

	// validate combattarget
	if (self->combattarget)
	{
		edict_t		*target;

		target = nullptr;
		while ((target = G_Find (target, FOFS(targetname), self->combattarget)) != nullptr)
		{
			if (strcmp(target->classname, "point_combat") != 0)
			{
				gi.dprintf("%s at (%i %i %i) has a bad combattarget %s : %s at (%i %i %i)\n",
					self->classname, (int32_t)self->s.origin[0], (int32_t)self->s.origin[1], (int32_t)self->s.origin[2],
					self->combattarget, target->classname, (int32_t)target->s.origin[0], (int32_t)target->s.origin[1],
					(int32_t)target->s.origin[2]);
			}
		}
	}

	if (self->target)
	{
		self->goalentity = self->movetarget = G_PickTarget(self->target);
		if (!self->movetarget)
		{
			gi.dprintf ("%s can't find target %s at %s\n", self->classname, self->target, vtos(self->s.origin));
			self->target = nullptr;
			self->monsterinfo.pausetime = 100000000;
			self->monsterinfo.stand (self);
		}
		else if (strcmp (self->movetarget->classname, "path_corner") == 0)
		{
			VectorSubtract (self->goalentity->s.origin, self->s.origin, v);
			self->ideal_yaw = self->s.angles[YAW] = vectoyaw(v);
			self->monsterinfo.walk (self);
			self->target = nullptr;
		}
		else
		{
			self->goalentity = self->movetarget = nullptr;
			self->monsterinfo.pausetime = 100000000;
			self->monsterinfo.stand (self);
		}
	}
	else
	{
		self->monsterinfo.pausetime = 100000000;
		self->monsterinfo.stand (self);
	}

	self->think = monster_think;
	self->nextthink = level.time + FRAMETIME;
}


void walkmonster_start_go (edict_t *self)
{
	if (!(self->spawnflags & 2) && level.time < 1)
	{
		M_droptofloor (self);

		if (self->groundentity)
			if (!M_walkmove (self, 0, 0))
				gi.dprintf ("%s in solid at %s\n", self->classname, vtos(self->s.origin));
	}
	
	if (!self->yaw_speed)
		self->yaw_speed = 20;
	self->viewheight = 25;

	monster_start_go (self);
}

void walkmonster_start (edict_t *self)
{
	self->think = walkmonster_start_go;
	monster_start (self);
}


void flymonster_start_go (edict_t *self)
{
	if (!M_walkmove (self, 0, 0))
		gi.dprintf ("%s in solid at %s\n", self->classname, vtos(self->s.origin));

	if (!self->yaw_speed)
		self->yaw_speed = 10;
	self->viewheight = 25;

	monster_start_go (self);
}


void flymonster_start (edict_t *self)
{
	self->flags |= FL_FLY;
	self->think = flymonster_start_go;
	monster_start (self);
}


void swimmonster_start_go (edict_t *self)
{
	if (!self->yaw_speed)
		self->yaw_speed = 10;
	self->viewheight = 10;

	monster_start_go (self);
}

void swimmonster_start (edict_t *self)
{
	self->flags |= FL_SWIM;
	self->think = swimmonster_start_go;
	monster_start (self);
}
