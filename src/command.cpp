/*
 * command.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: sushil
 */

#include <const.h>
#include <ent.h>
#include <group.h>
#include <target.h>
#include <engine.h>
#include <OgreVector3.h>


//#include <command.h>
inline bool FastEcslent::Move::done() {
	return (entity->moveData.pos.squaredDistance((target->location)) <= entity->staticData.turningRadius);
}

void FastEcslent::Move::init(){
	Ogre::Vector3 diff = target->location - entity->moveData.pos;
	entity->moveData.desiredHeading = -atan2(diff.z, diff.x);
	entity->moveData.desiredSpeed = entity->staticData.maxSpeed;
	return;
}

inline void FastEcslent::Move::tick(double dt) {
	if(!done()) {
		relativePos = target->location - entity->moveData.pos;
		entity->moveData.desiredHeading = -atan2(relativePos.z, relativePos.x);
		entity->moveData.desiredSpeed = entity->staticData.maxSpeed;
	} else {
		entity->moveData.desiredSpeed = 0.0f;
	}
	return;
}
//Gather for SCV
//------------------------------------------------------------------------------------------
inline void FastEcslent::Wait::init(){
	timeLeftToWait = target->waitTime;

}
inline bool FastEcslent::Wait::done(){
	return timeLeftToWait < 0.0;
}

inline void FastEcslent::Wait::tick(double dt){
	if(timeLeftToWait > 0) {
		timeLeftToWait -= dt;
	} else {
		std::cout << entity->uiname  << ": Done waiting" << std::endl;
	}
}
//------------------------------------------------------------------------------------------


// Potential fields move for groups

inline bool FastEcslent::PotentialMove::done(){
	//preturn false;
	return (entity->moveData.pos.squaredDistance(target->location) <= (entity->staticData.turningRadius * entity->staticData.turningRadius)*2.0);// && (int) ((entity->attractivePotential)) > -15);
}

void FastEcslent::PotentialMove::init(){
	//Ogre::Vector3 diff = target->location - entity->pos;
	entity->moveData.desiredSpeed = entity->staticData.maxSpeed;

}
const float EPSILON = 0.01f;
inline void FastEcslent::PotentialMove::tick(double dt){
	int nEnts = entity->engine->entityMgr->nEnts;
	//entity->desiredAltitude = target->location.y;

	if (!done()){
		// compute force
			double repulsivePotential = 0.0f;
			entity->potentialVec = Ogre::Vector3::ZERO;
			Ogre::Vector3 tmp;
			int nInRange = 1; // at least one so that you don't multiply by 0 later
			for (int i = 0; i < nEnts; i++){
				if(i != entity->entityId.id){// repulsed by all other entities
					if (entity->engine->distanceMgr->distance[entity->entityId.id][i] < RepulsionThresholdDistance) { // Don't care about entities too far away
						nInRange += 1;
						tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);
						repulsivePotential =  (B * entity->engine->entityMgr->ents[i]->staticData.mass) / pow(entity->engine->distanceMgr->distance[entity->entityId.id][i], m);
						entity->potentialVec += (tmp * repulsivePotential);
					}
				}
			}
			//attracted by target

			tmp = (entity->moveData.pos - target->location);
			//tmp = target->location - entity->pos;
			double targetDistance = tmp.length();
			entity->attractivePotential =  -(A ) / pow(targetDistance, n);// + (B) /pow (targetDistance, m);
			entity->potentialVec += (tmp.normalisedCopy() * entity->attractivePotential * nInRange); // nInRange needs to be at least 1

			//applyPotential(entity, potentialVec);

			entity->moveData.desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);
			//entity->desiredAltitude = (entity->maxAltitude - entity->minAltitude) *
			//std::cout << entity->potentialVec.y << std::endl;
			double cosDiffFrac = (1.0 - cos(entity->moveData.vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
			entity->moveData.desiredSpeed   = (entity->staticData.maxSpeed - entity->staticData.minSpeed) * (1.0 - cosDiffFrac);
			entity->moveData.desiredAltitude = entity->moveData.altitude + (entity->staticData.deltaAltitude * entity->potentialVec.y);



		// apply force
	} else {
		//std::cout << "Attractive Potential: " << entity->attractivePotential << std::endl;
		entity->moveData.desiredSpeed = 0.0f;
		entity->moveData.desiredHeading = entity->moveData.heading;
		entity->moveData.desiredAltitude = entity->moveData.altitude;
	}
}





