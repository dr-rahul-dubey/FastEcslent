/*
 * netThread.h
 *
 *  Created on: Jan 10, 2012
 *      Author: sushil
 */

#ifndef NETTHREAD_H_
#define NETTHREAD_H_

#include <deque>

//#include <engine.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include <messages.h>
#include <listener.h>
#include <sender.h>

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;


using boost::asio::ip::udp;

namespace FastEcslent {

	class Engine;

	class NetThread { //package and copy sim state to/from broadcaster and listener threads

	private:
		void runThread();
		boost::thread netThread;
		boost::posix_time::milliseconds *serverSleepTime;
		ptime  startTime;
		bool quit;


	protected:
		udp::endpoint getMyIP();
		int port;
		bool isServer;
		udp::endpoint myIP;

	public:
		Engine* engine;
		/*
		std::deque <Message *> recBuffer;
		std::deque <Message *> sendBuffer;
		void queMessage(Message *m);
		Message *dequeMessage();
*/
		Listener*    listener;
		Sender*      sender;

		NetThread(Engine* eng) {
			engine = eng;
		}
		void run();
		void init();
		void stopAndJoin();
		void stop();

		//helpers
		void clientTick();
		     void updateState();
		     void sendCommands();

		void serverTick();
			void handleClientMessages();
				Message* getMessage();
				void handleMessage(Message *m);
				void sendInfo(Message *m);

			void serve();

	};

}

#endif /* NETTHREAD_H_ */
