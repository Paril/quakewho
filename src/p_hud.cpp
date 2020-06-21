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

/*
======================================================================

INTERMISSION

======================================================================
*/

void MoveClientToIntermission (edict_t &ent)
{
	gclient_t &client = *ent.client;

	client.showscores = true;
	client.ps.pmove.origin[0] = level.intermission_origin[0]*8;
	client.ps.pmove.origin[1] = level.intermission_origin[1]*8;
	client.ps.pmove.origin[2] = level.intermission_origin[2]*8;
	client.ps.viewangles = level.intermission_angle;
	client.ps.pmove.pm_type = PM_FREEZE;
	client.ps.gunindex = MODEL_NONE;
	client.ps.blend[3] = 0;
	client.ps.rdflags &= ~RDF_UNDERWATER;
	
	ent.s.origin = level.intermission_origin;
	ent.viewheight = 0;
	ent.s.modelindex = MODEL_NONE;
	ent.s.modelindex2 = MODEL_NONE;
	ent.s.modelindex3 = MODEL_NONE;
	ent.s.modelindex4 = MODEL_NONE;
	ent.s.effects = EF_NONE;
	ent.s.sound = SOUND_NONE;
	ent.solid = SOLID_NOT;

	// add the layout
	DeathmatchScoreboardMessage (ent, nullptr);
	ent.Unicast(true);
}

void BeginIntermission (edict_t &targ)
{
	if (level.intermissiontime)
		return;		// already activated

	// respawn any dead clients
	for (size_t i = 0; i < game.maxclients; i++)
	{
		edict_t &client = g_edicts[1 + i];
		
		if (!client.inuse)
			continue;
		
		if (client.health <= 0)
			respawn(client);
	}

	level.intermissiontime = level.time;
	level.changemap = targ.map;
	level.exitintermission = 0;

	// find an intermission spot
	edict_ref ent = G_Find (nullptr, FOFS(classname), "info_player_intermission");
	if (!ent)
	{	// the map creator forgot to put in an intermission point...
		ent = G_Find (nullptr, FOFS(classname), "info_player_start");
		if (!ent)
			ent = G_Find (nullptr, FOFS(classname), "info_player_deathmatch");
	}
	else
	{	// chose one of four spots
		int32_t i = irandom(3);

		while (i--)
		{
			ent = G_Find (ent, FOFS(classname), "info_player_intermission");
			if (!ent)	// wrap around the list
				ent = G_Find (ent, FOFS(classname), "info_player_intermission");
		}
	}

	level.intermission_origin = ent->s.origin;
	level.intermission_angle = ent->s.angles;

	// move all clients to the intermission point
	for (size_t i = 0; i < game.maxclients; i++)
	{
		edict_t &client = g_edicts[1 + i];
		
		if (!client.inuse)
			continue;

		MoveClientToIntermission (client);
	}
}


/*
==================
DeathmatchScoreboardMessage

==================
*/
void DeathmatchScoreboardMessage (edict_t &client, const edict_ref &killer)
{
	// sort the clients by score
	int32_t		total = 0;
	int32_t		sorted[MAX_CLIENTS];
	int32_t		sortedscores[MAX_CLIENTS];

	for (size_t i = 0; i < game.maxclients; i++)
	{
		edict_t &cl_ent = g_edicts[1 + i];

		if (!cl_ent.inuse || game.clients[i].resp.spectator)
			continue;

		const int32_t score = game.clients[i].resp.score;
		int32_t j, k;

		for (j = 0; j < total; j++)
			if (score > sortedscores[j])
				break;

		for (k = total; k > j; k--)
		{
			sorted[k] = sorted[k - 1];
			sortedscores[k] = sortedscores[k - 1];
		}

		sorted[j] = i;
		sortedscores[j] = score;
		total++;
	}

	// print level name and exit rules
	char string[1400] = { 0 };
	size_t stringlength = strlen(string);

	// add the clients in sorted order
	if (total > 12)
		total = 12;

	for (int32_t i = 0; i < total; i++)
	{
		const gclient_t &cl = game.clients[sorted[i]];
		const edict_t &cl_ent = g_edicts[1 + sorted[i]];
		const int32_t x = (i>=6) ? 160 : 0;
		const int32_t y = 32 + 32 * (i%6);
		const char *tag = nullptr;
		char entry[1024];

		// add a dogtag
		if (cl_ent == client)
			tag = "tag1";
		else if (cl_ent == killer)
			tag = "tag2";

		if (tag)
		{
			snprintf (entry, sizeof(entry), "xv %i yv %i picn %s ", x + 32, y, tag);

			const size_t j = strlen(entry);
			if (stringlength + j > 1024)
				break;

			strcpy (string + stringlength, entry);
			stringlength += j;
		}

		// send the layout
		snprintf (entry, sizeof(entry),
			"client %i %i %i %i %i %i ",
			x, y, sorted[i], cl.resp.score, cl.ping, (level.framenum - cl.resp.enterframe) / 600);

		const size_t j = strlen(entry);
		if (stringlength + j > 1024)
			break;

		strcpy (string + stringlength, entry);
		stringlength += j;
	}

	gi.WriteByte (SVC_LAYOUT);
	gi.WriteString (string);
}


