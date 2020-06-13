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

struct spawn_t
{
	char	*name;
	void	(*spawn)(edict_t *ent);
};

void SP_info_player_start (edict_t *ent);
void SP_info_player_deathmatch (edict_t *ent);
void SP_info_player_coop (edict_t *ent);
void SP_info_player_intermission (edict_t *ent);

void SP_func_plat (edict_t *ent);
void SP_func_rotating (edict_t *ent);
void SP_func_button (edict_t *ent);
void SP_func_door (edict_t *ent);
void SP_func_door_secret (edict_t *ent);
void SP_func_door_rotating (edict_t *ent);
void SP_func_water (edict_t *ent);
void SP_func_train (edict_t *ent);
void SP_func_conveyor (edict_t *self);
void SP_func_wall (edict_t *self);
void SP_func_object (edict_t *self);
void SP_func_timer (edict_t *self);
void SP_func_areaportal (edict_t *ent);
void SP_func_clock (edict_t *ent);
void SP_func_killbox (edict_t *ent);

void SP_trigger_always (edict_t *ent);
void SP_trigger_once (edict_t *ent);
void SP_trigger_multiple (edict_t *ent);
void SP_trigger_relay (edict_t *ent);
void SP_trigger_push (edict_t *ent);
void SP_trigger_hurt (edict_t *ent);
void SP_trigger_counter (edict_t *ent);
void SP_trigger_elevator (edict_t *ent);
void SP_trigger_gravity (edict_t *ent);
void SP_trigger_monsterjump (edict_t *ent);

void SP_target_temp_entity (edict_t *ent);
void SP_target_speaker (edict_t *ent);
void SP_target_explosion (edict_t *ent);
void SP_target_splash (edict_t *ent);
void SP_target_spawner (edict_t *ent);
void SP_target_blaster (edict_t *ent);
void SP_target_laser (edict_t *self);
void SP_target_earthquake (edict_t *ent);
void SP_target_character (edict_t *ent);
void SP_target_string (edict_t *ent);

void SP_worldspawn (edict_t *ent);

void SP_light_mine1 (edict_t *ent);
void SP_light_mine2 (edict_t *ent);
void SP_info_notnull (edict_t *self);
void SP_path_corner (edict_t *self);

void SP_misc_teleporter (edict_t *self);
void SP_misc_teleporter_dest (edict_t *self);

void SP_monster_berserk (edict_t *self);
void SP_monster_gladiator (edict_t *self);
void SP_monster_gunner (edict_t *self);
void SP_monster_infantry (edict_t *self);
void SP_monster_soldier_light (edict_t *self);
void SP_monster_soldier (edict_t *self);
void SP_monster_soldier_ss (edict_t *self);
void SP_monster_tank (edict_t *self);
void SP_monster_medic (edict_t *self);
void SP_monster_chick (edict_t *self);
void SP_monster_parasite (edict_t *self);
void SP_monster_flyer (edict_t *self);
void SP_monster_brain (edict_t *self);
void SP_monster_floater (edict_t *self);
void SP_monster_hover (edict_t *self);
void SP_monster_mutant (edict_t *self);

spawn_t	spawns[] = {
	{"info_player_start", SP_info_player_start},
	{"info_player_deathmatch", SP_info_player_deathmatch},
	{"info_player_coop", SP_info_player_coop},
	{"info_player_intermission", SP_info_player_intermission},

	{"func_plat", SP_func_plat},
	{"func_button", SP_func_button},
	{"func_door", SP_func_door},
	{"func_door_secret", SP_func_door_secret},
	{"func_door_rotating", SP_func_door_rotating},
	{"func_rotating", SP_func_rotating},
	{"func_train", SP_func_train},
	{"func_water", SP_func_water},
	{"func_conveyor", SP_func_conveyor},
	{"func_areaportal", SP_func_areaportal},
	{"func_clock", SP_func_clock},
	{"func_wall", SP_func_wall},
	{"func_object", SP_func_object},
	{"func_timer", SP_func_timer},
	{"func_killbox", SP_func_killbox},

	{"trigger_always", SP_trigger_always},
	{"trigger_once", SP_trigger_once},
	{"trigger_multiple", SP_trigger_multiple},
	{"trigger_relay", SP_trigger_relay},
	{"trigger_push", SP_trigger_push},
	{"trigger_hurt", SP_trigger_hurt},
	{"trigger_counter", SP_trigger_counter},
	{"trigger_elevator", SP_trigger_elevator},
	{"trigger_gravity", SP_trigger_gravity},
	{"trigger_monsterjump", SP_trigger_monsterjump},

	{"target_temp_entity", SP_target_temp_entity},
	{"target_speaker", SP_target_speaker},
	{"target_explosion", SP_target_explosion},
	{"target_splash", SP_target_splash},
	{"target_spawner", SP_target_spawner},
	{"target_blaster", SP_target_blaster},
	{"target_laser", SP_target_laser},
	{"target_earthquake", SP_target_earthquake},
	{"target_character", SP_target_character},
	{"target_string", SP_target_string},

	{"worldspawn", SP_worldspawn},

	{"light_mine1", SP_light_mine1},
	{"light_mine2", SP_light_mine2},
	{"info_notnull", SP_info_notnull},
	{"path_corner", SP_path_corner},
	{"misc_teleporter", SP_misc_teleporter},
	{"misc_teleporter_dest", SP_misc_teleporter_dest},

	/*{"monster_berserk", SP_monster_berserk},
	{"monster_gladiator", SP_monster_gladiator},
	{"monster_gunner", SP_monster_gunner},
	{"monster_infantry", SP_monster_infantry},
	{"monster_soldier_light", SP_monster_soldier_light},
	{"monster_soldier", SP_monster_soldier},
	{"monster_soldier_ss", SP_monster_soldier_ss},
	{"monster_tank", SP_monster_tank},
	{"monster_tank_commander", SP_monster_tank},
	{"monster_medic", SP_monster_medic},
	{"monster_chick", SP_monster_chick},
	{"monster_parasite", SP_monster_parasite},
	{"monster_flyer", SP_monster_flyer},
	{"monster_brain", SP_monster_brain},
	{"monster_floater", SP_monster_floater},
	{"monster_hover", SP_monster_hover},
	{"monster_mutant", SP_monster_mutant},*/

	{nullptr, nullptr}
};

