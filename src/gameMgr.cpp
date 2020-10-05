/*
 * gameMgr.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: sushil
 */
#include <assert.h>

#include <const.h>
#include <utils.h>
#include <engine.h>
#include <ent.h>
#include <aspect.h>
#include <gather.h>
#include <commandHelp.h>
#include <unitAI.h>


#include <groupAI.h>
#include <flock.h>
#include <micro.h>

#include <enums.h>
#include <gameMgr.h>

#include <OgreVector3.h>
#include <cfloat>


//FastEcslent::GameMgr::GameMgr() {
//	gameNumber = 0;
	//reset();
//}

FastEcslent::GameMgr::GameMgr(Engine* engine, Options *opts): Mgr(engine) {
	//reset();
	options = opts;
	mySide = BATTLEMASTER;
	this->myPlayer = ONE;

}

void FastEcslent::GameMgr::init() {
	switch (options->gameNumber) {
	case 0:
		//ClumpSpread(BANSHEE, BLUE, TWO, ADAPTIVE, 3, ZEALOT, RED, ONE, BESTZEALOT, 25);
		//ClumpSpread(BANSHEE, BLUE, TWO, ADAPTIVE, 3, ZEALOT, RED, ONE, OPPONENT_FROM_CHROMOSOME, 25);

		//ClumpSpread(ZEALOT, BLUE, TWO, ADAPTIVE, 25, BANSHEE, RED, ONE, BESTVULTURE, 3);
		ClumpSpread(ZEALOT, BLUE, TWO, ADAPTIVE, 25, BANSHEE, RED, ONE, OPPONENT_FROM_CHROMOSOME, 3);
		break;
	case 1:
		//TwoClumps(BANSHEE, BLUE, TWO, ADAPTIVE, 3, ZEALOT, RED, ONE, BESTZEALOT, 25); //tightly grouped);
		//TwoClumps(BANSHEE, BLUE, TWO, ADAPTIVE, 3, ZEALOT, RED, ONE, OPPONENT_FROM_CHROMOSOME, 25);

		//TwoClumps(ZEALOT, BLUE, TWO, ADAPTIVE, 25, BANSHEE, RED, ONE, BESTVULTURE, 3); //tightly grouped);
		TwoClumps(ZEALOT, BLUE, TWO, ADAPTIVE, 25, BANSHEE, RED, ONE, OPPONENT_FROM_CHROMOSOME, 3);
		break;
	case 2:
		//ClumpSpread(ZEALOT, RED, ONE, BESTZEALOT, 25, BANSHEE, BLUE, TWO, ADAPTIVE, 3);
		//ClumpSpread(ZEALOT, RED, ONE, OPPONENT_FROM_CHROMOSOME, 25, BANSHEE, BLUE, TWO, ADAPTIVE, 3);

		//ClumpSpread(BANSHEE, RED, ONE, BESTVULTURE, 3, ZEALOT, BLUE, TWO, ADAPTIVE, 25);
		ClumpSpread(BANSHEE, RED, ONE, OPPONENT_FROM_CHROMOSOME, 3, ZEALOT, BLUE, TWO, ADAPTIVE, 25);
		break;
	case 3:
		worldLimits();
		break;
	case 4:
		game0();
		break;
	case 5:
		WaterCraft();
		break;
	case 6:
		tester();
		break;
	default:
		tester();
	}

}



//void FastEcslent::GameMgr::worldLimits(){
//
//	Entity *ent;
//	//Ogre::AxisAlignedBox box (Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(size, size, size)));
//	//OgreGFX::DebugDrawer::getSingletonPtr()->drawCuboid(box.getAllCorners(), Ogre::ColourValue(0.5, 0.5, 0.5, 0.2), true);
//	float xoff = 1000, zoff = 1000;
//	for (int i = -5; i < 5; i++){
//		for(int j = -5; j < 5; j++){
//			//ent = engine->entityMgr->createEntityAfterTime(BANSHEE, Ogre::Vector3(xoff + j * 20, 0, zoff + i * 20), 0.0f);
//			ent = engine->entityMgr->createEntityForPlayerAndSide(BANSHEE, Ogre::Vector3(xoff + j * 20, 0, zoff + i * 20), 0.0f, RED, ONE);
//			ent->init();
//			ent->switchState(ALIVE);
//			//ent = engine->entityMgr->createEntityAfterTime(ZEALOT, Ogre::Vector3(-xoff + j * 20, 0, -zoff + i * 20), 0.0f);
//			ent = engine->entityMgr->createEntityForPlayerAndSide(ZEALOT, Ogre::Vector3(-xoff + j * 20, 0, -zoff + i * 20), 0.0f, BLUE, TWO);
//			ent->init();
//			ent->switchState(ALIVE);
//		}
//	}
//
//	return;
//}

//build units 5*5*5

