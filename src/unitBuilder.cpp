/*
 * unitBuilder.cpp
 *
 *  Created on: Mar 2, 2013
 *      Author: sushil
 */
#include <ent.h>
#include <buildings.h>
#include <unitBuilder.h>

namespace FastEcslent {

	Builder::Builder(Building *building, UnitAspectType aspectType, EntityType entType): UnitAspect(dynamic_cast<Entity *>(building), aspectType){
		this->timeNeeded = 600.0;
		this->timeLeft = -1.0;
		this->building = building;
		this->entityType  = entType;

	}

	Builder::~Builder(){

	}


	void Builder::init(){


	}

	void startBuildingNextEntity(){

	}

	void Builder::tick(double dt) {
		if (!this->building->entityQueue.empty()){
			//this->timeleft -= dt;
			if (this->timeLeft < 0) {
				//this->building->engine->entityMgr->createEntityForPlayerAndSide(this->building->pos + offset, this->building->heading, this->building->entityId.side, this->building->entityId.player);
				this->building->entityQueue.pop_front();
				//startBuildingNextEntity();
				//create entity
				//popfront
				//start building new front ent

			}
		}




	}




}
