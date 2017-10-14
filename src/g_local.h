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

enum ammo_t : uint8_t
{
	AMMO_BULLETS,
	AMMO_SHELLS,
	AMMO_ROCKETS,
	AMMO_GRENADES,
	AMMO_CELLS,
	AMMO_SLUGS
};

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

//monster ai flags
enum aiflags_t : uint32_t
{
	AI_NONE					= 0,
	AI_STAND_GROUND			= bit(0),
	AI_TEMP_STAND_GROUND	= bit(1),
	AI_SOUND_TARGET			= bit(2),
	AI_LOST_SIGHT			= bit(3),
	AI_PURSUIT_LAST_SEEN	= bit(4),
	AI_PURSUE_NEXT			= bit(5),
	AI_PURSUE_TEMP			= bit(6),
	AI_HOLD_FRAME			= bit(7),
	AI_GOOD_GUY				= bit(8),
	AI_BRUTAL				= bit(9),
	AI_NOSTEP				= bit(10),
	AI_DUCKED				= bit(11),
	AI_COMBAT_POINT			= bit(12),
	AI_MEDIC				= bit(13),
	AI_RESURRECTING			= bit(14)
};

MAKE_BITFLAGS(aiflags_t);

//monster attack state
enum aistate_t : uint8_t
{
	AS_STRAIGHT,
	AS_SLIDING,
	AS_MELEE,
	AS_MISSILE
};

// armor types
enum armortype_t
{
	ARMOR_NONE,
	ARMOR_JACKET,
	ARMOR_COMBAT,
	ARMOR_BODY,
	ARMOR_SHARD
};

// power armor types
enum powerarmor_t : uint8_t
{
	POWER_ARMOR_NONE,
	POWER_ARMOR_SCREEN,
	POWER_ARMOR_SHIELD
};

// handedness values
enum handedness_t : uint8_t
{
	RIGHT_HANDED,
	LEFT_HANDED,
	CENTER_HANDED
};

// game.serverflags values
enum serverflags_t : uint8_t
{
	SFL_CROSS_TRIGGER_1 = bit(0),
	SFL_CROSS_TRIGGER_2 = bit(1),
	SFL_CROSS_TRIGGER_3 = bit(2),
	SFL_CROSS_TRIGGER_4 = bit(3),
	SFL_CROSS_TRIGGER_5 = bit(4),
	SFL_CROSS_TRIGGER_6 = bit(5),
	SFL_CROSS_TRIGGER_7 = bit(6),
	SFL_CROSS_TRIGGER_8 = bit(7),
	SFL_CROSS_TRIGGER_MASK = 0xFF
};

MAKE_BITFLAGS(serverflags_t);

// noise types for PlayerNoise
enum playernoise_t : uint8_t
{
	PNOISE_SELF,
	PNOISE_WEAPON,
	PNOISE_IMPACT,
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



struct gitem_armor_t
{
	int32_t		base_count;
	int32_t		max_count;
	vec_t	normal_protection;
	vec_t	energy_protection;
	int32_t		armor;
};


// gitem_t->flags
enum gitem_flags_t : uint8_t
{
	IT_NONE			= 0,
	IT_WEAPON		= bit(0),		// use makes active weapon
	IT_AMMO			= bit(1),
	IT_ARMOR		= bit(2),
	IT_STAY_COOP	= bit(3),
	IT_KEY			= bit(4),
	IT_POWERUP		= bit(5)
};

MAKE_BITFLAGS(gitem_flags_t);

// gitem_t->weapmodel for weapons indicates model index
enum gitem_weapmodel_t : uint8_t
{
	WEAP_NONE,
	WEAP_BLASTER,
	WEAP_SHOTGUN,
	WEAP_SUPERSHOTGUN,
	WEAP_MACHINEGUN,
	WEAP_CHAINGUN,
	WEAP_GRENADES,
	WEAP_GRENADELAUNCHER,
	WEAP_ROCKETLAUNCHER,
	WEAP_HYPERBLASTER,
	WEAP_RAILGUN,
	WEAP_BFG,
};

struct gitem_t
{
	char		*classname;	// spawning name
	bool		(*pickup)(edict_t *ent, edict_t *other);
	void		(*use)(edict_t *ent, gitem_t *item);
	void		(*drop)(edict_t *ent, gitem_t *item);
	void		(*weaponthink)(edict_t *ent);
	char		*pickup_sound;
	char		*world_model;
	entity_effects_t world_model_flags;
	char		*view_model;

