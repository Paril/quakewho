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
#include "m_player.h"

bool OnSameTeam (const edict_t &ent1, const edict_t &ent2)
{
	if (ent1.client && ent2.client)
		return ent1.client->resp.team == ent2.client->resp.team;

	return false;
}

//=================================================================================

/*
==================
Cmd_Give_f

Give items to a client
==================
*/
static void Cmd_Give_f (edict_t &ent)
{
	if (!sv_cheats->value)
	{
		ent.client->Print("You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	const char *name = gi.args();
	const bool give_all = (stricmp(name, "all") == 0);

	if (stricmp(gi.argv(1), "pain") == 0)
	{
		int32_t damage;

		if (gi.argc() == 3)
			damage = atoi(gi.argv(2));
		else
			damage = ent.health;

		T_Damage(ent.control ? static_cast<edict_t &>(ent.control) : ent, ent, ent, vec3_origin, vec3_origin, vec3_origin, damage, 0, DAMAGE_NONE);
		return;
	}

	if (give_all || stricmp(gi.argv(1), "health") == 0)
	{
		if (gi.argc() == 3)
			ent.health = atoi(gi.argv(2));
		else
			ent.health = ent.max_health;

		if (!give_all)
			return;
	}

	if (give_all || stricmp(name, "ammo") == 0)
	{
		for (auto &ammo : ent.client->pers.ammo)
			ammo = 999;

		if (!give_all)
			return;
	}

	if (give_all)
		return;

	ent.client->Print("only thing you can do here is \"health\", \"ammo\" or \"pain\".\n");
}

/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
static void Cmd_God_f (edict_t &ent)
{
	if (!sv_cheats->value)
	{
		ent.client->Print("You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	ent.flags ^= FL_GODMODE;
	ent.client->Print("godmode %s\n", (ent.flags & FL_GODMODE) ? "ON" : "OFF");
}

/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
static void Cmd_Noclip_f (edict_t &ent)
{
	if (!sv_cheats->value)
	{
		ent.client->Print("You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}

	if (ent.movetype == MOVETYPE_NOCLIP)
		ent.movetype = MOVETYPE_WALK;
	else
		ent.movetype = MOVETYPE_NOCLIP;
	
	ent.client->Print("noclip %s\n", (ent.movetype == MOVETYPE_NOCLIP) ? "ON" : "OFF");
}


/*
==================
Cmd_Use_f

Use an inventory item
==================
*/
static void Cmd_Use_f (edict_t &ent)
{
	const char *s = gi.args();

	for (gitem_weapmodel_t i = WEAP_BLASTER; i < WEAP_TOTAL; i++)
	{
		gitem_t *it = &g_weapons[i];

		if (stricmp(it->pickup_name, s))
			continue;

		it->use (ent, it);
		return;
	}

	ent.client->Print(PRINT_HIGH, "unknown item: %s\n", s);
}

/*
=================
Cmd_WeapPrev_f
=================
*/
static void Cmd_WeapPrev_f (edict_t &ent)
{
	if (!ent.client->pers.weapon)
		return;

	gitem_t *it = &g_weapons[(ent.client->pers.weapon->weapmodel - 1) % lengthof(g_weapons)];
	it->use (ent, it);
}

/*
=================
Cmd_WeapNext_f
=================
*/
static void Cmd_WeapNext_f (edict_t &ent)
{
	if (!ent.client->pers.weapon)
		return;

	gitem_t *it = &g_weapons[(ent.client->pers.weapon->weapmodel + 1) % lengthof(g_weapons)];
	it->use (ent, it);
}

/*
=================
Cmd_WeapLast_f
=================
*/
static void Cmd_WeapLast_f (edict_t &ent)
{
	if (!ent.client->pers.weapon || !ent.client->pers.lastweapon)
		return;

	ent.client->pers.lastweapon->use (ent, ent.client->pers.lastweapon);
}

/*
=================
Cmd_Kill_f
=================
*/
static void Cmd_Kill_f (edict_t &ent)
{
	if((level.time - ent.client->respawn_time) < 5)
		return;
	ent.flags &= ~FL_GODMODE;
	ent.health = 0;
	player_die (ent, ent, ent, 100000, vec3_origin);
}

/*
=================
Cmd_PutAway_f
=================
*/
static void Cmd_PutAway_f (edict_t &ent)
{
	ent.client->showscores = false;
}

static int32_t PlayerSort (const size_t &a, const size_t &b)
{
	const int32_t anum = game.clients[a].ps.stats[STAT_FRAGS];
	const int32_t bnum = game.clients[b].ps.stats[STAT_FRAGS];

	return (anum < bnum) ? 1 : (bnum < anum) ? -1 : 0;
}

/*
=================
Cmd_Players_f
=================
*/
static void Cmd_Players_f (const edict_t &ent)
{
	size_t	count;
	char	small[64];
	char	large[1280];
	std::array<size_t, MAX_CLIENTS>	index;

	count = 0;
	for (size_t i = 0; i < game.maxclients; i++)
		if (game.clients[i].pers.connected)
		{
			index[count] = i;
			count++;
		}

	// sort by frags
	std::sort(index.begin(), index.begin() + count, PlayerSort);

	// print information
	large[0] = 0;

	for (size_t i = 0; i < count; i++)
	{
		snprintf (small, sizeof(small), "%3i %s\n",
			game.clients[index[i]].ps.stats[STAT_FRAGS],
			game.clients[index[i]].pers.netname);
		if (strlen (small) + strlen(large) > sizeof(large) - 100 )
		{	// can't print all of them in one packet
			strcat (large, "...\n");
			break;
		}
		strcat (large, small);
	}

	ent.client->Print("%s\n%i players\n", large, count);
}

/*
=================
Cmd_Wave_f
=================
*/
static void Cmd_Wave_f (edict_t &ent)
{
	// can't wave when ducked
	if (ent.client->ps.pmove.pm_flags & PMF_DUCKED)
		return;

	if (ent.client->anim_priority > ANIM_WAVE)
		return;

	ent.client->anim_priority = ANIM_WAVE;

	static constexpr struct {
		const char	*title;
		int32_t		start_frame;
		int32_t		end_frame;
	} waves[] = {
		{ "flipoff", FRAME_flip01, FRAME_flip12 },
		{ "salute", FRAME_salute01, FRAME_salute11 },
		{ "taunt", FRAME_taunt01, FRAME_taunt17 },
		{ "wave", FRAME_wave01, FRAME_wave11 },
		{ "point", FRAME_point01, FRAME_point12 }
	};
	
	const size_t i = static_cast<size_t>(strtoul(gi.argv(1), nullptr, 10));

	if (i >= lengthof(waves))
		return;

	auto &wave = waves[i];
	ent.client->Print("%s\n", wave.title);
	ent.s.frame = wave.start_frame - 1;
	ent.client->anim_end = wave.end_frame;
}

/*
==================
Cmd_Say_f
==================
*/
static void Cmd_Say_f (edict_t &ent, const bool &team, const bool &arg0)
{
	if (gi.argc () < 2 && !arg0)
		return;

	char text[150];

	if (team)
		snprintf (text, sizeof(text), "(%s): ", ent.client->pers.netname);
	else
		snprintf (text, sizeof(text), "%s: ", ent.client->pers.netname);

	if (arg0)
	{
		strncat_s(text, sizeof(text), gi.argv(0), min(sizeof(text) - strnlen_s(text, sizeof(text)) - 1, strlen(gi.argv(0))));
		strncat_s(text, sizeof(text), " ", min(sizeof(text) - strnlen_s(text, sizeof(text)) - 1, 1u));
		strncat_s(text, sizeof(text), gi.args(), min(sizeof(text) - strnlen_s(text, sizeof(text)) - 1, strlen(gi.args())));
	}
	else
	{
		const char *p = gi.args();
		strncat_s(text, sizeof(text), p + 1, min(sizeof(text) - strnlen_s(text, sizeof(text)) - 1, strlen(p) - 2));
	}

	if (flood_msgs->value) {
		gclient_t *cl = ent.client;

        if (level.time < cl->flood_locktill) {
			ent.client->Print("You can't talk for %d more seconds\n", static_cast<int32_t>(cl->flood_locktill - level.time));
            return;
        }
        int32_t i = cl->flood_whenhead - flood_msgs->value + 1;
        if (i < 0)
            i = static_cast<int32_t>(lengthof(cl->flood_when) + i);
		if (cl->flood_when[i] && 
			level.time - cl->flood_when[i] < flood_persecond->value) {
			cl->flood_locktill = level.time + flood_waitdelay->value;
			ent.client->Print(PRINT_CHAT, "Flood protection: You can't talk for %d seconds.\n", static_cast<int32_t>(flood_waitdelay->value));
            return;
        }
		cl->flood_whenhead = (cl->flood_whenhead + 1) % lengthof(cl->flood_when);
		cl->flood_when[cl->flood_whenhead] = level.time;
	}

	if (dedicated->value)
		gi.dprintf("%s\n", text);

	for (size_t j = 1; j <= game.maxclients; j++)
	{
		const edict_t &other = g_edicts[j];
		if (!other.inuse)
			continue;
		if (!other.client)
			continue;
		if (team && !OnSameTeam(ent, other))
			continue;
		other.client->Print(PRINT_CHAT, "%s\n", text);
	}
}

static void Cmd_PlayerList_f(const edict_t &ent)
{
	char str[80];
	char text[1400];

	// connect time, ping, score, name
	*text = 0;
	for (size_t i = 0; i <= game.maxclients; i++)
	{
		const edict_t &e2 = g_edicts[i];
		
		if (!e2.inuse || !e2.client)
			continue;

		snprintf(str, sizeof(str), "%02d:%02d %4d %3d %s%s\n",
			(level.framenum - e2.client->resp.enterframe) / 600,
			((level.framenum - e2.client->resp.enterframe) % 600)/10,
			e2.client->ping,
			e2.client->resp.score,
			e2.client->pers.netname,
			e2.client->resp.spectator ? " (spectator)" : "");

		if (strlen(text) + strlen(str) > sizeof(text) - 50)
		{
			snprintf(text+strlen(text), sizeof(str) - strlen(text), "And more...\n");
			ent.client->Print("%s", text);
			return;
		}

		strcat(text, str);
	}

	ent.client->Print("%s", text);
}

static void Cmd_Team_f(edict_t &ent)
{
	const char *team = gi.argv(1);

	if (stricmp(team, "hiders") == 0)
		ent.client->resp.team = TEAM_HIDERS;
	if (stricmp(team, "hunters") == 0)
		ent.client->resp.team = TEAM_HUNTERS;

	ent.client->resp.spectator = false;
	respawn(ent);
}

/*
=================
ClientCommand
=================
*/
void ClientCommand (edict_t &ent)
{
	if (!ent.client)
		return;		// not fully in game yet

	const char *cmd = gi.argv(0);

	if (stricmp (cmd, "players") == 0)
		Cmd_Players_f (ent);
	else if (stricmp (cmd, "say") == 0)
		Cmd_Say_f (ent, false, false);
	else if (stricmp (cmd, "say_team") == 0)
		Cmd_Say_f (ent, true, false);
	else if (stricmp (cmd, "score") == 0 || stricmp (cmd, "help") == 0)
		Cmd_Help_f (ent);
	else if (!level.intermissiontime)
	{
		if (stricmp (cmd, "use") == 0)
			Cmd_Use_f (ent);
		else if (stricmp (cmd, "give") == 0)
			Cmd_Give_f (ent);
		else if (stricmp (cmd, "god") == 0)
			Cmd_God_f (ent);
		else if (stricmp (cmd, "noclip") == 0)
			Cmd_Noclip_f (ent);
		else if (stricmp (cmd, "inven") == 0)
			Cmd_Help_f (ent);
		else if (stricmp (cmd, "invuse") == 0)
			{}
		else if (stricmp (cmd, "invnext") == 0 ||
				 stricmp (cmd, "invnextw") == 0 ||
				 stricmp (cmd, "invnextp") == 0 ||
				 stricmp (cmd, "weapnext") == 0)
			Cmd_WeapNext_f (ent);
		else if (stricmp (cmd, "invprev") == 0 ||
				 stricmp (cmd, "invprevw") == 0 ||
				 stricmp (cmd, "invprevp") == 0 ||
				 stricmp (cmd, "weapprev") == 0)
			Cmd_WeapPrev_f (ent);
		else if (stricmp (cmd, "weaplast") == 0)
			Cmd_WeapLast_f (ent);
		else if (stricmp (cmd, "kill") == 0)
			Cmd_Kill_f (ent);
		else if (stricmp (cmd, "putaway") == 0)
			Cmd_PutAway_f (ent);
		else if (stricmp (cmd, "wave") == 0)
			Cmd_Wave_f (ent);
		else if (stricmp(cmd, "playerlist") == 0)
			Cmd_PlayerList_f(ent);
		//else if (stricmp(cmd, "team") == 0)
		//	Cmd_Team_f(ent);
		//else if (strcmp(cmd, "pos") == 0)
		//	Cmd_Pos_f(ent);
		else	// anything that doesn't match a command will be a chat
			Cmd_Say_f (ent, false, true);
	}
}
