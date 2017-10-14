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

#pragma once

// q_shared.h -- included first by ALL program modules

// float stuff, just to silence for now
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)
#pragma warning(disable : 4242)

#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdint>

#include "q_bitflags.h"

template <size_t size, typename T>
inline constexpr size_t lengthof(T (&arr)[size]) { return size; }

template<typename T>
inline constexpr T bit(const T &place) { return 1u << place; }

// type used for interop, but shouldn't be used by game. Use bool.
typedef int32_t	qboolean;

// this exists elsewhere
struct edict_t;

typedef uint8_t byte;

// angle indexes
enum
{
	PITCH,		// up / down
	YAW,		// left / right
	ROLL		// fall over
};

const size_t	MAX_STRING_CHARS	= 1024;	// max length of a string passed to Cmd_TokenizeString
const size_t	MAX_STRING_TOKENS	= 80;		// max tokens resulting from Cmd_TokenizeString
const size_t	MAX_TOKEN_CHARS		= 128;		// max length of an individual token

const size_t	MAX_QPATH			= 64;		// max length of a quake game pathname
const size_t	MAX_OSPATH			= 128;		// max length of a filesystem pathname

//
// per-level limits
//
const size_t	MAX_CLIENTS			= 256;		// absolute limit
const size_t	MAX_EDICTS			= 1024;	    // must change protocol to increase more
const size_t	MAX_LIGHTSTYLES		= 256;
const size_t	MAX_MODELS			= 256;		// these are sent over the net as bytes
const size_t	MAX_SOUNDS			= 256;		// so they cannot be blindly increased
const size_t	MAX_IMAGES			= 256;
const size_t	MAX_ITEMS			= 256;
const size_t	MAX_GENERAL			= (MAX_CLIENTS * 2);	// general config strings


// game print flags
enum printflags_t
{
	PRINT_LOW,		// pickup messages
	PRINT_MEDIUM,	// death messages
	PRINT_HIGH,		// critical messages
	PRINT_CHAT		// chat messages
};

// destination class for gi.multicast()
enum multicast_t
{
	MULTICAST_ALL,
	MULTICAST_PHS,
	MULTICAST_PVS,
	MULTICAST_ALL_R,
	MULTICAST_PHS_R,
	MULTICAST_PVS_R
};


/*
==============================================================

MATHLIB

==============================================================
*/

typedef float vec_t;
typedef vec_t vec3_t[3];

#ifndef M_PI
 constexpr float M_PI = 3.141592653589793238462643383279502884e+00;
#endif

struct cplane_t;

extern vec3_t vec3_origin;

template<typename Ta, typename Tb>
constexpr auto DotProduct(const Ta *x, const Tb *y)
{
	return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

template<typename Ta, typename Tb, typename Tc>
inline void VectorSubtract(const Ta *a, const Tb *b, Tc *c)
{
	for (int32_t i = 0; i < 3; i++)
		c[i] = a[i] - b[i];
}

template<typename Ta, typename Tb, typename Tc>
inline void VectorAdd(const Ta *a, const Tb *b, Tc *c)
{
	for (int32_t i = 0; i < 3; i++)
		c[i] = a[i] + b[i];
}

template<typename Ta, typename Tb>
inline void VectorCopy(const Ta *a, Tb *b)
{
	for (int32_t i = 0; i < 3; i++)
		b[i] = a[i];
}

template<typename Ta, typename Tx, typename Ty, typename Tz>
inline void VectorSet(Ta *a, const Tx &x, const Ty &y, const Tz &z)
{
	a[0] = x;
	a[1] = y;
	a[2] = z;
}

template<typename T>
inline void VectorClear(T *a)
{
	VectorCopy(vec3_origin, a);
}

template<typename Ta, typename Tb>
inline void VectorNegate(const Ta *a, Tb *b)
{
	for (int32_t i = 0; i < 3; i++)
		b[i] = -a[i];
}

template<typename Ta, typename Ts, typename Tb>
inline void VectorScale(const Ta *a, const Ts &s, Tb *b)
{
	for (int32_t i = 0; i < 3; i++)
		b[i] = a[i] * s;
}

template<typename Ta, typename Ts, typename Tb, typename Tc>
inline void VectorMA (const Ta *a, const Ts &s, const Tb *b, Tc *c)
{
	for (int32_t i = 0; i < 3; i++)
		c[i] = a[i] + s * b[i];
}

template<typename Ta, typename Tb>
constexpr bool VectorCompare (const Ta *v1, const Tb *v2)
{
	return (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2]);
}

template<typename T>
constexpr T VectorLengthSquared(const T *v)
{
	return DotProduct(v, v);
}

template<typename T>
constexpr T VectorLength(const T *v)
{
	return sqrt(VectorLengthSquared(v));
}

template<typename Ta, typename Tb>
inline Ta VectorNormalize (const Ta *v, Tb *o)
{
	Ta length = VectorLength(v);

	if (length)
		VectorScale(v, 1.0 / length, o);
	else
		VectorClear(o);
		
	return length;
}

template<typename T>
inline T VectorNormalize (T *v)
{
	return VectorNormalize(v, v);
}

template<typename Ta, typename Tb, typename Tc>
inline void CrossProduct (const Ta *v1, const Tb *v2, Tc *cross)
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

template<typename T>
inline void VectorInverse (T *v)
{
	for (int32_t i = 0; i < 3; i++)
		v[0] = -v[0];
}