void FastEcslent::GameMgr::worldLimits(){

	Entity *ent;
	//Ogre::AxisAlignedBox box (Ogre::AxisAlignedBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(size, size, size)));
	//OgreGFX::DebugDrawer::getSingletonPtr()->drawCuboid(box.getAllCorners(), Ogre::ColourValue(0.5, 0.5, 0.5, 0.2), true);
	float xoff = 1000, zoff = 1000;
	int n=5;
//
//	ent = engine->entityMgr->createEntityForPlayerAndSide(BANSHEE, Ogre::Vector3(xoff, 0, zoff), 0.0f, RED, ONE);
//	ent->init();
//	ent->switchState(ALIVE);

				ent = engine->entityMgr->createEntityForPlayerAndSide(ZEALOT, Ogre::Vector3(-xoff, 0, -zoff), 0.0f, BLUE, TWO);
				ent->init();
				ent->switchState(ALIVE);
//
//	ent = engine->entityMgr->createEntityForPlayerAndSide(BANSHEE, Ogre::Vector3(xoff+50, 0, zoff+50), 0.0f, RED, ONE);
//	ent->init();
//	ent->switchState(ALIVE);
//
//	ent = engine->entityMgr->createEntityForPlayerAndSide(ZEALOT, Ogre::Vector3(-xoff, 0, -zoff), 0.0f, BLUE, TWO);
//	ent->init();
//	ent->switchState(ALIVE);


	return;
}

void FixedMicro(FastEcslent::Entity * ent){
	ent->friends.attractCoeff = 500;
	ent->friends.attractExponent = 2;
	ent->friends.repelCoeff = 10000;
	ent->friends.repelExponent = 2;

	ent->enemies.attractCoeff = 8000;
	ent->enemies.attractExponent = 0.5;
	ent->enemies.repelCoeff = 800;
	ent->enemies.repelExponent = 2;

	ent->target.attractCoeff = 2500;
	ent->target.attractExponent = 1.0;
	//ent->target.attractCoeff = 1.0;

	//------------------------------------------------------------------------------
	ent->friends.damageAttractCoeff    = 0;
	ent->friends.damageAttractExponent = 0;
	ent->friends.damageRepelCoeff      = 0;
	ent->friends.damageRepelExponent   = 0;

	ent->enemies.damageAttractCoeff    = 0;
	ent->enemies.damageAttractExponent = 0;
	ent->enemies.damageRepelCoeff      = 0;
	ent->enemies.damageRepelExponent   = 0;

	//------------------------------------------------------------------------------
	ent->friends.cooldownAttractCoeff    = 0;
	ent->friends.cooldownAttractExponent = 0;
	ent->friends.cooldownRepelCoeff      = 0;
	ent->friends.cooldownRepelExponent   = 0;

	ent->enemies.cooldownAttractCoeff    = 0;
	ent->enemies.cooldownAttractExponent = 0;
	ent->enemies.cooldownRepelCoeff      = 0;
	ent->enemies.cooldownRepelExponent   = 0;

	ent->IMForEnemies.range         = 2;
	ent->IMForEnemies.deltaFraction = 0.5;
	ent->IMForEnemies.w1            = 1.0;
	ent->IMForEnemies.w2            = 1.0;
	ent->IMForEnemies.w3            = 10;

//	ent->common.distanceWeight = 1.0;
//	ent->common.hitpointsWeight = 0.0;

}

void ZealotBestEvolvedMicro(FastEcslent::Entity * ent){
	ent->friends.attractCoeff = -6845.7;
	ent->friends.attractExponent = 5.19;
	ent->friends.repelCoeff = -5083.01;
	ent->friends.repelExponent = 5.19;

	ent->enemies.attractCoeff = 703.12;
	ent->enemies.attractExponent = 5.19;
	ent->enemies.repelCoeff = 5209.96;
	ent->enemies.repelExponent = -2.31;

	ent->target.attractCoeff = -7192.38;
	ent->target.attractExponent = 5.19;
	//ent->target.attractCoeff = 1.0;

	//------------------------------------------------------------------------------
	ent->friends.damageAttractCoeff    = -2900.39;
	ent->friends.damageAttractExponent = 7.06;
	ent->friends.damageRepelCoeff      = -2119.14;
	ent->friends.damageRepelExponent   = 7.06;

	ent->enemies.damageAttractCoeff    = -1240.23;
	ent->enemies.damageAttractExponent = 3.31;
	ent->enemies.damageRepelCoeff      = -576.17;
	ent->enemies.damageRepelExponent   = 0.5;

	//------------------------------------------------------------------------------
	ent->friends.cooldownAttractCoeff    = 8964.84;
	ent->friends.cooldownAttractExponent = 7.06;
	ent->friends.cooldownRepelCoeff      = -732.42;
	ent->friends.cooldownRepelExponent   = 5.19;

	ent->enemies.cooldownAttractCoeff    = -2822.27;
	ent->enemies.cooldownAttractExponent = 2.38;
	ent->enemies.cooldownRepelCoeff      = -8989.26;
	ent->enemies.cooldownRepelExponent   = -7.0;

	ent->IMForEnemies.range         = 4.00;
	ent->IMForEnemies.deltaFraction = 0.5;
	ent->IMForEnemies.w1            = 0.69;
	ent->IMForEnemies.w2            = 0.69;
	ent->IMForEnemies.w3            = 1.0;

//	ent->common.distanceWeight = 1.0;
//	ent->common.hitpointsWeight = 0.0;

}


