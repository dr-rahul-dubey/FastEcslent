/*
 * netThread.cpp
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */


#include <boost/thread.hpp>
#include <boost/asio.hpp>

//#include <netThread.h>
#include <engine.h>
#include <listener.h>
#include <sender.h>

#include <messages.h>
#include <utils.h>

using boost::asio::ip::udp;

udp::endpoint FastEcslent::NetThread::getMyIP(){

	udp::endpoint me;
	try {
		boost::asio::io_service netService;
		udp::resolver   resolver(netService);
		udp::resolver::query query(udp::v4(), boost::asio::ip::host_name(), "");
		udp::resolver::iterator endpoints = resolver.resolve(query);
		std::cout << "Got endPoint:" << boost::asio::ip::host_name() << std::endl;
		udp::endpoint ep = *endpoints;
		std::cout << "Address: " << ep.address() << std::endl;
		std::cout << "IP Address:" << boost::asio::ip::address_v4().to_string() << std::endl;

		if (ep.address().to_string().substr(0,3) != "127"){
			me = ep;

		}
	} catch (std::exception& e){
		std::cerr << "getMyIP could not deal with socket. Exception: " << e.what() << std::endl;
	}
	try {
		boost::asio::io_service netService;
		udp::resolver   resolver(netService);
		udp::resolver::query query(udp::v4(), "google.com", "");
		udp::resolver::iterator endpoints = resolver.resolve(query);
		udp::endpoint ep = *endpoints;
		udp::socket socket(netService);
		socket.connect(ep);
		boost::asio::ip::address addr = socket.local_endpoint().address();
		std::cout << "My IP according to google is: " << addr.to_string() << std::endl;
		me =  ep;

	} catch (std::exception& e){
		std::cerr << "getMyIP could not deal with socket. Exception: " << e.what() << std::endl;
	}
	return me;
}

//something to make udpsockets

void FastEcslent::NetThread::init() { //initialize sockets, public
	std::cout << "Initializing Net before starting thread. Engine instance Id: " << engine->instanceId << std::endl;
	udp::endpoint myIP = getMyIP();
	listener = new Listener(myIP, engine->options->networkPort, engine->options->isServer);
	listener->init();
	sender   = new Sender(myIP, engine->options->networkPort, engine->options->isServer); // if isServer -> broadcast
	sender->init();
	quit = false;
	serverSleepTime = new boost::posix_time::milliseconds(50);
	startTime = getCurrentTime();

}


void FastEcslent::NetThread::run() { //start thread, public
	netThread = boost::thread(&FastEcslent::NetThread::runThread, this);
	std::cout << "Running thread: " << netThread.get_id() << " Engine instance Id: " << engine->instanceId << std::endl;
}

void FastEcslent::NetThread::runThread(){ // run the netManaager thread, private (each tick)
	listener->run();
	sender->run();

	if (engine->options->isServer) {
		serverTick();
	} else {
		clientTick();
	}
}

namespace FastEcslent {

	State* fillStateFromEnt(FastEcslent::Entity *ent){
		State *s = new State();

		s->id = ent->entityId.id;

		Ogre::Vector3 position = ent->moveData.pos;
		//pos
		s->px = position.x; //ent->moveData.pos.x;
		s->py = position.y; //ent->moveData.pos.y;
		s->pz = position.z; //ent->moveData.pos.z;

		Ogre::Vector3 velocity = ent->moveData.vel;

		s->vx = velocity.x;//ent->moveData.vel.x;
		s->vy = velocity.y;//ent->moveData.vel.y;
		s->vz = velocity.z;//ent->moveData.vel.z;

		s->dh = ent->moveData.desiredHeading;
		s->ds = ent->moveData.desiredSpeed;

		s->rSpeed = 0.0;
		s->yaw    = ent->moveData.yaw;

		s->flags  = (unsigned short) 0;

		return s;
	}

	Info *fillInfoFromEnt(FastEcslent::Entity *ent){
		Info *info = new Info();
		char tmp[256];
		sprintf(tmp, "%256i", ent->entityType);
		strcpy(info->type, tmp);// = makeTypeStringFromEntityType(ent->entityType);
		sprintf(tmp, "%256s", ent->uiname.c_str());
		strcpy(info->label, tmp);
		//strcpy(info->label, ent->uiname.c_str());
		//pad(info->label, 256);
		info->beam = ent->staticData.width;
		info->draft = ent->staticData.depth;
		info->id    = ent->entityId.id;
		info->length = ent->staticData.length;
		info->playerId = 0;
		info->side = 0;
		info->maxSpeed = ent->staticData.maxSpeed;
		info->maxSpeedReverse = ent->staticData.minSpeed;
		return info;
	}