template<typename T>
inline void AngleVectors (const T *angles, vec_t *forward, vec_t *right, vec_t *up)
{
	T angle = angles[YAW] * (M_PI*2 / 360);
	T sy = sin(angle);
	T cy = cos(angle);

	angle = angles[PITCH] * (M_PI*2 / 360);
	T sp = sin(angle);
	T cp = cos(angle);
	
	angle = angles[ROLL] * (M_PI*2 / 360);
	T sr = sin(angle);
	T cr = cos(angle);

	if (forward)
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	
	if (right)
	{
		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;
	}

	if (up)
	{
		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = cr * cp;
	}
}

template<typename T>
inline void ClearBounds (T *mins, T *maxs)
{
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}

template<typename Tv, typename Tm>
inline void AddPointToBounds (const Tv *v, Tm *mins, Tm *maxs)
{
	for (int32_t i = 0; i < 3; i++)
	{
		const Tv &val = v[i];

		if (val < mins[i])
			mins[i] = val;
		
		if (val > maxs[i])
			maxs[i] = val;
	}
}

template<typename T>
constexpr T anglemod(const T &x)
{
    T angle = fmod(x, 360);

	if (angle < 0)
        angle += 360;
    
	return angle;
}

template<typename T>
constexpr T LerpAngle (const T &a2, const T &a1, const float &frac)
{
	if (a1 - a2 > 180)
		a1 -= 360;

	if (a1 - a2 < -180)
		a1 += 360;

	return a2 + frac * (a1 - a2);
}

int BoxOnPlaneSide (vec3_t emins, vec3_t emaxs, cplane_t *plane);

void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal );
void PerpendicularVector( vec3_t dst, const vec3_t src );

//=============================================

char *COM_SkipPath (char *pathname);
void COM_StripExtension (char *in, char *out);
void COM_FileBase (char *in, char *out);
void COM_FilePath (char *in, char *out);
void COM_DefaultExtension (char *path, char *extension);

char *COM_Parse (char **data_p);
// data is an in/out parm, returns a parsed out token

#define Com_sprintf snprintf

//=============================================

// portable case insensitive compare
#define Q_stricmp stricmp
#define Q_strcasecmp stricmp
#define Q_strncasecmp strnicmp

//=============================================

short	BigShort(short l);
short	LittleShort(short l);
int		BigLong (int l);
int		LittleLong (int l);
float	BigFloat (float l);
float	LittleFloat (float l);

void	Swap_Init (void);
char	*va(char *format, ...);

//=============================================

//
// key / value info strings
//
const size_t	MAX_INFO_KEY		= 64;
const size_t	MAX_INFO_VALUE		= 64;
const size_t	MAX_INFO_STRING		= 512;

char *Info_ValueForKey (char *s, char *key);
void Info_RemoveKey (char *s, char *key);
void Info_SetValueForKey (char *s, char *key, char *value);
bool Info_Validate (char *s);

/*
==============================================================

SYSTEM SPECIFIC

==============================================================
*/

// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error (char *error, ...);
void Com_Printf (char *msg, ...);


/*
==========================================================

CVARS (console variables)

==========================================================
*/

#ifndef CVAR
#define	CVAR

enum cvarflags_t
{
	CVAR_NONE		= 0,
	CVAR_ARCHIVE	= bit(0),	// set to cause it to be saved to vars.rc
	CVAR_USERINFO	= bit(1),	// added to userinfo  when changed
	CVAR_SERVERINFO	= bit(2),	// added to serverinfo when changed
	CVAR_NOSET		= bit(3),	// don't allow change from console at all,
								// but can be set from the command line
	CVAR_LATCH		= bit(4)	// save changes until server restart
};

MAKE_BITFLAGS(cvarflags_t);

// nothing outside the Cvar_*() functions should modify these fields!
struct cvar_t
{
	char		*name;
	char		*string;
	char		*latched_string;	// for CVAR_LATCH vars
	cvarflags_t	flags;
	qboolean	modified;	// set each time the cvar is changed
	float		value;
	struct cvar_s *next;
};
#endif		// CVAR

/*
==============================================================

COLLISION DETECTION

==============================================================
*/

// lower bits are stronger, and will eat weaker brushes completely
enum brushcontents_t
{
	CONTENTS_NONE			= 0,
	CONTENTS_SOLID			= bit(0),		// an eye is never valid in a solid
	CONTENTS_WINDOW			= bit(1),		// translucent, but not watery
	CONTENTS_AUX			= bit(2),
	CONTENTS_LAVA			= bit(3),
	CONTENTS_SLIME			= bit(4),
	CONTENTS_WATER			= bit(5),
	CONTENTS_MIST			= bit(6),
	LAST_VISIBLE_CONTENTS	= CONTENTS_MIST,

	// remaining contents are non-visible, and don't eat brushes
	CONTENTS_AREAPORTAL		= bit(15),

	CONTENTS_PLAYERCLIP		= bit(16),
	CONTENTS_MONSTERCLIP	= bit(17),

	// currents can be added to any other contents, and may be mixed
	CONTENTS_CURRENT_0		= bit(18),
	CONTENTS_CURRENT_90		= bit(19),
	CONTENTS_CURRENT_180	= bit(20),
	CONTENTS_CURRENT_270	= bit(21),
	CONTENTS_CURRENT_UP		= bit(22),
	CONTENTS_CURRENT_DOWN	= bit(23),

	CONTENTS_ORIGIN			= bit(24),	// removed before bsping an entity

	CONTENTS_MONSTER		= bit(25),	// should never be on a brush, only in game
	CONTENTS_DEADMONSTER	= bit(26),
	CONTENTS_DETAIL			= bit(27),	// brushes to be added after vis leafs
	CONTENTS_TRANSLUCENT	= bit(28),	// auto set if any surface has trans
	CONTENTS_LADDER			= bit(29),