FastEcslent::Maintain::Maintain (Entity *ent, Target *tgt): UnitCommand(ent, MaintainCommand, tgt){
	if(valid(tgt->entity)) {
		//std::cout << "Maintaining with respect to: " << tgt->entity->uiname << std::endl;
		tgt->location = tgt->entity->moveData.pos + tgt->offset;
	}
}
bool FastEcslent::Maintain::valid(Entity *entity) {
	return true;
}

inline bool FastEcslent::Maintain::done() {
	return false;
}

void FastEcslent::Maintain::init(){

	if (kInvalidFloat == target->offsetDistance) { // offset must contain absolute position right now. Convert to offsetRot and offsetDistance
		Ogre::Quaternion q = Ogre::Vector3(cos(-target->entity->moveData.heading) * 100.0f, 0.0f, sin(-target->entity->moveData.heading) * 100.0f).getRotationTo(target->offset);
		target->offsetYaw = q.getYaw().valueRadians();
		target->offsetDistance = (target->entity->moveData.pos - target->offset).length();
		//std::cout << "OffsetYaw: " << target->offsetYaw << " offsetDistance: " << target->offsetDistance << std::endl;
	}
	return;
}

inline void FastEcslent::Maintain::tick(double dt) { //offsetRotation and offsetDistance set by init()
	target->offset = (target->entity->moveData.rot * Ogre::Quaternion(Ogre::Radian(target->offsetYaw), Ogre::Vector3::UNIT_Y)) * (Ogre::Vector3::UNIT_X * target->offsetDistance);
	target->location = target->entity->moveData.pos + target->offset;

	relativePos = target->location - entity->moveData.pos;
	relativeVel = target->entity->moveData.vel - entity->moveData.vel;
	relativeSpeed = relativeVel.length();
	predictedTimeToClose = relativePos.length()/relativeSpeed;
	predictedPos =  target->location + (target->entity->moveData.vel * predictedTimeToClose);
	interceptPos = predictedPos - entity->moveData.pos;

	entity->moveData.desiredHeading = -atan2(interceptPos.z, interceptPos.x);
	if ((relativeSpeed * predictedTimeToClose) > 30)
		entity->moveData.desiredSpeed = entity->staticData.maxSpeed;
	else
		entity->moveData.desiredSpeed = target->entity->moveData.speed;

	return;
}


void FastEcslent::Tactic::changeLeadership(LeadershipType selector){

	Ogre::Vector3 tpos;
	if(this->target->target.location == InvalidLocation){
		tpos = this->target->target.entity->moveData.pos;
	} else {
		tpos = this->target->target.location;
	}
	this->group->leaderIndex = 0;
	if (this->group->nEntitiesInGroup > 0) {
		switch(selector){
		case 0:
			this->group->leaderIndex = closestToTarget(true, tpos);
			break;
		case 1:
			this->group->leaderIndex = closestToTarget(false, tpos); //furthest from target
			break;
		case 2:
			this->group->leaderIndex = mostMassive(true);
			break;
		case 3:
			this->group->leaderIndex = mostMassive(true);//smallest mass
			break;
		case 4:
			this->group->leaderIndex = randInt(0, this->group->nEntitiesInGroup);
			break;
		default:
			this->group->leaderIndex = 0;
			break;
		}
	}

}


int FastEcslent::Tactic::mostMassive(bool massest){
	float minMass = 0;
	int minIndex = INT_MAX;
	float maxMass = 0;
	int maxIndex = 0;
	float mass;
	for (int i = 0; i < this->group->nEntitiesInGroup; i++){
		mass = this->group->members[i]->staticData.mass;
		if(mass < minMass){
			minMass = mass;
			minIndex = i;
		}
		if (mass > maxMass){
			maxMass = mass;
			maxIndex = i;
		}
	}
	if(!massest) {
		return minIndex;
	} else {
		return maxIndex;
	}

}


int FastEcslent::Tactic::closestToTarget(bool closest, Ogre::Vector3 tpos){
	//Entity** members = this->group->members;
		//int dist = members[leaderIndex]->pos.distance(this->target->target.location);

	float minDist = 0;
	int minIndex = INT_MAX;
	float maxDist = 0;
	int maxIndex = 0;
	float dist;
	for (int i = 0; i < this->group->nEntitiesInGroup; i++){
		dist = this->group->members[i]->moveData.pos.distance(tpos);
		if(dist < minDist) {
			minDist = dist;
			minIndex = i;
		}
		if (dist > maxDist){
			maxDist = dist;
			maxIndex = i;
		}
	}
	if(closest) {
		return minIndex;
	} else {
		return maxIndex;
	}
}
