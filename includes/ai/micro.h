/*
 * micro.h
 *
 *  Created on: Jan 26, 2016
 *      Author: sushil
 */

#ifndef INCLUDES_AI_MICRO_H_
#define INCLUDES_AI_MICRO_H_

#include <OgreVector3.h>
#include <ent.h>
#include <command.h>

const int NWEIGHTS = 10;

namespace FastEcslent {

typedef struct {
	float friendAttractCoeff;
	float enemyAttractCoeff;
	float friendRepelCoeff;
	float enemyRepelCoeff;

	float friendAttractExponent;
	float enemyAttractExponent;
	float friendRepelExponent;
	float enemyRepelExponent;

	float targetAttractCoeff;
	float targetAttractExponent;


	float weights[NWEIGHTS];
} MicroParamsStruct;

class Micro: public UnitCommand {
private:
	bool valid(Ogre::Vector3 pos){
		return true;
	}


public:
	MicroParamsStruct params;

	Micro (Entity *ent, Target *tgt): UnitCommand(ent, MicroCommand, tgt) {
		//params = p;
	}

	~Micro(){
		//std::cout << "destructor called deleting target" << std::endl;;
		delete target;
	}
	virtual bool done();
	virtual void init();
	virtual void tick(double dt);
	void tick2(double dt);

};

}

#endif /* INCLUDES_AI_MICRO_H_ */
