/*
 * physics3D1.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: sushil
 */

#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <physics.h>
#include <ent.h>
#include <utils.h>


void FastEcslent::Physics3D1::init(){
	entity->moveData.desiredSpeed = entity->moveData.speed;//kInvalidFloat;
	entity->moveData.desiredHeading = entity->moveData.heading;//kInvalidFloat;
	entity->moveData.desiredAltitude = entity->moveData.altitude;

}

void FastEcslent::Physics3D1::tick(double dtime){
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

	if (entity->moveData.altitude < entity->moveData.desiredAltitude) {
		entity->moveData.altitude += (entity->staticData.climbrate * dtime);
	} else if (entity->moveData.altitude > entity->moveData.desiredAltitude){
		entity->moveData.altitude -= (entity->staticData.climbrate *dtime);
	}
	clamp(entity->moveData.altitude, entity->staticData.minAltitude, entity->staticData.maxAltitude);

	Ogre::Quaternion desiredRot(Ogre::Radian(entity->moveData.desiredHeading), Ogre::Vector3::UNIT_Y);

	//update it all
	entity->moveData.rot = Ogre::Quaternion::Slerp(entity->staticData.maxRotationalSpeed * dtime, entity->moveData.rot, desiredRot, true);
	entity->moveData.heading = entity->moveData.rot.getYaw().valueRadians();
	entity->moveData.yaw     = entity->moveData.heading;

	//move speed
	entity->moveData.speed = clamp(3*entity->moveData.speed, entity->staticData.minSpeed, 3*entity->staticData.maxSpeed);

	entity->moveData.vel = Ogre::Vector3(cos(-entity->moveData.heading) * entity->moveData.speed, 0.0f, sin(-entity->moveData.heading) * entity->moveData.speed);
	//finally update pos
	entity->moveData.pos = entity->moveData.pos + (entity->moveData.vel * dtime);
	entity->moveData.pos.y = entity->moveData.altitude;

	//std::cout << "Speed: " << entity->speed << ", Heading: " << entity->heading << std::endl <<std::endl;

}



