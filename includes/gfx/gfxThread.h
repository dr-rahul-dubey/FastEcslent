/*
 * gfxThread.h
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 */

#ifndef GFXTHREAD_H_
#define GFXTHREAD_H_

//#include <GraphicsInteractionManager.h>
//#include <ThreadTester.h>
//#include <engine.h>

#include <boost/thread/thread.hpp>



namespace FastEcslent {
	class Engine;
	//class OgreGFX::GraphicsInteractionManager;

	class GfxThread {
	private:
		boost::thread gfx;
		//OgreGFX::GraphicsInteractionManager *gimPtr;
		//OgreGFX::GraphicsInteractionManager *gimPtr;
		//ThreadTester *gimPtr;
		void runThread();

	public:
		Engine* engine;

		GfxThread(Engine* eng){
			engine = eng;
			//gimPtr = NULL;
		}

		void init();
		void run();
		void join(bool waitForUser);
		void stop();
		void kill();
	};

}
#endif /* GFXTHREAD_H_ */
