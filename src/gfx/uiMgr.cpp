/*
 * uiMgr.cpp
 *
 *  Created on: Feb 18, 2012
 *      Author: sushil
 */

//using namespace OgreGFX;

#include<cfloat>


#include <engine.h>
#include <uiMgr.h>
#include <GraphicsInteractionManager.h>

#include <DebugDrawer.h>

#include <groupAI.h>
#include <unitAI.h>
#include <flock.h>
#include <target.h>
#include <command.h>

#include <commandHelp.h>
#include <const.h>
#include <utils.h>
#include <enums.h>
#include <colormap.h>

#include <creationMouseHandler.h>
#include <controlGroupsHandler.h>



OgreGFX::UIMgr::UIMgr(OgreGFX::GraphicsInteractionManager *gim) : GFXMgr(gim), OIS::KeyListener(), OIS::MouseListener()  {//Engine* eng,  Ogre::RenderWindow *win, Ogre::SceneManager *sm, Ogre::Camera* cam): OgreGFX::Mgr(eng), trayMgr(0), inputManager(0), mouse(0), keyboard(0) {
	shutDown = false;
	selectionDistanceSquaredThreshold = 10000;
	camera = gfx->mCamera;
	renderWindow = gfx->mWindow;
	sceneManager = gfx->mSceneMgr;
	createInputSystem();
	//then
	cameraMgr = new OgreGFX::CameraMgr(gfx);//, camera);

	selectionBox = new OgreGFX::SelectionBox("SelectionBox");
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(selectionBox);
	volQuery = sceneManager->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
	rayDistanceForVolume = 10;
	selectingNow         = false;
	volumeSelectingNow   = false;
	commandingNow        = false;

	clearModifiersDown();
	//currentSelection = gim->gfxNodes[0].node;

	new DebugDrawer(gfx->mSceneMgr, 0.5f);
	this->posUnderMouse = Ogre::Vector3::NEGATIVE_UNIT_Y;

}

void OgreGFX::UIMgr::initialize(){
	std::cout << "Calling uiMgr initializer" << std::endl;


	std::set<OIS::KeyCode> *creationMods = new std::set<OIS::KeyCode>();
	creationMods->insert(OIS::KC_LCONTROL);
	CreationMouseHandler *cmh = new CreationMouseHandler(this->gfx, creationMods);
	this->registerMouseHandler(creationMods, OIS::MB_Left, cmh);
	std::cout << "Registered creation mouse handler" << std::endl;

	controlGroupsHandler = new ControlGroupsHandler(gfx);

	minimap              = new Minimap(gfx);
	//minimap->init();

}

void OgreGFX::UIMgr::clearModifiersDown(){
	shiftDown = false;
	ctrlDown  = false;
	altDown   = false;
}

void OgreGFX::UIMgr::createInputSystem() {
	//Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	std::cout << "*** Initializing OIS ***" << std::endl;
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	renderWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


#if defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#elif defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#endif


	inputManager = OIS::InputManager::createInputSystem( pl );

	keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject( OIS::OISKeyboard, true ));
	mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject( OIS::OISMouse, true ));
	mouse->capture();
	ms = mouse->getMouseState();
	ms.width = gfx->mWindow->getWidth();
	ms.height = gfx->mWindow->getHeight();

	mouse->setEventCallback(this);
	keyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(renderWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(renderWindow, this);

	//mRoot->addFrameListener(this); Added in createInputSystem in GraphicsInteractionManager
}

void OgreGFX::UIMgr::DebugDrawTest(){
	for (int i = 0; i < 5; ++i)	   {
		for (int j = 0; j < 5; j++)	      {
			for (int k = 0; k < 5; k++)	         {
				Ogre::AxisAlignedBox box (Ogre::Vector3(i * 10.0f + 2.0f, j * 10.0f + 2.0f, k * 10.0f + 2.0f),
						Ogre::Vector3((i + 1) * 10.0f - 2.0f, (j + 1) * 10.0f - 2.0f, (k + 1) * 10.0f - 2.0f));
				DebugDrawer::getSingleton().drawCuboid(box.getAllCorners(),
						Ogre::ColourValue(51.0f * i / 255.0f, 51.0f * j / 255.0f, 51.0f * k / 255.0f), true);
			}
		}
	}
	//DebugDrawer::getSingletonPtr()->build();
}