/*
===============
ED_CallSpawn

Finds the spawn function for the entity and calls it
===============
*/
void ED_CallSpawn (edict_t *ent)
{
	spawn_t	*s;

	if (!ent->classname)
	{
		gi.dprintf ("ED_CallSpawn: nullptr classname\n");
		return;
	}

	// check normal spawn functions
	for (s=spawns ; s->name ; s++)
	{
		if (!strcmp(s->name, ent->classname))
		{	// found it
			ent->classname = s->name;
			s->spawn (ent);
			return;
		}
	}

#if defined(DEBUG)
	gi.dprintf ("%s doesn't have a spawn function\n", ent->classname);
#endif

	G_FreeEdict(ent);
}

/*
=============
ED_NewString
=============
*/
char *ED_NewString (char *string)
{
	char	*newb, *new_p;
	size_t 	i,l;
	
	l = strlen(string) + 1;

	newb = (char *) gi.TagMalloc ((int32_t) l, TAG_LEVEL);

	new_p = newb;

	for (i=0 ; i< l ; i++)
	{
		if (string[i] == '\\' && i < l-1)
		{
			i++;
			if (string[i] == 'n')
				*new_p++ = '\n';
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	}
	
	return newb;
}

#include "g_fields.h"

/*
===============
ED_ParseField

Takes a key/value pair and sets the binary values
in an edict
===============
*/
void ED_ParseField (char *key, char *value, edict_t *ent)
{
	const field_t	*f;
	uint8_t			*b;
	vec_t			v;
	vec3_t			vec;

	for (f=fields ; f->name ; f++)
	{
		if (!stricmp(f->name, key))
		{	// found it
			if (f->flags & FFL_SPAWNTEMP)
				b = (uint8_t *)&st;
			else
				b = (uint8_t *)ent;

			switch (f->type)
			{
			case F_LSTRING:
				*(char **)(b+f->ofs) = ED_NewString (value);
				break;
			case F_VECTOR:
				sscanf (value, "%f %f %f", &vec[0], &vec[1], &vec[2]);
				((vec_t *)(b+f->ofs))[0] = vec[0];
				((vec_t *)(b+f->ofs))[1] = vec[1];
				((vec_t *)(b+f->ofs))[2] = vec[2];
				break;
			case F_INT:
				*(int32_t *)(b+f->ofs) = atoi(value);
				break;
			case F_FLOAT:
				*(vec_t *)(b+f->ofs) = atof(value);
				break;
			case F_ANGLEHACK:
				v = atof(value);
				((vec_t *)(b+f->ofs))[0] = 0;
				((vec_t *)(b+f->ofs))[1] = v;
				((vec_t *)(b+f->ofs))[2] = 0;
				break;
			default:
				break;
			}
			return;
		}
	}
	gi.dprintf ("%s is not a field\n", key);
}

/*
====================
ED_ParseEdict

Parses an edict out of the given string, returning the new position
ed should be a properly initialized empty edict.
====================
*/
char *ED_ParseEdict (char *data, edict_t *ent)
{
	bool		init;
	char		keyname[256];
	char		*com_token;

	init = false;
	memset (&st, 0, sizeof(st));

// go through all the dictionary pairs
	while (1)
	{	
	// parse key
		com_token = COM_Parse (&data);
		if (com_token[0] == '}')
			break;
		if (!data)
			gi.error ("ED_ParseEntity: EOF without closing brace");

		strncpy (keyname, com_token, sizeof(keyname)-1);
		
	// parse value	
		com_token = COM_Parse (&data);
		if (!data)
			gi.error ("ED_ParseEntity: EOF without closing brace");

		if (com_token[0] == '}')
			gi.error ("ED_ParseEntity: closing brace without data");

		init = true;	

	// keynames with a leading underscore are used for utility comments,
	// and are immediately discarded by quake
		if (keyname[0] == '_')
			continue;

		ED_ParseField (keyname, com_token, ent);
	}

	if (!init)
		memset (ent, 0, sizeof(*ent));

	return data;
}


/*
================
G_FindTeams

Chain together all entities with a matching team field.

All but the first will have the FL_TEAMSLAVE flag set.
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams ()
{
	edict_t	*e, *e2, *chain;
	int32_t		i, j;
	int32_t		c, c2;

	c = 0;
	c2 = 0;
	for (i=1, e=g_edicts+i ; i < globals.num_edicts ; i++,e++)
	{
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		chain = e;
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < globals.num_edicts ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				chain->teamchain = e2;
				e2->teammaster = e;
				chain = e2;
				e2->flags |= FL_TEAMSLAVE;
			}
		}
	}

	gi.dprintf ("%i teams with %i entities\n", c, c2);
}

#include <array>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

using vec3_array = std::array<vec_t, 3>;
using grid_array = std::array<uint8_t, 3>;

const uint32_t spawn_grid_size = 64;
const uint32_t grid_max = 8192 / spawn_grid_size;

template<>
struct std::hash<grid_array>
{
	size_t operator()(const grid_array& _Keyval) const
	{
		union {
			size_t p;
			struct {
				uint8_t x, y, z;
			} xyz;
		};
		
		xyz.x = _Keyval[0];
		xyz.y = _Keyval[1];
		xyz.z = _Keyval[2];

		return p;
	}
};

struct nav_grid_node
{
	grid_array grid_position;
	vec3_array position;
	std::unordered_set<grid_array> connections;
};

std::unordered_map<grid_array, nav_grid_node> grid;

constexpr vec_t grid_to_vec(const uint8_t &val)
{
	return (val * spawn_grid_size) - 4096.f;
}

constexpr uint8_t vec_to_grid(const vec_t &vec)
{
	return (uint8_t)((vec + 4096) / spawn_grid_size);
}

constexpr vec3_array grid_to_vec_array(const grid_array &val)
{
	return { grid_to_vec(val[0]), grid_to_vec(val[1]), grid_to_vec(val[2]) };
}

constexpr grid_array vec_to_grid_array(const vec3_array &val)
{
	return { vec_to_grid(val[0]), vec_to_grid(val[1]), vec_to_grid(val[2]) };
}

constexpr vec3_t small_monster_mins = { -16, -16, 0 };
constexpr vec3_t small_monster_maxs = { 16, 16, 56 };

static trace_t TraceWrap(const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, edict_t *passent, brushcontents_t contentmask)
{
	trace_t tr = gi.trace(start, mins, maxs, end, passent, contentmask);

	if (tr.startsolid || tr.allsolid || tr.contents || tr.ent != &g_edicts[0] || tr.fraction < 1.0f || tr.surface || !VectorCompare(tr.endpos, end))
		return tr;

	// might be a runaway trace
	return gi.trace(start, mins, maxs, end, passent, contentmask);
}

static brushcontents_t PointContents(vec3_t pos)
{
	brushcontents_t contents = gi.pointcontents(pos);

	if (contents & (CONTENTS_PLAYERCLIP | CONTENTS_MONSTERCLIP))
		return TraceWrap(pos, vec3_origin, vec3_origin, pos, nullptr, MASK_ALL & ~(CONTENTS_DEADMONSTER | CONTENTS_MONSTER)).contents;

	return contents;
}

constexpr auto MASK_CLIP = MASK_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_MONSTERCLIP;

static bool CheckGoodOffsettedPoint(vec3_array &point, trace_t &tr)
{
	static int8_t offsets[] { 0, 8, 16, -8, -16 };

	for (int8_t offset : offsets)
	{
		vec3_array offsetted { point[0], point[1], point[2] + offset };

		brushcontents_t c = PointContents(offsetted.data()) & ~CONTENTS_WATER;

		if (c == CONTENTS_NONE)
		{
			tr = TraceWrap(offsetted.data(), vec3_origin, vec3_origin, vec3_t { offsetted[0], offsetted[1], offsetted[2] - 1024 }, nullptr, MASK_CLIP);
			point = offsetted;
			return true;
		}
	}

	return false;
}

static void AttemptPositioningFix(const vec3_t dir, vec3_t end)
{
	vec3_t negative, positive;

	VectorMA(end, -16, dir, negative);
	VectorMA(end, 16, dir, positive);

	// no fit; let's shift the x/y around. first, check X.
	trace_t x_trace = TraceWrap(negative, vec3_origin, vec3_origin, positive, nullptr, MASK_CLIP);

	// -16 to 16 is inside a wall *or* in a good spot already, try the opposite
	if (x_trace.startsolid || x_trace.allsolid || x_trace.fraction == 1.0)
		x_trace = TraceWrap(positive, vec3_origin, vec3_origin, negative, nullptr, MASK_CLIP);
		
	// we hit a wall; project outwards from this wall piece
	if (!x_trace.startsolid && !x_trace.allsolid && x_trace.fraction < 1.0)
		VectorMA(x_trace.endpos, 16, x_trace.plane.normal, end);
}

static bool PointIsGood(vec3_array &point)
{
	trace_t tr;

	if (!CheckGoodOffsettedPoint(point, tr))
		return false;

	vec3_t end { tr.endpos[0], tr.endpos[1], tr.endpos[2] + 0.125f };

	if (/*tr.startsolid || */tr.allsolid || tr.fraction >= 1.0f || tr.plane.normal[2] < 0.7)
		return false;

	if ((tr.surface->flags & (SURF_SKY | SURF_NODRAW)) || PointContents(end) != CONTENTS_NONE)
		return false;

	VectorMA(end, 16.f, tr.plane.normal, end);

	// test monster positioning
	trace_t fit_trace = TraceWrap(end, small_monster_mins, small_monster_maxs, end, nullptr, MASK_CLIP);

	if (fit_trace.startsolid || fit_trace.allsolid)
	{
		AttemptPositioningFix(vec3_t { 1, 0, 0 }, end);

		fit_trace = TraceWrap(end, small_monster_mins, small_monster_maxs, end, nullptr, MASK_CLIP);

		if (fit_trace.startsolid || fit_trace.allsolid)
		{
			AttemptPositioningFix(vec3_t { 0, 1, 0 }, end);

			fit_trace = TraceWrap(end, small_monster_mins, small_monster_maxs, end, nullptr, MASK_CLIP);
		}
	}

	if (fit_trace.startsolid || fit_trace.allsolid)
		return false;

	for (int16_t z = vec_to_grid(point[2]); z >= 0; z--)
	{
		const grid_array g { vec_to_grid(point[0]), vec_to_grid(point[1]), z };
		
		if (!grid.contains(g))
			continue;

		auto &p = grid[g];

		if (VectorDistance(p.position.data(), end) < 1)
			return false;
	}

	VectorCopy(end, point.data());
	return true;
}

