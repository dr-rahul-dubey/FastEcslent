/*
 * gather.cpp
 *
 *  Created on: Mar 20, 2013
 *      Author: sushil
 */

#include <gather.h>

namespace FastEcslent {

	FastEcslent::Gather::Gather(Entity* ent, Target *target): UnitCommand(ent, GatherCommand, target) {
		this->scv = dynamic_cast<SCVehicle *> (ent);
		if (target->entity){
			this->mineralEntity = dynamic_cast<Minerals *> (target->entity);
		} else {
			this->mineralEntity = 0;
		}
		this->commandCenter = findNearestCommandCenter();
		this->finished = false;
		this->state    = MOVETOMINERALS;
	}

	inline bool Gather::done(){
		return finished;
	}

	inline void Gather::init(){
		this->commandCenter = findNearestCommandCenter();
		this->state = MOVETOMINERALS;
		this->finished = false;
	}

	inline void Gather::switchState(GatherState newState){
		this->state = newState;
		switch(newState){
		case RETURNTOCC:
			this->commandCenter = findNearestCommandCenter();
			break;
		case GATHERINGMINERALS:
			this->scv->mineralCount = 0; //what happens if you run out of minerals while gathering?
			//if time to find a less utilized mineral ent, find and set your mineralEntity
			break;
		case MOVETOMINERALS:
			this->entity->engine->gameMgr->resources[this->scv->entityId.player].minerals += this->scv->mineralCount; // should be 0 or maxMineralCount
			break;
		default:
			break;
		}
	}

	inline void Gather::tick(double dt){
		switch(this->state){
		case RETURNTOCC:
			//this->commandCenter = findNearestCommandCenter();
			moveToEntityTick(this->commandCenter, dt);

			break;

		case GATHERINGMINERALS:
			gatherTick(dt);
			//checks for switching in gatherTick
			break;

		case MOVETOMINERALS:
			moveToEntityTick(this->mineralEntity, dt);

			break;
		default:
			break;
		}
	}

	void Gather::switchMoveToState(){
		if (this->state == MOVETOMINERALS) {
			switchState(GATHERINGMINERALS);
		} else if (this->state == RETURNTOCC){
			//increment player minerals by 5 == mineralCount == maxMineralCount here
			switchState(MOVETOMINERALS);
		}
	}

	inline void Gather::moveToEntityTick(Entity *targetEnt, double dt){
		if(!targetEnt){
			std::cout << "No target ent: state: " << this->state << std::endl;
			return;
		}
		relativePos = targetEnt->moveData.pos - this->entity->moveData.pos;
		if (relativePos.length() < this->scv->staticData.length * 2) {
			this->entity->moveData.desiredHeading = this->entity->moveData.heading;
			this->entity->moveData.desiredSpeed = 0;
			this->entity->moveData.speed = 0;
			switchMoveToState();//and count additional resources if scv was at CC
		} else {
			this->entity->moveData.desiredHeading = -atan2(relativePos.z, relativePos.x);
			this->entity->moveData.desiredSpeed = this->entity->staticData.maxSpeed;
		}
	}


	inline void Gather::gatherTick(double dt){
		float mineralsGathered = this->scv->mineralGatherRate * dt; // how does this change with number of scvs?
		float fractionOverMaxMineralsPerTrip = 0.0f;
		this->scv->mineralCount += mineralsGathered;
		this->mineralEntity->mineralAmount -= mineralsGathered;
		if(this->scv->mineralCount >= this->scv->maxMineralCount){
			fractionOverMaxMineralsPerTrip = this->scv->maxMineralCount - this->scv->mineralCount;
			this->mineralEntity->mineralAmount += fractionOverMaxMineralsPerTrip; // give back the fraction
			this->scv->mineralCount = this->scv->maxMineralCount;
			switchState(RETURNTOCC);
		}
		// If you run out of minerals
		if(this->mineralEntity->mineralAmount <= 0.0f){
			Entity *ent = this->entity->engine->gameMgr->findClosestEntityOfTypeWithinDistance(MINERALS, this->entity->moveData.pos, MaxMineralFindingDistance, this->entity->entityId.side, this->entity->entityId.player);
			if(ent){
				this->mineralEntity = dynamic_cast<Minerals *> (ent);
			} else {
				switchState(STOPPED);
			}
		}

		// If you are fourth scv on mineral patch try finding another



	}

	FastEcslent::CommandCenter * FastEcslent::Gather::findNearestCommandCenter(){
		//std::cout << "***************************Finding closest Command Center: " << std::endl;
		Entity *ent = this->entity->engine->gameMgr->findClosestEntityOfTypeWithinDistance(COMMANDCENTER, this->entity->moveData.pos, FLT_MAX, this->entity->entityId.side, this->entity->entityId.player);
		if (ent){
			//std::cout << "-----------------Found command center: -------------" << ent->uiname  << std::endl;
			return dynamic_cast<CommandCenter*>(ent);
		} else {
			//std::cout << "********************************************No command center found" << std::endl;
			return 0;
		}
	}

}
