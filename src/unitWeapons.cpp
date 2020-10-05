/*
 * weapon.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */
#include <ent.h>
#include <engine.h>
#include <distanceMgr.h>
#include "unitWeapons.h"

FastEcslent::Weapon::Weapon(Entity* ent, UnitAspectType ast, WeaponType wtype): UnitAspect(ent, ast) {
	weaponType = wtype;
	cooldownTimer = ent->weaponInfo.cooldown;
	//ent->weaponInfo.freezeTimeThreshold = 1.0;
}

void FastEcslent::Weapon::init(){
	distanceMgr      = this->entity->engine->distanceMgr;
	weaponMgr        = this->entity->engine->weaponMgr;
	entity->weaponInfo.hitpoints        = entity->weaponInfo.maxHitpoints;
	//entity->weaponInfo.armor            = entity->weaponInfo.armor;
	damageMultiplier = entity->weaponInfo.damageFactor;
	entity->weaponInfo.freezeTimeThreshold = 1.0;
	cooldownTimer = entity->weaponInfo.cooldown;
}


void FastEcslent::Weapon::tick(double dt) {
	if(distanceMgr->closestEnemyDistance[this->entity->entityId.id] < entity->weaponInfo.range){
		target.entity = this->entity->engine->entityMgr->ents[distanceMgr->closestEnemy[this->entity->entityId.id]];

		dealDamageToTarget(target, dt);
		//std::cout << "Entity: " << entity->entityId.id << ", Side: " << entity->entityId.side << " dealing : " //<< dealDamageToTarget(target, dt)
				//<< " to TargetEntity:  " << target.entity->entityId.id << ", target's Side: " << target.entity->entityId.side << std::endl;
	}
}

void FastEcslent::Weapon::dealDamageToTarget(Target tgt, double dt){
	//double damage = weaponMgr->damageMap[entity->entityType][tgt.entity->entityType] * damageMultiplier;
	if(entity->entityState == FastEcslent::ALIVE){
		cooldownTimer -= dt;
		if(cooldownTimer < 0) {
			//entity->entityState = FastEcslent::FROZEN;
			//entity->weaponInfo.freezeTime = 0.0;
			double damage = entity->weaponInfo.damage * damageMultiplier + entity->weaponInfo.damageBonus;
			tgt.entity->weapon->takeDamage(damage, dt);
			cooldownTimer = entity->weaponInfo.cooldown;
			entity->moveData.speed = 0.1f;
		}
	}

}

void FastEcslent::Weapon::takeDamage(double amt, double dt) {//I am target
	//std::cout <<
	if (this->entity->entityState == FastEcslent::ALIVE) {
		entity->weaponInfo.hitpoints -= (amt - entity->weaponInfo.armor);
		if (entity->weaponInfo.hitpoints <= 0){
			this->entity->switchState(DYING);
			entity->weaponInfo.hitpoints = 0;
			//entity->print();
		}
	}
}

