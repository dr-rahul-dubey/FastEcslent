/*
 * engine.h
 *
 *  Created on: November 20, 2011
 *      Author: sushil j louis
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <options.h>
#include <InfluenceMap.h>
#include <entityMgr.h>
#include <selectionMgr.h>
#include <weaponMgr.h>
#include <gameMgr.h>
#include <distanceMgr.h>

#include <groupMgr.h>


#include <gfxThread.h>
#include <netThread.h>
#include <timer.h>

//#include<params.h>

namespace FastEcslent {

	class Engine  {

	private:
		void tickAll(float dt);

		//test vars/methods
		FastEcslent::MilliSecondTimer* selectionTimer;
		FastEcslent::MilliSecondTimer* distanceTimer;
		void runTests();



	public:

		Options *options;
		int     instanceId;
		//Params  flockParameters;
		int     rands[1000];
		unsigned long int frames;
		float    dt;
		float    spedupDt;

		bool   quit;
		boost::mutex quitLock;

		InfluenceMap* IMMgr;
		EntityMgr*    entityMgr;
		SelectionMgr* selectionMgr;
		WeaponMgr*    weaponMgr;
		GameMgr*      gameMgr;
		DistanceMgr*  distanceMgr;
		GroupMgr*     groupMgr;


		//threads. optional Features

		NetThread *net;
		GfxThread *gfx;

		Engine(int instId, Options *opts) {
			instanceId = instId;
			options = opts;
			quit = false;
			selectionMgr   = NULL;
			distanceMgr    = NULL;
			IMMgr          = NULL;
			gameMgr        = NULL;
			groupMgr       = NULL;
			entityMgr      = NULL;
			weaponMgr      = NULL;
			selectionTimer = NULL;
			distanceTimer  = NULL;
			net = NULL;
			gfx = NULL;
			dt = 0;
			frames = 0;
			spedupDt = 0;
		}

		~Engine(){
			delete selectionMgr;
			delete distanceMgr;
			delete gameMgr;
			delete groupMgr;
			delete entityMgr;
			delete weaponMgr;
			delete selectionTimer;
			delete distanceTimer;
		}
		//Managers
		void showMenu();
		void constructManagers();
		void init();
		void loadLevel();
		void releaseLevel();
		void run();
		void stop();

	} ;

	typedef Engine * EnginePtr;
}



#endif /* ENGINE_H_ */