	// content masks
	MASK_ALL				= (-1),
	MASK_SOLID				= (CONTENTS_SOLID|CONTENTS_WINDOW),
	MASK_DEADSOLID			= (MASK_SOLID|CONTENTS_PLAYERCLIP),
	MASK_PLAYERSOLID		= (MASK_DEADSOLID|CONTENTS_MONSTER),
	MASK_MONSTERSOLID		= (MASK_SOLID|CONTENTS_MONSTERCLIP|CONTENTS_MONSTER),
	MASK_WATER				= (CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME),
	MASK_OPAQUE				= (CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA),
	MASK_SHOT				= (MASK_SOLID|CONTENTS_MONSTER|CONTENTS_DEADMONSTER),
	MASK_CURRENT			= (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
};

MAKE_BITFLAGS(brushcontents_t);

enum surfflags_t
{
	SURF_LIGHT		= bit(0),	// value will hold the light strength
	
	SURF_SLICK		= bit(1),	// effects game physics
	
	SURF_SKY		= bit(2),	// don't draw, but add to skybox
	SURF_WARP		= bit(3),	// turbulent water warp
	SURF_TRANS33	= bit(4),
	SURF_TRANS66	= bit(5),
	SURF_FLOWING	= bit(6),	// scroll towards angle
	SURF_NODRAW		= bit(7)	// don't bother referencing the texture
};

MAKE_BITFLAGS(surfflags_t);

// gi.BoxEdicts() can return a list of either solid or trigger entities
// FIXME: eliminate AREA_ distinction?
enum areatype_t
{
	AREA_SOLID		= 1,
	AREA_TRIGGERS	= 2
};

// plane_t structure
// !!! if this is changed, it must be changed in asm code too !!!
struct cplane_t
{
	vec3_t	normal;
	float	dist;
	byte	type;			// for fast side tests
	byte	signbits;		// signx + (signy<<1) + (signz<<1)
	byte	pad[2];
};

struct cmodel_t
{
	vec3_t		mins, maxs;
	vec3_t		origin;		// for sounds or lights
	int			headnode;
};

struct csurface_t
{
	char		name[16];
	surfflags_t	flags;
	int			value;
};

struct mapsurface_t  // used internally due to name len probs //ZOID
{
	csurface_t	c;
	char		rname[32];
};

// a trace is returned when a box is swept through the world
struct trace_t
{
	qboolean	allsolid;	// if true, plane is not valid
	qboolean	startsolid;	// if true, the initial point was in a solid area
	float		fraction;	// time completed, 1.0 = didn't hit anything
	vec3_t		endpos;		// final position
	cplane_t	plane;		// surface normal at impact
	csurface_t	*surface;	// surface hit
	brushcontents_t	contents;	// contents on other side of surface hit
	edict_t		*ent;		// not set by CM_*() functions
};



// pmove_state_t is the information necessary for client side movement
// prediction
enum pmtype_t
{
	// can accelerate and turn
	PM_NORMAL,
	PM_SPECTATOR,
	// no acceleration or turning
	PM_DEAD,
	PM_GIB,		// different bounding box
	PM_FREEZE
};

// pmove->pm_flags
enum pmflags_t : uint8_t
{
	PMF_DUCKED			= bit(0),
	PMF_JUMP_HELD		= bit(1),
	PMF_ON_GROUND		= bit(2),
	PMF_TIME_WATERJUMP	= bit(3),	// pm_time is waterjump
	PMF_TIME_LAND		= bit(4),	// pm_time is time before rejump
	PMF_TIME_TELEPORT	= bit(5),	// pm_time is non-moving time
	PMF_NO_PREDICTION	= bit(6),	// temporarily disables prediction (used for grappling hook)
};

MAKE_BITFLAGS(pmflags_t);

// this structure needs to be communicated bit-accurate
// from the server to the client to guarantee that
// prediction stays in sync, so no floats are used.
// if any part of the game code modifies this struct, it
// will result in a prediction error of some degree.
struct pmove_state_t
{
	pmtype_t	pm_type;

	short		origin[3];		// 12.3
	short		velocity[3];	// 12.3
	pmflags_t	pm_flags;		// ducked, jump_held, etc
	byte		pm_time;		// each unit = 8 ms
	short		gravity;
	short		delta_angles[3];	// add to command angles to get view direction
									// changed by spawns, rotating objects, and teleporters
};


//
// button bits
//
enum button_t : uint8_t
{
	BUTTON_NONE		= 0,
	BUTTON_ATTACK	= bit(0),
	BUTTON_USE		= bit(1),
	BUTTON_ANY		= 0x80			// any key whatsoever
};

MAKE_BITFLAGS(button_t);

// usercmd_t is sent to the server each client frame
struct usercmd_t
{
	byte		msec;
	button_t	buttons;
	short		angles[3];
	short		forwardmove, sidemove, upmove;
	byte		impulse;		// remove?
	byte		lightlevel;		// light level the player is standing on
};

enum waterlevel_t
{
	WATER_NONE,
	WATER_FEET,
	WATER_WAIST,
	WATER_UNDER
};

const size_t	MAXTOUCH	= 32;

struct pmove_t
{
	// state (in / out)
	pmove_state_t	s;

	// command (in)
	usercmd_t		cmd;
	qboolean		snapinitial;	// if s has been changed outside pmove

	// results (out)
	int			numtouch;
	edict_t		*touchents[MAXTOUCH];

