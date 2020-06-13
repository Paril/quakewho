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
// g_local.h -- local definitions for game module
#pragma once

// define GAME_INCLUDE so that game.h does not define the
// int16_t, server-visible gclient_t and edict_t structures,
// because we define the full size ones in this file
#define	GAME_INCLUDE
#include "game.h"

// the "gameversion" client command will print this plus compile date
extern const char *GAMEVERSION;

// protocol bytes that can be directly added to messages
enum
{
	SVC_BAD,

	SVC_MUZZLEFLASH,
	SVC_MUZZLEFLASH2,
	SVC_TEMP_ENTITY,
	SVC_LAYOUT,
	SVC_INVENTORY,
	SVC_NOP,
	SVC_DISCONNECT,
	SVC_RECONNECT,
	SVC_SOUND,					// <see code>
	SVC_PRINT,					// [uint8_t] id [string] null terminated string
	SVC_STUFFTEXT,				// [string] stuffed into client's console buffer, should be \n terminated
	SVC_SERVERDATA,				// [long] protocol ...
	SVC_CONFIGSTRING,			// [int16_t] [string]
	SVC_SPAWNBASELINE,		
	SVC_CENTERPRINT,			// [string] to put in center of the screen
	SVC_DOWNLOAD,				// [int16_t] size [size bytes]
	SVC_PLAYERINFO,				// variable
	SVC_PACKETENTITIES,			// [...]
	SVC_DELTAPACKETENTITIES,	// [...]
	SVC_FRAME,

	SVC_MAX_ENTTYPES
};

//==================================================================

// view pitching times
const vec_t DAMAGE_TIME		= 0.5f;
const vec_t FALL_TIME		= 0.3f;

// edict->flags
enum edictflags_t : uint32_t
{
	FL_NONE					= 0,
	FL_FLY					= bit(0),
	FL_SWIM					= bit(1),	// implied immunity to drowining
	FL_IMMUNE_LASER			= bit(2),
	FL_INWATER				= bit(3),
	FL_GODMODE				= bit(4),
	FL_NOTARGET				= bit(5),
	FL_IMMUNE_SLIME			= bit(6),
	FL_IMMUNE_LAVA			= bit(7),
	FL_PARTIALGROUND		= bit(8),	// not all corners are valid
	FL_WATERJUMP			= bit(9),	// player jumping out of water
	FL_TEAMSLAVE			= bit(10),	// not the first on the team
	FL_NO_KNOCKBACK			= bit(11),
	FL_POWER_ARMOR			= bit(12),	// power armor (if any) is active
	FL_RESPAWN				= bit(31u)	// used for item respawning
};

MAKE_BITFLAGS(edictflags_t);

const vec_t	FRAMETIME		= 0.1;

// memory tags to allow dynamic memory to be cleaned up
const int32_t TAG_GAME	= 765;		// clear when unloading the dll
const int32_t TAG_LEVEL	= 766;		// clear when loading a new level

const int32_t MELEE_DISTANCE	= 80;

const uint32_t BODY_QUEUE_SIZE		= 8;

enum damage_t : uint8_t
{
	DAMAGE_NO,
	DAMAGE_YES,			// will take damage if hit
	DAMAGE_AIM			// auto targeting recognizes this
};

enum weaponstate_t : uint8_t
{
	WEAPON_READY, 
	WEAPON_ACTIVATING,
	WEAPON_DROPPING,
	WEAPON_FIRING
};

enum
{
	AMMO_BULLETS,
	AMMO_SHELLS,
	AMMO_GRENADES,
	
	AMMO_TOTAL,
	AMMO_NONE = AMMO_TOTAL
};

typedef uint8_t ammo_t;

//deadflag
enum deadflag_t : uint8_t
{
	DEAD_NO,
	DEAD_DYING,
	DEAD_DEAD,
	DEAD_RESPAWNABLE
};

//range
enum range_t : uint8_t
{
	RANGE_MELEE,
	RANGE_NEAR,
	RANGE_MID,
	RANGE_FAR
};

