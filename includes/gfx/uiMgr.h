/*
 * uiMgr.h
 *
 *  Created on: Feb 18, 2012
 *      Author: sushil
 */

#ifndef UIMGR_H_
#define UIMGR_H_

#include <list>
#include <set>
#include <map>


#include <OgreCamera.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreSceneQuery.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <gfxNode.h>
#include <gfxMgr.h>
#include <cameraMgr.h>

#include <selectionBox.h>

#include <inputHandlers.h>
#include <inputEvents.h>
#include <controlGroupsHandler.h>
#include <minimap.h>

#include <group.h>

#include <const.h>
#include <enums.h>

namespace OgreGFX {

class GraphicsInteractionManager;

	class UIMgr : public GFXMgr, public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener {

	public:
	    Ogre::RenderWindow* renderWindow;
	    Ogre::SceneManager* sceneManager;
	    Ogre::Root *ogreRoot;
	    Ogre::Camera* camera;

	    CameraMgr* cameraMgr;

	    OIS::InputManager* inputManager;
	    OIS::Mouse*    mouse;
	    OIS::Keyboard* keyboard;

	    bool ctrlDown;
	    bool shiftDown;
	    bool altDown;

	    OIS::MouseState ms;
	    Ogre::Vector3 posUnderMouse;

	    bool shutDown;

		Ogre::RaySceneQuery* raySceneQuery;
		float selectionDistanceSquaredThreshold;
		std::list <OgreGFX::GFXNode*>  currentSelections;
		boost::mutex selectionUpdateLock;

		SelectionBox* selectionBox;
		Ogre::Vector2 startPos, stopPos;
		Ogre::PlaneBoundedVolumeListSceneQuery* volQuery;
		bool selectingNow;
		bool volumeSelectingNow;
		bool commandingNow;
		Ogre::Vector3 moveSurfaceLocation;
		Ogre::Vector3 move3DLocation;
		float move3DStartY;

		ControlGroupsHandler *controlGroupsHandler;
		Minimap              *minimap;
		//std::list<Ogre::MovableObject*> selected;

	    void createInputSystem();

	    void windowResized(Ogre::RenderWindow* win);
	    void windowClosed(Ogre::RenderWindow* win);

	    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	    virtual bool frameStarted(const Ogre::FrameEvent& evt);
	    virtual bool frameEnded(const Ogre::FrameEvent& evt);
	    void DebugDrawTest();
	    void drawIMs();
	    void drawMinsForSide(FastEcslent::Side side, Ogre::ColourValue col);
	    void drawIMsForSide(FastEcslent::Side side);

	    // OIS::KeyListener
	    virtual bool keyPressed( const OIS::KeyEvent &arg );
	    virtual bool keyReleased( const OIS::KeyEvent &arg );
	    // OIS::MouseListener
	    virtual bool mouseMoved( const OIS::MouseEvent &arg );
	    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


	    //Key and mouse handlers handling
	    bool registerKeyHandler(const std::set<OIS::KeyCode>   *modifiers, const OIS::KeyCode arg, OgreGFX::KeyHandler *handler);
	    bool registerMouseHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::MouseButtonID arg, OgreGFX::MouseHandler *handler);

	    bool callKeyHandlers(const OIS::KeyEvent &arg);
	    bool callMouseHandlers(const OIS::MouseEvent &arg, const OIS::MouseButtonID id);

	    std::map <std::string, std::list<OgreGFX::KeyHandler *> > keyHandlers;
	    //std::map <std::string, OgreGFX::KeyHandler *> keyHandlers;
	    //std::map <std::string, OgreGFX::MouseHandler *> mouseHandlers;
	    std::map <std::string, std::list<OgreGFX::MouseHandler *> > mouseHandlers;


	    //UIMgr(Engine* eng, Ogre::RenderWindow* win, Ogre::SceneManager *sm, Ogre::Camera* cam);
	    UIMgr(GraphicsInteractionManager *gim);//Engine* eng, Ogre::RenderWindow* win, Ogre::SceneManager *sm, Ogre::Camera* cam);
	    ~UIMgr();
	    void kill();
	    void updateFEEngineWithSelections();
	    void clearSelectionsAndUpdateFEEngine();
	    void printCurrentSelections();

	    void handleSingleSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	    void handleVolumeSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	    void doVolumeSelection(const Ogre::Vector2 &first, const Ogre::Vector2 &second);
	    void selectEntity(int index);
	    void decorateSelectedEntities();
	    void drawSelectionCircles();
	    void drawUnderMouseCircle();
	    void drawMoveLocations();
	    void showCommands(FastEcslent::Entity *ent, Ogre::Vector3 pos);

	    std::pair<int, Ogre::Vector3> entIndexUnderMouse();
	    std::pair<int, Ogre::Vector3> entIndexUnderMouse(const OIS::MouseEvent &arg);
	    std::pair<int, Ogre::Vector3> entIndexUnderMouse(const OIS::MouseState &arg);

	    void handleCommand(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	    void CommandMaintainAttack(int index);
	    void CommandMove(Ogre::Vector3 pos);
	    void CommandPotentialFieldsMove(Ogre::Vector3 pos);

	    FastEcslent::Group* groupFromSelections();

	    int rayDistanceForVolume;


	    virtual void initialize();

	private:
	    void checkSetModifiers(const OIS::KeyEvent &arg, bool value);
	    void clearModifiersDown();

	    std::set<OIS::KeyCode> *makeModifiersSet();
	    std::string modifierKeyHash(const OIS::KeyCode);
	    std::string modifierKeyMouseHash(const OIS::MouseButtonID);


	};

}

#endif /* UIMGR_H_ */