	vec3_t		viewangles;			// clamped
	float		viewheight;

	vec3_t		mins, maxs;			// bounding box size

	edict_t		*groundentity;
	brushcontents_t	watertype;
	waterlevel_t waterlevel;

	// callbacks to test the world
	trace_t		(*trace) (vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end);
	brushcontents_t	(*pointcontents) (vec3_t point);
};


// entity_state_t->effects
// Effects are things handled on the client side (lights, particles, frame animations)
// that happen constantly on the given entity.
// An entity that has effects will be sent to the client
// even if it has a zero index model.
enum entity_effects_t : uint32_t
{
	EF_NONE				= 0,
	EF_ROTATE			= bit(0),		// rotate (bonus items)
	EF_GIB				= bit(1),		// leave a trail
	EF_BLASTER			= bit(3),		// redlight + trail
	EF_ROCKET			= bit(4),		// redlight + trail
	EF_GRENADE			= bit(5),
	EF_HYPERBLASTER		= bit(6),
	EF_BFG				= bit(7),
	EF_COLOR_SHELL		= bit(8),
	EF_POWERSCREEN		= bit(9),
	EF_ANIM01			= bit(10),		// automatically cycle between frames 0 and 1 at 2 hz
	EF_ANIM23			= bit(11),		// automatically cycle between frames 2 and 3 at 2 hz
	EF_ANIM_ALL			= bit(12),		// automatically cycle through all frames at 2hz
	EF_ANIM_ALLFAST		= bit(13),		// automatically cycle through all frames at 10hz
	EF_FLIES			= bit(14),
	EF_QUAD				= bit(15),
	EF_PENT				= bit(16),
	EF_TELEPORTER		= bit(17),		// particle fountain
	EF_FLAG1			= bit(18),
	EF_FLAG2			= bit(19),
	// RAFAEL
	EF_IONRIPPER		= bit(20),
	EF_GREENGIB			= bit(21),
	EF_BLUEHYPERBLASTER = bit(22),
	EF_SPINNINGLIGHTS	= bit(23),
	EF_PLASMA			= bit(24),
	EF_TRAP				= bit(25),

	//ROGUE
	EF_TRACKER			= bit(26),
	EF_DOUBLE			= bit(27),
	EF_SPHERETRANS		= bit(28),
	EF_TAGTRAIL			= bit(29),
	EF_HALF_DAMAGE		= bit(30),
	EF_TRACKERTRAIL		= bit(31u)
	//ROGUE
};

MAKE_BITFLAGS(entity_effects_t);

// entity_state_t->renderfx flags
enum entity_renderfx_t : int32_t
{
	RF_NONE				= 0,
	RF_MINLIGHT			= bit(0),		// allways have some light (viewmodel)
	RF_VIEWERMODEL		= bit(1),		// don't draw through eyes, only mirrors
	RF_WEAPONMODEL		= bit(2),		// only draw through eyes
	RF_FULLBRIGHT		= bit(3),		// allways draw full intensity
	RF_DEPTHHACK		= bit(4),		// for view weapon Z crunching
	RF_TRANSLUCENT		= bit(5),
	RF_FRAMELERP		= bit(6),
	RF_BEAM				= bit(7),
	RF_CUSTOMSKIN		= bit(8),		// skin is an index in image_precache
	RF_GLOW				= bit(9),		// pulse lighting for bonus items
	RF_SHELL_RED		= bit(10),
	RF_SHELL_GREEN		= bit(11),
	RF_SHELL_BLUE		= bit(12),

	//ROGUE
	RF_IR_VISIBLE		= bit(15),		// 32768
	RF_SHELL_DOUBLE		= bit(16),		// 65536
	RF_SHELL_HALF_DAM	= bit(17),
	RF_USE_DISGUISE		= bit(18)
	//ROGUE
};

MAKE_BITFLAGS(entity_renderfx_t);

// player_state_t->refdef flags
enum refdef_flags_t
{
	RDF_NONE			= 0,
	RDF_UNDERWATER		= bit(0),		// warp the screen as apropriate
	RDF_NOWORLDMODEL	= bit(1),		// used for player configuration screen

	//ROGUE
	RDF_IRGOGGLES		= bit(2),
	RDF_UVGOGGLES		= bit(3)
	//ROGUE
};

MAKE_BITFLAGS(refdef_flags_t);

//
// muzzle flashes / player effects
//
enum muzzleflash_t : uint8_t
{
	MZ_BLASTER,
	MZ_MACHINEGUN,
	MZ_SHOTGUN,
	MZ_CHAINGUN1,
	MZ_CHAINGUN2,
	MZ_CHAINGUN3,
	MZ_RAILGUN,
	MZ_ROCKET,
	MZ_GRENADE,
	MZ_LOGIN,
	MZ_LOGOUT,
	MZ_RESPAWN,
	MZ_BFG,
	MZ_SSHOTGUN,
	MZ_HYPERBLASTER,
	MZ_ITEMRESPAWN,
	// RAFAEL
	MZ_IONRIPPER,
	MZ_BLUEHYPERBLASTER,
	MZ_PHALANX,

	//ROGUE
	MZ_ETF_RIFLE		= 30,
	MZ_SHOTGUN2			= 32,
	MZ_HEATBEAM,
	MZ_BLASTER2,
	MZ_TRACKER,
	MZ_NUKE1,
	MZ_NUKE2,
	MZ_NUKE4,
	MZ_NUKE8,
	//ROGUE