//gib types
enum gibtype_t : bool
{
	GIB_ORGANIC,
	GIB_METALLIC
};

// handedness values
enum handedness_t : uint8_t
{
	RIGHT_HANDED,
	LEFT_HANDED,
	CENTER_HANDED
};

// edict->movetype values
enum movetype_t : uint8_t
{
	MOVETYPE_NONE,			// never moves
	MOVETYPE_NOCLIP,		// origin and angles change with no interaction
	MOVETYPE_PUSH,			// no clip to world, push on box contact
	MOVETYPE_STOP,			// no clip to world, stops on box contact

	MOVETYPE_WALK,			// gravity
	MOVETYPE_STEP,			// gravity, special edge handling
	MOVETYPE_FLY,
	MOVETYPE_TOSS,			// gravity
	MOVETYPE_FLYMISSILE,	// extra size to monsters
	MOVETYPE_BOUNCE
};

// gitem_t->weapmodel for weapons indicates model index
enum
{
	WEAP_BLASTER,
	WEAP_SHOTGUN,
	WEAP_MACHINEGUN,
	WEAP_GRENADELAUNCHER,
	WEAP_TOTAL
};

typedef uint8_t gitem_weapmodel_t;

struct gitem_t
{
	void				(*use)(edict_t *ent, gitem_t *item);
	void				(*weaponthink)(edict_t *ent);
	char				*view_model;
	char				*vwep_model;
	char				*icon;
	char				*pickup_name;
	uint16_t			quantity;
	ammo_t				ammo;
	gitem_weapmodel_t	weapmodel;
	char				*precaches;
};

//
// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
//
struct game_locals_t
{
	gclient_t	*clients;		// [maxclients]

	// store latched cvars here that we want to get at often
	int32_t		maxclients;
	int32_t		maxentities;
};


//
// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
//
struct level_locals_t
{
	int32_t		framenum;
	vec_t		time;

	char		level_name[MAX_QPATH];	// the descriptive name (Outer Base, etc)
	char		mapname[MAX_QPATH];		// the server name (base1, etc)
	char		nextmap[MAX_QPATH];		// go here when fraglimit is hit

	// intermission state
	vec_t		intermissiontime;		// time the intermission was started
	char		*changemap;
	int32_t		exitintermission;
	vec3_t		intermission_origin;
	vec3_t		intermission_angle;

	int32_t		pic_health;

	edict_t		*current_entity;	// entity running from G_RunFrame
	int32_t		body_que;			// dead bodies
};


// spawn_temp_t is only used to hold entity field values that
// can be set from the editor, but aren't actualy present
// in edict_t during gameplay
struct spawn_temp_t
{
	// world vars
	char		*sky;
	vec_t		skyrotate;
	vec3_t		skyaxis;
	char		*nextmap;

	int32_t		lip;
	int32_t		distance;
	int32_t		height;
	char		*noise;
	vec_t		pausetime;
	char		*item;
	char		*gravity;

	vec_t		minyaw;
	vec_t		maxyaw;
	vec_t		minpitch;
	vec_t		maxpitch;
};

enum movestate_t : uint8_t
{
	STATE_TOP,
	STATE_BOTTOM,
	STATE_UP,
	STATE_DOWN
};

struct moveinfo_t
{
	// fixed data
	vec3_t		start_origin;
	vec3_t		start_angles;
	vec3_t		end_origin;
	vec3_t		end_angles;

	soundindex_t sound_start;
	soundindex_t sound_middle;
	soundindex_t sound_end;

	vec_t		accel;
	vec_t		speed;
	vec_t		decel;
	vec_t		distance;

	vec_t		wait;

	// state data
	movestate_t	state;
	vec3_t		dir;
	vec_t		current_speed;
	vec_t		move_speed;
	vec_t		next_speed;
	vec_t		remaining_distance;
	vec_t		decel_distance;
	void		(*endfunc)(edict_t *);
};


struct mframe_t
{
	void	(*aifunc)(edict_t *self, vec_t dist);
	vec_t	dist;
	void	(*thinkfunc)(edict_t *self);
};

