/*
 * gfxThread.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 */

#include <boost/thread.hpp>

#include <engine.h>
#include <gfxThread.h>

//#include <uiMgr.h>
#include <GraphicsInteractionManager.h>
//#include <uiMgr.h>
//#include <ThreadTester.h>


void FastEcslent::GfxThread::init() {
	std::cout << "Initialized Gfx before starting thread. Engine instance Id: " << engine->instanceId << std::endl;
}

void FastEcslent::GfxThread::run() {

	gfx = boost::thread(&FastEcslent::GfxThread::runThread, this);
	std::cout << "Started Gfx thread's runThread method" << gfx.get_id() << std::endl;

}
OgreGFX::GraphicsInteractionManager *gimPtr;
void FastEcslent::GfxThread::runThread(){


	//ThreadTester gim(engine);

	gimPtr = new OgreGFX::GraphicsInteractionManager(engine); //&gim;
	std::cout << "Ogre: created TA, getting ready to run:" << gfx.get_id() << std::endl;
	//engine->entityMgr->ents[0]->print();

	gimPtr->go();
	std::cout << "Ogre finishing: shutting down" << gfx.get_id() << std::endl;
	//engine->entityMgr->ents[1]->print();

}

void FastEcslent::GfxThread::stop(){

}

void FastEcslent::GfxThread::join(bool waitForUser){
	std::cout << "Ogre: Joining getting ready to stop:" << gfx.get_id() << std::endl;
	if (!waitForUser){
		std::cout << "not waiting for user" << std::endl;
		gimPtr->uiMgr->shutDown = true;
	}

	gfx.join();
}


void FastEcslent::GfxThread::kill(){
	gfx.interrupt();
}