void OgreGFX::UIMgr::drawIMs() {
	int colorIndex = 0;
	FastEcslent::InfluenceMap *im = this->gfx->engine->IMMgr;
	FastEcslent::Entity *ent;
	FastEcslent::D3CoordStruct spec;
	FastEcslent::Side side;
	for(int n = 0; n < this->gfx->engine->entityMgr->nEnts; n++){
		ent = this->gfx->engine->entityMgr->ents[n];
		spec = im->getIMCellCoordsFromPosition(ent->moveData.pos);
		side = ent->entityId.side;
		int range = ent->IMForEnemies.range;
		for(int x = spec.x - range; x <= spec.x + range; x++){
			if (x < 0 || x >= this->gfx->engine->options->imapx) continue;
			for(int y = spec.y - range; y <= spec.y + range; y++){
				if(y < 0 || y >= this->gfx->engine->options->imapy) continue;
				for(int z = spec.z - range; z <= spec.z + range; z++){
					if(z < 0 || z >= this->gfx->engine->options->imapz) continue;
					if( !closeTo(im->IMEnemy[side][x][y][z].imValue, 0)){
						//if(im->IM[i][j][k].imValue > 0){
						colorIndex = (int) clamp(im->IMEnemy[side][x][y][z].imValue, 0.0f, 255.0f);
#ifdef FE_DEBUG
						DebugDrawer::getSingleton().drawCuboid(im->IMEnemy[side][x][y][z].box.getAllCorners(), ColorMap[side][255 - colorIndex], true);
#endif
					}
				}
			}
		}
	}
}
void OgreGFX::UIMgr::drawIMsForSide(FastEcslent::Side side) {
	int colorIndex = 0;
	FastEcslent::InfluenceMap *im = this->gfx->engine->IMMgr;
	for(int x = 0; x < im->options->imapx; x++){
		for(int y = 0; y < im->options->imapy; y++){
			for(int z = 0; z < im->options->imapz; z++){
				//if( !closeTo(im->IMEnemy[side][x][y][z].imValue, 0)){
				if( im->IMEnemy[side][x][y][z].imValue >  0){
					colorIndex = (int) clamp(im->IMEnemy[side][x][y][z].imValue, 0.0f, 255.0f);
#ifdef FE_DEBUG
					DebugDrawer::getSingleton().drawCuboid(im->IMEnemy[side][x][y][z].box.getAllCorners(), ColorMap[side][255 - colorIndex], true);
#endif
				}
			}
		}
	}
	//Draw min
	FastEcslent::D3CoordStruct spec = im->enemyMin[side];
#ifdef FE_DEBUG
	DebugDrawer::getSingleton().drawCuboid(im->IMEnemy[side][spec.x][spec.y][spec.z].box.getAllCorners(), Ogre::ColourValue(1.0, 1.0, 1.0, 1.0), true);
#endif
}

void OgreGFX::UIMgr::drawMinsForSide(FastEcslent::Side side, Ogre::ColourValue col) {
	FastEcslent::D3CoordStruct spec = this->gfx->engine->IMMgr->enemyMin[side];
#ifdef FE_DEBUG
	DebugDrawer::getSingleton().drawCuboid(this->gfx->engine->IMMgr->IMEnemy[side][spec.x][spec.y][spec.z].box.getAllCorners(),
			col, true);
#endif
}

void OgreGFX::UIMgr::drawMoveLocations(){
	DebugDrawer::getSingletonPtr()->drawCircle(this->moveSurfaceLocation, 20, 16, Ogre::ColourValue(1.0, 0.0, 0.0, 1.0), true);
	DebugDrawer::getSingletonPtr()->drawLine(moveSurfaceLocation, move3DLocation, Ogre::ColourValue(1.0, 0.0, 0.0, 1.0));
}

bool OgreGFX::UIMgr::frameStarted(const Ogre::FrameEvent& evt){
	//DebugDrawTest();
	//drawSelectionCircles();
	decorateSelectedEntities();
	drawUnderMouseCircle();
	drawIMsForSide(FastEcslent::RED); //Also draws min
	drawIMsForSide(FastEcslent::BLUE);
	//drawMinsForSide(FastEcslent::RED, Ogre::ColourValue(1.0, 0.0, 0.0, 1.0));
	//drawMinsForSide(FastEcslent::BLUE, Ogre::ColourValue(0.0, 0.0, 1.0, 1.0));
	//drawIMs();
	if (commandingNow)
		drawMoveLocations();
	//int size = this->gfx->engine->options->size;
	//Ogre::AxisAlignedBox box (Ogre::AxisAlignedBox(Ogre::Vector3(-size/2, 0, -size/2), Ogre::Vector3(size/2, size/2, size/2)));
	//OgreGFX::DebugDrawer::getSingletonPtr()->drawCuboid(box.getAllCorners(), Ogre::ColourValue(0.5, 0.5, 0.5, 0.5), true);

	DebugDrawer::getSingletonPtr()->build(); //One build to draw all the debug objects

	//std::cout << renderWindow->getAverageFPS() << std::endl;
	return true;
}