/*
==================
DeathmatchScoreboard

Draw instead of help message.
Note that it isn't that hard to overflow the 1400 uint8_t message limit!
==================
*/
static void DeathmatchScoreboard (edict_t &ent)
{
	DeathmatchScoreboardMessage (ent, ent.enemy);
	ent.Unicast(true);
}


/*
==================
Cmd_Score_f

Display the scoreboard
==================
*/
static void Cmd_Score_f (edict_t &ent)
{
	if (ent.client->showscores)
	{
		ent.client->showscores = false;
		return;
	}

	ent.client->showscores = true;
	DeathmatchScoreboard (ent);
}

/*
==================
Cmd_Help_f

Display the current help message
==================
*/
void Cmd_Help_f (edict_t &ent)
{
	// this is for backwards compatability
	Cmd_Score_f (ent);
}

//=======================================================================

/*static radar_t G_Radar(edict_t &ent)
{
	vec_t best_distance = FLT_MAX;

	for (size_t i = game.maxclients + 1; i < globals.num_edicts; i++)
	{
		edict_t &e = g_edicts[i];

		if (!e.inuse || !(e.svflags & SVF_MONSTER) || !e.control)
			continue;

		if (!gi.inPHS(ent.s.origin, e.s.origin))
			continue;

		vec_t dist = ent.s.origin.Distance(e.s.origin);

		if (gi.trace(ent.s.origin, e.s.origin, MASK_SOLID).fraction < 1.0f)
			dist *= 2;

		best_distance = min(best_distance, dist);
	}

	if (best_distance < 512)
		return RADAR_HOT;
	else if (best_distance < 1024)
		return RADAR_WARM;
	return RADAR_COLD;
}*/

static const char *G_Radar(edict_t &ent)
{
	vec_t best_distance = FLT_MAX;
	edict_ref best_ref;
	
	ent.client->SendSound(gi.soundindex("world/scan1.wav"));

	for (size_t i = game.maxclients + 1; i < globals.num_edicts; i++)
	{
		edict_t &e = g_edicts[i];

		if (!e.inuse || !(e.svflags & SVF_MONSTER) || !e.control)
			continue;

		if (!gi.inPHS(ent.s.origin, e.s.origin))
			continue;

		vec_t dist = ent.s.origin.Distance(e.s.origin);

		if (gi.trace(ent.s.origin, e.s.origin, MASK_SOLID).fraction < 1.0f)
			dist *= 2;

		best_distance = min(best_distance, dist);
		best_ref = e;
	}

	if (!best_ref)
		return "No blips...";

	best_ref->control->client->Print("Your monster type was disclosed via radar!\n");
	best_ref->control->client->SendSound(gi.soundindex("world/scan1.wav"));

	return best_ref->monsterinfo.name;
}

