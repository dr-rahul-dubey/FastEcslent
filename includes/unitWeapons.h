/*
 * unitWeapons.h
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */

#ifndef UNITWEAPONS_H_
#define UNITWEAPONS_H_

#include <aspect.h>
#include <enums.h>

#include <target.h>

#include <weaponMgr.h>

namespace FastEcslent {

	class DistanceMgr;
	class Entity;

	class Weapon: public UnitAspect {
	private:
		Target target;


	public:
		WeaponType weaponType;

		double        damageDone;
		double      damageMultiplier;
		float       cooldownTimer;

		DistanceMgr* distanceMgr;
		WeaponMgr*   weaponMgr;

		Weapon(Entity *ent, UnitAspectType ast, WeaponType wType);
		void dealDamageToTarget(Target tgt, double dt); //sets target
		void  setTarget();
		Target getTarget();
		void takeDamage(double amt, double dt);
		virtual void  tick(double dt);
		virtual void  init();

	};



}


#endif /* UNITWEAPONS_H_ */
