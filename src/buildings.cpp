/*
 * buildings.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: sushil
 */
#include <engine.h>

#include <buildings.h>
#include <unitWeapons.h>

#include <const.h>

FastEcslent::Building::Building(Engine *engine, EntityType etype):
	Entity(engine, etype){
		this->meshName = "Barracks.mesh";
		this->uiname   = "Barracks";
		//this->uiname   = uiname.append(boost::lexical_cast<std::string>(id));

		entityClass   = STATIC;
		//entityType    = etype; taken care of in ent.h
		for(int i = 0; i < NENTITYTYPES; i++){
			buildableEntities[i] = false; // Entities buildable by this building
		}

		// and so on
}


FastEcslent::Building::~Building(){
}

void FastEcslent::Building::init(){
// do not add any aspects
	weapon = new Weapon(this, WEAPON, RIFLE);
	std::cout << "Created weapon aspect" << std::endl;
	weapon->init();
	std::cout << "Initialized weapon aspect" << std::endl;
	this->addAspect(weapon);
	std::cout << "Added weapon aspect" << std::endl;
	if(this->entityType == MINERALS)
		std::cout << "Building type: Minerals" << std::endl;
	else
		std::cout << "Building type: " << this->entityType << std::endl;

}

bool FastEcslent::Building::enqueue(EntityType entType){
	if(buildableEntities[entType]){
		if(this->engine->gameMgr->preReqExists(entType)){
			this->entityQueue.push_back(entType);
			if (this->entityQueue.size() == 1){// if this is first push, start entity building timer
				if (this->engine->gameMgr->notExceedPopCap(this->entityId)) {
					this->entityBeingBuilt = this->engine->entityMgr->createEntityAfterTime(entType, this->moveData.pos, this->moveData.heading);
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

bool FastEcslent::Building::cancelEntityBeingBuilt(){
	return this->engine->entityMgr->cancelGestatingEntity(this->entityBeingBuilt);
}


bool FastEcslent::Building::removeAtIndex(unsigned int pos){
	if(pos < this->entityQueue.size()) {
		this->entityQueue.erase(this->entityQueue.begin() + pos);
		return true;
	} else {
		return false;
	}
}

bool FastEcslent::Building::removeTail(void){
	if (this->entityQueue.empty()) {
		return false;
	} else {
		this->entityQueue.pop_back();
		return true;
	}
}

