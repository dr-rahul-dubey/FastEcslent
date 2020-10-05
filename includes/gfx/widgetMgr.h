/*
 * widgetMgr.h
 *
 *  Created on: Jan 7, 2013
 *      Author: sushil
 */

#ifndef WIDGETMGR_H_
#define WIDGETMGR_H_

#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreVector2.h>
#include <OgreColourValue.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <boost/lexical_cast.hpp>

#include <gfxMgr.h>
#include <widgets.h>

namespace OgreGFX {

	class GraphicsInteractionManager;

	class WidgetMgr : public GFXMgr, public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener {

	public:
		WidgetMgr(GraphicsInteractionManager *gim);
		~WidgetMgr();


	    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	    virtual bool frameStarted(const Ogre::FrameEvent& evt);
	    virtual bool frameEnded(const Ogre::FrameEvent& evt);


	    // OIS::KeyListener
	    virtual bool keyPressed( const OIS::KeyEvent &arg );
	    virtual bool keyReleased( const OIS::KeyEvent &arg );
	    // OIS::MouseListener
	    virtual bool mouseMoved( const OIS::MouseEvent &arg );
	    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	    int nextId;
	    //std::stringstream idStream; // no need to allocated it every time getNextId is called
	    std::string getNextId(std::string base);


	    virtual void initialize();// in gfxMgr;


	    //Defaults
	    Ogre::Vector2 POS;
	    Ogre::Vector2 LABEL_SIZE;
	    Ogre::Vector2 PANEL_SIZE;
	    Ogre::Vector2 MENU_ITEM_SIZE;
	    Ogre::Vector2 BUTTON_SIZE;

	    int WIDTH;
	    int HEIGHT;
	    int LABEL_X_OFFSET;
	    int LABEL_Y_OFFSET;

	    Ogre::ColourValue LABEL_TEXT_COLOR;
	    Ogre::ColourValue BUTTON_OFF_TEXT_COLOR;
	    Ogre::ColourValue BUTTON_ON_TEXT_COLOR;
	    Ogre::ColourValue MENU_DEFAULT_TEXT_COLOR;
	    Ogre::ColourValue MENU_SELECTED_TEXT_COLOR;


	    std::string PANEL_MATERIAL;
	    std::string LINE_SEP_MATERIAL;
	    std::string MENU_MATERIAL;

	    bool first; // a bad way to work around an TextAreaOverlayElement caption not displaying
	    UIOutline *uiBase;
	    FELabel   *testLabel;
	    FELabel   *testLabel2;
	    FELabel   *testLabel3;
	    FEPanel   *testPanel;
	    FELabelPair *testLabelPair;
	    FELabelPair *testLabelPair2;
	    FELabelPair *testLabelPair3;
	    FELabelPair *testLabelPair4;


	};





}



#endif /* WIDGETMGR_H_ */
