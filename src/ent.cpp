/*
 * ent.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: sushil
 */

#include <iostream>
#include <assert.h>
#include <ent.h>
#include <engine.h>
#include <aspect.h>
#include <command.h>

int FastEcslent::Entity::count = 0;

void FastEcslent::Entity::print(){
	std::cout << "Id: " << entityId.id << ", instanceId: " << engine->instanceId << std::endl;
	std::cout << "UIName " << uiname << ", meshName: " << meshName << std::endl;
	std::cout << "Position: " << moveData.pos.x << ", " << moveData.pos.y << ", " << moveData.pos.z << std::endl;
	std::cout << "Velocity: " << moveData.vel.x << ", " << moveData.vel.y << ", " << moveData.vel.z << std::endl;
	std::cout << "Accelert: " << moveData.acc.x << ", " << moveData.acc.y << ", " << moveData.acc.z << std::endl;
	std::cout << "Speed:    " << moveData.speed << std::endl;
	std::cout << "Heading:  " << moveData.heading << std::endl;
	std::cout << "dsrdSpd:  " << moveData.desiredSpeed << std::endl;
	std::cout << "dsrdHdg:  " << moveData.desiredHeading << std::endl;
	std::cout << "yaw:      " << moveData.yaw << std::endl;
	std::cout << "selected: " << isSelected << std::endl;
	std::cout << "Weapons" << std::endl;
	std::cout << "        MaxHitpoints: " << weaponInfo.maxHitpoints << " hitpoints: " << weaponInfo.hitpoints << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
}


void FastEcslent::Entity::reset(){
	//FastEcslent::Entity::tick = &FastEcslent::Entity::gestatingTick;
	//this->tick = &FastEcslent::Entity::gestatingTick;
	//tic = &FastEcslent::Entity::gestatingTick;

	moveData.pos = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	moveData.vel = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	moveData.acc = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	moveData.rot = Ogre::Quaternion(Ogre::Radian(0.0f), Ogre::Vector3::UNIT_Y);
	moveData.yaw = 0.0f;
	moveData.desiredSpeed = 0.0f;
	moveData.desiredHeading = 0.0f;
	moveData.speed = 0.0f;
	moveData.heading = 0.0f;
	moveData.altitude = 0;
	moveData.desiredAltitude = 0;

	staticData.maxSpeed = 13*40.0f;
	staticData.minSpeed = -13*10.0f;

	staticData.speedRange = staticData.maxSpeed - staticData.minSpeed;
	staticData.maxAcceleration = 1.0f;
	staticData.maxRotationalSpeed = 0.1;
	staticData.minAltitude = 0;
	staticData.maxAltitude = 0;
//	staticData.minAltitude = 1000000;
//	staticData.maxAltitude = -100000;

	staticData.length = 10.0f;
	staticData.width = 4.0f;
	staticData.mass  = tons(0.1);
	staticData.depth = 2.0f;
	staticData.turningRadius = 1000;


	//alive = true;
	selectable = false;
	actionable = true;



	//selection
	isSelected = false;

	entityId.side = BATTLEMASTER;
	entityId.player = ONE;
	entityClass   = SURFACE;
	entityState   = ALIVE;
	gestationPeriod = 10.0;
	timeLeftToBirth = 10.0;
	timeLeftToDie   = 10.0;

	aspects.clear();

}

FastEcslent::Entity::Entity(Engine *eng, EntityType entType) {

	engine = eng;
	//entityId.id = id;
	entityType = entType;
	meshName = "cigarette.mesh";
	uiname = "Cigarette";
	//uiname.append(boost::lexical_cast<std::string>(count++));
	reset();
}






// There should only be one aspect of each type

/*int FastEcslent::Entity::addAspect(Aspect *asp) {
	aspects[nAspects++] = asp;
	return nAspects - 1;
}*/
void FastEcslent::Entity::init(){
	for (int aType = 0; aType < NASPECTTYPES; aType++){ // the problem with enums in C++n
		switch (aType){
		case (PHYSICS):
			if(entityClass == AIR){
				physics = new Physics3D1(this, PHYSICS);
			} else {
				physics = new Physics2D2(this, PHYSICS);
			}
		    physics->init();
			this->addAspect(physics);
			break;
		case (UNITAI):
			ai = new UnitAI(this, UNITAI);
			ai->init();
		    this->addAspect(ai);
			break;
		case (WEAPON):
			weapon = new Weapon(this, WEAPON, RIFLE);
			weapon->init();
			this->addAspect(weapon);
			break;
		case (IM):
			imAspect = new IMAspect(this, IM);
			imAspect->init();
			this->addAspect(imAspect);
			break;
		}
		//this->addAspect(asp);
	}


	this->gestationPeriod = this->engine->gameMgr->entTypeData[this->entityType].buildTime;

}

