/*
 * gameMgr.h
 *
 *  Created on: Jan 9, 2012
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_
#include <const.h>
#include <enums.h>

#include <ent.h>
#include <buildings.h>

namespace FastEcslent {

	typedef struct { //SC2
		int gas;
		int minerals;
	} Resources;

	typedef struct { //SC2
		int minerals;
		int gas;
		int supply; //pop
		int buildTime;
		int HotKey;
		int transportSize;
		int sightRange;


		int hitPoints;
		float armor;
		float damage;
		int   targetPriority;
		float cooldown;
		float range;


		float speed;
		float acc;
		float dec;
		float rotSpeed;
		float collisionRadius;

		EntityType neededTypes[MaxNeededTypes]; //for (int i=0; i < nNeededTypes; i++) needTypes[i] = BARRACKS;
		int nNeededTypes; // if nNeededTypes == 0 , no prereqs

	} GameEntTypeProperties;


	class Engine;

	class GameMgr : public Mgr {

	private:
		//int gameNumber;


	public:

		Options *options;
		//game
		int popCap[NPLAYERS];
		int pop[NPLAYERS];
		int currentPopCap[NPLAYERS];
		int currentEntityCounts[NPLAYERS][NENTITYTYPES];
		Entity *playerEnts[NPLAYERS][PerPlayerMaxEnts];
		int     playerNEnts[NPLAYERS];

		GameEntTypeProperties entTypeData[NENTITYTYPES];
		Resources resources[NPLAYERS];

		Side   mySide;
		Player myPlayer;
		Entity *ent;


		GameMgr();
		GameMgr(Engine *eng, Options *opts);

		~GameMgr(){}

		void init();
		void tick(double dtime);
		void cleanup();

		bool preReqExists(EntityType entType);
		bool notExceedPopCap(Identity entId);
		void setupEntityBuildTimes();
		void setupEntitySupply();

		//FastWater
		void WaterCraft();
		void makeBaseForSidePlayer(Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset);
		void createNEntitiesRadial(EntityType entType, int n, Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset, int yawOffsetMultiplier = 1);
		void getAllMyEntsToMining(Side side, Player player);
		void getSelectedEntsToMining(Minerals *minerals, bool shiftDown);
		void getSCVToMine(Side side, Player player, SCVehicle *scv, Minerals *mineral, bool addOrSet);

		void getEntToMoveOrAttack(Side mySide, Player myPlayer, Entity *ent, Entity *targetEnt, bool addCommand);


		Entity* findClosestEntityOfTypeWithinDistance(EntityType entityType, Ogre::Vector3 pos, float distance, Side side, Player player);


	//games
		void game0();
		void tester();
		void worldLimits();
		void SC3D();
		void TwoClumps(EntityType t1, Side side1, Player player1, MicroType m1, int nt1,
				EntityType t2, Side side2, Player player2, MicroType m2, int nt2);

		void ClumpSpread(EntityType t1, Side side1, Player player1, MicroType m1, int nt1,
				EntityType t2, Side side2, Player player2, MicroType m2, int nt2);
		Entity* Clump(Ogre::Vector3 center, int nEnts, int limit, EntityType type1, Side side1,  Player pl, MicroType m);
		Entity* Spread(Ogre::Vector3 center, int nEnts, int limit, EntityType type1, Side side2,  Player pl, MicroType m);

		//void OpponentMicro(Entity *ent, std::string chrom);

	};

}


#endif /* GAMEMGR_H_ */