	// client side info
	char		*icon;
	char		*pickup_name;	// for printing on pickup
	int32_t			count_width;		// number of digits to display by icon

	int32_t			quantity;		// for ammo how much, for weapons how much is used per shot
	char		*ammo;			// for weapons
	gitem_flags_t flags;			// IT_* flags

	gitem_weapmodel_t weapmodel;		// weapon model index (for weapons)

	void		*info;
	int32_t			tag;

	char		*precaches;		// string of all models, sounds, and images this item will use
};



//
// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
//
struct game_locals_t
{
	char		helpmessage1[512];
	char		helpmessage2[512];
	int32_t			helpchanged;	// flash F1 icon if non 0, play sound
								// and increment only if 1, 2, or 3

	gclient_t	*clients;		// [maxclients]

	// can't store spawnpoint in level, because
	// it would get overwritten by the savegame restore
	char		spawnpoint[512];	// needed for coop respawns

	// store latched cvars here that we want to get at often
	int32_t			maxclients;
	int32_t			maxentities;

	// cross level triggers
	serverflags_t serverflags;

	// items
	int32_t			num_items;

	bool		autosaved;
};


//
// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
//
struct level_locals_t
{
	int32_t			framenum;
	vec_t		time;

	char		level_name[MAX_QPATH];	// the descriptive name (Outer Base, etc)
	char		mapname[MAX_QPATH];		// the server name (base1, etc)
	char		nextmap[MAX_QPATH];		// go here when fraglimit is hit

	// intermission state
	vec_t		intermissiontime;		// time the intermission was started
	char		*changemap;
	int32_t			exitintermission;
	vec3_t		intermission_origin;
	vec3_t		intermission_angle;

	edict_t		*sight_client;	// changed once each frame for coop games

	edict_t		*sight_entity;
	int32_t			sight_entity_framenum;
	edict_t		*sound_entity;
	int32_t			sound_entity_framenum;
	edict_t		*sound2_entity;
	int32_t			sound2_entity_framenum;

	int32_t			pic_health;

	int32_t			total_secrets;
	int32_t			found_secrets;

	int32_t			total_goals;
	int32_t			found_goals;

	int32_t			total_monsters;
	int32_t			killed_monsters;

	edict_t		*current_entity;	// entity running from G_RunFrame
	int32_t			body_que;			// dead bodies

	int32_t			power_cubes;		// ugly necessity for coop
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

	int32_t			lip;
	int32_t			distance;
	int32_t			height;
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
	int32_t			firstframe;
	int32_t			lastframe;
	mframe_t	*frame;
	void		(*endfunc)(edict_t *self);
};

struct monsterinfo_t
{
	mmove_t		*currentmove;
	aiflags_t	aiflags;
	int32_t			nextframe;
	vec_t		scale;

	void		(*stand)(edict_t *self);
	void		(*idle)(edict_t *self);
	void		(*search)(edict_t *self);
	void		(*walk)(edict_t *self);
	void		(*run)(edict_t *self);
	void		(*dodge)(edict_t *self, edict_t *other, vec_t eta);
	void		(*attack)(edict_t *self);
	void		(*melee)(edict_t *self);
	void		(*sight)(edict_t *self, edict_t *other);
	bool		(*checkattack)(edict_t *self);

	vec_t		pausetime;
	vec_t		attack_finished;

	vec3_t		saved_goal;
	vec_t		search_time;
	vec_t		trail_time;
	vec3_t		last_sighting;
	aistate_t	attack_state;
	bool		lefty;
	vec_t		idle_time;
	int32_t			linkcount;

