/*
 * weaponMgr.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: sushil
 */

#include <weaponMgr.h>
#include <engine.h>
#include <enums.h>

FastEcslent::WeaponMgr::WeaponMgr(Engine *eng, Options *opts): Mgr(eng) {
	options = opts;
	//establish weapon mapping
}

void FastEcslent::WeaponMgr::mapWeaponTypeToEntityType(){
	//float templatedDamageMap[NWEAPONTYPES][NENTITYTYPES] = {5,       2,        10, //Cigarette
                                                          //Rifle,  Flamer,  Shell
//	float templatedDamageMap[NENTITYTYPES][NWEAPONTYPES] = {{5,       2,        10}, //Cigarette
//			                                                {5,       2,        10}, //Speedboat
//			                                                {1,       1,         2}, //DDG51
//			                                                {1,       1,         2}, //CVN68
//			                                                {2,       2,         4}, //SLEEK
//			                                                {1,       5,         1}, //AlienShip
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//			                                                {0,       0,         0}, //Barracks
//															{0,       0,         0}, //Barracks
//															{5,       2,         10}, //Banshee
//	};
//
//	//easier than reading from a file
//	for(int i = 0; i < NWEAPONTYPES; i++){
//		for(int j = 0; j < NENTITYTYPES; j++){
//			damageMap[i][j] = templatedDamageMap[i][j];
//		}
//	}
//
//	float templatedRange[NWEAPONTYPES] = {400, 200, 800};
//	for(int i = 0; i < NWEAPONTYPES; i++){
//		range[i] = templatedRange[i];
//	}


}

void FastEcslent::WeaponMgr::fixFightingProperties(){
//	for (int entType = 0; entType < NENTITYTYPES; entType++){
//		switch(entType){
//		case MARINE:
//			maxHitpoints[MARINE] = 50.0;
//			initArmor[MARINE]     = 1.0;
//			initDamageMultiplier[MARINE] = 1.0;
//			break;
//		case REAPER:
//			maxHitpoints[REAPER] = 50.0;
//			initArmor[REAPER]     = 1.0;
//			initDamageMultiplier[REAPER] = 1.0;
//			break;
//		case TANK:
//			maxHitpoints[TANK] = 200.0;
//			initArmor[TANK]     = 1.0;
//			initDamageMultiplier[TANK] = 1.0;
//			break;
//		case THOR:
//			maxHitpoints[THOR] = 300.0;
//			initArmor[THOR]     = 1.0f;
//			initDamageMultiplier[THOR] = 1.0f;
//			break;
//		case MARAUDER:
//			maxHitpoints[MARAUDER] = 100;
//			initArmor[MARAUDER]     = 1.0;
//			initDamageMultiplier[MARAUDER] = 1.0;
//			break;
//		case HELLION:
//			maxHitpoints[HELLION] = 70;
//			initArmor[HELLION]     = 1.0;
//			initDamageMultiplier[HELLION] = 1.0;
//			break;
//			//-----------------------------------------------------
//		case BARRACKS:
//			maxHitpoints[BARRACKS] = 1000;
//			initArmor[BARRACKS]    = 1.0;
//			initDamageMultiplier[BARRACKS] = 1.0;
//			break;
//		case FACTORY:
//			maxHitpoints[FACTORY] = 1000;
//			initArmor[FACTORY]    = 1.0;
//			initDamageMultiplier[FACTORY] = 1.0;
//			break;
//		case ARMORY:
//			maxHitpoints[ARMORY] = 1000;
//			initArmor[ARMORY]    = 1.0;
//			initDamageMultiplier[ARMORY] = 1.0;
//			break;
//		case ENGINEERINGBAY:
//			maxHitpoints[ENGINEERINGBAY] = 1000;
//			initArmor[ENGINEERINGBAY]    = 1.0;
//			initDamageMultiplier[ENGINEERINGBAY] = 1.0;
//			break;
//		case SCV:
//			maxHitpoints[SCV] = 50;
//			initArmor[SCV]    = 1.0;
//			initDamageMultiplier[SCV] = 1.0;
//			break;
//		case GAS:
//			maxHitpoints[GAS] = 10000000;
//			initArmor[GAS]    = 1.0;
//			initDamageMultiplier[GAS] = 1.0;
//			break;
//		case MINERALS:
//			maxHitpoints[MINERALS] = 10000000;
//			initArmor[MINERALS]    = 1.0;
//			initDamageMultiplier[MINERALS] = 1.0;
//			break;
//		case COMMANDCENTER:
//			maxHitpoints[COMMANDCENTER] = 1500;
//			initArmor[COMMANDCENTER]    = 1.0;
//			initDamageMultiplier[COMMANDCENTER]= 1.0;
//			break;
//		case BANSHEE:
//			maxHitpoints[BANSHEE] = 200;
//			initArmor[BANSHEE]    = 1.0;
//			initDamageMultiplier[BANSHEE] = 1.0;
//			break;
//		default:
//			std::cerr << "Unknown Entity Type in WeaponManager. Need to exit: " << entType << std::endl;
//			break;
//		}
//	}
}




void FastEcslent::WeaponMgr::init(){
	//establish weaponType, entityType damage mapping
	//mapWeaponTypeToEntityType();
	//fixFightingProperties();

}


void FastEcslent::WeaponMgr::tick(double dt){

}