template<>
struct std::hash<std::array<float, 3>>
{
	size_t operator()(const std::array<float, 3>& _Keyval) const
	{
		union {
			size_t p;
			struct {
				uint8_t x, y, z;
			} xyz;
		};
		
		xyz.x = (_Keyval[0] - -4096) / spawn_grid_size;
		xyz.y = (_Keyval[1] - -4096) / spawn_grid_size;
		xyz.z = (_Keyval[2] - -4096) / spawn_grid_size;

		return p;
	}
};

std::vector<std::pair<grid_array, nav_grid_node*>> points;

void SP_monster_berserk (edict_t *self);
void SP_monster_gladiator (edict_t *self);
void SP_monster_gunner (edict_t *self);
void SP_monster_infantry (edict_t *self);
void SP_monster_soldier_light (edict_t *self);
void SP_monster_soldier (edict_t *self);
void SP_monster_soldier_ss (edict_t *self);
void SP_monster_tank (edict_t *self);
void SP_monster_medic (edict_t *self);
void SP_monster_chick (edict_t *self);
void SP_monster_parasite (edict_t *self);
void SP_monster_flyer (edict_t *self);
void SP_monster_brain (edict_t *self);
void SP_monster_floater (edict_t *self);
void SP_monster_hover (edict_t *self);
void SP_monster_mutant (edict_t *self);

