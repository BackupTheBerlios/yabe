/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <vector>
#include <iostream>		// Alleen nodig voor cerr, dus als log er is dit weg !
#include "include/node.h"
using namespace std;

Node::Node ()			//Port & Max = doorgegeven uit object ?
{
	CommandList[0].Command = "Verify";
	CommandList[0].pFunction = &Node::VerifyClient;

	CommandList[1].Command = "Disconnect";
	CommandList[1].pFunction = &Node::DisconnectClient;

	Port = 1050;		//Doorgeven van pointer naar object en zo initialiseren
	MaxConnections = 10;
}

void
Node::BindSocket ()
{
	int ReuseAddr = 1;
	MainSock = socket (AF_INET, SOCK_STREAM, 0);

	setsockopt (MainSock, SOL_SOCKET, SO_REUSEADDR, &ReuseAddr,
		    sizeof (ReuseAddr));

	SetNonBlocking (MainSock);

	memset ((char *) &ServerAddress, 0, sizeof (ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl (INADDR_ANY);
	ServerAddress.sin_port = htons (Port);

	if (bind (MainSock, (struct sockaddr *) &ServerAddress,
		  sizeof (ServerAddress)) < 0)
	{
		printf ("Cannot bind primary socket");
		exit (1);
	}

	listen (MainSock, MaxConnections);
	HighSock = MainSock;
}

void
Node::SetNonBlocking (int Sock)
{
	int opts;

	opts = fcntl (Sock, F_GETFL);
	if (opts < 0)
	{
		throw ErrorHandling (ErrorHandling::NonBlocking);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl (Sock, F_SETFL, opts) < 0)
	{
		throw ErrorHandling (ErrorHandling::NonBlocking);
	}
}

void
Node::BuildSelectList (fd_set * ArgSocksFd)
{
	FD_ZERO (ArgSocksFd);
	FD_SET (MainSock, ArgSocksFd);

	LoopTroughList (&Node::AddFdListItem, ArgSocksFd);
}

void
Node::ManageSocks (fd_set * ArgSocksFd)	// This is antoher set of SocksFd !
{
	if (FD_ISSET (MainSock, ArgSocksFd))	//!
	{
		try //algemeen try block ?
		{
			AcceptNewUser ();
		}

		catch (ErrorHandling E)
		{
			switch (E.GiveError ())
			{
			case ErrorHandling::NonBlocking:
				DisconnectClient (NULL);	//-> Voor log
				cerr << "NonBlocking error: Connection deleted" << endl;
			case ErrorHandling::Accept:
				cerr << "Error with accepting" << endl;
			}
		}
		return;		//niet echt nodig
	}

	try
	{
		LoopTroughList (&Node::ManageData, ArgSocksFd);
	}

	catch (ErrorHandling E)
	{
		if (E.GiveError () == ErrorHandling::Data)
			cerr << "Parse or recieve error" << endl;
	}
}

void
Node::AcceptNewUser ()		//Error handling p356 + Max aantal connecties !
{
	Connection *NewItem = AddItem ();

	NewItem->Socket = accept (MainSock, NULL, NULL);
	SetNonBlocking (NewItem->Socket);
}

void
Node::LoopTroughList (bool (Node::*pFunction) (fd_set * ArgSocksFd),
		      fd_set * ArgSocksFd)
{
	Connection *pStart = GetCurrentPtr ();
	if (pStart != NULL)
	{
		Connection *pTraverse = GetCurrentPtr ();
		do
		{
			if (!((this->*pFunction) (ArgSocksFd)))
				break;
			pTraverse = Advance ();
		}
		while (pStart != GetCurrentPtr ());
	}
}

bool Node::AddFdListItem (fd_set * ArgSocksFd)
{
	FD_SET ((GetCurrentPtr ())->Socket, ArgSocksFd);
	return 1;
}

bool Node::ManageData (fd_set * ArgSocksFd)
{
	if ((FD_ISSET ((GetCurrentPtr ())->Socket, ArgSocksFd)))
	{
		char *
			pBuffer;
		if ((pBuffer = Getline (&(GetCurrentPtr ()->Socket))) != NULL)
			ParseData (pBuffer);
		else
			DisconnectClient (NULL);	//Temp NULL -> moet voor log verwijzen !
		return 0;	//End the LoopTroughList function
	}
	else
		return 1;
}

char *
Node::Getline (int *Socket)
{
	vector < char >CharVector;
	char TempBuffer;
	int Value;

	while (true)
	{
		Value = recv (*(Socket), &TempBuffer, 1, 0);
		switch (Value)
		{
		case 0:
			return NULL;	//Disconnected
		case -1:	//Error
			throw ErrorHandling (ErrorHandling::Data);
		};

		if (TempBuffer == '\n')	// Let op \r is iets anders ?!
		{
			char *pData = new char[CharVector.size () + 1];
			memset (pData, 0, CharVector.size () + 1);

			for (unsigned int i = 0; i < CharVector.size ();
			     i += 1)
			{
				pData[i] = CharVector[i];
			}
			return pData;
		}
		else
		{
			CharVector.push_back (TempBuffer);
		}
	}
}

void
Node::ParseData (char *pBuffer)
{
	char *pDividedData;
	for (int Loop = 0; Loop < COMMAND; Loop++)
	{
		if ((strstr (pBuffer, (CommandList[Loop].Command))) != NULL)
		{
			if ((strstr (pBuffer, ":")) != NULL)
			{
				pDividedData = strstr (pBuffer, ":");	//pdividedata local scope new maken !
				(this->*(CommandList[Loop].pFunction))
					(pDividedData);
				return;
			}
			//delete (pBuffer); //verplaatsen !!
		}
	}
	delete[]pBuffer;
	throw ErrorHandling (ErrorHandling::Data);
}

void
Node::VerifyClient (char *pData)
{

}

void
Node::DisconnectClient (char *pData)
{
	close ((GetCurrentPtr ()->Socket));
	if (DeleteCurrentPtr () == 0)
		throw ErrorHandling (ErrorHandling::Data); // ander error type ?
}
