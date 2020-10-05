/*
 * unitBuilder.h
 *
 *  Created on: Mar 2, 2013
 *      Author: sushil
 */

#ifndef UNITBUILDER_H_
#define UNITBUILDER_H_

#include <aspect.h>
#include <enums.h>

namespace FastEcslent {

	class Building;

	class Builder: public UnitAspect {

	public:

		double   timeLeft;
		double   timeNeeded;
		Building *building;
		EntityType entityType;

		Builder(Building *ent, UnitAspectType ast, EntityType entType);
		virtual ~Builder();

		virtual void init();
		virtual void tick(double dt);

		void startBuildingNextEntity();

	};



}

#endif /* UNITBUILDER_H_ */