typedef void (*SP_SpawnFunc)(edict_t *);

SP_SpawnFunc monster_funcs[] = {
	SP_monster_berserk,
	SP_monster_gladiator,
	SP_monster_gunner,
	SP_monster_infantry,
	SP_monster_soldier_light,
	SP_monster_soldier,
	SP_monster_soldier_ss,
	SP_monster_tank,
	SP_monster_medic,
	SP_monster_chick,
	SP_monster_parasite,
	SP_monster_brain,
	//SP_monster_flyer,
	//SP_monster_floater,
	//SP_monster_hover,
	SP_monster_mutant
};

/*
================
EntitiesRangeFromSpot

Returns the distance to the nearest player from the given spot
================
*/
static vec_t EntitiesRangeFromSpot (nav_grid_node *spot)
{
	vec_t	bestdistance = 9999999;

	for (int32_t n = 1; n <= globals.num_edicts; n++)
	{
		edict_t *e = &g_edicts[n];

		if (!e->inuse || (!e->client && !(e->svflags & SVF_MONSTER)) || (e->svflags & SVF_NOCLIENT) || e->health <= 0)
			continue;

		vec_t playerdistance = VectorDistance (spot->position.data(), e->s.origin);

		if (playerdistance < bestdistance)
			bestdistance = playerdistance;
	}

	return bestdistance;
}

/*
================
SelectFarthestMonsterSpawnPoint
================
*/
static nav_grid_node *SelectFarthestMonsterSpawnPoint(std::unordered_set<nav_grid_node*> &skip_points)
{
	nav_grid_node *bestspot = nullptr;
	vec_t bestdistance = 0;

	for (auto &pt : points)
	{
		if (skip_points.count(pt.second))
			continue;

		vec_t bestplayerdistance = EntitiesRangeFromSpot (pt.second);

		if (bestplayerdistance > bestdistance)
		{
			bestspot = pt.second;
			bestdistance = bestplayerdistance;
		}
	}

	if (bestspot)
		return bestspot;

	return points[irandom(points.size() - 1)].second;
}

template<typename T>
struct std::hash<std::tuple<T, T>>
{
	size_t operator()(const std::tuple<T, T>& _Keyval) const
	{
		std::hash<T> ga;
		return ga(std::get<0>(_Keyval)) ^ ga(std::get<1>(_Keyval));
	}
};

nav_grid_node *ClosestNode(const vec3_t position)
{
	nav_grid_node *best = nullptr;
	float best_dist = -1;

	for (auto &pt : grid)
	{
		float len = VectorDistance(position, pt.second.position.data());

		if (!best || len < best_dist)
		{
			best = &pt.second;
			best_dist = len;
		}
	}

	return best;
}

typedef float (*AStar_EstimateCost)(nav_grid_node *node, nav_grid_node *goal);

struct nav_grid_astar
{
	nav_grid_node *node;
	float fScore;
};

template<>
struct std::less<nav_grid_astar>
{
	constexpr bool operator()(const nav_grid_astar &lhs, const nav_grid_astar &rhs) const 
	{
		return lhs.fScore < rhs.fScore;
	}
};

static std::vector<nav_grid_node *> AStarReconstruct(std::unordered_map<nav_grid_node *, nav_grid_node *> &cameFrom, nav_grid_node *current)
{
    std::vector<nav_grid_node *> total_path { current };

    while (cameFrom.contains(current))
	{
        current = cameFrom[current];
        total_path.push_back(current);
	}
	
	std::reverse(total_path.begin(), total_path.end());
    return total_path;
}

