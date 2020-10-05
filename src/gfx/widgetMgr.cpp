/*
 * widgetMgr.cpp
 *
 *  Created on: Jan 7, 2013
 *      Author: sushil
 */


#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>


#include <widgetMgr.h>

#include <GraphicsInteractionManager.h>

OgreGFX::WidgetMgr::WidgetMgr(GraphicsInteractionManager *gim) : GFXMgr(gim), Ogre::FrameListener(), Ogre::WindowEventListener(), OIS::KeyListener(), OIS::MouseListener() {
	std::cout << "Creating Widget Manager" << std::endl;
	nextId = 0;

    //Defaults
    POS = Ogre::Vector2(0, 0);
    LABEL_SIZE = Ogre::Vector2(100, 20);
    PANEL_SIZE = Ogre::Vector2(100, 20);
    MENU_ITEM_SIZE = Ogre::Vector2(100, 20);
    BUTTON_SIZE = Ogre::Vector2(50, 20);

    WIDTH = 100;
    HEIGHT = 20;
    LABEL_X_OFFSET = 5;
    LABEL_Y_OFFSET = 5;

    LABEL_TEXT_COLOR = Ogre::ColourValue(1.0, 1.0, 0.7, 1.0);
    BUTTON_OFF_TEXT_COLOR = Ogre::ColourValue(1.0, 1.0, 0.0, 1.0);
    BUTTON_ON_TEXT_COLOR =  Ogre::ColourValue(0.0, 0.0, 0.5, 1.0);
    MENU_DEFAULT_TEXT_COLOR = Ogre::ColourValue(1.0, 1.0, 0.0, 1.0);
    MENU_SELECTED_TEXT_COLOR = Ogre::ColourValue(0.0, 0.0, 0.5, 1.0);


    PANEL_MATERIAL    = "ECSLENT/navy/material/shipInfo/overlay";
    LINE_SEP_MATERIAL = "ECSLENT/line";
    MENU_MATERIAL     = "ECSLENT/navy/material/rmenu";

    uiBase = 0;
    //uiBase->show();



    std::cout << "Finished  Widget Manager: " << getNextId("Test") << std::endl;
}

OgreGFX::WidgetMgr::~WidgetMgr(){
}


void OgreGFX::WidgetMgr::initialize(){
	uiBase = new UIOutline(this->gfx);
	uiBase->show();

	testPanel = new OgreGFX::FEPanel(this->gfx, "TestPanel", (Widget*) 0, POS, PANEL_SIZE, PANEL_MATERIAL);

	testLabel = new OgreGFX::FELabel(this->gfx, "TestLabel", testPanel, LABEL_TEXT_COLOR, POS, LABEL_SIZE);
	testLabel2 = new OgreGFX::FELabel(this->gfx, "SecondRow", testPanel, LABEL_TEXT_COLOR, POS, LABEL_SIZE);
	testPanel->addItem(testLabel, Below);
	testPanel->addItem(testLabel2, Below);
	testLabel2->show();
	testLabel->show();
	testPanel->show();

	testLabelPair = new OgreGFX::FELabelPair(this->gfx, "dt", "NewValue", testPanel,
			LABEL_TEXT_COLOR,
			Ogre::Vector2(1000, 0), Ogre::Vector2(100, 200), 20, PANEL_MATERIAL);
	testLabelPair2 = new OgreGFX::FELabelPair(this->gfx, "SpedupDt", "Value2", testPanel,
			LABEL_TEXT_COLOR,
			Ogre::Vector2(1000, 0), Ogre::Vector2(100, 200), 20, PANEL_MATERIAL);
	testPanel->addItem(testLabelPair, Below);
	testPanel->addItem(testLabelPair2, Below);
	testLabelPair->setLeft(testLabelPair->left->caption);
	testLabelPair2->setLeft(testLabelPair2->left->caption);
	testLabelPair->show();
	testLabelPair2->show();
	first = true;
}


bool OgreGFX::WidgetMgr::frameStarted(const Ogre::FrameEvent& evt){
	return true;
}

bool OgreGFX::WidgetMgr::frameEnded(const Ogre::FrameEvent& evt){
	return true;
}

bool OgreGFX::WidgetMgr::frameRenderingQueued(const Ogre::FrameEvent& evt){
	std::string gasLabel      = "     Gas: ";
	std::string mineralsLabel = "Minerals: ";
	char dts[10], sdts[10];

	//std::cout << "Setting caption" << std::endl;
	if (first) {
		//first = false;
		//--------------------------------------------------------------------------
		//testLabel->textArea->setCaption(testLabel->caption);
		gasLabel.append(boost::lexical_cast<std::string>(this->gfx->engine->gameMgr->resources[FastEcslent::ONE].gas));
		mineralsLabel.append(boost::lexical_cast<std::string>(this->gfx->engine->gameMgr->resources[FastEcslent::ONE].minerals));
		testLabel->textArea->setCaption(gasLabel);
		testLabel2->textArea->setCaption(mineralsLabel);


		sprintf(dts, "%6.3f", this->gfx->engine->dt);
		sprintf(sdts,"%6.3f", this->gfx->engine->spedupDt);
		testLabelPair->setLeft(testLabelPair->left->caption);
		testLabelPair->setRight(boost::lexical_cast<std::string>(dts));

		//testLabelPair2->setLeft(testLabelPair2->left->caption);
		testLabelPair2->setRight(boost::lexical_cast<std::string>(sdts));
		//--------------------------------------------------------------------------
	}
    return true;
}

bool OgreGFX::WidgetMgr::keyPressed( const OIS::KeyEvent &arg ){
	std::cout << "Widget Manager key pressed: " << arg.key << std::endl;
	return true;
}

bool OgreGFX::WidgetMgr::keyReleased( const OIS::KeyEvent &arg ){
    return true;
}


bool OgreGFX::WidgetMgr::mouseMoved( const OIS::MouseEvent &arg ){
    return true;
}

bool OgreGFX::WidgetMgr::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	return true;
}

bool OgreGFX::WidgetMgr::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
    return true;
}

std::string OgreGFX::WidgetMgr::getNextId(std::string base){
	std::stringstream idStream;
	this->nextId++;
	idStream << base << "_" << nextId;
	return idStream.str();
}