	powerarmor_t power_armor_type;
	int32_t			power_armor_power;
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
extern	cvar_t	*deathmatch;
extern	cvar_t	*coop;
extern	cvar_t	*dmflags;
extern	cvar_t	*skill;
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
	FFL_SPAWNTEMP = bit(0),
	FFL_NOSPAWN = bit(1)
};

MAKE_BITFLAGS(fieldflags_t);

enum fieldtype_t : uint8_t
{
	F_INT, 
	F_FLOAT,
	F_LSTRING,			// string on disk, pointer in memory, TAG_LEVEL
	F_GSTRING,			// string on disk, pointer in memory, TAG_GAME
	F_VECTOR,
	F_ANGLEHACK,
	F_EDICT,			// index on disk, pointer in memory
	F_ITEM,				// index on disk, pointer in memory
	F_CLIENT,			// index on disk, pointer in memory
	F_FUNCTION,
	F_MMOVE,
	F_IGNORE
};

struct field_t
{
	char	*name;
	int32_t		ofs;
	fieldtype_t	type;
	fieldflags_t flags;
};


extern	field_t fields[];
extern	gitem_t	itemlist[];


//
// g_cmds.c
//
void Cmd_Help_f (edict_t *ent);
void Cmd_Score_f (edict_t *ent);

//
// g_items.c
//
void PrecacheItem (gitem_t *it);
void InitItems (void);
void SetItemNames (void);
gitem_t	*FindItem (char *pickup_name);
gitem_t	*FindItemByClassname (char *classname);
#define	ITEM_INDEX(x) ((x)-itemlist)
edict_t *Drop_Item (edict_t *ent, gitem_t *item);
void SetRespawn (edict_t *ent, vec_t delay);
void ChangeWeapon (edict_t *ent);
void SpawnItem (edict_t *ent, gitem_t *item);
void Think_Weapon (edict_t *ent);
int32_t ArmorIndex (edict_t *ent);
powerarmor_t PowerArmorType (edict_t *ent);
gitem_t	*GetItemByIndex (int32_t index);
bool Add_Ammo (edict_t *ent, gitem_t *item, int32_t count);
void Touch_Item (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);

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
edict_t	*G_Spawn (void);
void	G_FreeEdict (edict_t *e);

void	G_TouchTriggers (edict_t *ent);
void	G_TouchSolids (edict_t *ent);

char	*G_CopyString (char *in);

vec_t	*tv (vec_t x, vec_t y, vec_t z);
char	*vtos (vec3_t v);

vec_t vectoyaw (vec3_t vec);
void vectoangles (vec3_t vec, vec3_t angles);

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
void T_Damage (edict_t *targ, edict_t *inflictor, edict_t *attacker, vec3_t dir, vec3_t point, vec3_t normal, int32_t damage, int32_t knockback, damageflag_t dflags, meansofdeath_t mod);
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
void monster_fire_bullet (edict_t *self, vec3_t start, vec3_t dir, int32_t damage, int32_t kick, int32_t hspread, int32_t vspread, int32_t flashtype);
void monster_fire_shotgun (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t kick, int32_t hspread, int32_t vspread, int32_t count, int32_t flashtype);
void monster_fire_blaster (edict_t *self, vec3_t start, vec3_t dir, int32_t damage, int32_t speed, int32_t flashtype, entity_effects_t effect);
void monster_fire_grenade (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t speed, int32_t flashtype);
void monster_fire_rocket (edict_t *self, vec3_t start, vec3_t dir, int32_t damage, int32_t speed, int32_t flashtype);
void monster_fire_railgun (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t kick, int32_t flashtype);
void monster_fire_bfg (edict_t *self, vec3_t start, vec3_t aimdir, int32_t damage, int32_t speed, int32_t kick, vec_t damage_radius, int32_t flashtype);
void M_droptofloor (edict_t *ent);
void monster_think (edict_t *self);
void walkmonster_start (edict_t *self);
void swimmonster_start (edict_t *self);
void flymonster_start (edict_t *self);
void AttackFinished (edict_t *self, vec_t time);
void monster_death_use (edict_t *self);
void M_CatagorizePosition (edict_t *ent);
bool M_CheckAttack (edict_t *self);
void M_FlyCheck (edict_t *self);
void M_CheckGround (edict_t *ent);

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
void AI_SetSightClient (void);

void ai_stand (edict_t *self, vec_t dist);
void ai_move (edict_t *self, vec_t dist);
void ai_walk (edict_t *self, vec_t dist);
void ai_turn (edict_t *self, vec_t dist);
void ai_run (edict_t *self, vec_t dist);
void ai_charge (edict_t *self, vec_t dist);
range_t range (edict_t *self, edict_t *other);

void FoundTarget (edict_t *self);
bool infront (edict_t *self, edict_t *other);
bool visible (edict_t *self, edict_t *other);
bool FacingIdeal(edict_t *self);
bool FindTarget (edict_t *self);

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
// g_ptrail.c
//
void PlayerTrail_Init (void);
void PlayerTrail_Add (vec3_t spot);
void PlayerTrail_New (vec3_t spot);
edict_t *PlayerTrail_PickFirst (edict_t *self);
edict_t *PlayerTrail_PickNext (edict_t *self);
edict_t	*PlayerTrail_LastSpot (void);

//
// g_client.c
//
void respawn (edict_t *ent);
void BeginIntermission (edict_t *targ);
void PutClientInServer (edict_t *ent);
void InitClientPersistant (gclient_t *client);
void InitClientResp (gclient_t *client);
void InitBodyQue (void);
void ClientBeginServerFrame (edict_t *ent);

//
// g_player.c
//
void player_pain (edict_t *self, edict_t *other, vec_t kick, int32_t damage);
void player_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, vec3_t point);

