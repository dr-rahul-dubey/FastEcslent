/*
 * command.h
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#ifndef COMMAND_H_
#define COMMAND_H_


#include <OgreVector3.h>

#include<target.h>
//#include<ent.h>

namespace FastEcslent {

	enum CommandType {
		MoveCommand     = 0,
		AttackCommand   = 1,
		RamCommand      = 2,
		MaintainCommand = 3,
		GatherCommand   = 4,
		MicroCommand    = 5,

		FLOCK
	};


	enum LeadershipType {
		ClosestToTarget     = 0,
		FurthestFromTarget  = 1,
		MostMassive         = 2,
		LeastMassive        = 3,
		Random              = 4
	};

	class Entity;
	class Group;

	class Command {
	public:
		CommandType commandType;
		Command(CommandType ct){
			commandType = ct;
		}
		virtual ~Command(){}
		virtual bool done() = 0;// {return false;}
		virtual void init() = 0;
		virtual void tick(double dt) = 0;
	};


	class UnitCommand : public Command {

	public:

		Target *target;
		Entity *entity;

		UnitCommand(Entity *ent, CommandType ct, Target* targ): Command(ct) {
			entity = ent;
			target = targ;
			relativeSpeed = 0;
			predictedTimeToClose = 0;
		}
		virtual ~UnitCommand(){}
		// vars
		Ogre::Vector3 relativePos;
		Ogre::Vector3 relativeVel;
		Ogre::Vector3 predictedPos;
		Ogre::Vector3 interceptPos;

		double predictedTimeToClose;
		double relativeSpeed;
	};


	class Tactic: public Command {
	public:
		GroupTarget* target;
		Group* group;
		Tactic(Group* grp, CommandType ct, GroupTarget* trgt): Command(ct){
			group = grp;
			target = trgt;
		}

		int mostMassive(bool);
		int closestToTarget(bool, Ogre::Vector3 tpos);
		void changeLeadership(LeadershipType selector);

	};



	class Move: public UnitCommand {
	private:
		bool valid(Ogre::Vector3 pos){
			return true;
		}

	public:
		Move (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
			if(valid(tgt->location)) {
				//std::cout << "Moving to: " << tgt->location << std::endl;
			}
		}
		virtual bool done();
		virtual void init();
		virtual void tick(double dt);

	};

	//-----------------Wait--------------------------
	class Wait: public UnitCommand { // For SCVs
	private:
		bool valid(Ogre::Vector3 pos){
			return true;
		}

	public:
		Wait (Entity *ent, Target *tgt): UnitCommand(ent, GatherCommand, tgt) {
			timeLeftToWait = tgt->waitTime;
			std::cout << "Waiting for: " << tgt->waitTime << std::endl;
			finished = false;
		}
		bool finished;
		double timeLeftToWait;
		virtual bool done();
		virtual void init();
		virtual void tick(double dt);
	};
	//---------------END--Gather--------------------------


	class PotentialMove: public UnitCommand {
	private:
		bool valid(Ogre::Vector3 pos){
			return true;
		}
		double A, B, B2, m, n, RepulsionThresholdDistance;

	public:
		PotentialMove (Entity *ent, Target *tgt): UnitCommand(ent, MoveCommand, tgt) {
			if(valid(tgt->location)) {
				//std::cout << "Moving to: " << tgt->location << " using Potential Fields" << std::endl;
				A = 1000.0;
				B = 800000.0;
				B2 = 3000000.0;
				m = 4.0;
				n = 1;
				RepulsionThresholdDistance = 10000;
			}
		}
		virtual bool done();
		virtual void init();
		virtual void tick(double dt);

	};

	class Maintain: public UnitCommand {
	private:
		bool valid (Entity *ent);

	public:
		Maintain (Entity *ent, Target *tgt);
		virtual bool done();
		virtual void init();
		virtual void tick(double dt);

	};

}



#endif /* COMMAND_H_ */
