/***************************************************************************
                          clientsim.cpp  -  Client-side networked simulation
                             -------------------
    begin                : di jan 14 2003
    copyright            : (C) 2003 by CJP
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

#include "clientsim.h"

CClientSim::CClientSim(CClientNet *net, CWorld *w) : CSimulation(w)
{
	m_Net = net;

	//Temporary dummy implementation
	m_RuleControl = new CRuleControl(w);
}

CClientSim::~CClientSim()
{
	//Temporary dummy implementation
	delete m_RuleControl;
}

CString CClientSim::getTrackname()
{
	return "tracks/default.track"; //TODO: really find out what track should be used
}

bool CClientSim::update()
{
	//TODO: check network for new data, and update the world

	//Temporary dummy implementation
	return m_RuleControl->update();
}