struct mmove_t
{
	int32_t		firstframe;
	int32_t		lastframe;
	mframe_t	*frame;
	void		(*endfunc)(edict_t *self);
};

struct monsterinfo_t
{
	mmove_t		*currentmove;
	int32_t		nextframe;
	vec_t		scale;

	void		(*stand)(edict_t *self);
	void		(*idle)(edict_t *self);
	void		(*search)(edict_t *self);
	void		(*walk)(edict_t *self);
	void		(*run)(edict_t *self);

	vec_t		pausetime;

	vec3_t		saved_goal;
	vec_t		search_time;
	vec_t		trail_time;
	vec_t		idle_time;
	int32_t		linkcount;

	vec_t		next_runwalk_check, should_stand_check;
	int32_t		undamaged_skin, damaged_skin;
};

extern	game_locals_t	game;
extern	level_locals_t	level;
extern	game_import_t	gi;
extern	game_export_t	globals;
extern	spawn_temp_t	st;

extern	modelindex_t	sm_meat_index;
extern	soundindex_t	snd_fry;

// means of death
enum meansofdeath_t : uint8_t
{
	MOD_UNKNOWN,
	MOD_BLASTER,
	MOD_SHOTGUN,
	MOD_SSHOTGUN,
	MOD_MACHINEGUN,
	MOD_CHAINGUN,
	MOD_GRENADE,
	MOD_G_SPLASH,
	MOD_ROCKET,
	MOD_R_SPLASH,
	MOD_HYPERBLASTER,
	MOD_RAILGUN,
	MOD_BFG_LASER,
	MOD_BFG_BLAST,
	MOD_BFG_EFFECT,
	MOD_HANDGRENADE,
	MOD_HG_SPLASH,
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_HELD_GRENADE,
	MOD_EXPLOSIVE,
	MOD_BARREL,
	MOD_BOMB,
	MOD_EXIT,
	MOD_SPLASH,
	MOD_TARGET_LASER,
	MOD_TRIGGER_HURT,
	MOD_HIT,
	MOD_TARGET_BLASTER,

	MOD_FRIENDLY_FIRE = bit(7)
};

MAKE_BITFLAGS(meansofdeath_t);

extern	meansofdeath_t	meansOfDeath;


extern	edict_t			*g_edicts;

#define	FOFS(x)		offsetof(edict_t, x)
#define	STOFS(x)	offsetof(spawn_temp_t, x)
#define	LLOFS(x)	offsetof(level_locals_t, x)
#define	CLOFS(x)	offsetof(gclient_t, x)

#include "q_random.h"

extern	cvar_t	*maxentities;
extern	cvar_t	*dmflags;
extern	cvar_t	*fraglimit;
extern	cvar_t	*timelimit;
extern	cvar_t	*password;
extern	cvar_t	*spectator_password;
extern	cvar_t	*needpass;
extern	cvar_t	*g_select_empty;
extern	cvar_t	*dedicated;

extern	cvar_t	*filterban;

extern	cvar_t	*sv_gravity;
extern	cvar_t	*sv_maxvelocity;

extern	cvar_t	*gun_x, *gun_y, *gun_z;
extern	cvar_t	*sv_rollspeed;
extern	cvar_t	*sv_rollangle;

extern	cvar_t	*run_pitch;
extern	cvar_t	*run_roll;
extern	cvar_t	*bob_up;
extern	cvar_t	*bob_pitch;
extern	cvar_t	*bob_roll;

extern	cvar_t	*sv_cheats;
extern	cvar_t	*maxclients;
extern	cvar_t	*maxspectators;

extern	cvar_t	*flood_msgs;
extern	cvar_t	*flood_persecond;
extern	cvar_t	*flood_waitdelay;

extern	cvar_t	*sv_maplist;

#define world	(&g_edicts[0])

// item spawnflags
const uint32_t ITEM_TRIGGER_SPAWN		= bit(0);
const uint32_t ITEM_NO_TOUCH			= bit(1);
// 6 bits reserved for editor flags
// 8 bits used as power cube id bits for coop games
const uint32_t DROPPED_ITEM				= bit(16);
const uint32_t DROPPED_PLAYER_ITEM		= bit(17);
const uint32_t ITEM_TARGETS_USED		= bit(18);