bool OgreGFX::UIMgr::frameEnded(const Ogre::FrameEvent& evt){
	DebugDrawer::getSingletonPtr()->clear();
//	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
//		FastEcslent::Entity * ent = gfx->engine->entityMgr->ents[(*it)->id];
//		if (fabs(ent->attractivePotential) < 10 ) {
//			std::cout << "Potential vector length, attractive Potential: " << ent->potentialVec.length() << ", "
//					<< ent->attractivePotential << std::endl;
//		}
//	}
	return true;
}

bool mouseDidScroll(int z){
	return (z < -1.0 || z > 1.0);
}


bool OgreGFX::UIMgr::frameRenderingQueued(const Ogre::FrameEvent& evt){
    if(renderWindow->isClosed())
        return false;

    //Need to capture/update each device
    keyboard->capture();
    mouse->capture();

    cameraMgr->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera

    return true;
}



void OgreGFX::UIMgr::checkSetModifiers(const OIS::KeyEvent &arg, bool value){
	if (arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT){
		shiftDown = value;

	} else if (arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL){
		ctrlDown = value;

	} else if (arg.key == OIS::KC_LMENU || arg.key == OIS::KC_RMENU){
		altDown = value;
	}
}



bool OgreGFX::UIMgr::keyPressed( const OIS::KeyEvent &arg ){
	if (arg.key == OIS::KC_ESCAPE)  {
		shutDown = true;
		//return false;
	}
	if (arg.key == OIS::KC_F12){
		std::cout << cameraMgr->cameraNode->getPosition() << ", " << cameraMgr->cameraNode->getOrientation().getYaw() << std::endl;
		std::cout << cameraMgr->pitchNode->getOrientation().getPitch() << std::endl;
	}
	if (arg.key == OIS::KC_S)//"s" key for staring.
	{

	}
	std::cout << "Key pressed: " << arg.key << std::endl;
	checkSetModifiers(arg, true);

//	else if (arg.key == OIS::KC_F12) {
		//buildCubes();
	//}

	// Will need to check if camera related keys were pressed.
	// if a camera related key is pressed:
	cameraMgr->injectKeyDown(arg);
	// else if a registered key combo is pressed:
	//call key handler
	//else
	// do nothing

	callKeyHandlers(arg);
	std::cout << "Control down is: " << ctrlDown << std::endl;
	controlGroupsHandler->checkHandleControlGroupKeys(ctrlDown, arg.key);

	return true;
}

bool OgreGFX::UIMgr::keyReleased( const OIS::KeyEvent &arg ){
	checkSetModifiers(arg, false);
	cameraMgr->injectKeyUp(arg);
    return true;
}


//bool OgreGFX::UIMgr::mouseMoved( const OIS::MouseEvent &arg ){
//	if(selectingNow){
//		volumeSelectingNow = true;
//		ms = mouse->getMouseState();
//		stopPos.x = ms.X.abs/(float)ms.width;
//		stopPos.y = ms.Y.abs/(float)ms.height;
//		selectionBox->setCorners(startPos, stopPos);
//	} else if ( mouseDidScroll(arg.state.Z.rel) && ! arg.state.buttonDown(OIS::MB_Left) && !arg.state.buttonDown(OIS::MB_Right) && !arg.state.buttonDown(OIS::MB_Middle)){
//		cameraMgr->handleMouseScrollCameraZoom(arg);
//	}
//    return true;
//}

//-------------------------Manage Selection ---------------------------------------------
void OgreGFX::UIMgr::showCommands(FastEcslent::Entity *ent, Ogre::Vector3 pos){
	if(ent->entityState != FastEcslent::ALIVE) return;
	FastEcslent::UnitAI* ai = dynamic_cast<FastEcslent::UnitAI*>(ent->getAspect(FastEcslent::UNITAI));
	Ogre::Vector3 start = pos;
	Ogre::Vector3 tloc = pos;
	Ogre::ColourValue moveColor = Ogre::ColourValue(0.0, 0.5, 0.5, 0.5);
	boost::mutex::scoped_lock scoped_lock(ent->entLock);
	for(std::deque<FastEcslent::Command*>::iterator it = ai->commands.begin(); it != ai->commands.end(); ++it){
		if ((*it)->commandType == FastEcslent::MoveCommand){
			FastEcslent::PotentialMove *pm = dynamic_cast<FastEcslent::PotentialMove*>(*it);
			tloc = pm->target->location;
			DebugDrawer::getSingleton().drawLine(start, tloc, moveColor);
			DebugDrawer::getSingleton().drawSphere(tloc, 10, moveColor, true);
			DebugDrawer::getSingleton().drawLine(start, Ogre::Vector3(tloc.x, 0, tloc.z), moveColor);
			DebugDrawer::getSingleton().drawLine(tloc, Ogre::Vector3(tloc.x, 0, tloc.z), moveColor);
			start = tloc;
		} // else if commandType is Maintain
	}



}



