/*
 * InfluenceMap.h
 *
 *  Created on: Jan 23, 2016
 *      Author: sushil
 */

#ifndef INCLUDES_AI_INFLUENCEMAP_H_
#define INCLUDES_AI_INFLUENCEMAP_H_

#include <mgr.h>
#include <options.h>
#include <const.h>
#include <OgreAxisAlignedBox.h>
#include <enums.h>

namespace FastEcslent {

typedef struct {
	int x, y, z;
} D3CoordStruct;

typedef struct {
#ifdef FE_DEBUG
	Ogre::AxisAlignedBox box;
#endif
	float imValue;
} IMTypeStruct;


class Engine;
class Entity;

class InfluenceMap: public Mgr {

	private:

	void setupMap();

	public:
	IMTypeStruct IMFriend[NSIDES][IMSIZE][IMSIZE][IMSIZE];
	IMTypeStruct IMEnemy[NSIDES][IMSIZE][IMSIZE][IMSIZE];

	Options *options;
	D3CoordStruct size; //x, y, z sizes for current map
	D3CoordStruct friendMin[NSIDES], friendMax[NSIDES];
	D3CoordStruct enemyMin[NSIDES], enemyMax[NSIDES];
	double tickInterval;
	double tickIntervalThreshold;
	int slice;
	float sliceFraction;
	float posToCoordFraction;
	InfluenceMap(Engine *eng, Options *opts);
	void init();
	void tick(double dtime);

	void setMinMax();
	void updateEntNeighborhood(Entity *ent, D3CoordStruct spec, bool shouldAdd);
	D3CoordStruct getIMCellCoordsFromPosition(Ogre::Vector3 pos);
	Ogre::Vector3 getPositionFromIMCellCoords(D3CoordStruct imcell);
	void subtractInfluence(Entity *ent,  D3CoordStruct spec);
	void addInfluence(Entity *ent,  D3CoordStruct spec);

	void printCoords(D3CoordStruct dc);

};

}


#endif /* INCLUDES_AI_INFLUENCEMAP_H_ */
