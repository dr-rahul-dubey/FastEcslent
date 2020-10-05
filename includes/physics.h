/*
 * physics.h
 *
 *  Created on: Mar 3, 2012
 *      Author: sushil
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include<aspect.h>

namespace FastEcslent {


	class Entity;

	class Physics: public UnitAspect {

	public:
		float angleDiff;
		float dHeading;
		float timeScaledAcceleration;
		float timeScaledRotationalSpeed;
		float cosYaw;
		float sinYaw;
		void doHelmsman(double dt);

	public:
		Physics(Entity *ent, UnitAspectType at): UnitAspect(ent, at){};
		virtual ~Physics(){}
		virtual void init() {};//= 0;
		virtual void tick(double dt){};// = 0;
	};


	class Physics2D1: public Physics {

	private:
		void doHelmsman(double dt);

	public:
		Physics2D1(Entity* ent, UnitAspectType at): Physics(ent, at) {};
		~Physics2D1(){}
		virtual void init();
		virtual void tick(double dt);

	};

	class Physics2D2: public Physics {

	private:
		void doHelmsman(double dt);

	public:
		Physics2D2(Entity* ent, UnitAspectType at): Physics(ent, at) {}
		~Physics2D2(){}
		virtual void init();
		virtual void tick(double dt);

	};


	class Physics3D1: public Physics {

		private:
			//void doHelmsman(double dt);

		public:
			Physics3D1(Entity* ent, UnitAspectType at): Physics(ent, at) {};
			~Physics3D1(){}
			virtual void init();
			virtual void tick(double dt);

		};



}


#endif /* PHYSICS_H_ */