void OgreGFX::UIMgr::decorateSelectedEntities(){
	Ogre::Vector3 pos;
	Ogre::Vector3 start;
	Ogre::Vector3 end;
	float selectionCircleRadius = 100.0f;
	FastEcslent::Entity* ent;
	int entId;
	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
		pos = (*it)->node->getPosition();
		Ogre::Vector3 cpos = Ogre::Vector3(pos.x, pos.y+1, pos.z);
		selectionCircleRadius = gfx->engine->entityMgr->ents[(*it)->id]->staticData.length;
		DebugDrawer::getSingleton().drawCircle(cpos, selectionCircleRadius, FastEcslent::NCircleSegments, Ogre::ColourValue(0.5f, 0.5f, 0.9f, 0.2f), true );

		entId = (*it)->id;
		ent = gfx->engine->entityMgr->ents[entId];
		start = ent->moveData.pos;

		if (ent->entityClass != FastEcslent::STATIC) {
			float lineLength = ent->staticData.length * (1.0 + (ent->moveData.speed/ent->staticData.speedRange));
			DebugDrawer::getSingleton().drawAngleLine(cpos,  Ogre::Radian(ent->moveData.heading), lineLength, Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0));
			lineLength = ent->staticData.length * (1.0 + ent->moveData.desiredSpeed/ent->staticData.speedRange);
			DebugDrawer::getSingleton().drawAngleLine(cpos,  Ogre::Radian(ent->moveData.desiredHeading), lineLength, Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.5));
		}
		Ogre::Vector3 leftPoint = Ogre::Vector3(cpos.x - FastEcslent::healthLineLength/2, cpos.y, cpos.z - ent->staticData.length);
		Ogre::Vector3 rightPoint = Ogre::Vector3(cpos.x + FastEcslent::healthLineLength/2, cpos.y, cpos.z - ent->staticData.length);
		DebugDrawer::getSingleton().drawLine(leftPoint, rightPoint, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 0.5f));
		//std::cout << "Hit points: " << ent->weapon->hitpoints << " Max Hit Points: " << gfx->engine->weaponMgr->maxHitpoints[ent->entityType] << std::endl;
		float healthNow =  FastEcslent::healthLineLength * ent->weaponInfo.hitpoints/ent->weaponInfo.maxHitpoints;
		rightPoint.x = leftPoint.x + healthNow;
		DebugDrawer::getSingleton().drawLine(leftPoint, rightPoint, Ogre::ColourValue(0.0f, 1.0f, 0.0f, 0.5f));

		showCommands(ent, pos);

		// Debug potential fields
		//DebugDrawer::getSingleton().drawAngleLine(cpos, Ogre::Radian(ent->potentialVec.angleBetween(Ogre::Vector3::UNIT_X)), ent->potentialVec.length() * 100, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0));
		//DebugDrawer::getSingleton().drawLine(cpos, cpos + (ent->potentialVec * 100), Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0));
	}

}


//void OgreGFX::UIMgr::drawSelectionCircles(){
//	Ogre::Vector3 pos;
//
//	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
//		pos = (*it)->node->getPosition();
//		Ogre::Vector3 cpos = Ogre::Vector3(pos.x, pos.y+1, pos.z);
//		//DebugDrawer::getSingleton().drawCircle((*it)->node->getPosition(), 50.0f, 8, Ogre::ColourValue(255.0f, 255.0f, 255.0f), true );
//		//DebugDrawer::getSingleton().drawCircle(cpos, 50.0f, 8, Ogre::ColourValue(255.0f, 255.0f, 255.0f), true );
//		float radius = gfx->engine->entityMgr->ents[(*it)->id]->length;
//		DebugDrawer::getSingleton().drawCircle(cpos, radius, FastEcslent::NCircleSegments, Ogre::ColourValue(0.0f, 1.0f, 0.0f, 0.5f), true );
//	}
//}

void OgreGFX::UIMgr::drawUnderMouseCircle(){
	Ogre::Vector3 pos;
	Ogre::Vector3 newPos;
	std::pair<int, Ogre::Vector3>  underMouse;
	float radius;
	int id;
	underMouse = entIndexUnderMouse();
	if (underMouse.first >= 0 && underMouse.first < gfx->nGFXNodes) {
		//std::cout << "Found an ent under mouse" << "\n";
		id = gfx->gfxNodes[underMouse.first].id;
		pos = gfx->engine->entityMgr->ents[id]->moveData.pos;
		newPos = Ogre::Vector3(pos.x, pos.y+1, pos.z);
		radius = gfx->engine->entityMgr->ents[id]->staticData.length;
		DebugDrawer::getSingleton().drawCircle(newPos, radius*1.5, FastEcslent::NCircleSegments, Ogre::ColourValue(0.0f, 0.0f, 1.0f, 1.0f), false );
	}
}