// edict->spawnflags
// these are set with checkboxes on each entity in the map editor
const uint32_t	SPAWNFLAG_NOT_EASY			= bit(8);
const uint32_t	SPAWNFLAG_NOT_MEDIUM		= bit(9);
const uint32_t	SPAWNFLAG_NOT_HARD			= bit(10);
const uint32_t	SPAWNFLAG_NOT_DEATHMATCH	= bit(11);
const uint32_t	SPAWNFLAG_NOT_COOP			= bit(12);

//
// fields are needed for spawning from the entity string
// and saving / loading games
//
enum fieldflags_t : uint8_t
{
	FFL_NONE = 0,
	FFL_SPAWNTEMP = bit(0)
};

MAKE_BITFLAGS(fieldflags_t);

enum fieldtype_t : uint8_t
{
	F_INT, 
	F_FLOAT,
	F_LSTRING,			// string on disk, pointer in memory, TAG_LEVEL
	F_VECTOR,
	F_ANGLEHACK,
	F_IGNORE
};

struct field_t
{
	char	*name;
	int32_t		ofs;
	fieldtype_t	type;
	fieldflags_t flags;
};

extern gitem_t g_weapons[WEAP_TOTAL];

//
// g_cmds.c
//
void Cmd_Help_f (edict_t *ent);
void Cmd_Score_f (edict_t *ent);

//
// g_items.c
//
void InitItems ();
void ChangeWeapon (edict_t *ent);
void Think_Weapon (edict_t *ent);

//
// g_utils.c
//
bool	KillBox (edict_t *ent);
void	G_ProjectSource (const vec3_t point, const vec3_t distance, const vec3_t forward, const vec3_t right, vec3_t result);
edict_t *G_Find (edict_t *from, int32_t fieldofs, char *match);
edict_t *findradius (edict_t *from, vec3_t org, vec_t rad);
edict_t *G_PickTarget (char *targetname);
void	G_UseTargets (edict_t *ent, edict_t *activator);
void	G_SetMovedir (vec3_t angles, vec3_t movedir);

void	G_InitEdict (edict_t *e);
edict_t	*G_Spawn ();
void	G_FreeEdict (edict_t *e);

void	G_TouchTriggers (edict_t *ent);
void	G_TouchSolids (edict_t *ent);

vec_t	*tv (vec_t x, vec_t y, vec_t z);
char	*vtos (vec3_t v);

vec_t vectoyaw (vec3_t vec);
void vectoangles (vec3_t vec, vec3_t angles);

//
// g_spawn.c
//
void ED_CallSpawn (edict_t *ent);

//
// g_combat.c
//

// damage flags
enum damageflag_t : uint8_t
{
	DAMAGE_NONE				= 0,
	DAMAGE_RADIUS			= bit(0),	// damage was indirect
	DAMAGE_NO_ARMOR			= bit(1),	// armour does not protect from this damage
	DAMAGE_ENERGY			= bit(2),	// damage is from an energy based weapon
	DAMAGE_NO_KNOCKBACK		= bit(3),	// do not affect velocity, just view angles
	DAMAGE_BULLET			= bit(4),   // damage is from a bullet (used for ricochets)
	DAMAGE_NO_PROTECTION	= bit(5),   // armor, shields, invulnerability, and godmode have no effect
};

MAKE_BITFLAGS(damageflag_t);

bool OnSameTeam (edict_t *ent1, edict_t *ent2);
bool CanDamage (edict_t *targ, edict_t *inflictor);
void T_Damage (edict_t *targ, edict_t *inflictor, edict_t *attacker, const vec3_t dir, const vec3_t point, const vec3_t normal, int32_t damage, int32_t knockback, damageflag_t dflags, meansofdeath_t mod);
void T_RadiusDamage (edict_t *inflictor, edict_t *attacker, vec_t damage, edict_t *ignore, vec_t radius, meansofdeath_t mod);

