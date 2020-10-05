/*
 * physics2.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: sushil
 */

#include <assert.h>
//#include <stdio.h>

#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <physics.h>
#include <ent.h>
#include <utils.h>

void FastEcslent::Physics2D2::init(){
	entity->moveData.desiredSpeed = entity->moveData.speed;//kInvalidFloat;
	entity->moveData.desiredHeading = entity->moveData.heading;//kInvalidFloat;

}

/*void FastEcslent::Physics2D2::printVector(const Ogre::Vector3 &v){
	std::cout << "[" << v.x << ", "  << v.y << ", " << v.z << "]" << std::endl;
}*/

void FastEcslent::Physics2D2::doHelmsman(double dtime){// You have a desired speed and heading, get me new position and orientation

	if (kInvalidFloat == entity->moveData.desiredSpeed || kInvalidFloat == entity->moveData.desiredHeading)
		return;

	//std::cout << "Speed: " << entity->vel.length() << ", Heading: " << -atan2(entity->vel.z, entity->vel.x ) << std::endl;
	//FastEcslent::Physics2D2::printVector(entity->vel);
//	Ogre::Vector3 desiredVelocity = Ogre::Vector3(cos(-entity->desiredHeading) * entity->desiredSpeed, 0.0f, sin(-entity->desiredHeading) * entity->desiredSpeed);


	if (entity->moveData.speed < entity->moveData.desiredSpeed) {
		entity->moveData.speed += (entity->staticData.maxAcceleration * dtime);
	} else if (entity->moveData.speed > entity->moveData.desiredSpeed) {
		entity->moveData.speed -= (entity->staticData.maxAcceleration * dtime);
	}

	Ogre::Quaternion desiredRot(Ogre::Radian(entity->moveData.desiredHeading), Ogre::Vector3::UNIT_Y);

	//update it all
	entity->moveData.rot = Ogre::Quaternion::Slerp(entity->staticData.maxRotationalSpeed * dtime, entity->moveData.rot, desiredRot, true);
	entity->moveData.heading = entity->moveData.rot.getYaw().valueRadians();
	entity->moveData.yaw     = entity->moveData.heading;

	entity->moveData.speed = clamp(entity->moveData.speed, entity->staticData.minSpeed, entity->staticData.maxSpeed);

	entity->moveData.vel = Ogre::Vector3(cos(-entity->moveData.heading) * entity->moveData.speed, 0.0f, sin(-entity->moveData.heading) * entity->moveData.speed);
	//finally update pos
	entity->moveData.pos = entity->moveData.pos + (entity->moveData.vel * dtime);

	//std::cout << "Speed: " << entity->speed << ", Heading: " << entity->heading << std::endl <<std::endl;

}


void FastEcslent::Physics2D2::tick(double dtime){
	doHelmsman(dtime);
}