std::vector<nav_grid_node *> AStar(nav_grid_node *start, nav_grid_node *goal, AStar_EstimateCost h, AStar_EstimateCost d)
{
	std::set<nav_grid_astar> openSet;
	openSet.emplace(nav_grid_astar { .node = start, .fScore = h(start, goal) });

	std::unordered_set<nav_grid_node*> openSetNodes;
	openSetNodes.emplace(start);

	std::unordered_map<nav_grid_node *, nav_grid_node *> cameFrom;
	
	std::unordered_map<nav_grid_node *, float> gScore;
	gScore[start] = 0;

	while (!openSet.empty())
	{
		auto current = openSet.begin();
		auto currentNode = (*current).node;

        if (currentNode == goal)
            return AStarReconstruct(cameFrom, (*current).node);

		openSet.erase(current);
		openSetNodes.erase(currentNode);

        for (auto &neighbor_id : currentNode->connections)
		{
			auto neighbor = &grid[neighbor_id];
            auto tentative_gScore = gScore[currentNode] + d(currentNode, neighbor);

            if (tentative_gScore < (gScore.contains(neighbor) ? gScore[neighbor] : INFINITY))
			{
                cameFrom[neighbor] = currentNode;
                gScore[neighbor] = tentative_gScore;

				float score = gScore[neighbor] + h(neighbor, goal);

                if (!openSetNodes.contains(neighbor))
				{
					openSet.emplace(nav_grid_astar { .node = neighbor, .fScore = score });
					openSetNodes.emplace(neighbor);
				}
				else for (auto it = openSet.begin(); it != openSet.end(); it++)
				{
					if ((*it).node != neighbor)
						continue;

					openSet.erase(it);
					openSet.emplace(nav_grid_astar { .node = neighbor, .fScore = score });
					break;
				}
			}
		}
	}

	return std::vector<nav_grid_node *>();
}

std::vector<nav_grid_node*> the_path;
std::vector<nav_grid_node*>::iterator path_iterator;

float Estimate(nav_grid_node *current, nav_grid_node *goal)
{
	return VectorDistance(current->position.data(), goal->position.data());
}

void Cmd_Pos_f(edict_t *ent)
{
	return;
	/*auto v = grid_to_vec_array(vec_to_grid_array({ ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] }));
	PointIsGood(v);
	gi.WriteByte(SVC_TEMP_ENTITY);
	gi.WriteByte(TE_FLECHETTE);
	gi.WritePosition(v.data());
	gi.WriteDir(vec3_t { 0, 0, 1 });
	gi.multicast(v.data(), MULTICAST_ALL);
	return;*/

	nav_grid_node *goal = ClosestNode(G_Find(NULL, FOFS(classname), "info_player_deathmatch")->s.origin);
	nav_grid_node *current = ClosestNode(ent->s.origin);

	the_path = AStar(current, goal, Estimate, Estimate);
	path_iterator = the_path.begin();

	gi.cprintf(ent, PRINT_HIGH, "path: %u len\n", the_path.size());
}

void DrawPoints()
{
#if NODRAW
	int draw_count = 0;

	nav_grid_node *prev = nullptr;

	if (the_path.size())
	for (; ; path_iterator++)
	{
		if (path_iterator == the_path.end())
		{
			path_iterator = the_path.begin();
			prev = nullptr;
		}
		
		auto cur = *path_iterator;

		if (prev)
		{
			gi.WriteByte(SVC_TEMP_ENTITY);
			gi.WriteByte(TE_BFG_LASER);
			gi.WritePosition(prev->position.data());
			gi.WritePosition(cur->position.data());
			gi.multicast(cur->position.data(), MULTICAST_ALL);

			if (++draw_count > 8)
				break;
		}

		prev = cur;
	}

	auto closest = ClosestNode(g_edicts[1].s.origin);

	if (closest)
	{
		/*gi.WriteByte(SVC_TEMP_ENTITY);
		gi.WriteByte(TE_BFG_LASER);
		gi.WritePosition(vec3_t { closest->position[0] - 16, closest->position[1], closest->position[2] });
		gi.WritePosition(vec3_t { closest->position[0] + 16, closest->position[1], closest->position[2] });
		gi.multicast(closest->position.data(), MULTICAST_ALL);
		
		gi.WriteByte(SVC_TEMP_ENTITY);
		gi.WriteByte(TE_BFG_LASER);
		gi.WritePosition(vec3_t { closest->position[0], closest->position[1] - 16, closest->position[2] });
		gi.WritePosition(vec3_t { closest->position[0], closest->position[1] + 16, closest->position[2] });
		gi.multicast(closest->position.data(), MULTICAST_ALL);*/

		for (auto &connection : closest->connections)
		{
			auto connected_node = &grid[connection];

			gi.WriteByte(SVC_TEMP_ENTITY);
			gi.WriteByte(TE_BFG_LASER);
			gi.WritePosition(closest->position.data());
			gi.WritePosition(connected_node->position.data());
			gi.multicast(closest->position.data(), MULTICAST_ALL);
		}
	}
#endif
	static int num_monsters = 0;
	
	if (num_monsters < 64)
	{
		edict_t *ent = G_Spawn();
		monster_funcs[irandom(lengthof(monster_funcs) - 1)](ent);

		gi.unlinkentity(ent);

		std::unordered_set<nav_grid_node*> skip_points;

		while (true)
		{
			auto pt = SelectFarthestMonsterSpawnPoint(skip_points);
			VectorCopy(pt->position.data(), ent->s.origin);
			ent->s.origin[2] -= ent->mins[2];

			trace_t tr = TraceWrap(ent->s.origin, ent->mins, ent->maxs, ent->s.origin, nullptr, MASK_CLIP);

			if (tr.allsolid || tr.startsolid || tr.fraction != 1.0)
			{
				skip_points.emplace(pt);
				continue;
			}

			break;
		}

		gi.linkentity(ent);
		ent->s.angles[1] = random(360);
		ent->flags |= FL_PARTIALGROUND;

		gi.linkentity(ent);
		num_monsters++;
	}
}

