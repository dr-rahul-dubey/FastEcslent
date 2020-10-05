/*
 * entityMgr.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: sushil
 */

#include <iostream>
#include <assert.h>

#include <entityMgr.h>

#include <engine.h>
#include <ent.h>
#include <buildings.h>
#include <aspect.h>

#include <physics.h>

#include <unitAI.h>
#include <identity.h>
#include <const.h>

//using namespace FastEcslent;

//extern const int MaxEnts;

//FastEcslent::EntityMgr::EntityMgr() {
//
//	reset();
//}

FastEcslent::EntityMgr::EntityMgr(Engine* engine, Options *opts): Mgr(engine) {
	reset();
	options = opts;
}

void FastEcslent::EntityMgr::init() {
	//std::cout << "EntityMgr Initialized" << std::endl;
	reset();
	for (int i = 0; i < nEnts;i++){
		ents[i]->init();
	}


}

void FastEcslent::EntityMgr::tick(double dtime){
	//std::cout << "Before live ent ticks" << std::endl;
	for (int i = 0; i < nEnts; i++){
		this->ents[i]->tick(dtime);
	}
	//std::cout << "after gestating ent ticks" << std::endl;
}

FastEcslent::Entity *FastEcslent::EntityMgr::createEntity(EntityType etype, Ogre::Vector3 pos, float heading){
	assert(nEnts < MaxEnts - 2);

	Entity *ent;
	switch(etype){
	case SCV:
		ent = new SCVehicle(engine);
		break;
	case MARINE:
		ent = new FastEcslent::Marine(engine);
		break;
	case REAPER:
		ent = new Reaper(engine);
		break;
	case TANK:
		ent = new Tank(engine);
		break;
	case THOR:
		ent = new Thor(engine);
		break;
	case MARAUDER:
		ent = new Marauder(engine);
		break;
	case HELLION:
		ent = new Helion(engine);
		break;
		//----------------------------
	case BARRACKS:
		ent = new Barracks(engine);
		break;
	case COMMANDCENTER:
		ent = new CommandCenter(engine);
		break;
	case FACTORY:
		ent = new Factory(engine);
		break;
	case ARMORY:
		ent = new Armory(engine);
		break;
	case ENGINEERINGBAY:
		ent = new EngineeringBay(engine);
		break;
	case SUPPLYDEPOT:
		ent = new SupplyDepot(engine);
		break;
	case REFINERY:
		ent = new Refinery(engine);
		break;

	case MINERALS:
		ent = new Minerals(engine);
		break;

	case GAS:
		ent = new Gas(engine);
		break;

	case BANSHEE:
		ent = new Banshee(engine);
		break;

	case ZEALOT:
		ent = new Zealot(engine);
		break;

	default:
		ent = new FastEcslent::Marine(engine);
		break;
	}

	ent->moveData.pos = pos;
	ent->moveData.altitude = pos.y;
	ent->moveData.heading = heading; // yaw is not used.
	ent->moveData.yaw = heading;
	//	ent->entityId.id = nDevelopingEnts++;

	//std::cout << "EntityMgr: created ent: " << nEnts << std::endl;
//	ents[nEnts] = ent;
//	nEnts++;
	return ent;
}


bool FastEcslent::EntityMgr::cancelGestatingEntity(Entity *ent){
	ent->switchState(DYING);
	return true;
}


int FastEcslent::EntityMgr::addEntityToGame(Entity *ent){
	ent->entityId.id = nEnts;
	ents[nEnts] = ent;
	nEnts++;
	return ent->entityId.id;
}

FastEcslent::Entity *FastEcslent::EntityMgr::createEntityForPlayerAndSide(EntityType etype, Ogre::Vector3 pos, float heading,
		Side side, Player player){

	FastEcslent::Entity *ent = createEntityAfterTime(etype, pos, heading);
	//ent->entityId.id already set, so set rest of Id
	ent->entityId.player = player;
	ent->entityId.side = side;
	ent->moveData.desiredSpeed = 0.0f;

	//gaeSpecific Stuff
	return ent;
}

FastEcslent::Entity *FastEcslent::EntityMgr::createEntityForPlayerAndSideNow(EntityType etype, Ogre::Vector3 pos, float heading,
		Side side, Player player){

	FastEcslent::Entity *ent = createEntityNow(etype, pos, heading);
	//ent->entityId.id already set, so set rest of Id
	ent->entityId.player = player;
	ent->entityId.side = side;
	ent->moveData.desiredSpeed = 0.0f;

	//gaeSpecific Stuff
	return ent;
}

FastEcslent::Entity* FastEcslent::EntityMgr::createEntityAfterTime(EntityType etype, Ogre::Vector3 pos, float heading){
	FastEcslent::Entity *ent = createEntity(etype, pos, heading);
	ent->switchState(GESTATING);
	int id = this->addEntityToGame(ent);
	//std::cout << "Creating entity after time, id: " << id << std::endl;

	return ent;
}

FastEcslent::Entity* FastEcslent::EntityMgr::createEntityNow(EntityType etype, Ogre::Vector3 pos, float heading){
	FastEcslent::Entity *ent = createEntity(etype, pos, heading);
	ent->switchState(ALIVE);
	int id = this->addEntityToGame(ent);
	//std::cout << "CreateNow: Created id: " << id << std::endl;
	return ent;
}


bool FastEcslent::EntityMgr::preReqExists(EntityType entType){
	return true;
}

bool FastEcslent::EntityMgr::notExceedPopCap(Identity entId){
	return true;
}

void FastEcslent::EntityMgr::dumpOne(int i){
	assert(i >= 0 && i < nEnts);
	ents[i]->print();
}
void FastEcslent::EntityMgr::dumpAll(){
	for (int i = 0; i < nEnts; i++){
		ents[i]->print();
	}
}