const int32_t DEFAULT_BULLET_HSPREAD	= 300;
const int32_t DEFAULT_BULLET_VSPREAD	= 500;
const int32_t DEFAULT_SHOTGUN_HSPREAD	= 1000;
const int32_t DEFAULT_SHOTGUN_VSPREAD	= 500;
const int32_t DEFAULT_DEATHMATCH_SHOTGUN_COUNT	= 12;
const int32_t DEFAULT_SHOTGUN_COUNT	= 12;
const int32_t DEFAULT_SSHOTGUN_COUNT	= 20;

//
// g_monster.c
//
void M_droptofloor (edict_t *ent);
void monster_think (edict_t *self);
void walkmonster_start (edict_t *self);
void swimmonster_start (edict_t *self);
void flymonster_start (edict_t *self);
void monster_death_use (edict_t *self);
void M_CatagorizePosition (edict_t *ent);
void M_CheckGround (edict_t *ent);
bool M_FidgetCheck (edict_t *ent, int percent);

//
// g_misc.c
//
void ThrowHead (edict_t *self, char *gibname, int32_t damage, gibtype_t type);
void ThrowClientHead (edict_t *self, int32_t damage);
void ThrowGib (edict_t *self, char *gibname, int32_t damage, gibtype_t type);
void BecomeExplosion1(edict_t *self);

//
// g_ai.c
//
void ai_stand (edict_t *self, vec_t dist);
void ai_move (edict_t *self, vec_t dist);
void ai_walk (edict_t *self, vec_t dist);
void ai_run (edict_t *self, vec_t dist);
void ai_charge (edict_t *self, vec_t dist);
range_t range (edict_t *self, edict_t *other);

bool infront (edict_t *self, edict_t *other);
bool visible (edict_t *self, edict_t *other);
bool FacingIdeal(edict_t *self);

//
// g_weapon.c
//
void ThrowDebris (edict_t *self, char *modelname, vec_t speed, vec3_t origin);
bool fire_hit (edict_t *self, vec3_t aim, int32_t damage, int32_t kick);
void fire_bullet (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t kick, int32_t hspread, int32_t vspread, meansofdeath_t mod);
void fire_shotgun (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t kick, int32_t hspread, int32_t vspread, int32_t count, meansofdeath_t mod);
void fire_blaster (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t speed, entity_effects_t effect, bool hyper);
void fire_grenade (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t speed, vec_t timer, vec_t damage_radius);
void fire_grenade2 (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t speed, vec_t timer, vec_t damage_radius, bool held);
void fire_rocket (edict_t *self, vec3_t start, vec3_t dir, int32_t damage, int32_t speed, vec_t damage_radius, int32_t radius_damage);
void fire_rail (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t kick);
void fire_bfg (edict_t *self, vec3_t start, vec3_t dir, int32_t damage, int32_t speed, vec_t damage_radius);

//
// g_client.c
//
void respawn (edict_t *ent);
void BeginIntermission (edict_t *targ);
void PutClientInServer (edict_t *ent);
void InitClientPersistant (gclient_t *client);
void InitClientResp (gclient_t *client);
void InitBodyQue ();
void ClientBeginServerFrame (edict_t *ent);

//
// g_player.c
//
void player_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point);

//
// g_svcmds.c
//
void	ServerCommand ();
bool SV_FilterPacket (char *from);

//
// p_view.c
//
void ClientEndServerFrame (edict_t *ent);

//
// p_hud.c
//
void MoveClientToIntermission (edict_t *client);
void G_SetStats (edict_t *ent);
void G_SetSpectatorStats (edict_t *ent);
void G_CheckChaseStats (edict_t *ent);
void DeathmatchScoreboardMessage (edict_t *client, edict_t *killer);

//
// m_move.c
//
bool M_CheckBottom (edict_t *ent);
bool M_walkmove (edict_t *ent, vec_t yaw, vec_t dist);
void M_MoveToGoal (edict_t *ent, vec_t dist);
void M_MoveToController (edict_t *ent, vec_t dist, bool turn);
void M_ChangeYaw (edict_t *ent);