static void PrecacheMonsters()
{
	edict_t *ent = G_Spawn();

	for (auto func : monster_funcs)
		func(ent);

	G_FreeEdict(ent);
}

static void FindGridConnections(std::pair<const grid_array, nav_grid_node> &node)
{
	for (int8_t x = -1; x <= 1; x++)
	for (int8_t y = -1; y <= 1; y++)
	for (int8_t z = 0; z <= node.first[2] + 1; z++)
	{
		if (x == 0 && y == 0 && z == node.first[2])
			continue;

		const grid_array grid_val { node.first[0] + x,  node.first[1] + y, z };

		if (!grid.contains(grid_val))
			continue;

		const auto &n = grid[grid_val];

		trace_t tr = TraceWrap(node.second.position.data(), vec3_origin, vec3_origin, n.position.data(), nullptr, MASK_CLIP);

		if (tr.fraction != 1.0f)
		{
			// try a step up; trace from cur to up, then up to node
			if (n.position.data()[2] > node.second.position[2])
			{
				vec3_array step_up = node.second.position;

				while (fabs(step_up[2] - n.position.data()[2]))
				{
					// go upwards
					vec3_array moved_up = { step_up[0], step_up[1], min(n.position.data()[2], step_up[2] + 18) };
					tr = TraceWrap(step_up.data(), vec3_origin, vec3_origin, moved_up.data(), nullptr, MASK_CLIP);

					step_up = moved_up;

					// must be clear
					if (tr.fraction == 1.0f)
					{
						// go towards the node
						tr = TraceWrap(step_up.data(), vec3_origin, vec3_origin, n.position.data(), nullptr, MASK_CLIP);

						// we potentially hit a stair; copy endpos, try again on next loop
						if (tr.fraction < 1.0f)
							step_up = { tr.endpos[0], tr.endpos[1], tr.endpos[2] };
					}

					if (tr.fraction == 1.0f)
						break;
				}
			}

			if (tr.fraction != 1.0f && n.position.data()[2] < node.second.position[2])
			{
				// try a step down; trace from cur to node.xy, then node.xy to node
				tr = TraceWrap(node.second.position.data(), vec3_origin, vec3_origin, vec3_t { n.position[0], n.position[1], node.second.position[2] }, nullptr, MASK_CLIP);

				// missed straight trace, try a step up first
				if (tr.fraction != 1.0f)
				{
					trace_t step_up = TraceWrap(node.second.position.data(), vec3_origin, vec3_origin, vec3_t { node.second.position[0], node.second.position[1], node.second.position[2] + 18 }, nullptr, MASK_CLIP);

					if (step_up.fraction == 1.0f)
						tr = TraceWrap(vec3_t { node.second.position[0], node.second.position[1], node.second.position[2] + 18 }, vec3_origin, vec3_origin, vec3_t { n.position[0], n.position[1], node.second.position[2] + 18 }, nullptr, MASK_CLIP);
				}

				if (tr.fraction == 1.0f)
					tr = TraceWrap(vec3_t { n.position[0], n.position[1], node.second.position[2] }, vec3_origin, vec3_origin, n.position.data(), nullptr, MASK_CLIP);
			}
		}

		if (tr.fraction != 1.0f || tr.startsolid || tr.allsolid)
			continue;
	
		node.second.connections.emplace(grid_val);
	}
}

static void FloodFillMark(nav_grid_node *node, std::unordered_set<grid_array> &visited_nodes)
{
	if (!node || visited_nodes.contains(node->grid_position))
		return;

	visited_nodes.emplace(node->grid_position);

	for (auto connection : node->connections)
		FloodFillMark(&grid[connection], visited_nodes);
}

static void FindSpawnPoints()
{
	std::unordered_set<std::array<float, 3>> hashed_points;

	for (uint8_t z = 0; z < grid_max; z++)
	for (uint8_t y = 0; y < grid_max; y++)
	for (uint8_t x = 0; x < grid_max; x++)
	{
		const grid_array grid_val { x, y, z };
		vec3_array point = grid_to_vec_array(grid_val);

		if (!PointIsGood(point))
			continue;

		hashed_points.emplace(point);
		grid[grid_val] = nav_grid_node { .grid_position = grid_val, .position = point };
	}

	for (auto &p : grid)
		FindGridConnections(p);

	std::unordered_set<grid_array> visited_nodes;
	edict_t *point = nullptr;

	while (point = G_Find(point, FOFS(classname), "info_player_deathmatch"))
		FloodFillMark(ClosestNode(point->s.origin), visited_nodes);
	
	for (auto it = grid.begin(); it != grid.end(); )
	{
		if (!visited_nodes.contains((*it).first))
			it = grid.erase(it);
		else
			it++;
	}

	for (auto &p : grid)
		points.push_back(std::make_pair(p.first, &p.second));

	std::sort(points.begin(), points.end(), [] (auto &x, auto &y) {
		if (x.second->position[2] == y.second->position[2])
		{
			if (x.second->position[1] == y.second->position[1])
				return x.second->position[0] > y.second->position[0];

			return x.second->position[1] > y.second->position[1];
		}

		return x.second->position[2] > y.second->position[2];
	});

	gi.dprintf("Found %u spawn points\n", grid.size());
}

