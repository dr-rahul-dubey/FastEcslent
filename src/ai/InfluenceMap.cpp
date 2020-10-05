/*
 * InfluenceMap.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: sushil
 */

#include <InfluenceMap.h>
#include <engine.h>
#include <ent.h>
#include <cfloat>

//using namespace FastEcslent;

FastEcslent::InfluenceMap::InfluenceMap(FastEcslent::Engine *eng, Options *opts): FastEcslent::Mgr(eng) {
	options = opts;
	sliceFraction = 0;
	slice = 0;
	tickInterval = 0.0;
	tickIntervalThreshold = 20.0;
	posToCoordFraction = (float) options->imapx/(float) options->mapSize;
	for(int n = 0; n < NSIDES; n++){
		friendMin[n].x = friendMin[n].y = friendMin[n].z = 0;
		friendMax[n].x = friendMax[n].y = friendMax[n].z = 0;
		enemyMin[n].x = enemyMin[n].y = enemyMin[n].z = 0;
		enemyMin[n].x = enemyMin[n].y = enemyMin[n].z = 0;
	}
}

void FastEcslent::InfluenceMap::setupMap(){
	int mapSize = engine->options->mapSize;
	float cellSize = mapSize/engine->options->imapx;
	for(int n = 0; n < NSIDES; n++){
		for(int i = 0; i < size.x; i++)	{
			for(int j = 0; j < size.y; j++)	{
				for(int k = 0; k < size.z; k++){
					IMFriend[n][i][j][k].imValue = 0;
					IMEnemy[n][i][j][k].imValue = 0;
#ifdef FE_DEBUG
					IMFriend[n][i][j][k].box = Ogre::AxisAlignedBox(Ogre::Vector3(-mapSize/2 + (i * cellSize),(j * cellSize),-mapSize/2 + (k * cellSize)),
							Ogre::Vector3(-mapSize/2 + ((i+1) * cellSize) , ((j+1) * cellSize), -mapSize/2 + ((k+1) * cellSize)));
					IMEnemy[n][i][j][k].box = Ogre::AxisAlignedBox(Ogre::Vector3(-mapSize/2 + (i * cellSize),(j * cellSize),-mapSize/2 + (k * cellSize)),
							Ogre::Vector3(-mapSize/2 + ((i+1) * cellSize) , ((j+1) * cellSize), -mapSize/2 + ((k+1) * cellSize)));
#endif
				}
			}
		}
	}
}

void FastEcslent::InfluenceMap::init(){
	size.x = options->imapx;
	size.y = options->imapy;
	size.z = options->imapz;
	sliceFraction = 0.0625;
	slice = size.x * size.y * size.z * sliceFraction;
	setupMap();
}

FastEcslent::D3CoordStruct FastEcslent::InfluenceMap::getIMCellCoordsFromPosition(Ogre::Vector3 pos){
	D3CoordStruct coords;
	float mapToPlus = options->mapSize/2.0f;
	coords.x = (pos.x + mapToPlus) * posToCoordFraction;
	coords.y = (pos.y)             * posToCoordFraction;
	coords.z = (pos.z + mapToPlus) * posToCoordFraction;
	return coords;
}

Ogre::Vector3 FastEcslent::InfluenceMap::getPositionFromIMCellCoords(D3CoordStruct imcell){
	//center of box of any side's IM
	int mapSize = engine->options->mapSize;
	float cellSize = mapSize/engine->options->imapx; // fix when not a perfect cube

	Ogre::Vector3 corner1 = Ogre::Vector3(-mapSize/2 + (imcell.x * cellSize),(imcell.y * cellSize),-mapSize/2 + (imcell.z * cellSize));
	Ogre::Vector3 corner2 = Ogre::Vector3(-mapSize/2 + ((imcell.x+1) * cellSize),((imcell.y+1) * cellSize),-mapSize/2 + ((imcell.z + 1) * cellSize));
//	Ogre::Vector3 result1 = (corner1 + corner2)/2.0;
//	Ogre::Vector3 result2 = IMEnemy[RED][imcell.x][imcell.y][imcell.z].box.getCenter(); //Does not matter if Red/Blue/.... just coordinates not IM value
//	char tmp[256];
//	sprintf(tmp, "(%.2f, %.2f, %.2f)", result1.x, result1.y, result1.z);
//	std::cout << "Computed Result: " << tmp << std::endl;
//	sprintf(tmp, "(%.2f, %.2f, %.2f)", result2.x, result2.y, result2.z);
//	std::cout << "OgreBox  Result: " << tmp << std::endl;
	return (corner1 + corner2)/2.0;
	//return IMEnemy[RED][imcell.x][imcell.y][imcell.z].box.getCenter();
}

