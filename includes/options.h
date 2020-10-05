/*
 * options.h
 *
 *  Created on: Dec 4, 2011
 *      Author: sushil
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include<string>

#include <const.h>

typedef struct  {
	bool runAsEvaluator;
	bool enableGfx;
	bool enableInteraction;
	bool tacticalAI;
	bool shouldWaitForUserToShutGfx;
	int  imapx, imapy, imapz;

	bool enableNetworking;
	int  networkPort;
	bool isServer;

	long int instanceId;

	bool runDebugTests;
	double speedup;
	unsigned long int maxFrames;
	std::string inputFile;
	std::string outputFile;
	std::string terrainPngFilename;

	std::string chromosome;
	bool saveDecodedParams;

	int gameNumber; //which game?
	int mapSize;

	double fitness[FastEcslent::NCRITERIA];

	int opponentChromosomeCount;
	std::string opponentChromosomes[FastEcslent::MAX_OPPONENT_CHROMOSOMES];
	int opponentChromosomeIndexToUse;

} Options;


#endif /* OPTIONS_H_ */
