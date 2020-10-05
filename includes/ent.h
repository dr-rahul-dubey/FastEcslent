/*
 * ent.h
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#ifndef ENT_H_
#define ENT_H_


#include <iostream>
#include <cstring>
#include <assert.h>

#include <boost/lexical_cast.hpp>
#include <boost/thread/mutex.hpp>


#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <const.h>
#include <utils.h>

#include <command.h>
#include <unitWeapons.h>
#include <unitAI.h>
#include <physics.h>
#include <imAspect.h>

#include <enums.h>
#include <identity.h>

//#include <aspect.h>



namespace FastEcslent {

	typedef struct {
		float damage;
		float damageBonus;
		float damageFactor;
		DamageType damageType;
		ExplosionType explosionType;
		float cooldown;
		float range;
		float sight;
		float seek;
		float maxHitpoints;
		float hitpoints;
		float armor;
		float shields;
		double freezeTimeThreshold;
		double freezeTime;//how long do you need to freeze to fire
	}WeaponInfoStruct;

	typedef struct {
		float startValue;
		float range;
		float delta; //for each unit of range, startValue + delta
		float deltaFraction; //delta = deltaFraction * startValue
		float w1, w2, w3; // startValue = w1 * hitpoints + w2 * cooldown + w3
		//params for shape --> of change of value with range, linear,
		//quadratic, cubic, ..., probably function of unit type, ranged versus melee
	}IMSpec;

	typedef struct {
		Ogre::Vector3 pos;
		Ogre::Vector3 vel;
		Ogre::Vector3 acc;

		Ogre::Quaternion rot;
		float yaw;
		float desiredSpeed;
		float desiredHeading;
		float speed;
		float heading;
		float altitude;
		float desiredAltitude;

	} PhxDynamicInfoStruct;

	typedef struct {
		float turningRadius;

		float length, width, height, depth;
		float mass;

		float maxSpeed;
		float minSpeed;
		float speedRange;
		float maxAcceleration;
		float maxRotationalSpeed;
		float minAltitude;
		float maxAltitude;
		float deltaAltitude;
		float climbrate;


	}PhxStaticInfoStruct;


	typedef struct {
		Ogre::Vector3 potentialVec;
		float rangeCoeff, rangeExponent;
		float distanceCoeff, distanceExponent;
		float hitpointsCoeff, hitpointsExponent;
		float cooldownAttractCoeff, cooldownAttractExponent;
		float cooldownRepelCoeff, cooldownRepelExponent;
		float damageAttractCoeff, damageAttractExponent;
		float damageRepelCoeff, damageRepelExponent;

		float attractCoeff, attractExponent;
		float repelCoeff, repelExponent;
		float targetAttractCoeff, targetAttractExponent;

		float distanceWeight, hitpointsWeight;

	} AIInfoStruct;

	typedef struct {
		float attractCoeff, attractExponent;
		float repelCoeff,   repelExponent;
	} TargetPotentialStruct;

	class Engine ;
	class UnitAspect ;
//	class GameMgr;

	class Entity {

	public:
		static int count;
		//physics and AI

		PhxDynamicInfoStruct moveData;
		PhxStaticInfoStruct  staticData;


		Ogre::Vector3 potentialVec;
		double attractivePotential;


		std::string uiname;
		std::string meshName;

		Engine   *engine;
		Identity entityId;
		int      idNumber;
		EntityType entityType;   // cigarette, ddg, ...
		EntityClass entityClass;//surface, air, building

		//Game stuff
		//bool     alive;
		EntityState entityState;
		double      gestationPeriod;
		double      timeLeftToBirth;
		double      timeLeftToDie;


		boost::mutex entLock;

		std::deque<UnitAspect*> aspects;
		//UnitAspect   *aspects[NASPECTTYPES];
		Weapon*      weapon;
		UnitAI*      ai;
		Physics*     physics;
		IMAspect*    imAspect;

		// selection
		bool isSelected;
		bool selectable;
		bool actionable;

		IMSpec IMForFriends;
		IMSpec IMForEnemies;

		WeaponInfoStruct weaponInfo;

		AIInfoStruct friends;
		AIInfoStruct enemies;
		//AIInfoStruct common;
		TargetPotentialStruct target;

		//Distance

		/*Command *commands[MaxCommands];
		int     nCommands;
*/

	private:
		void reset();


	public:
		Entity(Engine *eng, EntityType entType) ;
		virtual ~Entity(){;}

		virtual void init();

		UnitAspect *getAspect(UnitAspectType i);
		void        removeAspect(UnitAspectType i);
		void         addAspect(UnitAspect* asp);
		void		switchState(EntityState newState);

		virtual void print();
		virtual void tick(double dt);
		//****************************************************************************************************************
		//void(Entity::*tick)(double); // The tick function called depends on entity's state. One of the next four possibilities