bool FastEcslent::Entity::sinking(double dt){
	this->timeLeftToDie -= dt;
	this->moveData.pos.y = -(-this->staticData.height  + (this->staticData.height * (this->gestationPeriod - this->timeLeftToDie)/this->gestationPeriod));
	if (this->timeLeftToDie <= 0.0) {
		//this->entityState = FastEcslent::DEAD;
		this->switchState(DEAD);
		//this->pos.y = -2 * this->height;
		this->moveData.pos.y = -100000.0;
		return false;
	}

	return true;
}

bool FastEcslent::Entity::raising(double dt){
	this->timeLeftToBirth -= dt;
	this->moveData.pos.y = -this->staticData.height  + (this->staticData.height * (this->gestationPeriod - this->timeLeftToBirth)/this->gestationPeriod);
	if (this->timeLeftToBirth <= 0.0) {
		this->switchState(ALIVE);
		//this->entityState = FastEcslent::ALIVE;
		//this->engine->entityMgr->addEntity(this);
		this->moveData.pos.y = 0.0f;
		return false;
	}
	return true;
}

void FastEcslent::Entity::addAspect(UnitAspect *asp) {
	aspects.push_back(asp);
}

FastEcslent::UnitAspect* FastEcslent::Entity::getAspect(UnitAspectType at) {
	for (std::deque<UnitAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++){
		if ((*it)->aspectType == at)
			return (*it);
	}
	return 0;
}

void FastEcslent::Entity::removeAspect(UnitAspectType at) {
	int index = -1;
	for (std::deque<UnitAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++){
		index++;
		if ((*it)->aspectType == at) {
			break;
		}
	}
	if(index >= 0 && index < (int) aspects.size()){
		aspects.erase(aspects.begin()+index);
	}
}

void FastEcslent::Entity::tick(double dt){
	switch(this->entityState) {
	case GESTATING:
		this->gestatingTick(dt);
		break;

	case ALIVE:
		this->aliveTick(dt);
		break;

//	case FROZEN:
//		this->frozenTick(dt);
//		break;

	case DYING:
		//this->dyingTick(dt);
		break;

	case DEAD:
		//this->deadTick();
		break;

	default:

		break;
	}
}
//
//void FastEcslent::Entity::frozenTick(double dt){
//	this->weaponInfo.freezeTime += dt;
//	if(this->weaponInfo.freezeTime > this->weaponInfo.freezeTimeThreshold){
//		this->weaponInfo.freezeTime = 0.0;
//		this->switchState(ALIVE);
//	}
//}

void FastEcslent::Entity::switchState(EntityState newState){
	this->entityState = newState;
	if(newState == GESTATING) {
		this->timeLeftToBirth = this->engine->gameMgr->entTypeData[this->entityType].buildTime;//this->gestationPeriod;
	} else if (newState == DYING) {
		this->timeLeftToDie = this->engine->gameMgr->entTypeData[this->entityType].buildTime;//this->gestationPeriod;
	}
}

void FastEcslent::Entity::gestatingTick(double dt){
	boost::mutex::scoped_lock scoped_lock(entLock);
	if (!raising(dt)){ // entity rises up through the surface. Return true when done
		this->switchState(ALIVE);
	}
}

void FastEcslent::Entity::aliveTick(double dt){
	//std::cout << "Ent: " << this->uiname << " EntityState: " << this->entityState << std::endl;
	boost::mutex::scoped_lock scoped_lock(entLock);
	for (std::deque<UnitAspect*>::iterator it = aspects.begin(); it != aspects.end(); it++){
		(*it)->tick(dt);
	}
}

void FastEcslent::Entity::dyingTick(double dt){
	boost::mutex::scoped_lock scoped_lock(entLock);
	if (!sinking(dt)) { //entity sinks. Returns true when sunk below surface of water
		this->switchState(DEAD);
	}
}

void FastEcslent::Entity::deadTick(double dt){
	return;
}