	MZ_SILENCED			= bit(7)		// bit flag ORed with one of the above numbers
};

MAKE_BITFLAGS(muzzleflash_t);

//
// monster muzzle flashes
//
enum monstermuzzleflash_t : uint8_t
{
	MZ2_TANK_BLASTER_1				= 1,
	MZ2_TANK_BLASTER_2,
	MZ2_TANK_BLASTER_3,
	MZ2_TANK_MACHINEGUN_1,
	MZ2_TANK_MACHINEGUN_2,
	MZ2_TANK_MACHINEGUN_3,
	MZ2_TANK_MACHINEGUN_4,
	MZ2_TANK_MACHINEGUN_5,
	MZ2_TANK_MACHINEGUN_6,
	MZ2_TANK_MACHINEGUN_7,
	MZ2_TANK_MACHINEGUN_8,
	MZ2_TANK_MACHINEGUN_9,
	MZ2_TANK_MACHINEGUN_10,
	MZ2_TANK_MACHINEGUN_11,
	MZ2_TANK_MACHINEGUN_12,
	MZ2_TANK_MACHINEGUN_13,
	MZ2_TANK_MACHINEGUN_14,
	MZ2_TANK_MACHINEGUN_15,
	MZ2_TANK_MACHINEGUN_16,
	MZ2_TANK_MACHINEGUN_17,
	MZ2_TANK_MACHINEGUN_18,
	MZ2_TANK_MACHINEGUN_19,
	MZ2_TANK_ROCKET_1,
	MZ2_TANK_ROCKET_2,
	MZ2_TANK_ROCKET_3,

	MZ2_INFANTRY_MACHINEGUN_1,
	MZ2_INFANTRY_MACHINEGUN_2,
	MZ2_INFANTRY_MACHINEGUN_3,
	MZ2_INFANTRY_MACHINEGUN_4,
	MZ2_INFANTRY_MACHINEGUN_5,
	MZ2_INFANTRY_MACHINEGUN_6,
	MZ2_INFANTRY_MACHINEGUN_7,
	MZ2_INFANTRY_MACHINEGUN_8,
	MZ2_INFANTRY_MACHINEGUN_9,
	MZ2_INFANTRY_MACHINEGUN_10,
	MZ2_INFANTRY_MACHINEGUN_11,
	MZ2_INFANTRY_MACHINEGUN_12,
	MZ2_INFANTRY_MACHINEGUN_13,

	MZ2_SOLDIER_BLASTER_1,
	MZ2_SOLDIER_BLASTER_2,
	MZ2_SOLDIER_SHOTGUN_1,
	MZ2_SOLDIER_SHOTGUN_2,
	MZ2_SOLDIER_MACHINEGUN_1,
	MZ2_SOLDIER_MACHINEGUN_2,

	MZ2_GUNNER_MACHINEGUN_1,
	MZ2_GUNNER_MACHINEGUN_2,
	MZ2_GUNNER_MACHINEGUN_3,
	MZ2_GUNNER_MACHINEGUN_4,
	MZ2_GUNNER_MACHINEGUN_5,
	MZ2_GUNNER_MACHINEGUN_6,
	MZ2_GUNNER_MACHINEGUN_7,
	MZ2_GUNNER_MACHINEGUN_8,
	MZ2_GUNNER_GRENADE_1,
	MZ2_GUNNER_GRENADE_2,
	MZ2_GUNNER_GRENADE_3,
	MZ2_GUNNER_GRENADE_4,

	MZ2_CHICK_ROCKET_1,

	MZ2_FLYER_BLASTER_1,
	MZ2_FLYER_BLASTER_2,

	MZ2_MEDIC_BLASTER_1,

	MZ2_GLADIATOR_RAILGUN_1,

	MZ2_HOVER_BLASTER_1,

	MZ2_ACTOR_MACHINEGUN_1,

	MZ2_SUPERTANK_MACHINEGUN_1,
	MZ2_SUPERTANK_MACHINEGUN_2,
	MZ2_SUPERTANK_MACHINEGUN_3,
	MZ2_SUPERTANK_MACHINEGUN_4,
	MZ2_SUPERTANK_MACHINEGUN_5,
	MZ2_SUPERTANK_MACHINEGUN_6,
	MZ2_SUPERTANK_ROCKET_1,
	MZ2_SUPERTANK_ROCKET_2,
	MZ2_SUPERTANK_ROCKET_3,

	MZ2_BOSS2_MACHINEGUN_L1,
	MZ2_BOSS2_MACHINEGUN_L2,
	MZ2_BOSS2_MACHINEGUN_L3,
	MZ2_BOSS2_MACHINEGUN_L4,
	MZ2_BOSS2_MACHINEGUN_L5,
	MZ2_BOSS2_ROCKET_1,
	MZ2_BOSS2_ROCKET_2,
	MZ2_BOSS2_ROCKET_3,
	MZ2_BOSS2_ROCKET_4,

	MZ2_FLOAT_BLASTER_1,

	MZ2_SOLDIER_BLASTER_3,
	MZ2_SOLDIER_SHOTGUN_3,
	MZ2_SOLDIER_MACHINEGUN_3,
	MZ2_SOLDIER_BLASTER_4,
	MZ2_SOLDIER_SHOTGUN_4,
	MZ2_SOLDIER_MACHINEGUN_4,
	MZ2_SOLDIER_BLASTER_5,
	MZ2_SOLDIER_SHOTGUN_5,
	MZ2_SOLDIER_MACHINEGUN_5,
	MZ2_SOLDIER_BLASTER_6,
	MZ2_SOLDIER_SHOTGUN_6,
	MZ2_SOLDIER_MACHINEGUN_6,
	MZ2_SOLDIER_BLASTER_7,
	MZ2_SOLDIER_SHOTGUN_7,
	MZ2_SOLDIER_MACHINEGUN_7,
	MZ2_SOLDIER_BLASTER_8,
	MZ2_SOLDIER_SHOTGUN_8,
	MZ2_SOLDIER_MACHINEGUN_8,

