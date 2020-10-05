/*
 * GraphicsInteractionManager.h
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 */


#ifndef GRAPHICSINTERACTIONMANAGER_H_
#define GRAPHICSINTERACTIONMANAGER_H_


#include <map>

#include <OgreCamera.h>
/*
 *
#include <OgreLogManager.h>
*/
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreSceneNode.h>

/*
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
*/

//#include <SdkTrays.h>
//#include <SdkCameraMan.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector3.h>

#include <gfxNode.h>
#include <const.h>
#include <engine.h>
#include <uiMgr.h>
#include <widgetMgr.h>
#include <gfxConst.h>
#include <terrain.h>


extern const int FastEcslent::MaxEnts;

namespace OgreGFX {

	//class GraphicsInteractionManager : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
	class GraphicsInteractionManager : public Ogre::FrameListener, public Ogre::WindowEventListener {
	public:
		GraphicsInteractionManager(FastEcslent::Engine* eng);
		~GraphicsInteractionManager(void);

		void go(void);
		FastEcslent::Engine* engine;
		UIMgr*  uiMgr;
		WidgetMgr* widgetMgr;
		Terrain* terrain;

		Ogre::Root *mRoot;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera* mCamera;

		Ogre::SceneNode* headNode;
		Ogre::SceneNode* referenceNode;
		Ogre::SceneNode* armoury;
		//Ogre::SceneNode* commandCenter;
		GFXNode          gfxNodes[FastEcslent::MaxEnts];
		std::map<Ogre::SceneNode*, int> sceneNodeToEntIdMap;
		int              nGFXNodes;
		/*
		Ogre::SceneNode* entityNodes[FastEcslent::MaxEnts];
		bool             selectable[FastEcslent::MaxEnts];
		*/
		Ogre::String mResourcesCfg;
		Ogre::String mPluginsCfg;

		Ogre::Plane oceanSurface;//(Ogre::Vector3::UNIT_Y, 0);

	public:
		bool setup();
		void initManagers();
		bool configure(void);
		void chooseSceneManager(void);
		void createCamera(void);
		void createFrameListener(void);

		void createUIMgr(void);
		void createWidgetMgr(void);
//		 void createInputSystem(void);

		void createScene(void);// = 0; // Override me!
		void createTerrain(void);
		void destroyScene(void);
		void createViewports(void);
		void setupResources(void);
		void createResourceListener(void);
		void loadResources(void);

		// Ogre::FrameListener
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// Ogre::WindowEventListener
		//Adjust mouse clipping area
		void windowResized(Ogre::RenderWindow* rw);
		//Unattach OIS before window shutdown (very important under Linux)
		void windowClosed(Ogre::RenderWindow* rw);

		//Simulation bridge
		void makeInitialEntityNodes();
		void makeNode(FastEcslent::Entity *);

		void handleDevelopingEnts();
		void handleBornEnts();
		void copySimState();



		bool mCursorWasVisible;                    // was cursor visible before dialog appeared
		bool mShutDown;

	};

}

#endif /* GRAPHICSINTERACTIONMANAGER_H_ */
