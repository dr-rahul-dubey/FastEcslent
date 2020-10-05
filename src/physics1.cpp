/*
 * physics.cpp
 *
 *  Created on: Dec 19, 2011
 *      Author: sushil
 */
#include <assert.h>

#include <OgreVector3.h>

#include <physics.h>
#include <ent.h>
#include <utils.h>

void FastEcslent::Physics2D1::init(){
	entity->moveData.desiredSpeed = entity->moveData.speed;
	entity->moveData.desiredHeading = entity->moveData.heading;
}

void FastEcslent::Physics2D1::doHelmsman(double dtime){

	if (kInvalidFloat == entity->moveData.desiredSpeed || kInvalidFloat == entity->moveData.desiredHeading)
		return;
	//speed
	timeScaledAcceleration = entity->staticData.maxAcceleration * dtime;
	entity->moveData.speed += clamp(entity->moveData.desiredSpeed - entity->moveData.speed, -timeScaledAcceleration, +timeScaledAcceleration);
	cosYaw = cos(-entity->moveData.heading);
	sinYaw = sin(-entity->moveData.heading);
	entity->moveData.vel = Ogre::Vector3(entity->moveData.speed * cosYaw, 0.0f, entity->moveData.speed * sinYaw);
	entity->moveData.pos = entity->moveData.pos + (entity->moveData.vel * dtime);

	//heading
	this->timeScaledRotationalSpeed = entity->staticData.maxRotationalSpeed * dtime;
	//rotate only if you are moving at > 0.5
	if(entity->moveData.speed > 0.5f){
		angleDiff = differenceBetweenAngles(entity->moveData.desiredHeading, entity->moveData.heading);
		dHeading = clamp(angleDiff, -timeScaledRotationalSpeed, +timeScaledRotationalSpeed);
		entity->moveData.heading += dHeading;
	}

	std::cout << "Speed: " << entity->moveData.speed << ", Heading: " << entity->moveData.heading << std::endl;
}


void FastEcslent::Physics2D1::tick(double dtime){
	doHelmsman(dtime);
}