	// --- Xian shit below ---
	MZ2_MAKRON_BFG,
	MZ2_MAKRON_BLASTER_1,
	MZ2_MAKRON_BLASTER_2,
	MZ2_MAKRON_BLASTER_3,
	MZ2_MAKRON_BLASTER_4,
	MZ2_MAKRON_BLASTER_5,
	MZ2_MAKRON_BLASTER_6,
	MZ2_MAKRON_BLASTER_7,
	MZ2_MAKRON_BLASTER_8,
	MZ2_MAKRON_BLASTER_9,
	MZ2_MAKRON_BLASTER_10,
	MZ2_MAKRON_BLASTER_11,
	MZ2_MAKRON_BLASTER_12,
	MZ2_MAKRON_BLASTER_13,
	MZ2_MAKRON_BLASTER_14,
	MZ2_MAKRON_BLASTER_15,
	MZ2_MAKRON_BLASTER_16,
	MZ2_MAKRON_BLASTER_17,
	MZ2_MAKRON_RAILGUN_1,
	MZ2_JORG_MACHINEGUN_L1,
	MZ2_JORG_MACHINEGUN_L2,
	MZ2_JORG_MACHINEGUN_L3,
	MZ2_JORG_MACHINEGUN_L4,
	MZ2_JORG_MACHINEGUN_L5,
	MZ2_JORG_MACHINEGUN_L6,
	MZ2_JORG_MACHINEGUN_R1,
	MZ2_JORG_MACHINEGUN_R2,
	MZ2_JORG_MACHINEGUN_R3,
	MZ2_JORG_MACHINEGUN_R4,
	MZ2_JORG_MACHINEGUN_R5,
	MZ2_JORG_MACHINEGUN_R6,
	MZ2_JORG_BFG_1,
	MZ2_BOSS2_MACHINEGUN_R1,
	MZ2_BOSS2_MACHINEGUN_R2,
	MZ2_BOSS2_MACHINEGUN_R3,
	MZ2_BOSS2_MACHINEGUN_R4,
	MZ2_BOSS2_MACHINEGUN_R5,