/*
===============
G_SetStats
===============
*/
void G_SetStats (edict_t &ent)
{
	gclient_t &client = *ent.client;

	client.ps.stats[STAT_HUNTER] = client.ps.stats[STAT_HIDER] = 0;
	client.ps.stats[STAT_CONTROL] = 0;

	if (roundlimit->value)
	{
		if (level.state == GAMESTATE_PLAYING)
		{
			const int32_t time_left = static_cast<int32_t>(level.round_end - level.time);
			const int32_t m = time_left / 60;
			const int32_t s = time_left % 60;

			gi.configstring(CS_GENERAL + 2, va("ROUND TIME: %02d:%02d", m, s));
		}
		
		if (level.state >= GAMESTATE_SPAWNING)
			client.ps.stats[STAT_ROUND_TIMER] = CS_GENERAL + 2;
	}

	if (!client.resp.spectator)
	{
		switch (client.resp.team)
		{
		case TEAM_HIDERS:
			client.ps.stats[STAT_HIDER] = 1;

			if (level.control_delay > level.time)
			{
				client.last_num_jumps = 0;
				gi.configstring(CS_GENERAL, va("You'll take control in %i...", static_cast<int32_t>(trunc(level.control_delay - level.time + 1))));
			}
			else
			{
				if (client.last_num_jumps != client.num_jumps)
				{
					gi.WriteByte(SVC_CONFIGSTRING);
					gi.WriteShort(CS_GENERAL);
					gi.WriteString(va("STROGG-SHIFTS LEFT: %i", client.num_jumps));
					gi.unicast(ent, true);
					client.last_num_jumps = client.num_jumps;
				}
			
				client.ps.stats[STAT_CONTROL] = static_cast<int16_t>(CS_GENERAL);
			}

			client.ps.stats[STAT_CONTROL] = static_cast<int16_t>(CS_GENERAL);
			break;
		case TEAM_HUNTERS:
			client.ps.stats[STAT_HUNTER] = 1;

			if (level.time >= level.control_delay)
			{
				const int32_t radar_left = static_cast<int32_t>(trunc(level.radar_time - level.time + 1));

				if (level.radar_time <= level.time || !client.last_radar)
				{
					client.last_radar = G_Radar(ent);
					level.radar_time = level.time + 8;
				}

				if (level.last_radar_left != radar_left)
				{
					gi.WriteByte(SVC_CONFIGSTRING);
					gi.WriteShort(CS_GENERAL + 1);
					gi.WriteString(va("RADAR: %s (ping in %i...)", client.last_radar, radar_left));
					gi.unicast(ent, true);
				}

				client.ps.stats[STAT_CONTROL] = static_cast<int16_t>(CS_GENERAL + 1);
			}
			break;
		case TEAM_NONE:
			break;
		}
	}

	//
	// health
	//
	client.ps.stats[STAT_HEALTH_ICON] = level.pic_health;
	client.ps.stats[STAT_HEALTH] = static_cast<int16_t>(ent.control ? ent.control->health : ent.health);

	//
	// ammo
	//
	client.ps.stats[STAT_AMMO_ICON] = 0;
	client.ps.stats[STAT_AMMO] = 0;
	
	//client.ps.stats[STAT_BULLETS] = static_cast<int16_t>(client.pers.ammo[AMMO_BULLETS]);
	client.ps.stats[STAT_SHELLS] = static_cast<int16_t>(client.pers.ammo[AMMO_SHELLS]);
	client.ps.stats[STAT_GRENADES] = static_cast<int16_t>(client.pers.ammo[AMMO_GRENADES]);

	if (client.resp.team == TEAM_HUNTERS && client.pers.weapon && client.pers.weapon->ammo != AMMO_NONE)
	{
		client.ps.stats[STAT_AMMO_ICON] = gi.imageindex (client.pers.weapon->icon);

		//if (client.pers.weapon->ammo != AMMO_NONE)
			client.ps.stats[STAT_AMMO] = static_cast<int16_t>(client.pers.ammo[client.pers.weapon->ammo]);
	}

	//
	// layouts
	//
	client.ps.stats[STAT_LAYOUTS] = 0;

	if (client.pers.health <= 0 || level.intermissiontime || client.showscores)
		client.ps.stats[STAT_LAYOUTS] |= 1;

	//
	// frags
	//
	client.ps.stats[STAT_FRAGS] = static_cast<int16_t>(client.resp.score);

	client.ps.stats[STAT_SPECTATOR] = 0;
}

/*
===============
G_CheckChaseStats
===============
*/
void G_CheckChaseStats (edict_t &ent)
{
	for (size_t i = 1; i <= game.maxclients; i++)
	{
		gclient_t &cl = *g_edicts[i].client;

		if (!g_edicts[i].inuse || cl.chase_target != ent)
			continue;
		
		cl.ps.stats = ent.client->ps.stats;
		G_SetSpectatorStats(g_edicts[i]);
	}
}

/*
===============
G_SetSpectatorStats
===============
*/
void G_SetSpectatorStats (edict_t &ent)
{
	gclient_t &cl = *ent.client;

	if (!cl.chase_target)
		G_SetStats (ent);

	cl.ps.stats[STAT_SPECTATOR] = 1;

	// layouts are independant in spectator
	cl.ps.stats[STAT_LAYOUTS] = 0;
	if (cl.pers.health <= 0 || level.intermissiontime || cl.showscores)
		cl.ps.stats[STAT_LAYOUTS] |= 1;

	if (cl.chase_target && cl.chase_target->inuse)
		cl.ps.stats[STAT_CHASE] = static_cast<int16_t>(CS_PLAYERSKINS + cl.chase_target->s.number - 1);
	else
		cl.ps.stats[STAT_CHASE] = 0;
}

