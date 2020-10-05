/*
 * state.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef STATE_H_
#define STATE_H_


namespace FastEcslent {

	const int MaxMessageSize = 65536; //udp limit

	struct __attribute__((__packed__)) PackedHeader{
		unsigned char msgType;
		int   millisecondsFromStart;
		int   numberOfStructs;
		int   sizeOfStruct;
	};

	typedef struct PackedHeader Header;

	enum MessageTypes {
	  INFOMESSAGETYPE              = 0,
	  STATEMESSAGETYPE             = 1,
	  REQUESTINFOMESSAGETYPE       = 2,
	  REQUESTSHIPSTATUSMESSAGETYPE = 3,
	  COMMANDENTITYMESSAGETYPE     = 4,
	  CREATEENTITYMESSAGETYPE      = 5,
	  REPOSITIONENTITYMESSAGETYPE  = 6,
	  SQUELCHMESSAGETYPE           = 7,
	  NUMBEROFMESSAGETYPES         = 8
	};

	struct __attribute__((__packed__)) PackedState{
		int   id;
		float px, py, pz;
		float vx, vy, vz;
		float yaw;
		float rSpeed;//rotational speed
		float ds;
		float dh;
		unsigned short flags;
	};

	typedef struct PackedState State;

	struct __attribute__((__packed__)) PackedInfo {
		int id;
		char label[256];
		char type[256];
		float maxSpeed;
		float maxSpeedReverse;
		float length;
		float beam;
		float draft;
		int   playerId;
		int   side;

	};

	typedef struct PackedInfo Info;

	struct __attribute__((__packed__)) PackedCommandEntity {
		int id;
		float dh;
		float ds;
	} ;

	typedef struct PackedCommandEntity CommandEntity;

	struct __attribute__((__packed__)) PackedCreateEntity{
		char label[256];
		char entType[256];
		float px, py, pz;
		float yaw;
	} ;

	typedef struct PackedCreateEntity CreateEntity;

	struct  __attribute__((__packed__)) PackedRepositionEntity { //Force move entities
		int id;
		float px, py, pz;
		float vx, vy, vz;
		float yaw;
		char  label[256]; // optional
	};

	typedef struct PackedRepositionEntity RepositionEntity;

	struct __attribute__((__packed__)) PackedMessage {
		Header head;
		char   data[MaxMessageSize - sizeof(Header)];
	};

	typedef struct PackedMessage Message;

	const int MessageSize          = sizeof(Message);
	const int HeaderSize           = sizeof(Header);
	const int StateSize            = sizeof(State);
	const int InfoSize             = sizeof(Info);
	const int CommandEntitySize    = sizeof(CommandEntity);
	const int CreateEntitySize     = sizeof(CreateEntity);
	const int RepositionEntitySize = sizeof(RepositionEntity);

	void printMessageHeader(Header h);
	void printMessageData(Message *m);
	void printMessage(Message *m);
	void printState(State *s);


}

#endif /* STATE_H_ */
