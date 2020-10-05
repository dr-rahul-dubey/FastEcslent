/*
 * imAspect.h
 *
 *  Created on: Jan 25, 2016
 *      Author: sushil
 */

#ifndef INCLUDES_AI_IMASPECT_H_
#define INCLUDES_AI_IMASPECT_H_


#include <aspect.h>
#include <enums.h>

#include <InfluenceMap.h>

#include <OgreVector3.h>


namespace FastEcslent {

	class DistanceMgr;
	class Entity;

	class IMAspect: public UnitAspect {
	private:

		InfluenceMap* im;
		Ogre::Vector3 oldPos;
		D3CoordStruct oldSpec;
		D3CoordStruct newSpec;

	public:
		IMAspect(Entity *ent, UnitAspectType ast);
		~IMAspect(){}
		//
		virtual void  tick(double dt);
		virtual void  init();

	};



}



#endif /* INCLUDES_AI_IMASPECT_H_ */