std::pair<int, Ogre::Vector3> OgreGFX::UIMgr::entIndexUnderMouse(){
	OIS::MouseState ms;
	ms = mouse->getMouseState();
	return entIndexUnderMouse(ms);
}

std::pair<int, Ogre::Vector3> OgreGFX::UIMgr::entIndexUnderMouse(const OIS::MouseEvent &arg){
	OIS::MouseState ms;
	ms = mouse->getMouseState();
	return entIndexUnderMouse(ms);
}

std::pair<int, Ogre::Vector3> OgreGFX::UIMgr::entIndexUnderMouse(const OIS::MouseState &arg){

	int index = -1;
	//Ogre::Vector3 pos = Ogre::Vector3::NEGATIVE_UNIT_Y;
	Ogre::Ray mouseRay = camera->getCameraToViewportRay(arg.X.abs/(float) ms.width, arg.Y.abs/(float)ms.height);
	//this->raySceneQuery->setRay(mouseRay);
	std::pair<bool, float> result = mouseRay.intersects(gfx->oceanSurface);
	if (result.first){
		this->posUnderMouse = mouseRay.getPoint(result.second);
		//this->posUnderMouse = pos;
		float minDistanceSquared = FLT_MAX;
		float distanceSquared;
		// find ent that is 1. Selectable or Actionable (things can be done to it) && 2. Within threshold distance && 3. Nearest to mouse cursor
		for(int i = 0; i < gfx->nGFXNodes; i++){
			if (gfx->gfxNodes[i].actionable || gfx->gfxNodes[i].selectable) { // Should probably be ising entity ids, not i
				distanceSquared = this->posUnderMouse.squaredDistance(gfx->gfxNodes[i].node->getPosition());
				if (distanceSquared < selectionDistanceSquaredThreshold){
					if (distanceSquared < minDistanceSquared){
						index = i;
						minDistanceSquared = distanceSquared;
					}
				}
			}
		}
	}
	return std::pair<int, Ogre::Vector3> (index, this->posUnderMouse);
}

void OgreGFX::UIMgr::selectEntity(int index){
	currentSelections.push_back(&gfx->gfxNodes[index]); // add node to empty or non-empty list
	//gfx->gfxNodes[index].node->showBoundingBox(true);   // and show boundingbox
	//gfx->engine->entityMgr->ens[gfxNode.id] is entity
}

void OgreGFX::UIMgr::handleSingleSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id ){

	if (volumeSelectingNow) return;
	std::pair<int, Ogre::Vector3> result = entIndexUnderMouse(arg);
	int index = result.first;
	// Found an ent or none
	if (index >= 0 && index < gfx->nGFXNodes){ //if something is near enough the cursor to be selected
		if (!shiftDown) { // if it is not to be added to the selection
			clearSelectionsAndUpdateFEEngine(); //clear currently selected
		}
		selectEntity(index);

	} else { //None: nothing close enough to be selected, so clear selections
		clearSelectionsAndUpdateFEEngine();
	}
	updateFEEngineWithSelections();
}



FastEcslent::Group* OgreGFX::UIMgr::groupFromSelections(){

	FastEcslent::Entity* ents[FastEcslent::MaxEnts];
	int i = 0;
	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
		ents[i++] = gfx->engine->entityMgr->ents[(*it)->id];
	}
	FastEcslent::Group* group = gfx->engine->groupMgr->createGroup(ents, i);

	//group->addMember(gfx->engine->entityMgr->ents[(*it)->id]);
	return group;
}

void OgreGFX::UIMgr::handleVolumeSelection(const OIS::MouseEvent &arg, OIS::MouseButtonID id ){

		startPos.x = arg.state.X.abs/(float) ms.width;
		startPos.y = arg.state.Y.abs/(float) ms.height;
		stopPos = startPos;
		selectingNow = true;
		selectionBox->clear();
		selectionBox->setVisible(true);
}

void OgreGFX::UIMgr::printCurrentSelections() {
	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
		std::cout << (*it)->id << ", ";
	}
	std::cout << std::endl;
//	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); it++){
//		std::cout << (*it)->id << ", ";
//	}
	std::cout << std::endl;
}

void OgreGFX::UIMgr::updateFEEngineWithSelections(){
	boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
		gfx->engine->selectionMgr->select((*it)->id);
	}
}


void OgreGFX::UIMgr::clearSelectionsAndUpdateFEEngine(){
	boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
	for(std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
		//boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
		gfx->engine->selectionMgr->unselect((*it)->id);
	}

	currentSelections.clear();
}

void swap(float &x, float &y){
	float tmp = x;
	x = y;
	y = tmp;
}