//
// g_phys.c
//
void G_RunEntity (edict_t *ent);

//
// g_main.c
//
void SaveClientData ();
void FetchClientEntData (edict_t *ent);

//
// g_chase.c
//
void UpdateChaseCam(edict_t *ent);
void UpdateTargetCam(edict_t *ent);
void ChaseNext(edict_t *ent);
void ChasePrev(edict_t *ent);
void GetChaseTarget(edict_t *ent);

//============================================================================

// client_t->anim_priority
enum animpriority_t : uint8_t
{
	ANIM_BASIC,		// stand / run
	ANIM_WAVE,
	ANIM_JUMP,
	ANIM_PAIN,
	ANIM_ATTACK,
	ANIM_DEATH,
	ANIM_REVERSE
};

enum playerteam_t : uint8_t
{
	TEAM_NONE,

	TEAM_HIDERS,
	TEAM_HUNTERS
};

// client data that stays across multiple level loads
struct client_persistant_t
{
	char		userinfo[MAX_INFO_STRING];
	char		netname[16];
	handedness_t hand;

	bool		connected;			// a loadgame will leave valid entities that
									// just don't have a connection yet

	// values saved and restored from edicts when changing levels
	int32_t		health;
	int32_t		max_health;
	edictflags_t savedFlags;

	uint32_t	ammo[AMMO_TOTAL];

	gitem_t		*weapon;
	gitem_t		*lastweapon;

	int32_t		score;			// for calculating total unit score in coop games

	bool		spectator;			// client is a spectator
};

// client data that stays across deathmatch respawns
struct client_respawn_t
{
	int32_t			enterframe;			// level.framenum the client entered the game
	int32_t			score;				// frags, etc
	vec3_t			cmd_angles;			// angles sent over in the last command

	bool			spectator;			// client is a spectator
	playerteam_t	team;
};

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
struct gclient_t
{
	player_state_t	ps;				// communicated by server to clients
	int32_t			ping;
	
	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!

	//================================
	client_persistant_t	pers;
	client_respawn_t	resp;
	pmove_state_t		old_pmove;	// for detecting out-of-pmove changes

	bool		showscores;			// set layout stat

	button_t	buttons;
	button_t	oldbuttons;
	button_t	latched_buttons;

	bool		weapon_thunk;

	gitem_t		*newweapon;

	// sum up damage over an entire frame, so
	// shotgun blasts give a single big kick
	int32_t		damage_blood;		// damage taken out of health
	int32_t		damage_knockback;	// impact damage
	vec3_t		damage_from;		// origin for vector calculation

	vec_t		killer_yaw;			// when dead, look at killer

	weaponstate_t	weaponstate;
	vec3_t		kick_angles;	// weapon kicks
	vec3_t		kick_origin;
	vec_t		v_dmg_roll, v_dmg_pitch, v_dmg_time;	// damage kicks
	vec_t		fall_time, fall_value;		// for view drop on fall
	vec_t		damage_alpha;
	vec_t		bonus_alpha;
	vec3_t		damage_blend;
	vec3_t		v_angle;			// aiming direction
	vec_t		bobtime;			// so off-ground doesn't change it
	vec3_t		oldviewangles;
	vec3_t		oldvelocity;

	vec_t		next_drown_time;
	waterlevel_t old_waterlevel;

	// animation vars
	int32_t		anim_end;
	animpriority_t anim_priority;
	bool		anim_duck;
	bool		anim_run;

	soundindex_t weapon_sound;

	vec_t		flood_locktill;		// locked from talking
	vec_t		flood_when[10];		// when messages were said
	int32_t		flood_whenhead;		// head pointer for when said

	vec_t		respawn_time;		// can respawn when time > this

	edict_t		*chase_target;		// player we are chasing
	bool		update_chase;		// need to update chase info?

	bool		control_waitjump;
	usercmd_t	cmd;
	vec_t		jump_sound_debounce;
};