void FastEcslent::InfluenceMap::printCoords(D3CoordStruct coords){
	char tmp[1024];
	sprintf(tmp, "(%3i, %3i, %3i)", coords.x, coords.y, coords.z);
	std::cout << tmp << std::endl;
}

//So difficult to pass functions in c/c++
void FastEcslent::InfluenceMap::updateEntNeighborhood(Entity *ent, D3CoordStruct spec, bool shouldAdd){
	//D3CoordStruct spec = getIMCellCoordsFromPosition(pos);
	//printCoords(spec);
	float range = ent->IMForEnemies.range;
	int distance = range;
	Side side = ent->entityId.side;
	for(int x = spec.x - range; x <= spec.x + range; x++){
		if (x < 0 || x >= engine->options->imapx) continue;
		for(int y = spec.y - range; y <= spec.y + range; y++){
			if(y < 0 || y >= engine->options->imapy) continue;
			for(int z = spec.z - range; z <= spec.z + range; z++){
				if(z < 0 || z >= engine->options->imapz) continue;
				distance = std::max(std::max(std::abs(x - spec.x), std::abs(y - spec.y)), std::abs(z - spec.z));
				if(shouldAdd){
					ent->IMForEnemies.startValue = ent->IMForEnemies.w1 * ent->weaponInfo.hitpoints/ent->weaponInfo.maxHitpoints
							+ ent->IMForEnemies.w2 * ent->weapon->cooldownTimer/ent->weaponInfo.cooldown
							+ ent->IMForEnemies.w3;
					ent->IMForEnemies.delta = ent->IMForEnemies.startValue * ent->IMForEnemies.deltaFraction;
					IMEnemy[side][x][y][z].imValue += (ent->IMForEnemies.startValue - (distance * ent->IMForEnemies.delta));
				} else {
					IMEnemy[side][x][y][z].imValue -= (ent->IMForEnemies.startValue - (distance * ent->IMForEnemies.delta));
				}
				//std::cout << "Distance: " << distance << " Value: " << IM[x][y][z].imValue << std::endl;
			}
		}
	}
}


void FastEcslent::InfluenceMap::subtractInfluence(Entity *ent,  D3CoordStruct spec){
	this->updateEntNeighborhood(ent, spec, false);
	//this->updateEntNeighborhood(ent, spec, false);
}


void FastEcslent::InfluenceMap::addInfluence(Entity *ent, D3CoordStruct spec){
	this->updateEntNeighborhood(ent, spec, true);
	//this->updateEntNeighborhood(ent, spec, true);
}

void FastEcslent::InfluenceMap::setMinMax(){
	float min[NSIDES];
	float max[NSIDES];
	for(int i = 0; i < NSIDES; i++) {
		min[i] = FLT_MAX;
		max[i] = FLT_MIN;
	}


	for(int i = 0; i < engine->entityMgr->nEnts; i++){
		Entity *ent = engine->entityMgr->ents[i];
		if (ent->entityState != ALIVE) continue;
		FastEcslent::D3CoordStruct spec = getIMCellCoordsFromPosition(ent->moveData.pos);
		Side side = ent->entityId.side;
		float range = ent->IMForEnemies.range;
		for(int x = spec.x - range; x <= spec.x + range; x++){
			if (x < 0 || x >= engine->options->imapx) continue; // need to get rid of all these ifs or optimize these away
			for(int y = spec.y - range; y <= spec.y + range; y++){
				if(y < 0 || y >= engine->options->imapy) continue;
				for(int z = spec.z - range; z <= spec.z + range; z++){
					if(z < 0 || z >= engine->options->imapz) continue;
					//-----------------------------------------------
					if(IMEnemy[side][x][y][z].imValue < min[side]) {
						min[side] = IMEnemy[side][x][y][z].imValue;
						this->enemyMin[side].x = x; this->enemyMin[side].y = y; this->enemyMin[side].z = z;
					}
					if(IMEnemy[side][x][y][z].imValue > max[side]) {
						max[side] = IMEnemy[side][x][y][z].imValue;
						this->enemyMax[side].x = x; this->enemyMax[side].y = y; this->enemyMax[side].z = z;
					}
					//-----------------------------------------------
				}
			}
		}
	}

}

void FastEcslent::InfluenceMap::tick(double dt){
//	FastEcslent::Entity *ent;
//	for(int i = 0; i < engine->entityMgr->nEnts; i++){
//		ent = engine->entityMgr->ents[i];
//		updateEntNeighborhood(ent);
//	}
	tickInterval += dt;
	if (tickInterval > tickIntervalThreshold){
		tickInterval = 0.0;
		setMinMax();
	}


}