void VultureBestEvolvedMicro(FastEcslent::Entity * ent){
	ent->friends.attractCoeff = 4892.58;
	ent->friends.attractExponent = 1.44;
	ent->friends.repelCoeff = 5976.56;
	ent->friends.repelExponent = 2.38;

	ent->enemies.attractCoeff = -2177.73;
	ent->enemies.attractExponent = -5.12;
	ent->enemies.repelCoeff = 8686.52;
	ent->enemies.repelExponent = 5.19;

	ent->target.attractCoeff = -8359.38;
	ent->target.attractExponent = -0.44;
	//ent->target.attractCoeff = 1.0;

	//------------------------------------------------------------------------------
	ent->friends.damageAttractCoeff    = 6547.85;
	ent->friends.damageAttractExponent = -5.12;
	ent->friends.damageRepelCoeff      = 1562.50;
	ent->friends.damageRepelExponent   = 1.44;

	ent->enemies.damageAttractCoeff    = 8486.33;
	ent->enemies.damageAttractExponent = 6.12;
	ent->enemies.damageRepelCoeff      = 8310.55;
	ent->enemies.damageRepelExponent   = 6.12;

	//------------------------------------------------------------------------------
	ent->friends.cooldownAttractCoeff    = 9511.72;
	ent->friends.cooldownAttractExponent = -7.00;
	ent->friends.cooldownRepelCoeff      = -6889.65;
	ent->friends.cooldownRepelExponent   = -6.06;

	ent->enemies.cooldownAttractCoeff    = -7954.10;
	ent->enemies.cooldownAttractExponent = 7.06;
	ent->enemies.cooldownRepelCoeff      = -7919.92;
	ent->enemies.cooldownRepelExponent   = 0.5;

	ent->IMForEnemies.range         = 0.0;;
	ent->IMForEnemies.deltaFraction = 0.25;
	ent->IMForEnemies.w1            = 0.12;
	ent->IMForEnemies.w2            = 0.56;
	ent->IMForEnemies.w3            = 3.00;

//	ent->common.distanceWeight = 1.0;
//	ent->common.hitpointsWeight = 0.0;

}



float decode(const char* chrom, int start, int end, float min, float max){
	int sum = 0;
	for(int i = 0; i < end - start; i++){
		sum += (chrom[start + i] == '0' ? 0 : pow(2.0, (double) i));
	}
	float precision = (max - min)/pow(2.0, (double)(end-start));
	return min + (precision * sum);
}



void makeParamString(char *out, FastEcslent::Entity *ent){
	std::string formatString =
			std::string("Distance: Friends: attract(%9.2f, %9.2f), repel(%9.2f, %9.2f) \tEnemies: attract(%9.2f, %9.2f), repel(%9.2f, %9.2f)\n") +
			std::string("Target Attract   : (coeff, expt) = (%9.2f, %9.2f)\n") +
			std::string("Damage:   Friends: attract(%9.2f, %9.2f), repel(%9.2f, %9.2f) \tEnemies: attract(%9.2f, %9.2f), repel(%9.2f, %9.2f)\n") +
			std::string("Cooldown: Friends: attract(%9.2f, %9.2f), repel(%9.2f, %9.2f) \tEnemies: attract(%9.2f, %9.2f), repel(%9.2f, %9.2f)\n") +
			std::string("IMEnemy : wi(%9.2f, %9.2f, %9.2f), dF, range(%9.2f, %9.2f)") ;

	sprintf(out, formatString.c_str(),
			ent->friends.attractCoeff, ent->friends.attractExponent, ent->friends.repelCoeff, ent->friends.repelExponent,
			ent->enemies.attractCoeff, ent->enemies.attractExponent, ent->enemies.repelCoeff, ent->enemies.repelExponent,
			ent->target.attractCoeff, ent->target.attractExponent,
			ent->friends.damageAttractCoeff, ent->friends.damageAttractExponent, ent->friends.damageRepelCoeff, ent->friends.damageRepelExponent,
			ent->enemies.damageAttractCoeff, ent->enemies.damageAttractExponent, ent->enemies.damageRepelCoeff, ent->enemies.damageRepelExponent,
			ent->friends.cooldownAttractCoeff, ent->friends.cooldownAttractExponent, ent->friends.cooldownRepelCoeff, ent->friends.cooldownRepelExponent,
			ent->enemies.cooldownAttractCoeff, ent->enemies.cooldownAttractExponent, ent->enemies.cooldownRepelCoeff, ent->enemies.cooldownRepelExponent,
			ent->IMForEnemies.w1, ent->IMForEnemies.w2, ent->IMForEnemies.w3, ent->IMForEnemies.deltaFraction, ent->IMForEnemies.range
			);
}

