/*
 * engine.cpp
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

#include <utils.h>

#include <timer.h>

//#include <entityMgr.h>
#include <engine.h>

using namespace std;

void FastEcslent::Engine::constructManagers() {
	entityMgr = new EntityMgr(this, options);
	selectionMgr = new SelectionMgr(this, options);
	weaponMgr    = new WeaponMgr(this, options);

	gameMgr   = new GameMgr(this, options);

	//optional managers
	if(options->tacticalAI) {
		distanceMgr = new DistanceMgr(this, options);
		groupMgr    = new GroupMgr(this, options);
	}

	//optional threads

	if(options->enableNetworking) net = new NetThread(this);
	if(options->enableGfx)        gfx = new GfxThread(this);

	//construct others
	IMMgr = new InfluenceMap(this, options);
}
void FastEcslent::Engine::init() {
	selectionTimer = new FastEcslent::MilliSecondTimer(200.0f);
	distanceTimer   = new FastEcslent::MilliSecondTimer(500.0f);


	entityMgr->init();
	selectionMgr->init();
	weaponMgr->init();
	IMMgr->init(); // must be initialized before gameMgr

	gameMgr->init();

	if (options->tacticalAI) {
		distanceMgr->init();
		groupMgr->init();
	}

	//optional threads
	if (options->enableNetworking) net->init();
	if (options->enableGfx)        gfx->init();
	//initialize others

}
void FastEcslent::Engine::showMenu() {
	return;
}

void FastEcslent::Engine::loadLevel(){


	//
	if (options->enableNetworking) net->run();
	if (options->enableGfx)        gfx->run();
	//sleep(2.0); //
	return;
}

void FastEcslent::Engine::releaseLevel(){


	//optional
	if (options->enableNetworking) net->stopAndJoin();
	if (options->enableGfx){
		gfx->join(options->shouldWaitForUserToShutGfx);

	}
	entityMgr->cleanup();
	selectionMgr->cleanup();
	weaponMgr->cleanup();
	IMMgr->cleanup(); // must be initialized before gameMgr
	gameMgr->cleanup();
	if (options->tacticalAI) {
		distanceMgr->cleanup();
		groupMgr->cleanup();
	}
	return;
}

void FastEcslent::Engine::stop(){
	//
	//std::cout << "Engine stopping" << std::endl;
	return;
}

void FastEcslent::Engine::tickAll(float dt){
	entityMgr->tick(dt);
	selectionMgr->tick(dt);
	weaponMgr->tick(dt);
	IMMgr->tick(dt);

	gameMgr->tick(dt);
	// tick other managers

	//optional Managers must be ticked
	if (options->tacticalAI) {
		distanceMgr->tick(dt);
		groupMgr->tick(dt);
	}
	//Threads are not ticked

}

void FastEcslent::Engine::run(){
	//double runTime = 0.0f;
	ptime oldTime = getCurrentTime();
	ptime newTime;
	time_duration diff;
	double speedup = options->speedup;
	frames = 0;
	dt = 0.001f; //fixed clock for simulations
	spedupDt = dt * speedup;
	while (frames < options->maxFrames && !this->quit){
		tickAll(spedupDt); // tick all managers
		frames++;
	}

	newTime = getCurrentTime();
	diff = newTime - oldTime;
	cout << endl << "Finished, running. Time run: " << diff.total_microseconds() * 0.000001f << endl;
	cout << "Frames: " << frames << endl << endl;
	//cout << "Quit: " << this->quit << endl;
	this->quit = true;



}

//update times
//runTime += dt;
//
//		newTime = getCurrentTime();
//		diff = newTime - oldTime;
//		oldTime = newTime;
//		dtime = diff.total_microseconds() * 0.000001f; //insecond


void FastEcslent::Engine::runTests(){
	int entIndex;
	Entity** selected;


	//test distance mgr
	if(options->tacticalAI){
		if(distanceTimer->hasFired()){
			distanceMgr->dumpAll();
		}
	}

	//test selection

	if(selectionTimer->hasFired()){
		entIndex = random()% this->entityMgr->nEnts;
		cout << "Unselected: " << entIndex << endl;
		this->selectionMgr->unselect(entIndex);
		selected = this->selectionMgr->getSelectedEnts();
		cout << "Selected: ";
		for(int i = 0; i < this->selectionMgr->getNSelectedEnts(); i++){
			cout << "(" << selected[i]->entityId.id << ": " << selected[i]->isSelected << ") ";
		}
		cout << endl;
	}
	//cout << "Iteration: " << n++ << endl;;
	//std::cout << "dtime: " << dtime << std::endl;

}




