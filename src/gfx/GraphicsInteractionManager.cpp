/*
 * GraphicsInteractionManager.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: sushil
 ----------------------------------------------------------------------------
Filename:    GraphicsInteractionManager.cpp originally  named BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/

#include <OgreMeshManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <Overlay/OgreOverlaySystem.h>

#include <engine.h>
#include <GraphicsInteractionManager.h>
#include <uiMgr.h>

#include <map>


using namespace OgreGFX;

//-------------------------------------------------------------------------------------
GraphicsInteractionManager::GraphicsInteractionManager(FastEcslent::Engine* eng):
	mRoot(0),
	mSceneMgr(0),
	mWindow(0),
	mCamera(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	oceanSurface(Ogre::Vector3::UNIT_Y, 0),
	mCursorWasVisible(false),
	mShutDown(false)
{
	this->engine = eng;

}

//-------------------------------------------------------------------------------------
GraphicsInteractionManager::~GraphicsInteractionManager(void)
{
    //if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool GraphicsInteractionManager::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog())){
		return false;
	}
	mWindow = mRoot->initialise(true, "FastEcslent::GraphicsInteractionManager's Render Window");
	return true;

}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
	//mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
	Ogre::OverlaySystem *ovSys = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(ovSys);

}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction

    //mCamera->setPosition(Ogre::Vector3(100,500,100));//set in camera manager

    // Look back along -Z
//	    mCamera->lookAt(Ogre::Vector3(size/2,0,0));
    mCamera->setNearClipDistance(5);

    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
//void GraphicsInteractionManager::createFrameListener(void)
void GraphicsInteractionManager::createUIMgr(void)
{
	uiMgr = new OgreGFX::UIMgr(this);
	mRoot->addFrameListener(uiMgr);
}

void GraphicsInteractionManager::createWidgetMgr(void)
{
	widgetMgr = new OgreGFX::WidgetMgr(this);
	mRoot->addFrameListener(widgetMgr);
}

/*void GraphicsInteractionManager::createInputSystem(void)
{
	inputSystem = new FastEcslent::InputSystem(this, uiMgr->keyboard, uiMgr->mouse);
	mRoot->addFrameListener(inputSystem);
}*/


void GraphicsInteractionManager::createFrameListener(void) {
	mRoot->addFrameListener(this);
}


//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);

    vp->setVisibilityMask(OgreGFX::PLAYER_CAMERA_MASK);
    //vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    //vp->setBackgroundColour(Ogre::ColourValue::White);

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())  {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i){
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "config/resources_d.cfg";
    mPluginsCfg = "config/plugins_d.cfg";
#else
    mResourcesCfg = "config/resources.cfg";
    mPluginsCfg = "config/plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
void GraphicsInteractionManager::initManagers(void) {

		uiMgr->initialize();
		widgetMgr->initialize();
}

bool GraphicsInteractionManager::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);
    setupResources();
    bool carryOn = configure();
    if (!carryOn) return false;
    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();
    // Create the scene
    createScene();
    createTerrain();
    createUIMgr();
    createWidgetMgr();
//    createInputSystem();
    createFrameListener();
    initManagers();

    return true;
};
//-------------------------------------------------------------------------------------

void GraphicsInteractionManager::handleBornEnts(){
	if (this->engine->entityMgr->nEnts > this->nGFXNodes) {
		for (int i = nGFXNodes; i < this->engine->entityMgr->nEnts; i++){
				makeNode(this->engine->entityMgr->ents[i]);
		}
		this->nGFXNodes = engine->entityMgr->nEnts;
	}

}


void GraphicsInteractionManager::handleDevelopingEnts(){

}


bool GraphicsInteractionManager::frameRenderingQueued(const Ogre::FrameEvent& evt){
    if(mWindow->isClosed())
        return false;
    mShutDown = uiMgr->shutDown;
    //std::cout << "mShutDown: " << mShutDown << std::endl;
    if(mShutDown){
    	std::cout << "Trying to shutdown" << std::endl;
    	if(uiMgr) {
    		uiMgr->kill();
    		delete uiMgr;
    		boost::mutex::scoped_lock scoped_lock(engine->quitLock);
    		engine->quit = true;
    	}
        return false;
    }
    handleDevelopingEnts(); // create nodes
    handleBornEnts();       // move   nodes
    copySimState(); //handleLivingEnts


    return true;
}

//Adjust mouse clipping area
void GraphicsInteractionManager::windowResized(Ogre::RenderWindow* rw){
}

