/*
 * commandHelp.h
 *
 *  Created on: Feb 26, 2012
 *      Author: sushil
 */

#ifndef COMMANDHELP_H_
#define COMMANDHELP_H_

#include <OgreVector3.h>

#include <ent.h>

#include <command.h>
#include <gather.h>
#include <micro.h>

namespace FastEcslent {

Move* createMoveForEnt(Entity* ent, Ogre::Vector3 pos);
Gather* createGatherForEntAndMineral(SCVehicle *scv, Minerals *mineral);

void  setMoveForEnt(Entity* ent, Ogre::Vector3 pos);
void  addMoveForEnt(Entity* ent, Ogre::Vector3 pos);

void  setPotentialMoveForEnt(Entity* ent, Ogre::Vector3 pos);
void  addPotentialMoveForEnt(Entity* ent, Ogre::Vector3 pos);

void setMicroForEnt(Entity *ent, Ogre::Vector3 pos);//, MicroParamsStruct p);



}

#endif /* COMMANDHELP_H_ */