void OgreGFX::UIMgr::doVolumeSelection(const Ogre::Vector2& first, const Ogre::Vector2& second){

	float left = first.x, right = second.x, top = first.y, bottom = second.y;
	if(left > right) swap(left, right);
	if(top > bottom) swap(top, bottom);
	if( (right - left) * (bottom - top) < 0.01) return;

	Ogre::Ray topLeft     = camera->getCameraToViewportRay(left, top);
	Ogre::Ray topRight    = camera->getCameraToViewportRay(right, top);
	Ogre::Ray bottomLeft  = camera->getCameraToViewportRay(left, bottom);
	Ogre::Ray bottomRight = camera->getCameraToViewportRay(right, bottom);

	Ogre::PlaneBoundedVolume vol;
	vol.planes.push_back(Ogre::Plane(topLeft.getPoint(3),    topRight.getPoint(3),     bottomRight.getPoint(3))); //front (closest to cam) plane
	vol.planes.push_back(Ogre::Plane(topLeft.getOrigin(),    topLeft.getPoint(10),     topRight.getPoint(10))); //top plane
	vol.planes.push_back(Ogre::Plane(topLeft.getOrigin(),    bottomLeft.getPoint(10),  topLeft.getPoint(10))); //left plane
	vol.planes.push_back(Ogre::Plane(bottomLeft.getOrigin(), bottomRight.getPoint(10), bottomLeft.getPoint(10))); //bottom Plane
	vol.planes.push_back(Ogre::Plane(topRight.getOrigin(),   topRight.getPoint(10),    bottomRight.getPoint(10))); //right Plane

	Ogre::PlaneBoundedVolumeList volList;
	volList.push_back(vol);

	volQuery->setVolumes(volList);
	Ogre::SceneQueryResult result = volQuery->execute();
	clearSelectionsAndUpdateFEEngine();

	Ogre::SceneQueryResultMovableList::iterator iter; // for some reason the last sceneNode is always wrong
	int n = result.movables.size(); //have to throw away last item in iterator. I don't know why?
	//	int i = 0;                      //ToDo -> fix iterator, why is it n-1?
	// The issue is std::map with returns an iterator that points to the end of the map if the key is not in the map.
	//Causes all kinds of strange behavior
	std::cout << "Number of movables: " << n << std::endl;

	for(iter = result.movables.begin(); iter != result.movables.end(); ++iter){
	//for(iter = result.movables.begin(); iter != result.movables.end(); ++iter){
		//std::cout << gfx->sceneNodeToEntIdMap[(*iter)->getParentSceneNode()] << std::endl;
		if (gfx->sceneNodeToEntIdMap.find((*iter)->getParentSceneNode()) != gfx->sceneNodeToEntIdMap.end()) {
			selectEntity(gfx->sceneNodeToEntIdMap[(*iter)->getParentSceneNode()]); //why parent scene node?
		}
	}
	updateFEEngineWithSelections();
}
//-------------------------End Manage Selection ---------------------------------------------

//------------------------Handle Commands------------------------------------------------
void OgreGFX::UIMgr::CommandMaintainAttack(int i){
	std::cout << "MaintainAttacking: (" << gfx->engine->entityMgr->ents[i]->uiname << ")" << std::endl;
	FastEcslent::Entity *ent = gfx->engine->entityMgr->ents[i];
	switch(ent->entityType){
	case FastEcslent::MINERALS:
		std::cout << "Mining minerals" << std::endl;
		gfx->engine->gameMgr->getSelectedEntsToMining(dynamic_cast<FastEcslent::Minerals *>(ent), shiftDown);
		break;
	default:
		std::cout << "Don't know what to do to: " << ent->uiname << std::endl;
		break;

	}



}

void OgreGFX::UIMgr::CommandPotentialFieldsMove(Ogre::Vector3 pos){
	//std::cout << "Potential Fields: Moving to: (" << pos.x << ", " << pos.z << ")" << std::endl;
	FastEcslent::Entity * ent;
	float height = pos.y; // remember y value for air ents
	for (std::list<OgreGFX::GFXNode*>::iterator it = currentSelections.begin(); it != currentSelections.end(); ++it){
		ent = gfx->engine->entityMgr->ents[(*it)->id];
		if (ent->entityClass == FastEcslent::STATIC) {
			continue;
		}

		if(ent->entityClass == FastEcslent::SURFACE)
			pos.y = 0; //surface ents only move on the surface with y = 0
		else if (ent->entityClass == FastEcslent::AIR)
			pos.y = height; //air ents retain the remembered y value

		//ent->desiredAltitude = pos.y;
		if(shiftDown){
			FastEcslent::addPotentialMoveForEnt(ent, pos);
		} else {
			FastEcslent::setPotentialMoveForEnt(ent, pos);
		}
	}
}