//
// g_svcmds.c
//
void	ServerCommand (void);
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
void ValidateSelectedItem (edict_t *ent);
void DeathmatchScoreboardMessage (edict_t *client, edict_t *killer);

//
// g_pweapon.c
//
void PlayerNoise(edict_t *who, vec3_t where, playernoise_t type);

//
// m_move.c
//
bool M_CheckBottom (edict_t *ent);
bool M_walkmove (edict_t *ent, vec_t yaw, vec_t dist);
void M_MoveToGoal (edict_t *ent, vec_t dist);
void M_ChangeYaw (edict_t *ent);

//
// g_phys.c
//
void G_RunEntity (edict_t *ent);

//
// g_main.c
//
void SaveClientData (void);
void FetchClientEntData (edict_t *ent);

//
// g_chase.c
//
void UpdateChaseCam(edict_t *ent);
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

// client data that stays across multiple level loads
struct client_persistant_t
{
	char		userinfo[MAX_INFO_STRING];
	char		netname[16];
	handedness_t hand;

	bool		connected;			// a loadgame will leave valid entities that
									// just don't have a connection yet

	// values saved and restored from edicts when changing levels
	int32_t			health;
	int32_t			max_health;
	edictflags_t savedFlags;

	int32_t			selected_item;
	int32_t			inventory[MAX_ITEMS];

	// ammo capacities
	int32_t			max_bullets;
	int32_t			max_shells;
	int32_t			max_rockets;
	int32_t			max_grenades;
	int32_t			max_cells;
	int32_t			max_slugs;

	gitem_t		*weapon;
	gitem_t		*lastweapon;

	int32_t			power_cubes;	// used for tracking the cubes in coop games
	int32_t			score;			// for calculating total unit score in coop games

	int32_t			game_helpchanged;
	int32_t			helpchanged;

	bool		spectator;			// client is a spectator
};

// client data that stays across deathmatch respawns
struct client_respawn_t
{
	client_persistant_t	coop_respawn;	// what to set client->pers to on a respawn
	int32_t			enterframe;			// level.framenum the client entered the game
	int32_t			score;				// frags, etc
	vec3_t		cmd_angles;			// angles sent over in the last command