	//ROGUE
	MZ2_CARRIER_MACHINEGUN_L1,
	MZ2_CARRIER_MACHINEGUN_R1,
	MZ2_CARRIER_GRENADE,
	MZ2_TURRET_MACHINEGUN,
	MZ2_TURRET_ROCKET,
	MZ2_TURRET_BLASTER,
	MZ2_STALKER_BLASTER,
	MZ2_DAEDALUS_BLASTER,
	MZ2_MEDIC_BLASTER_2,
	MZ2_CARRIER_RAILGUN,
	MZ2_WIDOW_DISRUPTOR,
	MZ2_WIDOW_BLASTER,
	MZ2_WIDOW_RAIL,
	MZ2_WIDOW_PLASMABEAM,		// PMM - not used
	MZ2_CARRIER_MACHINEGUN_L2,
	MZ2_CARRIER_MACHINEGUN_R2,
	MZ2_WIDOW_RAIL_LEFT,
	MZ2_WIDOW_RAIL_RIGHT,
	MZ2_WIDOW_BLASTER_SWEEP1,
	MZ2_WIDOW_BLASTER_SWEEP2,
	MZ2_WIDOW_BLASTER_SWEEP3,
	MZ2_WIDOW_BLASTER_SWEEP4,
	MZ2_WIDOW_BLASTER_SWEEP5,
	MZ2_WIDOW_BLASTER_SWEEP6,
	MZ2_WIDOW_BLASTER_SWEEP7,
	MZ2_WIDOW_BLASTER_SWEEP8,
	MZ2_WIDOW_BLASTER_SWEEP9,
	MZ2_WIDOW_BLASTER_100,
	MZ2_WIDOW_BLASTER_90,
	MZ2_WIDOW_BLASTER_80,
	MZ2_WIDOW_BLASTER_70,
	MZ2_WIDOW_BLASTER_60,
	MZ2_WIDOW_BLASTER_50,
	MZ2_WIDOW_BLASTER_40,
	MZ2_WIDOW_BLASTER_30,
	MZ2_WIDOW_BLASTER_20,
	MZ2_WIDOW_BLASTER_10,
	MZ2_WIDOW_BLASTER_0,
	MZ2_WIDOW_BLASTER_10L,
	MZ2_WIDOW_BLASTER_20L,
	MZ2_WIDOW_BLASTER_30L,
	MZ2_WIDOW_BLASTER_40L,
	MZ2_WIDOW_BLASTER_50L,
	MZ2_WIDOW_BLASTER_60L,
	MZ2_WIDOW_BLASTER_70L,
	MZ2_WIDOW_RUN_1,
	MZ2_WIDOW_RUN_2,
	MZ2_WIDOW_RUN_3,
	MZ2_WIDOW_RUN_4,
	MZ2_WIDOW_RUN_5,
	MZ2_WIDOW_RUN_6,
	MZ2_WIDOW_RUN_7,
	MZ2_WIDOW_RUN_8,
	MZ2_CARRIER_ROCKET_1,
	MZ2_CARRIER_ROCKET_2,
	MZ2_CARRIER_ROCKET_3,
	MZ2_CARRIER_ROCKET_4,
	MZ2_WIDOW2_BEAMER_1,
	MZ2_WIDOW2_BEAMER_2,
	MZ2_WIDOW2_BEAMER_3,
	MZ2_WIDOW2_BEAMER_4,
	MZ2_WIDOW2_BEAMER_5,
	MZ2_WIDOW2_BEAM_SWEEP_1,
	MZ2_WIDOW2_BEAM_SWEEP_2,
	MZ2_WIDOW2_BEAM_SWEEP_3,
	MZ2_WIDOW2_BEAM_SWEEP_4,
	MZ2_WIDOW2_BEAM_SWEEP_5,
	MZ2_WIDOW2_BEAM_SWEEP_6,
	MZ2_WIDOW2_BEAM_SWEEP_7,
	MZ2_WIDOW2_BEAM_SWEEP_8,
	MZ2_WIDOW2_BEAM_SWEEP_9,
	MZ2_WIDOW2_BEAM_SWEEP_10,
	MZ2_WIDOW2_BEAM_SWEEP_11
// ROGUE
};

extern	vec3_t monster_flash_offset [];


// temp entity events
//
// Temp entity events are for things that happen
// at a location seperate from any existing entity.
// Temporary entity messages are explicitly constructed
// and broadcast.
enum temp_event_t : uint8_t
{
	TE_GUNSHOT,
	TE_BLOOD,
	TE_BLASTER,
	TE_RAILTRAIL,
	TE_SHOTGUN,
	TE_EXPLOSION1,
	TE_EXPLOSION2,
	TE_ROCKET_EXPLOSION,
	TE_GRENADE_EXPLOSION,
	TE_SPARKS,
	TE_SPLASH,
	TE_BUBBLETRAIL,
	TE_SCREEN_SPARKS,
	TE_SHIELD_SPARKS,
	TE_BULLET_SPARKS,
	TE_LASER_SPARKS,
	TE_PARASITE_ATTACK,
	TE_ROCKET_EXPLOSION_WATER,
	TE_GRENADE_EXPLOSION_WATER,
	TE_MEDIC_CABLE_ATTACK,
	TE_BFG_EXPLOSION,
	TE_BFG_BIGEXPLOSION,
	TE_BOSSTPORT,			// used as '22' in a map, so DON'T RENUMBER!!!
	TE_BFG_LASER,
	TE_GRAPPLE_CABLE,
	TE_WELDING_SPARKS,
	TE_GREENBLOOD,
	TE_BLUEHYPERBLASTER,
	TE_PLASMA_EXPLOSION,
	TE_TUNNEL_SPARKS,
//ROGUE
	TE_BLASTER2,
	TE_RAILTRAIL2,
	TE_FLAME,
	TE_LIGHTNING,
	TE_DEBUGTRAIL,
	TE_PLAIN_EXPLOSION,
	TE_FLASHLIGHT,
	TE_FORCEWALL,
	TE_HEATBEAM,
	TE_MONSTER_HEATBEAM,
	TE_STEAM,
	TE_BUBBLETRAIL2,
	TE_MOREBLOOD,
	TE_HEATBEAM_SPARKS,
	TE_HEATBEAM_STEAM,
	TE_CHAINFIST_SMOKE,
	TE_ELECTRIC_SPARKS,
	TE_TRACKER_EXPLOSION,
	TE_TELEPORT_EFFECT,
	TE_DBALL_GOAL,
	TE_WIDOWBEAMOUT,
	TE_NUKEBLAST,
	TE_WIDOWSPLASH,
	TE_EXPLOSION1_BIG,
	TE_EXPLOSION1_NP,
	TE_FLECHETTE
//ROGUE
};

enum splashtype_t : uint8_t
{
	SPLASH_UNKNOWN,
	SPLASH_SPARKS,
	SPLASH_BLUE_WATER,
	SPLASH_BROWN_WATER,
	SPLASH_SLIME,
	SPLASH_LAVA,
	SPLASH_BLOOD
};

// sound channels
// channel 0 never willingly overrides
// other channels (1-7) allways override a playing sound on that channel
enum soundchannel_t
{
	CHAN_AUTO,
	CHAN_WEAPON,
	CHAN_VOICE,
	CHAN_ITEM,
	CHAN_BODY,
	
	// modifier flags
	CHAN_NO_PHS_ADD			= bit(3),	// send to all clients, not just ones in PHS (ATTN 0 will also do this)
	CHAN_RELIABLE			= bit(4)	// send by reliable message, not datagram
};

MAKE_BITFLAGS(soundchannel_t);

// sound attenuation values
enum soundattn_t
{
	ATTN_NONE,	// full volume the entire level
	ATTN_NORM,
	ATTN_IDLE,
	ATTN_STATIC	// diminish very rapidly with distance
};

// player_state->stats[] indexes
enum statindex_t : uint8_t
{
	STAT_HEALTH_ICON,
	STAT_HEALTH,
	STAT_AMMO_ICON,
	STAT_AMMO,
	STAT_ARMOR_ICON,
	STAT_ARMOR,
	STAT_SELECTED_ICON,
	STAT_PICKUP_ICON,
	STAT_PICKUP_STRING,
	STAT_TIMER_ICON,
	STAT_TIMER,
	STAT_HELPICON,
	STAT_SELECTED_ITEM,
	STAT_LAYOUTS,
	STAT_FRAGS,
	STAT_FLASHES,		// cleared each frame, 1 = health, 2 = armor
	STAT_CHASE,
	STAT_SPECTATOR,

