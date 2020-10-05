/*
 * cameraMgr.cpp
 *
 *  Created on: Feb 19, 2012
 *      Author: sushil
 */

#include <OgreRoot.h>

#include <cameraMgr.h>
#include <GraphicsInteractionManager.h>


//#include <gfxMgr.h>

OgreGFX::CameraMgr::CameraMgr(GraphicsInteractionManager *gim): GFXMgr(gim) { //Engine *eng, Ogre::SceneManager *sm, Ogre::Camera* cam) : Mgr(eng) {
//	gfx = gim;
	camera = gim->mCamera;
	orbiting = false;
	zooming  = false;
	topSpeed    = 2000;
	speedUp     = 5.0f;
	topRotation = 0.002;
	forward  = false;
	back     = false;
	left     = false;
	right    = false;
	up       = false;
	down     = false;
	yawLeft  = false;
	yawRight = false;
	pitchUp  = false;
	pitchDown = false;

	fastMove = false;
	//camera->setPosition(0,0,0);
	//camera->setOrientation(Ogre::Quaternion::ZERO);
	sceneManager = gim->mSceneMgr;
	int size = gim->engine->options->mapSize;
	cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("cameraNode", Ogre::Vector3(-1200, 900, 1900));
	cameraNode->yaw(Ogre::Radian(-0.5));
	pitchNode    = cameraNode->createChildSceneNode("pitchNode");
	//yawNode  = yawNode->createChildSceneNode("pitchNode");
	pitchNode->attachObject(camera);
	pitchNode->pitch(Ogre::Radian(-0.5));
	//pitchNode->lookAt(Ogre::Vector3(0,0,0), Ogre::Node::TS_WORLD, Ogre::Vector3(0,0,0));

	mouseScrollTranslation    = Ogre::Vector3::ZERO;
	mouseScrollTranslationSrc = Ogre::Vector3::ZERO;
	zeroTranslation           = Ogre::Vector3::ZERO;
	lerpTime                  = 0.0f;
	timeToStop                = 1.0f;
	mouseScrollSpeed          = 10;
	cameraMaxHeight           = size;

}

//void OgreGFX::CameraMgr::init(){
//
//}


Ogre::Vector3 mylerp(Ogre::Vector3 &src, Ogre::Vector3 &dest, float fraction) {

	return src + (dest - src)*fraction;

}

void OgreGFX::CameraMgr::injectKeyDown(const OIS::KeyEvent& evt)
{

	if (evt.key == OIS::KC_W ) forward = true;
	else if (evt.key == OIS::KC_S) back = true;
	else if (evt.key == OIS::KC_A) left = true;
	else if (evt.key == OIS::KC_D) right = true;
	else if (evt.key == OIS::KC_PGUP) up = true;
	else if (evt.key == OIS::KC_PGDOWN) down = true;
	else if (evt.key == OIS::KC_LSHIFT) fastMove = true;

	else if (evt.key == OIS::KC_Q) yawLeft = true;
	else if (evt.key == OIS::KC_E) yawRight = true;
	else if (evt.key == OIS::KC_Z) pitchUp = true;
	else if (evt.key == OIS::KC_X) pitchDown = true;

}
void OgreGFX::CameraMgr::injectKeyUp(const OIS::KeyEvent& evt){

	if (evt.key == OIS::KC_W ) forward = false;
	else if (evt.key == OIS::KC_S) back = false;
	else if (evt.key == OIS::KC_A) left = false;
	else if (evt.key == OIS::KC_D) right = false;
	else if (evt.key == OIS::KC_PGUP) up = false;
	else if (evt.key == OIS::KC_PGDOWN) down = false;
	else if (evt.key == OIS::KC_LSHIFT) fastMove = false;

	else if (evt.key == OIS::KC_Q) yawLeft = false;
	else if (evt.key == OIS::KC_E) yawRight = false;
	else if (evt.key == OIS::KC_Z) pitchUp = false;
	else if (evt.key == OIS::KC_X) pitchDown = false;


}

void OgreGFX::CameraMgr::handleMouseScrollCameraZoom(const OIS::MouseEvent &e){
	mouseScrollTranslationSrc.y = - mouseScrollSpeed * e.state.Z.rel;
	lerpTime = 0.0f;
}


bool OgreGFX::CameraMgr::frameRenderingQueued(const Ogre::FrameEvent& evt){
	float speed = 0.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float rot;
	Ogre::Vector3 translation = Ogre::Vector3::ZERO;

	speed = (fastMove ? topSpeed * speedUp: topSpeed);

	if (forward) translation.z = -speed;
	if (back)    translation.z =  speed;
	if (right)   translation.x =  speed;
	if (left)    translation.x = -speed;
	if (up)      translation.y =  speed;
	if (down)    translation.y = -speed;

	cameraNode->translate(cameraNode->getOrientation() * translation * evt.timeSinceLastFrame);

	if(lerpTime < timeToStop) {
		lerpTime += evt.timeSinceLastFrame;
		mouseScrollTranslation = mylerp(mouseScrollTranslationSrc, zeroTranslation, lerpTime/timeToStop);
		cameraNode->translate(cameraNode->getOrientation() * mouseScrollTranslation * evt.timeSinceLastFrame);
	}

	Ogre::Vector3 camp = cameraNode->getPosition();
	if (camp.y < 5){
		camp.y = 5;
		cameraNode->setPosition(camp);
	}
	if(camp.y > cameraMaxHeight){
		camp.y = cameraMaxHeight;
		cameraNode->setPosition(camp);
	}


	rot = (fastMove ? topRotation * 10: topRotation);

	if (yawLeft)  yaw = rot;
	if (yawRight) yaw = -rot;
	if (pitchUp)  pitch = rot;
	if (pitchDown)pitch = -rot;

	cameraNode->yaw(Ogre::Radian(yaw));
	pitchNode->pitch(Ogre::Radian(pitch));



  	return true;



}