//		/TickType tick;;
		//****************************************************************************************************************
		virtual void gestatingTick(double dt);
		virtual void aliveTick(double dt);
		virtual void dyingTick(double dt);
		virtual void deadTick(double dt);
		//virtual void frozenTick(double dt);
		//****************************************************************************************************************
		virtual bool raising(double dt);
		virtual bool sinking(double dt);

	};




	class Marine : public Entity { //marine

	public:

		Marine(Engine *eng) : Entity(eng, MARINE) {
			meshName = "cigarette.mesh";
			uiname = "Cigarette.";
			uiname.append(boost::lexical_cast<std::string>(count++));


			// properties
			staticData.length = meters(20.0f);
			staticData.width  = meters(4.0f);
			staticData.height = meters(6.0f);

			staticData.maxSpeed = knots(45.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;
			staticData.maxAcceleration = feetPerSecondSquared(15.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(6.5f);
			staticData.turningRadius = 10;
			staticData.mass = pounds(1100);

			selectable = true;

			entityId.side = RED;

		}
	};

	class Reaper : public Entity { // flamer

	public:

		Reaper(Engine *eng) : Entity(eng, REAPER) {
			meshName = "boat.mesh";
			uiname = "SpeedBoat.";
			uiname.append(boost::lexical_cast<std::string>(count++));


			// properties
			staticData.length = 10.0f;
			staticData.width  = 4.0f;
			staticData.height = 6.0f;
			staticData.maxSpeed = knots(40.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;
			staticData.maxAcceleration = 20.0f;
			staticData.maxRotationalSpeed = 2.5f;
			staticData.turningRadius = 10;
			staticData.mass = pounds(1100);

			selectable = true;

			entityId.side = RED;

		}
	};

	class Tank : public Entity { //tank

	public:

		Tank(Engine *eng) : Entity(eng, TANK) {
			meshName = "ddg51.mesh";
			uiname = "DDG51.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = feet(466.0f);
			staticData.width  = feet(59.0f);
			staticData.height = feet(100.0f);
			staticData.maxSpeed = knots(30.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;
			staticData.maxAcceleration = feetPerSecondSquared(10.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(1.0f);
			staticData.turningRadius = 750;
			staticData.mass = tons(842);

			selectable = true;

			entityId.side = BLUE;


		}
	};


	class Thor : public Entity {

	public:

		Thor(Engine *eng) : Entity(eng, THOR) {
			meshName = "cvn68.mesh";
			uiname = "CVN68.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = meters(332.0f);
			staticData.width  = meters(200.0f);
			staticData.height = meters(100.0f);

			staticData.maxSpeed = knots(30.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;

			staticData.maxAcceleration = feetPerSecondSquared(9.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.5f);
			staticData.turningRadius = 100;
			staticData.mass = tons(1100);

			selectable = true;

			entityId.side = BLUE;
		}
	};

	class Marauder : public Entity {

	public:

		Marauder(Engine *eng) : Entity(eng, MARAUDER) {
			meshName = "sleek.mesh";
			uiname = "Sleek.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = meters(30.0f);
			staticData.width  = meters(20.0f);
			staticData.height = meters(10.0f);
			staticData.maxSpeed = knots(40.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;

			staticData.maxAcceleration = feetPerSecondSquared(20.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(1.5f);
			staticData.turningRadius = 10;
			staticData.mass = tons(75);

			selectable = true;

			entityId.side = BLUE;

		}
	};

	class Helion : public Entity {

	public:

		Helion(Engine *eng) : Entity(eng, HELLION) {
			meshName = "alienship.mesh";
			uiname = "AlienShip.";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = meters(5.0f);
			staticData.width  = meters(2.0f);
			staticData.height = meters(5.0f);
			staticData.maxSpeed = knots(60.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;

			staticData.maxAcceleration = feetPerSecondSquared(20.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(2.5f);
			staticData.turningRadius = 8;
			staticData.mass = tons(0.6);

			selectable = true;

			entityId.side = BLUE;

		}
	};


	class SCVehicle : public Entity {

	public:

		float mineralCount;
		float mineralOccupyTime;
		float maxMineralCount;
		float mineralGatherRate;

		float gasCount;
		float gasOccupyTime;
		float maxGasCount;
		float gasGatherRate;

		SCVehicle(Engine *eng) : Entity(eng, SCV) {
			//meshName = "scv.mesh";
			meshName = "scv.mesh";
			uiname = "SpaceConstructionVehicle";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = meters(30.0f);
			staticData.width  = meters(2.0f);
			staticData.height = meters(5.0f);
			staticData.maxSpeed = knots(20.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;
			moveData.speed = 0.0f;
			moveData.desiredSpeed = 0.0f;

			staticData.maxAcceleration = feetPerSecondSquared(20.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(5.5f);
			staticData.turningRadius = 100;
			staticData.mass = tons(0.5);

			selectable = true;

			entityId.side = BLUE;

			// http://wiki.teamliquid.net/starcraft2/Resources
			mineralCount = 0;
			maxMineralCount = 5; //per trip
			mineralOccupyTime = 2.786;
			mineralGatherRate = maxMineralCount/mineralOccupyTime;

			gasCount = 0;
			maxGasCount = 4; // per trip
			gasOccupyTime = 1.981;
			gasGatherRate = maxGasCount/gasOccupyTime;
		}
	};

	class Banshee : public Entity {

			public:

			// This is our 3Dvulture. Properties from liquipedia
	//		Cost:￼ 75 ￼ 0 ￼ 25 ￼ 2
	//		Attributes:Light, Mechanical
	//		Ground Attack:10 (+2)
	//		Ground DPS:5.9 (+1.2)
	//		Bonus:+15 vs light
	//		Bonus DPS:+8.9 vs light
	//		Range:6
	//		Cooldown:1.69 s
	//		Defence:￼ 75 ￼ 1 (+1)
	//		Hotkey:V
	//		Sight:9
	//		Speed:4.25
	//		Cargo Size:2
	//		Strong against:
	//		Zealot
	//		Marine
	//		Weak against:
	//		Siege Tank
	//		Colossus

			Banshee(Engine *eng) : Entity(eng, BANSHEE) { //let's think of this as a 3D vulture
//				meshName = "Cube.001.mesh";
				meshName = "ninja.mesh";
				uiname = "Banshee";
				uiname.append(boost::lexical_cast<std::string>(count++));

				// properties
				staticData.length = meters(10.0f);
				staticData.width  = meters(2.0f);
				staticData.height = meters(5.0f);
				staticData.maxSpeed = knots(6.67f);
				staticData.minSpeed = knots(0.0f);
				staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;
				moveData.speed = 0.0f;
				moveData.desiredSpeed = 0.0f;

				moveData.altitude = 0.0f;
				moveData.desiredAltitude = 200.0f;
				staticData.minAltitude = 0.0f;
				staticData.maxAltitude = 0.0f;
				staticData.deltaAltitude = (staticData.maxAltitude - staticData.minAltitude)/100.0;
				staticData.climbrate = 2.0f;

				staticData.maxAcceleration = feetPerSecondSquared(10.0f);
				staticData.maxRotationalSpeed = degreesPerSecond(5.5f);
				staticData.turningRadius = 100;
				staticData.mass = tons(0.5);

				selectable = true;

				entityId.side = BLUE;
				entityClass = AIR;

				IMForFriends.range = 2; //in number of cells
				IMForFriends.startValue = 1;
				IMForFriends.delta = -1;

				//We will evolve this first
				IMForEnemies.range = 2; //in number of cells
				IMForEnemies.startValue = -4;
				IMForEnemies.delta = 1;

				//
				this->weaponInfo.damage = 20; //Against light biological like zealot
				this->weaponInfo.damageBonus = 2;
				this->weaponInfo.cooldown = 2.1; //2 seconds or 30 frames
				this->weaponInfo.damageFactor = 1;
				this->weaponInfo.damageType = Concussive;
				this->weaponInfo.explosionType = NormalExplosion;
				this->weaponInfo.range = 160; //5 tiles


				this->weaponInfo.seek = 0;
				this->weaponInfo.sight = 256; //8 tiles

				this->weaponInfo.maxHitpoints = 80;
				this->weaponInfo.armor = 0;
				this->weaponInfo.shields = 0;



			}
		};

	class Zealot : public Entity {

			public:


			Zealot(Engine *eng) : Entity(eng, ZEALOT) { //3D Zealot
				//meshName = "scv.mesh";
				//meshName = "cigarette.mesh";
//				meshName = "Wing_Greebles.mesh";
				meshName = "robot.mesh";
				uiname = "ZEALOT";
				uiname.append(boost::lexical_cast<std::string>(count++));

				// properties
				staticData.length = meters(10.0f);
				staticData.width  = meters(2.0f);
				staticData.height = meters(5.0f);
				staticData.maxSpeed = knots(4.0f);
				staticData.minSpeed = knots(0.0f);
				staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;
				moveData.speed = 0.0f;
				moveData.desiredSpeed = 0.0f;

				moveData.altitude = 0.0f;
				moveData.desiredAltitude = 200.0f;
				staticData.minAltitude = 0.0f;
				staticData.maxAltitude = 1000.0f;
				staticData.deltaAltitude = (staticData.maxAltitude - staticData.minAltitude)/100.0;
				staticData.climbrate = 2.0f;

				staticData.maxAcceleration = feetPerSecondSquared(100.0f);
				staticData.maxRotationalSpeed = degreesPerSecond(5.5f);
				staticData.turningRadius = 100;
				staticData.mass = tons(0.5);

				selectable = true;

				entityId.side = RED;
				entityClass = AIR;

				IMForFriends.range = 4; //in number of cells
				IMForFriends.startValue = 4;
				IMForFriends.delta = -1;

				//We will evolve this first
				IMForEnemies.range = 4; //in number of cells
				IMForEnemies.startValue = 4;
				IMForEnemies.delta = -1;

				//
				//

				this->weaponInfo.damage = 8;
				this->weaponInfo.damageBonus = 1;
				this->weaponInfo.damageFactor = 1;
				this->weaponInfo.damageType = NormalDamage;
				this->weaponInfo.explosionType = NormalExplosion;
				this->weaponInfo.range = 15; //
				this->weaponInfo.cooldown = 1.46667; // 22

				this->weaponInfo.seek = 96;
				this->weaponInfo.sight = 224; //8 tiles

				this->weaponInfo.maxHitpoints = 100;//100 is correct + 60 to model shields which don't regenerate
				this->weaponInfo.shields = 60;
				this->weaponInfo.armor = 1;


			}
		};

}

#endif /* ENT_H_ */
