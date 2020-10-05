/*
 * enums.h
 *
 *  Created on: Jan 27, 2012
 *      Author: sushil
 */

#ifndef ENUMS_H_
#define ENUMS_H_


namespace FastEcslent {

	enum UnitAspectType {

		PHYSICS = 0,
		UNITAI = 1,
		WEAPON = 2,
		IM     = 3,
		//DISTANCE = 2,

		NASPECTTYPES
	};

	enum GroupAspectType {

		GROUPAI = 0,
		//LEADER = 1, //A leader aspect to pick current leader

		NGROUPASPECTTYPES
	};
	enum WeaponType {

		RIFLE  = 0,
		FLAMER = 1,
		SHELL  = 2,
		//DISTANCE = 2,

		NWEAPONTYPES
	};

	enum DamageType {
		NormalDamage = 0,
		Concussive   = 1,
		Explosive    = 2,
		NDAMAGETYPES
	};

	enum ExplosionType {
		NormalExplosion = 0,
		RadialSplash    = 1,
		EnemySplace     = 2,
		NEXPLOSIONTYPES
	};

	enum EntityType {

		SCV          = 0,
		MARINE       = 1, // Marine
		REAPER       = 2, // Marauder
		TANK         = 3, // Tank
		THOR         = 4, //
		MARAUDER     = 5, //
		HELLION       = 6, // Hellion


		BARRACKS      = 7,
		COMMANDCENTER = 8,
		REFINERY      = 9,
		FACTORY       = 10,
		ARMORY        = 11,
		ENGINEERINGBAY= 12,
		SUPPLYDEPOT   = 13,
//		REACTOR       = 10,
//		TECHLAB       = 11,
//		SPACEPORT     = 12,
//		GHOSTACADEMY  = 13,

		MINERALS      = 14,
		GAS           = 15,

		BANSHEE       = 16,
		ZEALOT        = 17,


		NENTITYTYPES
	};

	enum EntityState {

		GESTATING = 0,
		ALIVE     = 1,
		DYING     = 2,
		DEAD      = 3,
//		FROZEN    = 4,

		NENTITYSTATES

	};

	enum EntityClass { // Euphemism for buildings and

		SURFACE    = 0,
		AIR        = 1,
		SUBSURFACE = 2,
		STATIC     = 3, //Buildings, turrets, fixed weapons, OtherNonMoving obstacles

		NENTITYCLASSTYPES

	};

	enum Side {

		BATTLEMASTER = 0,
		RED,
		BLUE,

		NSIDES
	};

	enum Player {
				ONE = 0,
				TWO,
				THREE,
				FOUR,

				NPLAYERS
	};

	enum MicroType {
		FIXED = 0,
		ADAPTIVE,
		OPPONENT_FROM_CHROMOSOME,
		BESTZEALOT,
		BESTVULTURE,

		NMICROS
	};


} // end FastEcslent namespace enums



#endif /* ENUMS_H_ */