void extractParametersFromChromosome(FastEcslent::Entity *ent, std::string chromosome){
	const char *chrom = chromosome.c_str();
	//std::cout << "Chromosome: " << chrom << std::endl;
	ent->friends.attractCoeff    = decode(chrom, 0, 12, -10000, 10000);
	ent->friends.attractExponent = decode(chrom, 12, 16, -7, 8);
	ent->friends.repelCoeff      = decode(chrom, 16, 28, -10000, 10000);
	ent->friends.repelExponent   = decode(chrom, 28, 32, -7, 8);

	ent->enemies.attractCoeff    = decode(chrom, 32, 44, -10000, 10000);
	ent->enemies.attractExponent = decode(chrom, 44, 48, -7, 8);
	ent->enemies.repelCoeff      = decode(chrom, 48, 60, -10000, 10000);
	ent->enemies.repelExponent   = decode(chrom, 60, 64, -7, 8);
	//------------------------------------------------------------------------------
	ent->friends.damageAttractCoeff    = decode(chrom, 64, 76, -10000, 10000);
	ent->friends.damageAttractExponent = decode(chrom, 76, 80, -7, 8);
	ent->friends.damageRepelCoeff      = decode(chrom, 80, 92, -10000, 10000);
	ent->friends.damageRepelExponent   = decode(chrom, 92, 96, -7, 8);

	ent->enemies.damageAttractCoeff    = decode(chrom, 96, 108, -10000, 10000);;
	ent->enemies.damageAttractExponent = decode(chrom, 108, 112, -7, 8);
	ent->enemies.damageRepelCoeff      = decode(chrom, 112, 124, -10000, 10000);
	ent->enemies.damageRepelExponent   = decode(chrom, 124, 128, -7, 8);
	//------------------------------------------------------------------------------

	ent->friends.cooldownAttractCoeff    = decode(chrom, 128, 140, -10000, 10000);
	ent->friends.cooldownAttractExponent = decode(chrom, 140, 144, -7, 8);
	ent->friends.cooldownRepelCoeff      = decode(chrom, 144, 156, -10000, 10000);
	ent->friends.cooldownRepelExponent   = decode(chrom, 156, 160, -7, 8);

	ent->enemies.cooldownAttractCoeff    = decode(chrom, 160, 172, -10000, 10000);;
	ent->enemies.cooldownAttractExponent = decode(chrom, 172, 176, -7, 8);
	ent->enemies.cooldownRepelCoeff      = decode(chrom, 176, 188, -10000, 10000);
	ent->enemies.cooldownRepelExponent   = decode(chrom, 188, 192, -7, 8);
	//------------------------------------------------------------------------------

	ent->target.attractCoeff     = decode(chrom, 192, 204, -10000, 10000);
	ent->target.attractExponent  = decode(chrom, 204, 208, -7, 8);

	ent->IMForEnemies.range         = decode(chrom, 208, 211, 0, 8);
	ent->IMForEnemies.deltaFraction = decode(chrom, 211, 214, 0, 1);
	ent->IMForEnemies.w1            = decode(chrom, 214, 218, 0, 1);
	ent->IMForEnemies.w2            = decode(chrom, 218, 222, 0, 1);
	ent->IMForEnemies.w3            = decode(chrom, 222, 226, 0, 8);
}

void OpponentMicro(FastEcslent::Entity *ent, std::string chromosome){
	assert (chromosome != "" || chromosome.length() == 226);
	extractParametersFromChromosome(ent, chromosome);
}

void AdaptiveMicro(FastEcslent::Entity *ent, Options *opts){
	if(opts->chromosome == "" || opts->chromosome.length() != 226) {
		//std::cout << opts->chromosome << std::endl;
		FixedMicro(ent);
	} else {
		extractParametersFromChromosome(ent, opts->chromosome);
	}
}


void assignMicro(FastEcslent::Entity *ent, FastEcslent::MicroType micro, Options* options){
	switch(micro){
	case FastEcslent::FIXED:
		FixedMicro(ent);
		break;
	case FastEcslent::ADAPTIVE:
		AdaptiveMicro(ent, options);
		break;
	case FastEcslent::OPPONENT_FROM_CHROMOSOME:
		OpponentMicro(ent, options->opponentChromosomes[options->opponentChromosomeIndexToUse]);
		break;
	case FastEcslent::BESTZEALOT:
		ZealotBestEvolvedMicro(ent);
		break;
	case FastEcslent::BESTVULTURE:
		VultureBestEvolvedMicro(ent);
		break;
	default:
		FixedMicro(ent);
	}
}

//---------------------------------------------------------------------------------------------------

FastEcslent::Entity* FastEcslent::GameMgr::Clump(Ogre::Vector3 center, int nEnts, int limit,
		EntityType etype, Side side, Player pl, MicroType m){

	Entity *ent;
	int spread = limit;
	int xoffset = center.x, yoffset = center.y, zoffset = center.z;
	int x, y, z;
	float yaw;
	Ogre::Vector3 pos;
	for (int i = 0; i < nEnts; i++){
		x = spread - random() % (spread * 2); //[-spread .. spread]
		x = x + xoffset;
		y = spread - random() % (spread * 2);           // nothing underwater [ 10 .. spread + 10]
		y = y + yoffset;
		if (y <= 0) y = 0;
		z = spread - random()%(spread * 2);   //[-spread .. spread]
		z = z + zoffset;
		pos = Ogre::Vector3(x, y, z);
		//std::cout << "Position: " << pos << std::endl;
		yaw = (100 - random()%(100 * 2)) * 0.01 * PI;
		ent = engine->entityMgr->createEntityForPlayerAndSideNow(etype, pos, yaw, side, pl);
		ent->moveData.altitude = pos.y;
		ent->init();
		ent->switchState(ALIVE);
		assignMicro(ent, m, options);


	}
	return ent;
}