	MAX_STATS				= 32
};

// dmflags->value flags
enum
{
	DF_NO_HEALTH		= bit(0),	// 1
	DF_NO_ITEMS			= bit(1),	// 2
	DF_WEAPONS_STAY		= bit(2),	// 4
	DF_NO_FALLING		= bit(3),	// 8
	DF_INSTANT_ITEMS	= bit(4),	// 16
	DF_SAME_LEVEL		= bit(5),	// 32
	DF_SKINTEAMS		= bit(6),	// 64
	DF_MODELTEAMS		= bit(7),	// 128
	DF_NO_FRIENDLY_FIRE	= bit(8),	// 256
	DF_SPAWN_FARTHEST	= bit(9),	// 512
	DF_FORCE_RESPAWN	= bit(10),	// 1024
	DF_NO_ARMOR			= bit(11),	// 2048
	DF_ALLOW_EXIT		= bit(12),	// 4096
	DF_INFINITE_AMMO	= bit(13),	// 8192
	DF_QUAD_DROP		= bit(14),	// 16384
	DF_FIXED_FOV		= bit(15),	// 32768

	// RAFAEL
	DF_QUADFIRE_DROP	= bit(16),	// 65536

	//ROGUE
	DF_NO_MINES			= bit(17),
	DF_NO_STACK_DOUBLE	= bit(18),
	DF_NO_NUKES			= bit(19),
	DF_NO_SPHERES		= bit(20)
	//ROGUE
};

// FIXME
typedef int dmflags_t;

// ROGUE
/*
==========================================================

  ELEMENTS COMMUNICATED ACROSS THE NET

==========================================================
*/

inline short ANGLE2SHORT(const float &x) { return ((int)(x * 65536 / 360) & 65535); }
inline float SHORT2ANGLE(const short &x) { return x * (360.0 / 65536); }


//
// config strings are a general means of communication from
// the server to all connected clients.
// Each config string can be at most MAX_QPATH characters.
//
enum
{
	CS_NAME,
	CS_CDTRACK,
	CS_SKY,
	CS_SKYAXIS,		// %f %f %f format
	CS_SKYROTATE,
	CS_STATUSBAR,	// display program string

	CS_AIRACCEL			= 29,		// air acceleration control
	CS_MAXCLIENTS,
	CS_MAPCHECKSUM,		// for catching cheater maps

	CS_MODELS,
	CS_SOUNDS			= (CS_MODELS + MAX_MODELS),
	CS_IMAGES			= (CS_SOUNDS + MAX_SOUNDS),
	CS_LIGHTS			= (CS_IMAGES + MAX_IMAGES),
	CS_ITEMS			= (CS_LIGHTS + MAX_LIGHTSTYLES),
	CS_PLAYERSKINS		= (CS_ITEMS + MAX_ITEMS),
	CS_GENERAL			= (CS_PLAYERSKINS + MAX_CLIENTS),
	MAX_CONFIGSTRINGS	= (CS_GENERAL + MAX_GENERAL)
};

//==============================================


// entity_state_t->event values
// ertity events are for effects that take place reletive
// to an existing entities origin.  Very network efficient.
// All muzzle flashes really should be converted to events...
enum entity_event_t
{
	EV_NONE,
	EV_ITEM_RESPAWN,
	EV_FOOTSTEP,
	EV_FALLSHORT,
	EV_FALL,
	EV_FALLFAR,
	EV_PLAYER_TELEPORT,
	EV_OTHER_TELEPORT
};

enum modelindex_t
{
	MODEL_NONE		= 0,
	MODEL_WORLD		= 1,
	MODEL_PLAYER	= 255
};

enum soundindex_t
{
	SOUND_NONE		= 0
};

enum imageindex_t
{
	IMAGE_NONE		= 0
};

// entity_state_t is the information conveyed from the server
// in an update message about entities that the client will
// need to render in some way
struct entity_state_t
{
	int		number;			// edict index

	vec3_t	origin;
	vec3_t	angles;
	vec3_t	old_origin;		// for lerping
	modelindex_t modelindex;
	modelindex_t modelindex2, modelindex3, modelindex4;	// weapons, CTF flags, etc
	int		frame;
	int		skinnum;
	entity_effects_t		effects;		// PGM - we're filling it, so it needs to be unsigned
	entity_renderfx_t		renderfx;
	int		solid;			// for client side prediction, 8*(bits 0-4) is x/y radius
							// 8*(bits 5-9) is z down distance, 8(bits10-15) is z up
							// gi.linkentity sets this properly
	soundindex_t sound;			// for looping sounds, to guarantee shutoff
	entity_event_t event;	// impulse events -- muzzle flashes, footsteps, etc
							// events only go out for a single frame, they
							// are automatically cleared each frame
};

//==============================================


// player_state_t is the information needed in addition to pmove_state_t
// to rendered a view.  There will only be 10 player_state_t sent each second,
// but the number of pmove_state_t changes will be reletive to client
// frame rates
struct player_state_t
{
	pmove_state_t	pmove;		// for prediction

	// these fields do not need to be communicated bit-precise

	vec3_t		viewangles;		// for fixed views
	vec3_t		viewoffset;		// add to pmovestate->origin
	vec3_t		kick_angles;	// add to view direction to get render angles
								// set by weapon kicks, pain effects, etc

	vec3_t		gunangles;
	vec3_t		gunoffset;
	modelindex_t gunindex;
	int			gunframe;

	float		blend[4];		// rgba full screen effect
	
	float		fov;			// horizontal field of view

	refdef_flags_t	rdflags;		// refdef flags

	short		stats[MAX_STATS];		// fast status bar updates
};

#include "g_local.h"