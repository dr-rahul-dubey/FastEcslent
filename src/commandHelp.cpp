/*
 * commandHelp.cpp
 *
 *  Created on: Feb 26, 2012
 *      Author: sushil
 */

#include <OgreVector3.h>
#include <gather.h>
#include <command.h>
#include <ent.h>
#include <buildings.h>
#include <target.h>
#include <group.h>
#include <unitAI.h>
#include <groupAI.h>
#include <micro.h>

using namespace FastEcslent;

Move *createMoveForEnt(Entity* ent, Ogre::Vector3 pos){
	Target* t = new Target;
	t->entity = 0;
	t->location = pos;
	t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	Move* m = new Move(ent, t);
	m->init();
	return m;
}


namespace FastEcslent {

	Gather *createGatherForEntAndMineral(SCVehicle *scv, Minerals *mineral){
		Target *t = new Target;
		t->entity = mineral;
		t->location = scv->moveData.pos;
		t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		Gather *g = new Gather(scv, t);
		g->init();
		return g;
	}
	//-------------------------------------------------------------------

	void setMoveForEnt(Entity* ent, Ogre::Vector3 pos){
		boost::mutex::scoped_lock scoped_lock(ent->entLock);
		Move *m = createMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->setCommand(m);
	}

	void addMoveForEnt(Entity* ent, Ogre::Vector3 pos){
		boost::mutex::scoped_lock scoped_lock(ent->entLock);
		Move *m = createMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->addCommand(m);
	}
	//-------------------------------------------------------------------
	PotentialMove *createPotentialMoveForEnt(Entity *ent, Ogre::Vector3 pos){
			Target* t = new Target;
			t->entity = 0;
			t->location = pos;
			t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
			PotentialMove *pm = new PotentialMove(ent, t);
			pm->init();
			return pm;
		}

	void setPotentialMoveForEnt(Entity *ent, Ogre::Vector3 pos) {
		boost::mutex::scoped_lock scoped_lock(ent->entLock);
		PotentialMove *pm = createPotentialMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->setCommand(pm);
	}



	void addPotentialMoveForEnt(Entity* ent, Ogre::Vector3 pos){
		boost::mutex::scoped_lock scoped_lock(ent->entLock);
		PotentialMove *pm = createPotentialMoveForEnt(ent, pos);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->addCommand(pm);
	}
	//-------------------------------------------------------------------

	Micro *createMicroForEnt(Entity *ent, Ogre::Vector3 pos){//, MicroParamsStruct p){
		Target* t = new Target;
		t->entity = 0;
		t->location = pos;
		t->offset   = Ogre::Vector3(0.0f, 0.0f, 0.0f);
		Micro *micro = new Micro(ent, t);//, p);
		micro->init();
		return micro;
	}

	void setMicroForEnt(Entity *ent, Ogre::Vector3 pos){//, MicroParamsStruct p) {
		boost::mutex::scoped_lock scoped_lock(ent->entLock);
		Micro *micro = createMicroForEnt(ent, pos);//, p);
		UnitAI *ai = dynamic_cast<UnitAI *> (ent->getAspect(UNITAI));
		ai->setCommand(micro);
	}

}