FastEcslent::Entity* FastEcslent::GameMgr::Spread(Ogre::Vector3 center, int nEnts, int limit,
		EntityType etype, Side side, Player pl, MicroType m){

	Entity *ent;
	int spread = 10; // fixed distance from boundary
	int x, y, z;
	Ogre::Real tmp;
	Ogre::Vector3 rVec, pos;
	float yaw;
	for (int i = 0; i < nEnts; i++){
		int radius = limit + (spread - (random()%(spread * 2)));
		x = spread - random() % (spread*2); //[-spread .. spread]
		y = spread - random() % (spread*2); //[-spread .. spread]
		z = spread - random() % (spread*2); //[-spread .. spread]
		rVec = Ogre::Vector3(x, y, z);
		tmp = rVec.normalise();
		pos = rVec * radius;
		yaw = (100 - random()%(100 * 2)) * 0.01 * PI;
		ent = engine->entityMgr->createEntityForPlayerAndSideNow(etype, pos, yaw, side, pl);
		ent->moveData.altitude = pos.y;
		ent->init();
		ent->switchState(ALIVE);
		assignMicro(ent, m, options);

	}
	return ent;
}


void FastEcslent::GameMgr::SC3D(){

	Entity *ent;
	int rootN = 2;
	float xoff = 500, zoff = 500;
	for (int i = -rootN; i < rootN; i++){
		for(int j = -rootN; j < rootN; j++){
			ent = engine->entityMgr->createEntityForPlayerAndSideNow(BANSHEE, Ogre::Vector3(xoff + j * 20, 0, zoff + i * 20), 0.0f, RED, ONE);
			ent->init();
			ent->switchState(ALIVE);
			FixedMicro(ent);
		}
	}

	for (int i = -rootN; i < rootN; i++){
		for(int j = -rootN; j < rootN; j++){
			//ent = engine->entityMgr->createEntityAfterTime(BANSHEE, Ogre::Vector3(xoff + j * 20, 0, zoff + i * 20), 0.0f);
			//ent = engine->entityMgr->createEntityAfterTime(ZEALOT, Ogre::Vector3(-xoff + j * 20, 0, -zoff + i * 20), 0.0f);
			ent = engine->entityMgr->createEntityForPlayerAndSideNow(ZEALOT, Ogre::Vector3(-xoff + j * 20, 0, -zoff + i * 20), 0.0f, BLUE, TWO);
			ent->init();
			ent->switchState(ALIVE);
			AdaptiveMicro(ent, options);
		}
	}
	//for last ent with adaptive micro, can save param values.
	if(options->saveDecodedParams){
		std::ofstream ofs((options->outputFile + ".params").c_str());
		char out[1024];
		makeParamString(out, ent);
		ofs << out;
		ofs.close();
	}

	return;
}

void WriteDecodedParams(FastEcslent::Entity *ent, std::string filext, Options * options){
	//for last ent with adaptive micro, can save param values.
	if(options->saveDecodedParams){
		std::ofstream ofs((options->outputFile + filext).c_str());
		char out[1024];
		makeParamString(out, ent);
		ofs << out;
		ofs.close();
	}
}


void FastEcslent::GameMgr::TwoClumps(EntityType t1, Side side1, Player p1, MicroType m1, int nt1,
		EntityType t2, Side side2, Player p2, MicroType m2, int nt2){

	Entity *ent;
	ent = Clump(Ogre::Vector3(-250, 400, 0), nt1, 100, t1, side1, p1, m1);
	if(m1 == ADAPTIVE) WriteDecodedParams(ent, ".m1Params", options);
	ent = Clump(Ogre::Vector3(250, 400, 0), nt2, 100, t2, side2, p2, m2);
	if(m2 == ADAPTIVE) WriteDecodedParams(ent, ".m2Params", options);
	return;
}


void FastEcslent::GameMgr::ClumpSpread(EntityType t1, Side side1, Player p1, MicroType m1, int nt1,
		EntityType t2, Side side2, Player p2, MicroType m2, int nt2){

	Entity *ent;
	ent = Clump(Ogre::Vector3(0, 400, 0), nt1, 100, t1, side1, p1, m1);
	if(m1 == ADAPTIVE) WriteDecodedParams(ent, ".m1Params", options);
	ent = Spread(Ogre::Vector3(0, 400, 0), nt2, 500, t2, side2, p2, m2);
	if(m2 == ADAPTIVE) WriteDecodedParams(ent, ".m2Params", options);

	return;
}


void FastEcslent::GameMgr::tick(double dtime){

	if(options->gameNumber >= 3) return;

	Entity *ent;
	D3CoordStruct cell;
	int nAliveFriends = 0, nAliveEnemies = 0;

	for(int i = 0; i < engine->entityMgr->nEnts; i++){
		ent = engine->entityMgr->ents[i];
		if (ent->entityId.side == RED){
			cell = engine->IMMgr->enemyMin[BLUE];
			if(ent->entityState == ALIVE) nAliveEnemies++;
		} else {
			cell = engine->IMMgr->enemyMin[RED];
			if(ent->entityState == ALIVE) nAliveFriends++;
		}
		setMicroForEnt(ent, engine->IMMgr->getPositionFromIMCellCoords(cell));//, params);
	}
	if (nAliveEnemies <= 0 || nAliveFriends <= 0) {
		engine->quit = true;
		std::cout << "dt: " << dtime << "   Enemies (alive): " << nAliveEnemies << ", Friends (alive): " << nAliveFriends << std::endl;
	}
	return;
}


