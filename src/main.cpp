/*
 * main.cpp
 *
 *  Created on: Nov 20, 2011
 *      Author: sushil
 */

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <engine.h>
#include <entityMgr.h>

#include <options.h>


using namespace FastEcslent;

//extern void gfxManager();

void modifyOptions(Options *options, int argc, char * argv[]){
	options->inputFile  = argv[1];
	options->gameNumber = atoi(argv[2]);
	options->outputFile = argv[3];

	options->chromosome = std::string(argv[4]);
	//std::cout << "Chrom: " << options->chromosome << std::endl;
	//return options;
}

void setOptions(Options *options){
	options->enableNetworking           = false;
	options->networkPort                = 54321;
	options->isServer                   = true;
	options->runAsEvaluator             = true;
	options->instanceId                 = random(); //assumes srandom has been called
	options->tacticalAI                 = true;
	options->shouldWaitForUserToShutGfx = true;
	options->imapx = options->imapy = options->imapz = 128;
	options->enableGfx                  = false;
	options->runDebugTests              = false;
	options->speedup                    = 2.0;
	options->maxFrames                  = 1000000;
	options->mapSize                    = 10000;
	options->gameNumber                 = 3;
	options->saveDecodedParams          = false;
	options->terrainPngFilename = "feTerrain.png";
	options->chromosome = "";
	options->inputFile = "engine.cfg";
	options->outputFile = "output";
	options->opponentChromosomeCount = 0;
	options->opponentChromosomeIndexToUse = -1;
	for(int i = 0; i < NCRITERIA; i++){
		options->fitness[i] = -1000.0;
	}

	std::ifstream ifs (options->inputFile.c_str());
	if (ifs.is_open()){
		ifs >> options->speedup;
		ifs >> options->gameNumber;
		ifs >> options->enableGfx;
		ifs >> options->saveDecodedParams;
		ifs >> options->shouldWaitForUserToShutGfx;
		ifs >> options->mapSize;
		ifs >> options->maxFrames;
		ifs >> options->opponentChromosomeIndexToUse;
		ifs >> options->opponentChromosomeCount;
		assert(options->opponentChromosomeCount > options->opponentChromosomeIndexToUse);
		for(int i = 0; i < options->opponentChromosomeCount && i < MAX_OPPONENT_CHROMOSOMES; i++){
			ifs >> options->opponentChromosomes[i];
		}
	}

	ifs.close();
	std::cout << "Input file: " << options->inputFile <<  " Output file: " << options->outputFile << std::endl;
	//return options;
}

Options* makeOptions(int argc, char*argv[]){
	Options *options = new Options;
	setOptions(options);
	if(argc == 5) {
		modifyOptions(options, argc, argv); //this changes some options (inputfile (no effect), gameNumber, outputFile, Chromosome)
	}
	return options;
}

void writeFitness(Options *options){
	//std::cout << "Writing fitnesses" << std::endl;
	std::ofstream ofs;
	ofs.open(options->outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(ofs.is_open()){
		for(int i = 0; i < NCRITERIA; i++){
			ofs << options->fitness[i] << " ";
		}
		ofs << std::endl;
	}

	ofs.close();//close it anyway
	return;
}

int main(int argc, char *argv[]){

	//srandom(time(NULL));
	srandom(199);
	Options *options = makeOptions(argc, argv);

	Engine *engine = new Engine(random(), options);
	engine->showMenu();
	engine->constructManagers();
	engine->init();

	engine->loadLevel();
	engine->run();
	engine->releaseLevel();//computes multiple fitnesses

	engine->stop();

	writeFitness(engine->options);

	delete options;
	delete engine;

	return 0;
}


/*
	NetThread *net;
	if(options->enableNetworking){
		net = new NetThread(engine);
		net->init();
		net->start();
	}

	GfxThread *gfx;
	if(options->enableGfx){
		gfx = new GfxThread(engine);
		gfx->init();
		gfx->start();
	}
	sleep(2.0); //wait for graphics thread to get going, start gets the thread running



	while (!engine->quit) {
		engine->loadLevel();
		engine->run();
	}

	if (options->enableNetworking){
		net->join();
	}

	if (options->enableGfx) {
		//gfx->stop();
		gfx->join();

	}

*/