//Unattach OIS before window shutdown (very important under Linux)
void GraphicsInteractionManager::windowClosed(Ogre::RenderWindow* rw){
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow ) {
    	if(uiMgr) {
    		uiMgr->kill();
    	}
    }
}



void GraphicsInteractionManager::makeInitialEntityNodes(){
	this->nGFXNodes = engine->entityMgr->nEnts;
	std::cout << "GFX: Number of ents: " << nGFXNodes << std::endl;
	for (int i = 0; i < nGFXNodes; i++){
		makeNode(engine->entityMgr->ents[i]);
	}

}


void GraphicsInteractionManager::makeNode(FastEcslent::Entity *feEnt){
	std::cout << "Name: " << feEnt->uiname << std::endl;
	Ogre::Entity* ent = mSceneMgr->createEntity(feEnt->uiname, feEnt->meshName);

	ent->setVisibilityFlags(OgreGFX::ENT_VISIBILITY_FLAG);
//		if (engine->entityMgr->ents[i]->entityClass == FastEcslent::STATIC){
//			ent->setVisibilityFlags(OgreGFX::STATIC_VISIBILITY_FLAG);
//		}
	std::cout << "GFX creating: " << feEnt->uiname << std::endl;

	Ogre::Vector3 position = feEnt->moveData.pos;
	Ogre::SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	std::stringstream ss;
	ss << feEnt->uiname << "_mini" ;
	Ogre::Entity* minimapEnt = mSceneMgr->createEntity(ss.str(), "cube.mesh" );
	minimapEnt->setVisibilityFlags(OgreGFX::MINIMAP_VISIBILITY_FLAG);
	Ogre::SceneNode* minimapNode = sceneNode->createChildSceneNode();
	minimapNode->attachObject(minimapEnt);
	minimapNode->setScale(2.0, 2.0, 2.0);
	int id = feEnt->entityId.id;
	gfxNodes[id].node = sceneNode;
	gfxNodes[id].node->attachObject(ent);
	gfxNodes[id].node->resetOrientation();
	gfxNodes[id].node->yaw(Ogre::Radian(feEnt->moveData.heading));
	gfxNodes[id].id = id;
	gfxNodes[id].selectable = feEnt->selectable;
	gfxNodes[id].actionable = feEnt->actionable;
	sceneNodeToEntIdMap[sceneNode] = id;
}


void GraphicsInteractionManager::createScene(){

	//this->mShutDown = false;
	Ogre::Entity*         mOceanSurfaceEnt;
	makeInitialEntityNodes();

	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));
	//Ogre::Light* l1 = mSceneMgr->createLight("LightOne");
	//l1->setPosition(20, 80, 50);

	//mSceneMgr->setSkyBox(true, "SkyBox", 1000);
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 10, 8);
	//mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 1000, false);


    // Define a plane mesh that will be used for the ocean surface
	//Ogre::Plane oceanSurface;

    oceanSurface.normal = Ogre::Vector3::UNIT_Y;
    oceanSurface.d = 0;
    int size = this->engine->options->mapSize;
    Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        oceanSurface,
        size, size, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

    mOceanSurfaceEnt = mSceneMgr->createEntity( "OceanSurface", "OceanSurface" );
    mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, -100, 0))->attachObject(mOceanSurfaceEnt);
    //mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mOceanSurfaceEnt);
    mOceanSurfaceEnt->setCastShadows(false);
    //mOceanSurfaceEnt->setMaterialName("OceanHLSL_GLSL");
    mOceanSurfaceEnt->setMaterialName("NavyCg");
    //mOceanSurfaceEnt->setMaterialName("OceanCg");
    //mOceanSurfaceEnt->setMaterialName("Ocean2_Cg");

    //Setup terrain now
}

void GraphicsInteractionManager::createTerrain(){
	this->terrain = new Terrain(this);
	this->terrain->initialize();

}


void GraphicsInteractionManager::copySimState(){

	for(int i = 0; i < nGFXNodes; i++){
		//std::cout << engine->entityMgr->ents[i]->pos.x << ", " << engine->entityMgr->ents[0]->pos.y << ", " <<  engine->entityMgr->ents[0]->pos.z << std::endl;
		Ogre::Vector3 position = engine->entityMgr->ents[i]->moveData.pos;
		gfxNodes[i].node->setPosition(position);
		//gfxNodes[i].node->setPosition(engine->entityMgr->ents[i]->moveData.pos.x, engine->entityMgr->ents[0]->pos.y, engine->entityMgr->ents[0]->pos.z);
		gfxNodes[i].node->resetOrientation();
		gfxNodes[i].node->yaw(Ogre::Radian(engine->entityMgr->ents[i]->moveData.heading));
	}
}


