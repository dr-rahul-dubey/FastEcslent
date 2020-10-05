/*
 * unutAI.cpp
 *
 *  Created on: Dec 21, 2011
 *      Author: sushil
 */

#include <unitAI.h>

inline void FastEcslent::UnitAI::tick(double dt) {

	if (!commands.empty()) {
		//do the command
		commands.front()->tick(dt);
		if (commands.front()->done()){ // if done
			commands.pop_front();      // pop top
			if(!commands.empty()){     // if more commands
				commands.front()->init(); // init next command
			}
		}

	}

}

void FastEcslent::UnitAI::init(){
	//std::cout << "Starting unitAi for Entity: " << entity->entityId.id << std::endl;
	if(!commands.empty()){
		commands.front()->init();
	}
}

void FastEcslent::UnitAI::addCommand(Command* command){
	commands.push_back(command);
}

void FastEcslent::UnitAI::deleteCommands(){
	Command *c;
	while (!commands.empty()) {
		c = commands.front();
		commands.pop_front();

		delete c;
	}
}

void FastEcslent::UnitAI::setCommand(Command* command){
	deleteCommands();
	commands.clear();
	commands.push_back(command);
}

void FastEcslent::UnitAI::setCommandList(std::deque<Command*> cmds){
	//commands.clear();
	deleteCommands();
	commands.clear();
	commands = cmds;
}