void FastEcslent::GameMgr::cleanup(){
	std::cout << "Cleaning up GameMgr: " << engine->entityMgr->nEnts << std::endl;
	double damageDone = 0, damageReceived = 0; // We are always BLUE
	int nFriends = 0, nEnemies = 0;
	Entity *ent;
	for(int i = 0; i < engine->entityMgr->nEnts; i++){
		ent = engine->entityMgr->ents[i];
		if(ent->entityId.side == BLUE){
			nFriends++;
			damageReceived += (ent->weaponInfo.maxHitpoints - ent->weaponInfo.hitpoints)/ent->weaponInfo.maxHitpoints;
		} else {
			//ent->print();
			damageDone += (ent->weaponInfo.maxHitpoints - ent->weaponInfo.hitpoints)/ent->weaponInfo.maxHitpoints;
			nEnemies++;
		}
	}
	options->fitness[0] = damageDone/nEnemies; //(damageDone/nFriends - damageReceived/nEnemies);
	options->fitness[1] = 1.0 - damageReceived/nFriends;
	options->fitness[2] = (double) (options->maxFrames - engine->frames)/(double) options->maxFrames;
	options->fitness[3] = damageDone;
	options->fitness[4] = damageReceived;


//	std::ofstream ofs(options->outputFile.c_str());
//	if(ofs.good()){
//		ofs << fitness << std::endl;
//		ofs.close();
//	}

}





bool FastEcslent::GameMgr::notExceedPopCap(Identity entId) {
	Entity *ent = this->engine->entityMgr->ents[entId.id];
	return (this->pop[ent->entityId.player] + this->entTypeData[ent->entityType].supply <= this->currentPopCap[ent->entityId.player]);
	//return true;
}

bool FastEcslent::GameMgr::preReqExists(EntityType entType) {
	return true;
}

void FastEcslent::GameMgr::setupEntityBuildTimes(){
	this->entTypeData[SCV].buildTime = 17;
	this->entTypeData[MARINE].buildTime = 25;
	this->entTypeData[REAPER].buildTime = 45;
	this->entTypeData[TANK].buildTime = 45;
	this->entTypeData[THOR].buildTime = 60;
	this->entTypeData[MARAUDER].buildTime = 30;
	this->entTypeData[HELLION].buildTime = 30;

	this->entTypeData[COMMANDCENTER].buildTime = 100;
	this->entTypeData[BARRACKS].buildTime = 65;
	this->entTypeData[FACTORY].buildTime = 60;
	this->entTypeData[ARMORY].buildTime = 65;
	this->entTypeData[ENGINEERINGBAY].buildTime = 35;

	this->entTypeData[REFINERY].buildTime = 30;
	this->entTypeData[SUPPLYDEPOT].buildTime = 30;

	this->entTypeData[MINERALS].buildTime = 0;
	this->entTypeData[GAS].buildTime      = 0;
}

void FastEcslent::GameMgr::setupEntitySupply(){
	for (int i = 0; i < NENTITYTYPES; i++){
		this->entTypeData[i].supply = 0;
	}
	this->entTypeData[SCV].supply      = 1;
	this->entTypeData[MARINE].supply   = 1;
	this->entTypeData[REAPER].supply   = 1;
	this->entTypeData[TANK].supply     = 3;
	this->entTypeData[THOR].supply     = 5;
	this->entTypeData[MARAUDER].supply = 2;
	this->entTypeData[HELLION].supply  = 2;
}

void FastEcslent::GameMgr::game0(){
	for(int i = 0; i < NPLAYERS; i++){
		this->popCap[i] = 200;
		this->currentPopCap[i] = 10;
		this->pop[i] = 0;
	}
	setupEntityBuildTimes();

	tester();
}

void FastEcslent::GameMgr::WaterCraft(){

	mySide   = RED;
	myPlayer = ONE;

	for(int i = 0; i < NPLAYERS; i++){
		this->popCap[i] = 200;
		this->pop[i]    = 0;
		this->currentPopCap[i] = 10;
		this->resources[i].gas      = 0;
		this->resources[i].minerals = 50;
		this->playerNEnts[i] = 0;
	}
	setupEntityBuildTimes();
	setupEntitySupply();
	float offset = 3500.0f;
	makeBaseForSidePlayer(RED, ONE, Ogre::Vector3(-offset, 0, -offset), 550.0f, 0.06f);
	makeBaseForSidePlayer(BLUE, TWO, Ogre::Vector3(offset, 0, offset), 550.0f, 0.06f);
	//makeBaseForSidePlayer(YELLOW, THREE, Ogre::Vector3(-offset, 0, offset), 550.0f, 0.06f);
	//makeBaseForSidePlayer(GREEN, FOUR, Ogre::Vector3(offset, 0, -offset), 550.0f, 0.06f);
	getAllMyEntsToMining(RED, ONE);
	getAllMyEntsToMining(BLUE, TWO);
}

