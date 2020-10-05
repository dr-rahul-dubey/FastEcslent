/*
 * messages.cpp
 *
 *  Created on: Jan 12, 2012
 *      Author: sushil
 */


#include <deque>

#include <messages.h>
#include <iostream>
using namespace FastEcslent;
using namespace std;
void FastEcslent::printMessageHeader(FastEcslent::Header h)	{
	std::cout << "Type: " << (int) h.msgType
			<< " Time: " << (int) h.millisecondsFromStart
			<< " MessageStructSize: " << (int) h.sizeOfStruct
			<< " NumberOfStructs  : " << (int) h.numberOfStructs
			<< std::endl;
	flush(cout);
}

void FastEcslent::printState(State *s){
	cout << "State of: " << s->id << endl
			<<	"Pos: (" << s->px << ", " << s->py << ", " << s->pz << ") " << endl
			<<	"Vel: (" << s->vx << ", " << s->vy << ", " << s->vz << ") " << endl
			<<  "Dh: "  << s->dh << ", Ds: " << s->ds << endl
			<<  "Yaw:"  << s->yaw << endl;
}

void FastEcslent::printMessageData(Message *m){

	int offset = 0;
	int *ptr;
	State *sptr;

	for (int i = 0; i < m->head.numberOfStructs; i++){
		switch ((int) m->head.msgType){
		case 0:
			cout << "Message type 0" << endl;
			offset += InfoSize;
			break;
		case 1:
			cout << "Message type 1" << endl;
			sptr = (State *) m->data;
			printState(sptr + i);
			//offset += StateSize;
			break;
		case 2:
			cout << "Before data" << endl; flush(cout);
			ptr = (int *) m->data;
			cout << "IRM: Id: " << *(ptr + i) << endl;
			flush(cout);
			offset += sizeof(int);

			break;
		default:
			cout << "Unknonw message type" << endl;
			break;
		}
	}

}
void FastEcslent::printMessage(Message *m){
	printMessageHeader(m->head);
	printMessageData(m);
	flush(cout);

}
