/*
 * terrain.h
 *
 *  Created on: Jan 21, 2016
 *      Author: sushil
 */

#ifndef INCLUDES_GFX_TERRAIN_H_
#define INCLUDES_GFX_TERRAIN_H_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <SdkTrays.h>

namespace OgreGFX {

	class GraphicsInteractionManager;

	class Terrain {

	public:
		Terrain(GraphicsInteractionManager *gimPtr);
		~Terrain();
		void initialize();
		void enable();
		void destroyTerrainResources();

		GraphicsInteractionManager* gimPtr;


	private:
		void defineTerrain(long x, long y);
		void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
		void initBlendMaps(Ogre::Terrain *terrain);
		void configureTerrainDefaults(Ogre::Light *light);

		bool mTerrainsImported; //?
		Ogre::TerrainGroup *mTerrainGroup;
		Ogre::TerrainGlobalOptions* mTerrainGlobals;

		OgreBites::Label *mInfoLabel;


	};

}
#endif /* INCLUDES_GFX_TERRAIN_H_ */
