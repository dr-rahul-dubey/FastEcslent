/*
 * imAspect.cpp
 *
 *  Created on: Jan 25, 2016
 *      Author: sushil
 */

#include <imAspect.h>
#include <InfluenceMap.h>
#include <ent.h>
#include <engine.h>
#include <cstddef>

FastEcslent::IMAspect::IMAspect(Entity* ent, UnitAspectType ast): UnitAspect(ent, ast) {
	oldPos = Ogre::Vector3::NEGATIVE_UNIT_Y;
	im = NULL;
}

inline bool D3CoordIsNotEqual(FastEcslent::D3CoordStruct a, FastEcslent::D3CoordStruct b){
	return (a.x != b.x || a.y != b.y || a.z != b.z);
}


void FastEcslent::IMAspect::tick(double dt){
	newSpec = im->getIMCellCoordsFromPosition(entity->moveData.pos);

	if(D3CoordIsNotEqual(oldSpec, newSpec)){
		//1. remove influence from oldpos
		im->subtractInfluence(entity, oldSpec);

		//2. add stuff in new pos
		im->addInfluence(entity, newSpec);

		oldPos = entity->moveData.pos; // Should be fine outside if
		oldSpec = newSpec;
	}
}

void FastEcslent::IMAspect::init(){
	im = entity->engine->IMMgr;
	oldPos = entity->moveData.pos;
	//entity->print();
	oldSpec = im->getIMCellCoordsFromPosition(entity->moveData.pos);
	//im->printCoords(oldSpec);
	newSpec = oldSpec;
	//im->printCoords(newSpec);
	im->addInfluence(entity, oldSpec);

}