	Message *makeMessageWithStateHeader(int nEnts, long dtime){
		Message *m = new Message();
		m->head.msgType = STATEMESSAGETYPE;
		m->head.millisecondsFromStart = dtime; //boost::posix_time::milliseconds(500)
		m->head.numberOfStructs = nEnts;
		m->head.sizeOfStruct    = StateSize;
		return m;
	}

	Message *makeMessageWithInfoHeader(int nEnts, long dtime){
		Message *m = new Message();
		m->head.msgType = INFOMESSAGETYPE;
		m->head.millisecondsFromStart = dtime; //boost::posix_time::milliseconds(500)
		m->head.numberOfStructs = nEnts;
		m->head.sizeOfStruct    = InfoSize;
		return m;
	}

}

void FastEcslent::NetThread::clientTick(){
	updateState();
	sendCommands();
}

void FastEcslent::NetThread::updateState(){

}

void FastEcslent::NetThread::sendCommands(){
}

void FastEcslent::NetThread::serverTick(){
	while (!quit) {
		handleClientMessages();
		serve();
	}
}

void FastEcslent::NetThread::serve(){

	Message *message;
	State   *state;
	unsigned int offset = 0;

	int n = engine->entityMgr->nEnts;

	ptime currentTime = getCurrentTime();
	time_duration dtime = currentTime - startTime;

	//std::cout << "Serving at dtime: " << dtime.total_milliseconds() << std::endl;
	message = makeMessageWithStateHeader(n, dtime.total_milliseconds());
	offset = 0;
	for (int i = 0; i < n; i++) {
		state = fillStateFromEnt(engine->entityMgr->ents[i]);
		//engine->entityMgr->dumpOne(i);
		memcpy((void*)(message->data + offset), (void *)state, StateSize);
		offset += StateSize;
		delete state;
	}
	//printMessage(message);
	sender->addMessage(message);
	boost::this_thread::sleep(*serverSleepTime);


}

void FastEcslent::NetThread::handleClientMessages(){
	Message *m;
	while (!listener->recBuffer.empty()) {
		m = getMessage();
		if((int) m->head.msgType != 1) {
			handleMessage(m);
		}
		delete m;
	}
}

FastEcslent::Message *FastEcslent::NetThread::getMessage(){
	Message *m = new Message();
	m = listener->recBuffer.front();
	listener->recBuffer.pop_front();
	return m;
}

void FastEcslent::NetThread::handleMessage(Message *m){
	switch((int) m->head.msgType) {
	case FastEcslent::REQUESTINFOMESSAGETYPE:
		sendInfo(m);
		break;
	default:
		break;

	}
}

void FastEcslent::NetThread::sendInfo(Message *m){
	Entity *ent;
	Info   *info;
	ptime currentTime = getCurrentTime();
	time_duration diff = currentTime - startTime;
	Message *sendInfoMessage = makeMessageWithInfoHeader(m->head.numberOfStructs, diff.total_milliseconds());

	int offset = HeaderSize;
	//std::cout << "Readying info to be sent" << std::endl;
	FastEcslent::printMessageHeader(m->head);
	FastEcslent::printMessageData(m);
	for(int i = 0; i < m->head.numberOfStructs; i++){
		ent = engine->entityMgr->ents[*((int *)m->data + i)];//convert to int pointer, add i, then dereference
		info = fillInfoFromEnt(ent);
		std::cout << "Sending info message about ent with id: " << ent->entityId.id << std::endl;
		memcpy((void* ) ((char*)sendInfoMessage + offset), info, InfoSize);
		offset += InfoSize;

		delete info;
	}

	sender->addMessage(sendInfoMessage);
}

void FastEcslent::NetThread::stop(){ // end thread, public
	quit = true;
}


void FastEcslent::NetThread::stopAndJoin(){ // end thread, public
	stop();
	std::cout << "NetThread Stopping...Quit = " << std::endl;
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	sender->stop();
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	listener->stop();
	boost::this_thread::sleep(boost::posix_time::seconds(2));

	std::cout << "NetThread Joining..." << std::endl;


	sender->join();
	boost::this_thread::sleep(boost::posix_time::seconds(2));
	//listener->join();
	//boost::this_thread::sleep(boost::posix_time::seconds(2));
	listener->kill();
	sender->kill();

	boost::this_thread::sleep(boost::posix_time::seconds(2));
	//listener->listenerThread.interrupt();//I would like to join listener but that keeps crashing
	netThread.join();
}
