/*
 * buildings.h
 *
 *  Created on: Jan 9, 2013
 *      Author: sushil
 */

#ifndef BUILDINGS_H_
#define BUILDINGS_H_
#include <boost/lexical_cast.hpp>
#include <deque>
#include <ent.h>
#include <enums.h>

namespace FastEcslent {
	class Engine;
	class Building : public Entity {
	public:
		//Entity queue

		std::deque<EntityType> entityQueue;
		bool buildableEntities[NENTITYTYPES];
		//double buildTimeForEntity[NENTITYTYPES];
		Entity *entityBeingBuilt;

		Building(Engine *engine, EntityType etype);
		~Building();

		virtual void init();

		bool enqueue(EntityType entityType);
		bool removeAtIndex(unsigned int pos);
		bool removeTail(void);
		bool cancelEntityBeingBuilt();

	};

	class Barracks : public Building {
	public:
		//static int count = 0;
		Barracks(Engine *engine) : Building(engine, BARRACKS){
			meshName = "Barracks.mesh";
			uiname =    "Barracks";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(50);
			selectable = true;
			entityId.side = BLUE;

			buildableEntities[MARINE]   = true;
			buildableEntities[MARAUDER] = true;
			buildableEntities[REAPER]   = true;
			//buildableEntities[GHOST] = true;

		};
		~Barracks(){};
		//virtual void init(){};
	};

	class CommandCenter : public Building {
	public:
		CommandCenter(Engine *engine) : Building(engine, COMMANDCENTER){
			// properties
			meshName = "CommandCenter.mesh";
			uiname = "CommandCenter";
			uiname.append(boost::lexical_cast<std::string>(count++));

			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(60);
			selectable = true;
			entityId.side = BLUE;
			//entityType    = COMMANDCENTER;


			buildableEntities[SCV] = true;

		};
		~CommandCenter(){};
		//virtual void init(){};
	};

	class Factory : public Building {
	public:
		Factory(Engine *engine) : Building(engine, FACTORY){
			// properties
			meshName = "Factory.mesh";
			uiname = "Factory";
			uiname.append(boost::lexical_cast<std::string>(count++));

			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(50);
			selectable = true;
			entityId.side = BLUE;

			buildableEntities[TANK] = true;
			buildableEntities[THOR] = true;
			buildableEntities[HELLION] = true;

		};
		~Factory(){};
		//virtual void init(){};
	};

	class Refinery : public Building {
	public:
		Refinery(Engine *engine) : Building(engine, REFINERY){
			// properties
			meshName = "Refinery.mesh";
			uiname = "GasRefinery";
			uiname.append(boost::lexical_cast<std::string>(count++));

			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(50);
			selectable = true;
			entityId.side = BLUE;


		};
		~Refinery(){};
		//virtual void init(){};
	};


	class SupplyDepot : public Building {
	public:
		SupplyDepot(Engine *engine) : Building(engine, SUPPLYDEPOT){
			// properties
			meshName = "SupplyDepot.mesh";
			uiname = "SupplyDepot";
			uiname.append(boost::lexical_cast<std::string>(count++));

			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(40);
			selectable = true;
			entityId.side = BLUE;


		};
		~SupplyDepot(){};
		//virtual void init(){};
	};


	class EngineeringBay : public Building {
	public:
		EngineeringBay(Engine *engine) : Building(engine, ENGINEERINGBAY){
			// properties
			meshName = "EngineeringBay.mesh";
			uiname = "EngineeringBay";
			uiname.append(boost::lexical_cast<std::string>(count++));

			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(40);
			selectable = true;
			entityId.side = BLUE;


		};
		~EngineeringBay(){};
		//virtual void init(){};
	};
	class Armory : public Building {
	public:
		Armory(Engine *engine) : Building(engine, ARMORY){
			// properties
			meshName = "Armory.mesh";
			uiname = "Armory";
			uiname.append(boost::lexical_cast<std::string>(count++));

			staticData.length = feet(550.0f);
			staticData.width  = feet(200.0f);
			staticData.height = feet(200.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(40);
			selectable = true;
			entityId.side = BLUE;


		};
		~Armory(){};
		//virtual void init(){};
	};


	class Minerals: public Building {

	public:
		float mineralAmount;
		Minerals(Engine *eng) : Building(eng, MINERALS) {
			meshName = "Cube.mesh";
			uiname = "DilithiumCrystals";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = meters(2.0f);
			staticData.width  = meters(2.0f);
			staticData.height = meters(4.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;

			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(0);

			selectable = false;
			actionable = true;

			entityId.side = BLUE;

			entityState   = ALIVE;

			mineralAmount = 1500;

		}
	};



	class Gas: public Building {

	public:
		float gasAmount;
		Gas(Engine *eng) : Building(eng, GAS) {
			meshName = "Oil.mesh";
			uiname = "Vespene Oil";
			uiname.append(boost::lexical_cast<std::string>(count++));

			// properties
			staticData.length = meters(2.0f);
			staticData.width  = meters(2.0f);
			staticData.height = meters(4.0f);
			staticData.maxSpeed = knots(0.0f);
			staticData.minSpeed = knots(0.0f);
			staticData.speedRange = staticData.maxSpeed - staticData.minSpeed + EPSILON;

			staticData.maxAcceleration = feetPerSecondSquared(0.0f);
			staticData.maxRotationalSpeed = degreesPerSecond(0.0f);
			staticData.turningRadius = 0;
			staticData.mass = tons(0);

			selectable = false;

			entityId.side = BLUE;

			entityState   = ALIVE;
			gasAmount = 2500; //sc2 (sc1 == 5000)

		}
	};






}


#endif /* BUILDINGS_H_ */
