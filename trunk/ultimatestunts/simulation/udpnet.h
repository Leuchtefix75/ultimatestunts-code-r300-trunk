/***************************************************************************
                          udpnet.h  -  Basic UDP/IP functionality
                             -------------------
    begin                : ma jan 17 2005
    copyright            : (C) 2005 by CJP
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

#ifndef UDPNET_H
#define UDPNET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <vector>
namespace std {}
using namespace std;

#include "messagebuffer.h"
#include "usmacros.h"

/**
  *@author CJP
  */

class CUDPNet {
public: 
	CUDPNet(unsigned int port); //0 = take a free port (like for clients)
	virtual ~CUDPNet();

	virtual bool receiveData(unsigned int millisec=1); //returns true if new data has arrived
	vector<CMessageBuffer> m_ReceiveBuffer;

	virtual bool sendData(CMessageBuffer &data);

private:
	unsigned int m_SendCounter;

	int m_Socket;

	struct sockaddr_in m_MyAddress;
};

#endif
