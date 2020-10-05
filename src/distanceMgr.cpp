/*
 * distance.cpp
 *
 *  Created on: Jan 27, 2012
 *      Author: sushil
 */

#include <distanceMgr.h>
#include <engine.h>
#include <ent.h>
#include <const.h>

#include <cfloat>

//FastEcslent::DistanceMgr::DistanceMgr(){
//}

FastEcslent::DistanceMgr::DistanceMgr(Engine* eng, Options *opts): Mgr (eng){
	options = opts;
	other = 0;
	ent = 0;
	collisionTotal = 0;
	//nEnts = 0;
}

void FastEcslent::DistanceMgr::init(){
	//nEnts = engine->entityMgr->nEnts;
	//ents  = engine->entityMgr->ents;
	for(int i = 0; i < MaxEnts; i++){
		collisions[i] = 0;
		closestEnemy[i] = -1;
		closestEnemyDistance[i] = DBL_MAX;
		furthestEnt[i] = -1;
		furthestDistance[i] = 0;

	}
	collisionTotal = 0;
	//std::cout << "Distance Manager: " << nEnts << " number of ents in game" << std::endl;

}



float FastEcslent::DistanceMgr::cpaTime(const Entity *ent1, const Entity *ent2){
	Ogre::Vector3 dv = ent1->moveData.vel - ent2->moveData.vel;
	float dvDotdv = dv.dotProduct(dv);
	if (dvDotdv < FastEcslent::EPSILON) {
		return 0.0;
	}
	Ogre::Vector3 w0 = ent1->moveData.pos - ent2->moveData.pos;
	return w0.dotProduct(dv)/dvDotdv;
}

float FastEcslent::DistanceMgr::cpaSquaredDistance(const Entity *ent1, const Entity *ent2){
	float cpaT = cpaTime(ent1, ent2);
	Ogre::Vector3 p1 = ent1->moveData.pos + (ent1->moveData.vel * cpaT);
	Ogre::Vector3 p2 = ent2->moveData.pos + (ent2->moveData.vel * cpaT);
	return p1.squaredDistance(p2);
}

void FastEcslent::DistanceMgr::tick (double dt){

	for (int i = 0; i < engine->entityMgr->nEnts; i++){
		ent = engine->entityMgr->ents[i];
		distance[i][i] = 0.0;
		distanceVec[i][i] = Ogre::Vector3(0.0, 0.0, 0.0);
		for (int j = i+1; j < engine->entityMgr->nEnts; j++){
			other = engine->entityMgr->ents[j];
			distanceVec[i][j] = other->moveData.pos - ent->moveData.pos;
			normalizedDistanceVec[i][j] = distanceVec[i][j].normalisedCopy();
			distanceVec[j][i] = ent->moveData.pos - other->moveData.pos;
			normalizedDistanceVec[j][i] = distanceVec[j][i].normalisedCopy();
			/*if(distanceVec[i][j].isNaN()){
				ent->print();
				other->print();
				exit(1);
			}*/
			distance[i][j]    = distanceVec[i][j].length();
			distance[j][i]    = distance[i][j];
/*			if(distance[i][j] < (ent->length + other->length)*2){
				collisions[i]++;
				collisions[j]++;
				collisionTotal++;
			}*/
			//if(distance[i][j] < closestEnemyDistance[i] && other->alive && other->entityId.side != ent->entityId.side){
			if(distance[i][j] < closestEnemyDistance[i] && other->entityState == FastEcslent::ALIVE && other->entityId.side != ent->entityId.side){
				closestEnemyDistance[i] = distance[i][j];
				closestEnemy[i] = j;

			}
			//if(distance[j][i] < closestEnemyDistance[j] && ent->alive && other->entityId.side != ent->entityId.side){
			if(distance[j][i] < closestEnemyDistance[j] && ent->entityState == FastEcslent::ALIVE && other->entityId.side != ent->entityId.side){
				closestEnemyDistance[j] = distance[j][i];
				closestEnemy[j] = i;
			}
			if(distance[i][j] > furthestDistance[i]){
				furthestDistance[i] = distance[i][j];
				furthestEnt[i] = j;
			}
			if(distance[j][i] > furthestDistance[j]){
				furthestDistance[j] = distance[j][i];
				furthestEnt[j] = i;
			}
			cpa[i][j].cpaTime = cpaTime(ent, other);
			cpa[j][i].cpaTime = cpa[i][j].cpaTime;
			cpa[i][j].cpaSquaredDistance = cpaSquaredDistance(ent, other);
			cpa[j][i].cpaSquaredDistance = cpa[i][j].cpaSquaredDistance;

		}
	}
}

void FastEcslent::DistanceMgr::dumpAll (){
	for (int i = 0; i < engine->entityMgr->nEnts; i++){
		dumpOne(i);
	}
}

void FastEcslent::DistanceMgr::dumpOne (int index){
	//Entity *ent = engine->entityMgr->ents[index];
	for (int j = 0; j < engine->entityMgr->nEnts; j++){
		//if (j == index) continue;
		//std::cout << "Distance from ent: " << ent->entityId.id <<  " to " << ents[j]->entityId.id << " is " << distance[index][j] << std::endl;
		std::cout << distance[index][j] << ", ";
	}
	std::cout << std::endl;
}