void FastEcslent::GameMgr::makeBaseForSidePlayer(Side side, Player player, Ogre::Vector3 location, float offset, float yawOffset){
	//create command center
	Entity *ent;
	ent = engine->entityMgr->createEntityForPlayerAndSide(COMMANDCENTER, location, 0.0f, side, player);
	ent->moveData.pos = location;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();
	this->currentEntityCounts[player][ent->entityType]++;
	this->playerEnts[player][this->playerNEnts[player]++] = ent;
	ent->entityState = ALIVE;

	//create Minerals
	createNEntitiesRadial(MINERALS, 8, side, player, location, offset, yawOffset);
	// create Gas
	createNEntitiesRadial(GAS, 2, side, player, location, offset, yawOffset * 5, 2.0f);
	//create SCVS
	createNEntitiesRadial(SCV, 5, side, player, location, offset/3.0f, yawOffset/2.0f);

}

void FastEcslent::GameMgr::createNEntitiesRadial(EntityType entType, int nEntities, Side side, Player player,
		Ogre::Vector3 location, float offset, float yawOffset, int yawOffsetMultiplier){
	Entity *ent;
	Ogre::Vector3 entityLocation(location.x, 0, location.z);
	float radius = entityLocation.length();
	radius = radius + fabs(offset);
	float yaw  = atan2(location.z, location.x);
	yaw = yaw - yawOffset*nEntities/2.0f;

	entityLocation.x = cos(yaw) * radius;
	entityLocation.z = sin(yaw) * radius;
	for (int i = 0; i < nEntities; i++) {
		ent = engine->entityMgr->createEntityForPlayerAndSide(entType, entityLocation, yaw, side, player );
		ent->init();

		this->pop[player] += this->entTypeData[ent->entityType].supply;
		this->currentEntityCounts[player][ent->entityType]++;
		this->playerEnts[player][this->playerNEnts[player]++] = ent;
		ent->entityState = ALIVE;

		yaw += yawOffset * yawOffsetMultiplier;
		entityLocation.x = cos(yaw) * radius;
		entityLocation.z = sin(yaw) * radius;
	}
}

FastEcslent::Entity *FastEcslent::GameMgr::findClosestEntityOfTypeWithinDistance(EntityType entityType, Ogre::Vector3 pos, float maxDistance, Side side, Player player){
	float minDistance = FLT_MAX;
	float distance;
	Entity *minEnt = 0;
	Entity *ent = 0;
	for (int i = 0; i < this->playerNEnts[player]; i++){
		ent = this->playerEnts[player][i];
		//if (ent->entityType == MINERALS && ent->entityId.side == side && ent->entityId.player == player){

		if (ent->entityType == entityType && ent->entityState == ALIVE){
			//std::cout << "Found: " << ent->uiname << ": " << ent->entityState << std::endl;
			distance = pos.distance(ent->moveData.pos);
			if (distance < maxDistance) {
				//std::cout << ent->uiname << ", distance: " << distance << std::endl;
				if (distance < minDistance) {
					minDistance = distance;
					minEnt      = ent;
				}
			}
		}
	}
	return minEnt;
}

void FastEcslent::GameMgr::getAllMyEntsToMining(Side side, Player player){
	Entity *ent;
	SCVehicle *scv;
	for (int i = 0; i < this->playerNEnts[player]; i++){
		ent = this->playerEnts[player][i];
		if(ent->entityType == SCV){
			scv = dynamic_cast<SCVehicle *> (ent);
			Minerals *mineral = dynamic_cast<Minerals *> (findClosestEntityOfTypeWithinDistance(MINERALS, scv->moveData.pos, FLT_MAX, side, player));
			getSCVToMine(side, player, scv, mineral, false);
		}
	}
}

void FastEcslent::GameMgr::getSCVToMine(Side side, Player player, SCVehicle *scv, Minerals *mineral, bool addCommand){
	assert(mineral != 0);
	Gather *g = createGatherForEntAndMineral(scv, mineral);
	UnitAI *ai = dynamic_cast<UnitAI *> (scv->getAspect(UNITAI));
	if(addCommand) {
		ai->addCommand(g);
	} else {
		ai->setCommand(g);
	}
}


void FastEcslent::GameMgr::getSelectedEntsToMining(Minerals* mineral, bool shiftDown){
	Entity *ent;
	Entity **ents = this->engine->selectionMgr->getSelectedEnts();
	for (int i = 0; i < this->engine->selectionMgr->getNSelectedEnts();i++){
		ent = ents[i];
		if (ent->entityType == SCV ){
			SCVehicle *scv = dynamic_cast<SCVehicle *>(ent);
			getSCVToMine(mySide, myPlayer, scv, mineral, shiftDown);
		} else {
			getEntToMoveOrAttack(mySide, myPlayer, ent, mineral, shiftDown);
		}

	}
}


void FastEcslent::GameMgr::getEntToMoveOrAttack(Side mySide, Player myPlayer, Entity *ent, Entity *targetEnt, bool addCommand){
	std::cout << "Ent: " << ent->uiname << " is moving to or attacking " << targetEnt->uiname << std::endl;

}