/*
==============
SpawnEntities

Creates a server's entity / program execution context by
parsing textual entity definitions out of an ent file.
==============
*/
void SpawnEntities (char *mapname, char *entities, char *spawnpoint)
{
	cvar_t *deathmatch = gi.cvar ("deathmatch", "1", CVAR_LATCH);
	cvar_t *coop = gi.cvar ("coop", "0", CVAR_LATCH);

	if (!deathmatch->value || coop->value)
	{
		gi.dprintf("Needs dm 1/coop 0, setting...\n");
		gi.cvar_forceset("sv_allow_map", "1");
		gi.cvar_forceset("deathmatch", "1");
		gi.cvar_forceset("coop", "0");
		gi.AddCommandString(va("map %s\n", mapname));
	}

	edict_t		*ent;
	int32_t			inhibit;
	char		*com_token;
	int32_t			i;

	SaveClientData ();

	gi.FreeTags (TAG_LEVEL);

	memset (&level, 0, sizeof(level));
	memset (g_edicts, 0, game.maxentities * sizeof (g_edicts[0]));

	strncpy (level.mapname, mapname, sizeof(level.mapname)-1);

	// set client fields on player ents
	for (i=0 ; i<game.maxclients ; i++)
		g_edicts[i+1].client = game.clients + i;

	ent = nullptr;
	inhibit = 0;

// parse ents
	while (1)
	{
		// parse the opening brace	
		com_token = COM_Parse (&entities);
		if (!entities)
			break;
		if (com_token[0] != '{')
			gi.error ("ED_LoadFromFile: found %s when expecting {",com_token);

		if (!ent)
			ent = g_edicts;
		else
			ent = G_Spawn ();
		entities = ED_ParseEdict (entities, ent);

		// yet another map hack
		if (!stricmp(level.mapname, "command") && !stricmp(ent->classname, "trigger_once") && !stricmp(ent->model, "*27"))
			ent->spawnflags &= ~SPAWNFLAG_NOT_HARD;

		// remove things (except the world) from different skill levels or deathmatch
		if (ent != g_edicts)
		{
			if ( ent->spawnflags & SPAWNFLAG_NOT_DEATHMATCH )
			{
				G_FreeEdict (ent);	
				inhibit++;
				continue;
			}

			ent->spawnflags &= ~(SPAWNFLAG_NOT_EASY|SPAWNFLAG_NOT_MEDIUM|SPAWNFLAG_NOT_HARD|SPAWNFLAG_NOT_COOP|SPAWNFLAG_NOT_DEATHMATCH);
		}

		ED_CallSpawn (ent);
	}	

	gi.dprintf ("%i entities inhibited\n", inhibit);

	FindSpawnPoints();

#if defined(DEBUG)
	i = 1;
	ent = EDICT_NUM(i);
	while (i < globals.num_edicts) {
		if (ent->inuse != 0 || ent->inuse != 1)
			Com_DPrintf("Invalid entity %d\n", i);
		i++, ent++;
	}
#endif

	G_FindTeams ();
}


//===================================================================

#if 0
	// cursor positioning
	xl <value>
	xr <value>
	yb <value>
	yt <value>
	xv <value>
	yv <value>

	// drawing
	statpic <name>
	pic <stat>
	num <fieldwidth> <stat>
	string <stat>

	// control
	if <stat>
	ifeq <stat> <value>
	ifbit <stat> <value>
	endif

#endif

char *dm_statusbar =
"yb	-24 "

// health
"xv	0 "
"hnum "
"xv	50 "
"pic 0 "

// ammo
"if 3 "
"	xv	100 "
"	anum "
"	xv	150 "
"	pic 2 "
"endif "

//  frags
"xr	-50 "
"yt 2 "
"num 3 14 "

// spectator
"if 17 "
  "xv 0 "
  "yb -58 "
  "string2 \"SPECTATOR MODE\" "
"endif "

// chase camera
"if 16 "
  "xv 0 "
  "yb -68 "
  "string \"Chasing\" "
  "xv 64 "
  "stat_string 16 "
"endif "

// ammo types
"xr -28 "

"yt 64 "
"picn w_machinegun "

"yt 92 "
"picn w_shotgun "

"yt 120 "
"picn w_glauncher "

"xr -80 "

"yt 64 "
"num 3 18 "

"yt 92 "
"num 3 19 "

"yt 120 "
"num 3 20 "
;

