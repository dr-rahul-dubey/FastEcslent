/*
 * micro.cpp
 *
 *  Created on: Jan 26, 2016
 *      Author: sushil
 */

#include <micro.h>
#include <engine.h>


// Potential fields move for groups

inline bool FastEcslent::Micro::done(){
	//preturn false;
	return (entity->moveData.pos.squaredDistance(target->location) <= (entity->staticData.turningRadius * entity->staticData.turningRadius)*2.0);// && (int) ((entity->attractivePotential)) > -15);
}

void FastEcslent::Micro::init(){
	//Ogre::Vector3 diff = target->location - entity->pos;
	entity->moveData.desiredSpeed = entity->staticData.maxSpeed;

}
const float EPSILON = 0.01f;
inline void FastEcslent::Micro::tick2(double dt){
	if (!done()){
		//0. compute attraction to friends
		//   distance, mass,
		entity->potentialVec = Ogre::Vector3::ZERO;
		int nEnts = entity->engine->entityMgr->nEnts;
		int nFriends = 0, nEnemies = 0;
		Entity *other;
		Ogre::Vector3 tmp;
		double attractivePotential = 0.0, repulsivePotential = 0.0;
		double dist, hits;
		double substrate;
		for(int i = 0; i < nEnts; i++){
			other = entity->engine->entityMgr->ents[i];
			if (other->entityState == ALIVE &&  i != entity->entityId.id ) {
				tmp = (entity->engine->distanceMgr->normalizedDistanceVec[entity->entityId.id][i]); //from me to other
				dist = entity->engine->distanceMgr->distance[entity->entityId.id][i];
				hits = abs(entity->weaponInfo.hitpoints - other->weaponInfo.hitpoints);
				substrate = dist;
				//substrate = entity->common.distanceWeight * dist + entity->common.hitpointsWeight * hits;
				if(entity->entityId.side == other->entityId.side) {
					nFriends++;
					attractivePotential = entity->friends.attractCoeff/pow(substrate, entity->friends.attractExponent);
					repulsivePotential  = entity->friends.repelCoeff/pow(substrate, entity->friends.repelExponent);
					entity->potentialVec += (tmp * attractivePotential);
					entity->potentialVec -= (tmp * repulsivePotential);
				} else {
					nEnemies++;
					attractivePotential = entity->enemies.attractCoeff/pow(substrate, entity->enemies.attractExponent);
					repulsivePotential  = entity->enemies.repelCoeff/pow(substrate, entity->enemies.repelExponent);
					entity->potentialVec += (tmp * attractivePotential);
					entity->potentialVec -= (tmp * repulsivePotential);
				}
			}
		}
		tmp = (entity->moveData.pos - target->location);
		//tmp = target->location - entity->pos;
		double targetDistance = tmp.length();
		attractivePotential =  (entity->target.attractCoeff) / pow(targetDistance, entity->target.attractExponent);
		//params.targetAttractExponent);
		entity->potentialVec += (tmp.normalisedCopy() * attractivePotential); // nInRange needs to be at least 1
		entity->potentialVec = entity->potentialVec * (nFriends + nEnemies + 1); //scale by the number of ents

		//-----------------------------------------------------------------------------------------------------

		entity->moveData.desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);
		//entity->desiredAltitude = (entity->maxAltitude - entity->minAltitude) *
		//std::cout << entity->potentialVec.y << std::endl;
		double cosDiffFrac = (1.0 - cos(entity->moveData.vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
		entity->moveData.desiredSpeed   = (entity->staticData.maxSpeed - entity->staticData.minSpeed) * (1.0 - cosDiffFrac);
		entity->moveData.desiredAltitude = entity->moveData.altitude + (entity->staticData.deltaAltitude * entity->potentialVec.y);
		entity->moveData.desiredAltitude = clamp(entity->moveData.desiredAltitude, 2, entity->staticData.maxAltitude);
	} else {
		//std::cout << "Attractive Potential: " << entity->attractivePotential << std::endl;
		entity->moveData.desiredSpeed = 0.0f;
		entity->moveData.desiredHeading = entity->moveData.heading;
		entity->moveData.desiredAltitude = entity->moveData.altitude;
	}

}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

inline void FastEcslent::Micro::tick(double dt){
	if (!done()){
		//0. compute attraction to friends
		//   distance, mass,
		entity->potentialVec = Ogre::Vector3::ZERO;
		int nEnts = entity->engine->entityMgr->nEnts;
		int 	nFriends = 0, nEnemies = 0;
		Entity *other;
		Ogre::Vector3 tmp;
		double attractivePotential = 0.0, repulsivePotential = 0.0;
		double dist, ehits, ohits, ecool, ocool;
		for(int i = 0; i < nEnts; i++){
			other = entity->engine->entityMgr->ents[i];
			if (other->entityState == ALIVE &&  i != entity->entityId.id ) {
				tmp   = (entity->engine->distanceMgr->normalizedDistanceVec[entity->entityId.id][i]); //from me to other
				dist  = entity->engine->distanceMgr->distance[entity->entityId.id][i];
				dist  = 1 + dist/1000.0;
				ehits = 1 + entity->weaponInfo.hitpoints/(double) entity->weaponInfo.maxHitpoints;
				ohits = 1 + other->weaponInfo.hitpoints/(double) other->weaponInfo.maxHitpoints;
				ecool = 1 + entity->weapon->cooldownTimer/entity->weaponInfo.cooldown; //this should be in weaponInfo
				ocool = 1 + other->weapon->cooldownTimer/other->weaponInfo.cooldown;
				//std::cout << dist << " " << ehits<< " " << ohits << " " << ecool << " " << ocool << std::endl;

				if(entity->entityId.side == other->entityId.side) {
					nFriends++;
					//for distance
					attractivePotential = entity->friends.attractCoeff/pow(dist, entity->friends.attractExponent);
					repulsivePotential  = entity->friends.repelCoeff/pow(dist, entity->friends.repelExponent);
					//for damage to self
					attractivePotential += entity->friends.damageAttractCoeff/pow(ehits, entity->friends.damageAttractExponent);
					repulsivePotential  += entity->friends.damageRepelCoeff/pow(ehits, entity->friends.damageRepelExponent);

					attractivePotential += entity->friends.cooldownAttractCoeff/pow(ecool, entity->friends.cooldownAttractExponent);
					repulsivePotential  += entity->friends.cooldownRepelCoeff/pow(ecool, entity->friends.cooldownRepelExponent);

					entity->potentialVec += (tmp * attractivePotential);
					entity->potentialVec -= (tmp * repulsivePotential);

				} else {
					nEnemies++;
					//for distance
					attractivePotential = entity->enemies.attractCoeff/pow(dist, entity->enemies.attractExponent);
					repulsivePotential  = entity->enemies.repelCoeff/pow(dist, entity->enemies.repelExponent);
					//for damage to other
					attractivePotential += entity->enemies.damageAttractCoeff/pow(ohits, entity->enemies.damageAttractExponent);
					repulsivePotential  += entity->enemies.damageRepelCoeff/pow(ohits, entity->enemies.damageRepelExponent);

					attractivePotential += entity->enemies.cooldownAttractCoeff/pow(ocool, entity->enemies.cooldownAttractExponent);
					repulsivePotential  += entity->enemies.cooldownRepelCoeff/pow(ocool, entity->enemies.cooldownRepelExponent);

					entity->potentialVec += (tmp * attractivePotential);
					entity->potentialVec -= (tmp * repulsivePotential);
				}
				// for damage



			}
		}
		tmp = (entity->moveData.pos - target->location);
		//tmp = target->location - entity->pos;
		double targetDistance = tmp.length() + 1;
		attractivePotential =  (entity->target.attractCoeff) / pow(targetDistance, entity->target.attractExponent);
		//params.targetAttractExponent);
		entity->potentialVec += (tmp.normalisedCopy() * attractivePotential); // nInRange needs to be at least 1
		entity->potentialVec = entity->potentialVec * (nFriends + nEnemies + 1); //scale by the number of ents

		//-----------------------------------------------------------------------------------------------------

		entity->moveData.desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);
		//entity->desiredAltitude = (entity->maxAltitude - entity->minAltitude) *
		//std::cout << entity->potentialVec.y << std::endl;
		double cosDiffFrac = (1.0 - cos(entity->moveData.vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
		entity->moveData.desiredSpeed   = (entity->staticData.maxSpeed - entity->staticData.minSpeed) * (1.0 - cosDiffFrac);
		entity->moveData.desiredAltitude = entity->moveData.altitude + (entity->staticData.deltaAltitude * entity->potentialVec.y);
		entity->moveData.desiredAltitude = clamp(entity->moveData.desiredAltitude, 2, entity->staticData.maxAltitude);
		//std::cout << entity->moveData.desiredHeading << ", " << entity->moveData.desiredSpeed << ", " << entity->moveData.desiredAltitude << std::endl;
	} else {
		//std::cout << "Attractive Potential: " << entity->attractivePotential << std::endl;
		entity->moveData.desiredSpeed = 0.0f;
		entity->moveData.desiredHeading = entity->moveData.heading;
		entity->moveData.desiredAltitude = entity->moveData.altitude;
	}

}


//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//1. compute repulsion to friends
		//   distance, mass
		//0. compute attraction to enemies
		//   distance, mass, hitpoints, ehitpoints, range, erange, isolation, cooldown
		//1. compute repulsion to enemies
		//   distance, mass, hitpoints, ehitpoints, range, erange, isolation, cooldown
		//apply
//		double repulsivePotential = 0.0f;
//		entity->potentialVec = Ogre::Vector3::ZERO;
//		Ogre::Vector3 tmp;
//		int nInRange = 1; // at least one so that you don't multiply by 0 later
//		for (int i = 0; i < nEnts; i++){
//			if(i != entity->entityId.id){// repulsed by all other entities
//				if (entity->engine->distanceMgr->distance[entity->entityId.id][i] < RepulsionThresholdDistance) { // Don't care about entities too far away
//					nInRange += 1;
//					tmp = (entity->engine->distanceMgr->normalizedDistanceVec[i][entity->entityId.id]);
//					repulsivePotential =  (params.friendRepelCoeff * entity->engine->entityMgr->ents[i]->staticData.mass) / pow(entity->engine->distanceMgr->distance[entity->entityId.id][i], params.friendRepelExponent);
//					entity->potentialVec += (tmp * repulsivePotential);
//				}
//			}
//		}
//		//attracted by target
//
//		tmp = (entity->moveData.pos - target->location);
//		//tmp = target->location - entity->pos;
//		double targetDistance = tmp.length();
//		entity->attractivePotential =  -(params.enemyAttractCoeff ) / pow(targetDistance, params.enemyAttractExponent);// + (B) /pow (targetDistance, m);
//		entity->potentialVec += (tmp.normalisedCopy() * entity->attractivePotential * nInRange); // nInRange needs to be at least 1
//
//		//applyPotential(entity, potentialVec);
//
//		entity->moveData.desiredHeading = atan2(-entity->potentialVec.z, entity->potentialVec.x);
//		//entity->desiredAltitude = (entity->maxAltitude - entity->minAltitude) *
//		//std::cout << entity->potentialVec.y << std::endl;
//		double cosDiffFrac = (1.0 - cos(entity->moveData.vel.angleBetween(entity->potentialVec).valueRadians()))/2.0;// between 0 and 2 divided by 2.0 gives something between 0 and 1
//		entity->moveData.desiredSpeed   = (entity->staticData.maxSpeed - entity->staticData.minSpeed) * (1.0 - cosDiffFrac);
//		entity->moveData.desiredAltitude = entity->moveData.altitude + (entity->staticData.deltaAltitude * entity->potentialVec.y);
