/***************************************************************************
                          main.cpp  -  UltimateStunts main
                             -------------------
    begin                : wo nov 20 23:23:28 CET 2002
    copyright            : (C) 2002 by CJP
    email                : cornware-cjp@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <vector>
namespace std {}
using namespace std;

//Frequently used
#include "cstring.h"
#include "lconfig.h"
#include "filecontrol.h"

//Simulation stuff
#include "world.h"
#include "simulation.h"
#include "clientsim.h"
#include "physics.h"

#include "usserver.h"

//Player stuff
#include "objectchoice.h"
#include "aiplayercar.h"
#include "humanplayer.h"

//Graphics stuff
#include "winsystem.h"
#include "gamerenderer.h"
#include "gui.h"
#include "gamecamera.h"

//Sound stuff
#include "sound.h"

CWorld *world = NULL;

vector<CPlayer *> players;

CSimulation *sim = NULL;
CUSServer *server = NULL;

CWinSystem *winsys = NULL;
CGameRenderer *renderer = NULL;
CGUI *gui = NULL;
CGameCamera *camera = NULL;

CSound *soundsystem = NULL;

bool mainloop()
{
 //Debugging 'display'
	printf("\033[1H");
	printf("\033[1G");
	printf("**********\n");
	for(unsigned int i=0; i<world->m_MovObjs.size(); i++)
	{
		CMovingObject *mo = world->m_MovObjs[i];
		CVector r = mo->getPosition();
		CVector v = mo->getVelocity();
		printf("Object %d: position (%.2f,%.2f,%.2f), velocity (%.2f,%.2f,%.2f)\n",
		               i,            r.x, r.y, r.z,       v.x, v.y, v.z);
	}
	printf("**********\n");

	bool retval = true;

	//Escape:
	retval = retval && ( !(bool)(winsys->wasPressed('\e')) );

	//Camera:
	if(winsys->wasPressed('c')) camera->swithCameraMode();
	if(winsys->wasPressed('t')) camera->switchTrackedObject();

	for(unsigned int i=0; i<players.size(); i++)
		players[i]->update(); //Makes moving decisions

	retval = retval && sim->update(); //Modifies world object

	camera->update();
	renderer->update();
	soundsystem->update();

	return retval;
}

bool addPlayer(CString desc)
{
	CPlayer *p;

	if(desc[0]=='H')
		p = new CHumanPlayer(world, winsys);
	else
		p = new CAIPlayerCar(world);

	CObjectChoice choice;

	int id = sim->addPlayer(choice);
	p->m_MovingObjectId = id;
	p->m_PlayerId = 0;
	if(id < 0)
	{
		printf("Sim doesn't accept player\n");
		delete p;
		return false;
	}

	if(desc[0]=='H') //set the camera to this player:
		camera->setTrackedObject(id);

	players.push_back(p);
	return true;
}

void start(int argc, char *argv[]) //first things before becoming interactive
{
	printf("Welcome to " PACKAGE " version " VERSION "\n");

	CLConfig conffile(argc, argv);
	if(!conffile.setFilename("ultimatestunts.conf"))
	{
		printf("Error: could not read ultimatestunts.conf\n");
		//TODO: create a default one
	} else {printf("Using ultimatestunts.conf\n");}

	CFileControl *fctl = new CFileControl;
	{
		//Default:
		CString DataDir = ""; //try in default directories, like "./"

		CString cnf = conffile.getValue("files", "datadir");
		if(cnf != "")
		{
			if(cnf[cnf.length()-1] != '/') cnf += '/';
			DataDir = cnf;
		}
		fctl->setDataDir(DataDir);
	}

	printf("Initialising Ultimate Stunts:\n---Window system\n");
	winsys = new CWinSystem(conffile);

	world = new CWorld();

	printf("---GUI\n");
	gui = new CGUI(conffile, winsys);

	printf("---Sound system\n");
	soundsystem = new CSound(conffile, world);

	printf("---Rendering engine\n");
	renderer = new CGameRenderer(conffile, world);

	printf("---Camera\n");
	camera = new CGameCamera(world);
	renderer->setCamera(camera);
	soundsystem->setCamera(camera);
}

void end() //Last things before exiting
{
	printf("Unloading Ultimate Stunts:\n---Sound system\n");
	if(soundsystem!=NULL)
		delete soundsystem;
	printf("---Rendering engine\n");
	if(renderer!=NULL)
		delete renderer;
	printf("---Simulation\n");
	if(sim!=NULL)
		delete sim;

	printf("---Players\n");
	for(unsigned int i=0; i<players.size(); i++)
		delete players[i];
	players.clear();

	printf("---World data\n");
	if(world!=NULL)
		delete world;

	printf("---Server\n");
	if(server!=NULL)
		delete server; //also stops the server process (if existing)

	printf("---GUI\n");
	if(gui!=NULL)
		delete gui;

	printf("---Window system\n");
	if(winsys!=NULL)
		delete winsys; //Important; don't remove: this calls SDL_Quit!!!

	printf("\nProgram finished succesfully\n");
}

int main(int argc, char *argv[])
{
	start(argc, argv);

	//The track filename:
	CString trackfile = "tracks/default.track";

	//MENU SECTION:
	gui->startFrom("mainmenu");
	while(!( gui->isPassed("mainmenu") )); //waiting for input
	CGUI::eMainMenu maininput =
		(CGUI::eMainMenu)gui->getValue("mainmenu").toInt();

	switch(maininput)
	{
		case CGUI::LocalGame:
			printf("Creating physics simulation\n");
			sim = new CPhysics(world);
			while(!( gui->isPassed("trackmenu") )); //waiting for input
			trackfile = gui->getValue("trackmenu");
			break;
		case CGUI::NewNetwork:
		{
			//Server menu:
			while(!( gui->isPassed("servermenu") )); //waiting for input
			CString name = "localhost";
			int port = gui->getValue("servermenu", "portnumber").toInt();

			//Track menu:
			while(!( gui->isPassed("trackmenu") )); //waiting for input
			trackfile = gui->getValue("trackmenu");

			server = new CUSServer(port, trackfile);
			printf("Creating client-type simulation with %s:%d\n", name.c_str(), port);
			sim = new CClientSim(world, name, port);
			break;
		}
		case CGUI::JoinNetwork:
		{
			while(!( gui->isPassed("hostmenu") )); //waiting for input
			CString name = gui->getValue("hostmenu", "hostname");
			int port = gui->getValue("hostmenu", "portnumber").toInt();
			printf("Creating client-type simulation with %s:%d\n", name.c_str(), port);
			sim = new CClientSim(world, name, port);
			trackfile = ((CClientSim *)sim)->getTrackname();
			break;
		}
		default:
		case CGUI::Exit:
			end();
			return EXIT_SUCCESS;
	}

	printf("\nLoading the track\n");
	world->loadTrack(trackfile);
	renderer->loadTrackData();

	while(!( gui->isPassed("playermenu") )); //waiting for input
	int num_players = gui->getValue("playermenu", "number").toInt();
	printf("\nNumber of players to be added: %d\n", num_players);
	for(int i=0; i<num_players; i++)
	{
		CString desc = gui->getValue("playermenu", i);
		printf("%d: \"%s\"\n", i, desc.c_str());
		addPlayer(desc);
	}

	printf("\nLoading moving objects\n");
	if(!sim->loadObjects())
		{printf("Error while loading moving objects\n");}
	if(!renderer->loadObjData())
		{printf("Error while loading object graphics\n");}


	printf("\nLoading the sound samples\n");
	soundsystem->load();

	printf("\nEntering mainloop\n");
	//Creating some white space
	for(int i=1; i<40; i++)
		printf("\n");
	winsys->runLoop(mainloop, true); //true: swap buffers
	printf("\nLeaving mainloop\n");

	end();
	return EXIT_SUCCESS;
}
