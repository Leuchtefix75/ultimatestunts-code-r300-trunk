/***************************************************************************
                          networkthread.h  -  Listening to messages from clients
                             -------------------
    begin                : do jan 13 2005
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

#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include "bthread.h"
#include "stuntsnet.h"
#include "client.h"
#include "criticalvector.h"

//Message types:
#include "chatmessage.h"
#include "textmessage.h"
#include "objectchoice.h"

/**
  *@author CJP
  */

class CNetworkThread : public CBThread  {
public: 
	CNetworkThread(unsigned int port=1500);
	virtual ~CNetworkThread();

	void setPort(unsigned int port);
	unsigned int getPort()
		{return m_Port;}

	virtual void threadFunc();

	//thrrad-safe sending of messages:
	void sendToClient(const CMessageBuffer &b, unsigned int ID);
	void sendToPlayer(const CMessageBuffer &b, unsigned int ID);
	void sendToAll(const CMessageBuffer &b);
	void sendToClient(const CString &s, unsigned int ID);
	void sendToPlayer(const CString &s, unsigned int ID);
	void sendToAll(const CString &s);

protected:
	unsigned int m_Port;
	CStuntsNet *m_Net;

	CCriticalVector<CMessageBuffer> m_OutputBuffer;


	int identify(const CIPNumber &ip, unsigned int port);
	void addClient(const CIPNumber &ip, unsigned int port);

	Uint8 processMessage(const CMessageBuffer &buffer);
	Uint8 processMessage(int ID, const CString &message);
};

#endif