void FastEcslent::GameMgr::tester(){
	int x = 0;
	int z = 0;

	int rangex = 2000;
	int rangez = rangex;
	int nEnts  = 50;
	//std::cout << "Game2...nEnts: " << nEnts << std::endl;
	engine->selectionMgr->resetAll();
	//Group *group = engine->groupMgr->createGroup();

	Entity *ent;
	ent = engine->entityMgr->createEntityAfterTime(BARRACKS, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	ent = engine->entityMgr->createEntityAfterTime(COMMANDCENTER, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	ent = engine->entityMgr->createEntityAfterTime(FACTORY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	ent = engine->entityMgr->createEntityAfterTime(REFINERY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	ent = engine->entityMgr->createEntityAfterTime(SUPPLYDEPOT, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	ent = engine->entityMgr->createEntityAfterTime(ENGINEERINGBAY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	ent = engine->entityMgr->createEntityAfterTime(ARMORY, Ogre::Vector3(0, 0, 0), 0.0f);
	ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
	x += 500;
	std::cout << "Game Manager: " << ent->uiname << std::endl;
	ent->init();

	x = 0;
	z = 0;
	for(int i = 0; i < nEnts; i++){
		ent = engine->entityMgr->createEntityAfterTime(static_cast<EntityType>(i%7), Ogre::Vector3(0, 0, 0), 0.0f);
		std::cout << "Game Manager: " << ent->uiname << std::endl;
		ent->init();
		ent->moveData.pos = Ogre::Vector3(x, 0.0f, z);
		z = rangez - random() % (2 * rangez);
		x = rangex - random() % (2 * rangex);
		ent->moveData.heading = (random()%180) * 0.0174532925;
		ent->moveData.yaw = ent->moveData.heading;
		ent->moveData.desiredHeading = ent->moveData.heading;
		ent->moveData.desiredSpeed = 0.0f;
	}

}
/*const char *chrom = opts->chromosome.c_str();
//std::cout << "Chromosome: " << chrom << std::endl;
ent->friends.attractCoeff    = decode(chrom, 0, 12, -10000, 10000);
ent->friends.attractExponent = decode(chrom, 12, 16, -7, 8);
ent->friends.repelCoeff      = decode(chrom, 16, 28, -10000, 10000);
ent->friends.repelExponent   = decode(chrom, 28, 32, -7, 8);

ent->enemies.attractCoeff    = decode(chrom, 32, 44, -10000, 10000);
ent->enemies.attractExponent = decode(chrom, 44, 48, -7, 8);
ent->enemies.repelCoeff      = decode(chrom, 48, 60, -10000, 10000);
ent->enemies.repelExponent   = decode(chrom, 60, 64, -7, 8);
//------------------------------------------------------------------------------
ent->friends.damageAttractCoeff    = decode(chrom, 64, 76, -10000, 10000);
ent->friends.damageAttractExponent = decode(chrom, 76, 80, -7, 8);
ent->friends.damageRepelCoeff      = decode(chrom, 80, 92, -10000, 10000);
ent->friends.damageRepelExponent   = decode(chrom, 92, 96, -7, 8);

ent->enemies.damageAttractCoeff    = decode(chrom, 96, 108, -10000, 10000);;
ent->enemies.damageAttractExponent = decode(chrom, 108, 112, -7, 8);
ent->enemies.damageRepelCoeff      = decode(chrom, 112, 124, -10000, 10000);
ent->enemies.damageRepelExponent   = decode(chrom, 124, 128, -7, 8);
//------------------------------------------------------------------------------

ent->friends.cooldownAttractCoeff    = decode(chrom, 128, 140, -10000, 10000);
ent->friends.cooldownAttractExponent = decode(chrom, 140, 144, -7, 8);
ent->friends.cooldownRepelCoeff      = decode(chrom, 144, 156, -10000, 10000);
ent->friends.cooldownRepelExponent   = decode(chrom, 156, 160, -7, 8);

ent->enemies.cooldownAttractCoeff    = decode(chrom, 160, 172, -10000, 10000);;
ent->enemies.cooldownAttractExponent = decode(chrom, 172, 176, -7, 8);
ent->enemies.cooldownRepelCoeff      = decode(chrom, 176, 188, -10000, 10000);
ent->enemies.cooldownRepelExponent   = decode(chrom, 188, 192, -7, 8);
//------------------------------------------------------------------------------

ent->target.attractCoeff     = decode(chrom, 192, 204, -10000, 10000);
ent->target.attractExponent  = decode(chrom, 204, 208, -7, 8);

ent->IMForEnemies.range         = decode(chrom, 208, 211, 0, 8);
ent->IMForEnemies.deltaFraction = decode(chrom, 211, 214, 0, 1);
ent->IMForEnemies.w1            = decode(chrom, 214, 218, 0, 1);
ent->IMForEnemies.w2            = decode(chrom, 218, 222, 0, 1);
ent->IMForEnemies.w3            = decode(chrom, 222, 226, 0, 8);

//		ent->common.distanceWeight   = decode(chrom, 80, 90,  0, 100000);
//		ent->common.hitpointsWeight  = decode(chrom, 90, 100, 0, 100000);

//	char out[1024];
//	makeParamString(out, ent);
//	std::cout << out;
}
}*/