struct edict_t
{
	entity_state_t	s;
	gclient_t	*client;	// nullptr if not a player
									// the server expects the first part
									// of gclient_t to be a player_state_t
									// but the rest of it is opaque

	qboolean	inuse;
	int32_t		linkcount;

	// FIXME: move these fields to a server private sv_entity_t
	link_t		area;				// linked to a division node or leaf
	
	int32_t		num_clusters;		// if -1, use headnode instead
	int32_t		clusternums[MAX_ENT_CLUSTERS];
	int32_t		headnode;			// unused if num_clusters != -1
	int32_t		areanum, areanum2;

	//================================

	svflags_t	svflags;
	vec3_t		mins, maxs;
	vec3_t		absmin, absmax, size;
	solid_t		solid;
	brushcontents_t	clipmask;
	edict_t		*owner;


	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!

	//================================
	movetype_t		movetype;
	edictflags_t	flags;

	char		*model;
	vec_t		freetime;			// sv.time when the object was freed
	
	//
	// only used locally in game, not by server
	//
	char		*message;
	char		*classname;
	uint32_t	spawnflags;

	vec_t		timestamp;

	vec_t		angle;			// set in qe3, -1 = up, -2 = down
	char		*target;
	char		*targetname;
	char		*killtarget;
	char		*team;
	char		*pathtarget;
	char		*deathtarget;
	char		*combattarget;
	edict_t		*target_ent;

	vec_t		speed, accel, decel;
	vec3_t		movedir;
	vec3_t		pos1, pos2;

	vec3_t		velocity;
	vec3_t		avelocity;
	int32_t		mass;
	vec_t		air_finished;
	vec_t		gravity;		// per entity gravity multiplier (1.0 is normal)
								// use for lowgrav artifact, flares

	edict_t		*goalentity;
	edict_t		*movetarget;
	vec_t		yaw_speed;
	vec_t		ideal_yaw;

	vec_t		nextthink;
	void		(*prethink) (edict_t *ent);
	void		(*think)(edict_t *self);
	void		(*blocked)(edict_t *self, edict_t *other);	//move to moveinfo?
	void		(*touch)(edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
	void		(*use)(edict_t *self, edict_t *other, edict_t *activator);
	void		(*pain)(edict_t *self, edict_t *other, vec_t kick, int32_t damage);
	void		(*die)(edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, const vec3_t point);

	vec_t		touch_debounce_time;		// are all these legit?  do we need more/less of them?
	vec_t		pain_debounce_time;
	vec_t		damage_debounce_time;
	vec_t		fly_sound_debounce_time;	//move to clientinfo
	vec_t		last_move_time;

	int32_t		health;
	int32_t		max_health;
	int32_t		gib_health;
	deadflag_t	deadflag;
	vec_t		show_hostile;

	vec_t		powerarmor_time;

	char		*map;			// target_changelevel

	int32_t		viewheight;		// height above origin where eyesight is determined
	damage_t	takedamage;
	int32_t		dmg;
	int32_t		radius_dmg;
	vec_t		dmg_radius;
	int32_t		sounds;			//make this a spawntemp var?
	int32_t		count;

	edict_t		*chain;
	edict_t		*enemy;
	edict_t		*oldenemy;
	edict_t		*activator;
	edict_t		*groundentity;
	int32_t		groundentity_linkcount;
	edict_t		*teamchain;
	edict_t		*teammaster;

	soundindex_t noise_index;
	soundindex_t noise_index2;
	vec_t		volume;
	vec_t		attenuation;

	// timing variables
	vec_t		wait;
	vec_t		delay;			// before firing targets
	vec_t		random;

	vec_t		teleport_time;

	brushcontents_t	watertype;
	waterlevel_t waterlevel;

	vec3_t		move_origin;
	vec3_t		move_angles;

	// move this to clientinfo?
	int32_t		light_level;

	int32_t		style;			// also used as areaportal number

	// common data blocks
	moveinfo_t		moveinfo;
	monsterinfo_t	monsterinfo;

	edict_t		*control;
	vec_t		control_dist;
	pmove_state_t pmove_state, old_pmove_state;
};