	bool		spectator;			// client is a spectator
};

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
struct gclient_t
{
	player_state_t	ps;				// communicated by server to clients
	int32_t				ping;
	
	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!

	//================================
	client_persistant_t	pers;
	client_respawn_t	resp;
	pmove_state_t		old_pmove;	// for detecting out-of-pmove changes

	bool		showscores;			// set layout stat
	bool		showinventory;		// set layout stat
	bool		showhelp;
	bool		showhelpicon;

	int32_t			ammo_index;

	button_t	buttons;
	button_t	oldbuttons;
	button_t	latched_buttons;

	bool		weapon_thunk;

	gitem_t		*newweapon;

	// sum up damage over an entire frame, so
	// shotgun blasts give a single big kick
	int32_t			damage_armor;		// damage absorbed by armor
	int32_t			damage_parmor;		// damage absorbed by power armor
	int32_t			damage_blood;		// damage taken out of health
	int32_t			damage_knockback;	// impact damage
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
	bool		breather_sound;

	int32_t			machinegun_shots;	// for weapon raising

	// animation vars
	int32_t			anim_end;
	animpriority_t anim_priority;
	bool		anim_duck;
	bool		anim_run;

	// powerup timers
	vec_t		quad_framenum;
	vec_t		invincible_framenum;
	vec_t		breather_framenum;
	vec_t		enviro_framenum;

	bool		grenade_blew_up;
	vec_t		grenade_time;
	int32_t			silencer_shots;
	soundindex_t weapon_sound;

	vec_t		pickup_msg_time;

	vec_t		flood_locktill;		// locked from talking
	vec_t		flood_when[10];		// when messages were said
	int32_t			flood_whenhead;		// head pointer for when said

	vec_t		respawn_time;		// can respawn when time > this

	edict_t		*chase_target;		// player we are chasing
	bool		update_chase;		// need to update chase info?
};


struct edict_t
{
	entity_state_t	s;
	gclient_t	*client;	// nullptr if not a player
									// the server expects the first part
									// of gclient_t to be a player_state_t
									// but the rest of it is opaque

	qboolean	inuse;
	int32_t			linkcount;

	// FIXME: move these fields to a server private sv_entity_t
	link_t		area;				// linked to a division node or leaf
	
	int32_t			num_clusters;		// if -1, use headnode instead
	int32_t			clusternums[MAX_ENT_CLUSTERS];
	int32_t			headnode;			// unused if num_clusters != -1
	int32_t			areanum, areanum2;

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
	int32_t			mass;
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
	void		(*die)(edict_t *self, edict_t *inflictor, edict_t *attacker, int32_t damage, vec3_t point);

	vec_t		touch_debounce_time;		// are all these legit?  do we need more/less of them?
	vec_t		pain_debounce_time;
	vec_t		damage_debounce_time;
	vec_t		fly_sound_debounce_time;	//move to clientinfo
	vec_t		last_move_time;

	int32_t			health;
	int32_t			max_health;
	int32_t			gib_health;
	deadflag_t	deadflag;
	vec_t		show_hostile;

	vec_t		powerarmor_time;

	char		*map;			// target_changelevel

	int32_t			viewheight;		// height above origin where eyesight is determined
	damage_t	takedamage;
	int32_t			dmg;
	int32_t			radius_dmg;
	vec_t		dmg_radius;
	int32_t			sounds;			//make this a spawntemp var?
	int32_t			count;

	edict_t		*chain;
	edict_t		*enemy;
	edict_t		*oldenemy;
	edict_t		*activator;
	edict_t		*groundentity;
	int32_t			groundentity_linkcount;
	edict_t		*teamchain;
	edict_t		*teammaster;

	edict_t		*mynoise;		// can go in client only
	edict_t		*mynoise2;

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
	int32_t			light_level;

	int32_t			style;			// also used as areaportal number

	gitem_t		*item;			// for bonus items

	// common data blocks
	moveinfo_t		moveinfo;
	monsterinfo_t	monsterinfo;
};

