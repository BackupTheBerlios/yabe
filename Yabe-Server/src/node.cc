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
#include "include/node.h"

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
		fprintf (stderr, "Error binding to port");
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
		perror ("fcntl(F_GETFL)");
		exit (EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl (Sock, F_SETFL, opts) < 0)
	{
		perror ("fcntl(F_SETFL)");
		exit (EXIT_FAILURE);
	}
}

void
Node::BuildSelectList (fd_set *ArgSocksFd)
{
	SocksFd = ArgSocksFd;
	FD_ZERO (SocksFd);
	FD_SET (MainSock, SocksFd);

    LoopTroughList(&Node::AddFdListItem);
}

void
Node::ManageSocks (fd_set * ArgSocksFd) //!
{
	if (FD_ISSET (MainSock, SocksFd))   //!
	{
		AcceptNewUser ();
	}

	LoopTroughList(&Node::ManageData);
}

void
Node::AcceptNewUser ()		//Error handling p356 + Max aantal connecties !
{
	Connection *NewItem = AddItem ();

	NewItem->Socket = accept (MainSock, NULL, NULL);
	SetNonBlocking (NewItem->Socket);
}

void Node::LoopTroughList (bool(Node::*pFunction)(Connection *pTraverseArgument))
{
	Connection *pStart = GetCurrentPtr ();
	if (pStart != NULL)
	{
		Connection *pTraverse = GetCurrentPtr ();
		do
		{
			if (!(this->*pFunction)(pTraverse))
				break;
			pTraverse = Advance ();
		}
while (pStart != GetCurrentPtr ());
}
}

bool Node::AddFdListItem(Connection *pTraverseArgument)
{
FD_SET (pTraverseArgument->Socket, SocksFd);
return 1;
}