/*QUAKED worldspawn (0 0 0) ?

Only used for the world.
"sky"	environment map name
"skyaxis"	vector axis for rotating sky
"skyrotate"	speed of rotation in degrees/second
"sounds"	music cd track number
"gravity"	800 is default gravity
"message"	text to print at user logon
*/
void SP_worldspawn (edict_t *ent)
{
	ent->movetype = MOVETYPE_PUSH;
	ent->solid = SOLID_BSP;
	ent->inuse = true;			// since the world doesn't use G_Spawn()
	ent->s.modelindex = MODEL_WORLD;		// world model is always index 1

	//---------------

	// reserve some spots for dead player bodies for coop / deathmatch
	InitBodyQue ();

	if (st.nextmap)
		strcpy (level.nextmap, st.nextmap);

	// make some data visible to the server

	if (ent->message && ent->message[0])
	{
		gi.configstring (CS_NAME, ent->message);
		strncpy (level.level_name, ent->message, sizeof(level.level_name));
	}
	else
		strncpy (level.level_name, level.mapname, sizeof(level.level_name));

	if (st.sky && st.sky[0])
		gi.configstring (CS_SKY, st.sky);
	else
		gi.configstring (CS_SKY, "unit1_");

	gi.configstring (CS_SKYROTATE, va("%f", st.skyrotate) );

	gi.configstring (CS_SKYAXIS, va("%f %f %f",
		st.skyaxis[0], st.skyaxis[1], st.skyaxis[2]) );

	gi.configstring (CS_CDTRACK, va("%i", ent->sounds) );

	gi.configstring (CS_MAXCLIENTS, va("%i", (int32_t)(maxclients->value) ) );

	// status bar program
	gi.configstring (CS_STATUSBAR, dm_statusbar);

	// items
	InitItems ();

	//---------------

	level.pic_health = gi.imageindex ("i_health");
	gi.imageindex ("help");

	if (!st.gravity)
		gi.cvar_set("sv_gravity", "800");
	else
		gi.cvar_set("sv_gravity", st.gravity);

	snd_fry = gi.soundindex ("player/fry.wav");	// standing in lava / slime

	gi.soundindex ("player/lava1.wav");
	gi.soundindex ("player/lava2.wav");

	gi.soundindex ("misc/talk1.wav");

	gi.soundindex ("misc/udeath.wav");

	// sexed sounds
	gi.soundindex ("*death1.wav");
	gi.soundindex ("*death2.wav");
	gi.soundindex ("*death3.wav");
	gi.soundindex ("*death4.wav");
	gi.soundindex ("*fall1.wav");
	gi.soundindex ("*fall2.wav");	
	gi.soundindex ("*gurp1.wav");		// drowning damage
	gi.soundindex ("*gurp2.wav");	
	gi.soundindex ("*jump1.wav");		// player jump
	gi.soundindex ("*pain25_1.wav");
	gi.soundindex ("*pain25_2.wav");
	gi.soundindex ("*pain50_1.wav");
	gi.soundindex ("*pain50_2.wav");
	gi.soundindex ("*pain75_1.wav");
	gi.soundindex ("*pain75_2.wav");
	gi.soundindex ("*pain100_1.wav");
	gi.soundindex ("*pain100_2.wav");

	//-------------------

	gi.soundindex ("player/gasp1.wav");		// gasping for air
	gi.soundindex ("player/gasp2.wav");		// head breaking surface, not gasping

	gi.soundindex ("player/watr_in.wav");	// feet hitting water
	gi.soundindex ("player/watr_out.wav");	// feet leaving water

	gi.soundindex ("player/watr_un.wav");	// head going underwater
	
	gi.soundindex ("player/u_breath1.wav");
	gi.soundindex ("player/u_breath2.wav");

	gi.soundindex ("items/pkup.wav");		// bonus item pickup
	gi.soundindex ("world/land.wav");		// landing thud
	gi.soundindex ("misc/h2ohit1.wav");		// landing splash

	gi.soundindex ("weapons/noammo.wav");

	sm_meat_index = gi.modelindex ("models/objects/gibs/sm_meat/tris.md2");
	gi.modelindex ("models/objects/gibs/arm/tris.md2");
	gi.modelindex ("models/objects/gibs/bone/tris.md2");
	gi.modelindex ("models/objects/gibs/bone2/tris.md2");
	gi.modelindex ("models/objects/gibs/chest/tris.md2");
	gi.modelindex ("models/objects/gibs/skull/tris.md2");
	gi.modelindex ("models/objects/gibs/head2/tris.md2");

//
// Setup light animation tables. 'a' is total darkness, 'z' is doublebright.
//

	// 0 normal
	gi.configstring(CS_LIGHTS+0, "m");
	
	// 1 FLICKER (first variety)
	gi.configstring(CS_LIGHTS+1, "mmnmmommommnonmmonqnmmo");
	
	// 2 SLOW STRONG PULSE
	gi.configstring(CS_LIGHTS+2, "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");
	
	// 3 CANDLE (first variety)
	gi.configstring(CS_LIGHTS+3, "mmmmmaaaaammmmmaaaaaabcdefgabcdefg");
	
	// 4 FAST STROBE
	gi.configstring(CS_LIGHTS+4, "mamamamamama");
	
	// 5 GENTLE PULSE 1
	gi.configstring(CS_LIGHTS+5,"jklmnopqrstuvwxyzyxwvutsrqponmlkj");
	
	// 6 FLICKER (second variety)
	gi.configstring(CS_LIGHTS+6, "nmonqnmomnmomomno");
	
	// 7 CANDLE (second variety)
	gi.configstring(CS_LIGHTS+7, "mmmaaaabcdefgmmmmaaaammmaamm");
	
	// 8 CANDLE (third variety)
	gi.configstring(CS_LIGHTS+8, "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");
	
	// 9 SLOW STROBE (fourth variety)
	gi.configstring(CS_LIGHTS+9, "aaaaaaaazzzzzzzz");
	
	// 10 FLUORESCENT FLICKER
	gi.configstring(CS_LIGHTS+10, "mmamammmmammamamaaamammma");

	// 11 SLOW PULSE NOT FADE TO BLACK
	gi.configstring(CS_LIGHTS+11, "abcdefghijklmnopqrrqponmlkjihgfedcba");
	
	// styles 32-62 are assigned by the light program for switchable lights

	// 63 testing
	gi.configstring(CS_LIGHTS+63, "a");

	PrecacheMonsters();
}