void OgreGFX::UIMgr::CommandMove(Ogre::Vector3 pos){
	//std::cout << "Moving to: (" << pos.x << ", " << pos.z << ")" << std::endl;

	if(currentSelections.size() <= 0 ) return;

	if (currentSelections.size() > 1) {
		FastEcslent::Group* group = groupFromSelections();
		for(int i = 0; i < group->nEntitiesInGroup; i++){
			group->members[i]->ai->commands.clear();
		}
		group->setLeaderByIndex(0);
		FastEcslent::GroupAI* gai = new FastEcslent::GroupAI(group, FastEcslent::GROUPAI);
		FastEcslent::GroupTarget* gt = new FastEcslent::GroupTarget;
		gt->target.location = pos;
		//FastEcslent::Flock* flock = new FastEcslent::Flock(group, gt);
		//flock->changeLeadership(FastEcslent::MostMassive);
		std::cout << "Left Shift: " << shiftDown << std::endl;
		if(shiftDown) {
			std::cout << "Adding flock" << std::endl;
			FastEcslent::addMoveForEnt(gfx->engine->entityMgr->ents[currentSelections.front()->id], pos);//commandHelper
			//gai->addCommand(flock);
		} else {
			FastEcslent::setMoveForEnt(gfx->engine->entityMgr->ents[currentSelections.front()->id], pos);//commandHelper
			//gai->setCommand(flock);
		}
		group->addAspect(gai);
	} else {
		FastEcslent::Entity * ent = gfx->engine->entityMgr->ents[currentSelections.front()->id];
		boost::mutex::scoped_lock scoped_lock(selectionUpdateLock);
		if(shiftDown) {
			//FastEcslent::addMoveForEnt(ent, pos);
			FastEcslent::addPotentialMoveForEnt(ent, pos);
		} else {
			//FastEcslent::setMoveForEnt(ent, pos);
			FastEcslent::setPotentialMoveForEnt(ent, pos);
		}
	}
}

void OgreGFX::UIMgr::handleCommand(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	std::pair<int, Ogre::Vector3> result = entIndexUnderMouse(arg);
	int index = result.first;

	if (index >= 0 && index < gfx->nGFXNodes){ //do_maintain()
		CommandMaintainAttack(index);
	} else {
		CommandPotentialFieldsMove(this->move3DLocation);
	}

}
//------------------------Handle Mouse Commands------------------------------------------------

bool OgreGFX::UIMgr::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	//std::cout << "Input System: Mouse Pressed (" << arg.state.X.abs << ", " << arg.state.Y.abs << ")" << std::endl;
	if (id == OIS::MB_Left) { //select
		handleSingleSelection(arg, id);
		handleVolumeSelection(arg, id);
	} else if (id == OIS::MB_Right) {
		//std::cout << "Input System: Right Mouse Pressed (" << arg.state.X.abs << ", " << arg.state.Y.abs << ")" << std::endl;
		commandingNow = true;
		std::pair<int, Ogre::Vector3> result = entIndexUnderMouse(arg);
		moveSurfaceLocation = result.second;
		move3DLocation = moveSurfaceLocation;
		ms = mouse->getMouseState();
		move3DStartY = ms.Y.abs/(float)ms.height;
		//OgreGFX::DebugDrawer::getSingleton().drawCircle(result.second, 20, 16, Ogre::ColourValue(1.0, 0.0, 0.0, 1.0), true);
		//handleCommand(arg, id);
	}
	callMouseHandlers(arg, id);
	return true;
}

float delta;
bool OgreGFX::UIMgr::mouseMoved( const OIS::MouseEvent &arg ){
	if(selectingNow){
		volumeSelectingNow = true;
		ms = mouse->getMouseState();
		stopPos.x = ms.X.abs/(float)ms.width;
		stopPos.y = ms.Y.abs/(float)ms.height;
		selectionBox->setCorners(startPos, stopPos);
	} else if (commandingNow){
		ms = mouse->getMouseState();
		move3DLocation = moveSurfaceLocation;
		delta = ((ms.Y.abs/(float)ms.height) - move3DStartY);
		move3DLocation.y += (delta < 0 ? -delta * ms.height * 2 : 0);

		//OgreGFX::DebugDrawer::getSingletonPtr()->drawLine(moveSurfaceLocation, move3DLocation, Ogre::ColourValue(1.0, 0.0, 0.0, 1.0));
	} else if ( mouseDidScroll(arg.state.Z.rel) && ! arg.state.buttonDown(OIS::MB_Left) && !arg.state.buttonDown(OIS::MB_Right) && !arg.state.buttonDown(OIS::MB_Middle)){
		cameraMgr->handleMouseScrollCameraZoom(arg);
	}
    return true;
}

bool OgreGFX::UIMgr::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
	if (id == OIS::MB_Left){
		doVolumeSelection(startPos, stopPos);
		selectingNow = false;
		volumeSelectingNow = false;
		selectionBox->setVisible(false);
	} else if (id == OIS::MB_Right){
		handleCommand(arg, id);
		commandingNow = false;
	}
	callMouseHandlers(arg, id);
    return true;
}




