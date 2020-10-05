/*
 * gfxNode.h
 *
 *  Created on: Feb 20, 2012
 *      Author: sushil
 */

#ifndef GFXNODE_H_
#define GFXNODE_H_

#include <OgreSceneNode.h>

namespace OgreGFX {

	typedef struct {
		Ogre::SceneNode* node;
		bool  selectable;
		bool  actionable;
		int   id;
	} GFXNode;

}


#endif /* GFXNODE_H_ */