//----------------------------------------Handle key registration-----------------------------------


bool OgreGFX::UIMgr::registerKeyHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::KeyCode key,  OgreGFX::KeyHandler *handler){
	OgreGFX::KeySet keySet = OgreGFX::KeySet(modifiers, key);
	std::string keySetString = keySet.toString();//convertKeySetToString(keySet);
	//keyHandlers[keySetString] = handler; // Map should make it easier to select handler to run
	keyHandlers[keySetString].push_back(handler); // Map should make it easier to select handler to run
	return true;
}

bool OgreGFX::UIMgr::registerMouseHandler(const std::set<OIS::KeyCode> *modifiers, const OIS::MouseButtonID buttonId, OgreGFX::MouseHandler *handler){
	OgreGFX::MouseButtonKeySet mbKeySet = OgreGFX::MouseButtonKeySet(modifiers, buttonId);
	std::string mouseButtonKeySetString = mbKeySet.toString();//convertMouseButtonKeySetToString(mbKeySet);
	//mouseHandlers[mouseButtonKeySetString] = handler;
	mouseHandlers[mouseButtonKeySetString].push_back(handler);
	return true;
}

// --------------------------------------Call key/mouse Handlers-------------------------------------------

std::set<OIS::KeyCode> *OgreGFX::UIMgr::makeModifiersSet(){
	std::set<OIS::KeyCode> *mods = new std::set<OIS::KeyCode>();
	if (shiftDown)
		mods->insert(OIS::KC_LSHIFT);
	if (altDown)
		mods->insert(OIS::KC_LMENU);
	if (ctrlDown)
		mods->insert(OIS::KC_LCONTROL);
	return mods;
}

std::string OgreGFX::UIMgr::modifierKeyHash(const OIS::KeyCode keyCode){

	std::set<OIS::KeyCode> *mods = makeModifiersSet();
	OgreGFX::KeySet ks = OgreGFX::KeySet(mods, keyCode);
	return ks.toString();
}
std::string OgreGFX::UIMgr::modifierKeyMouseHash(const OIS::MouseButtonID id){

	std::set<OIS::KeyCode> *mods = makeModifiersSet();
	OgreGFX::MouseButtonKeySet mbks = OgreGFX::MouseButtonKeySet(mods, id);
	return mbks.toString();
}

bool OgreGFX::UIMgr::callKeyHandlers(const OIS::KeyEvent &arg){

	std::string keySetHash = modifierKeyHash(arg.key);
	if (this->keyHandlers.count(keySetHash) > 0){
		for(std::list<KeyHandler *>::iterator khi = this->keyHandlers[keySetHash].begin(); khi != this->keyHandlers[keySetHash].end(); ++khi){
			(*khi)->handleKeyEvent();
		}
		//this->keyHandlers[keySetHash]->handleKeyEvent();
		return true;
	}
	return false;
}

bool OgreGFX::UIMgr::callMouseHandlers(const OIS::MouseEvent &arg, const OIS::MouseButtonID id){

	std::string mouseButtonKeySetHash = modifierKeyMouseHash(id);
	if (this->mouseHandlers.count(mouseButtonKeySetHash) > 0){
		for(std::list<MouseHandler *>::iterator mhi = this->mouseHandlers[mouseButtonKeySetHash].begin(); mhi != this->mouseHandlers[mouseButtonKeySetHash].end(); ++mhi){
			(*mhi)->handleMouseEvent(arg);
		}
		//this->mouseHandlers[mouseButtonKeySetHash]->handleMouseEvent(arg);
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------

//void OgreGFX::UIMgr::createNewEntity(FastEcslent::Identity id){
//
//}

//--------------------------------------------------------------------------------------------------

//Adjust mouse clipping area
void OgreGFX::UIMgr::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

OgreGFX::UIMgr::~UIMgr(){

    kill();
}

void OgreGFX::UIMgr::kill(){
	//sceneManager->destroyQuery(volQuery);



	if(selectionBox) {
		//delete selectionBox;
	}

	delete DebugDrawer::getSingletonPtr();

	if(cameraMgr) {
		//delete cameraMgr;
		cameraMgr = 0;
	}
    if( inputManager ) {
        inputManager->destroyInputObject( mouse );
        inputManager->destroyInputObject( keyboard );

        OIS::InputManager::destroyInputSystem(inputManager);
        inputManager = 0;
    }
    if(renderWindow) {
    	Ogre::WindowEventUtilities::removeWindowEventListener(renderWindow, this);
    	//if(renderWindow) windowClosed(renderWindow);
    }


}

void OgreGFX::UIMgr::windowClosed(Ogre::RenderWindow* rw){
    //Only close for window that created OIS (the main window in these demos)
    if( rw == renderWindow ) {
    	kill();
    }
}

