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
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define COMMAND 2
#include "include/linkedlist.h"

struct Connection
{
	int Socket;
	Connection *Next;
	  Connection ()
	{
		Next = NULL;
	}
};

class Node:private LinkedList < Connection >
{
      private:
	int MainSock;
	int HighSock;		//!
	int MaxConnections;
	int Port;
	struct sockaddr_in ServerAddress;

	void DisconnectClient (char *pData);
	void VerifyClient (char *pData);

	struct NetCommands
	{
		char *Command;
		void (Node::*pFunction) (char *pData);
	} CommandList[COMMAND];

	void SetNonBlocking (int Sock);
	//Also possible to use global fd_set or to give pTraverse as argument
	void LoopTroughList (bool (Node::*pFunction) (fd_set * ArgSocksFd),
			     fd_set * ArgSocksFd);
	bool AddFdListItem (fd_set * ArgSocksFd);
	bool ManageData (fd_set * ArgSocksFd);
	void AcceptNewUser ();
	char *Getline (int *Socket);
	void ParseData (char *pBuffer);

	class ErrorHandling
	{
	      public:
		enum TypeOfError
		{ Data, Accept, NonBlocking };
		  ErrorHandling (TypeOfError S):Error (S)
		{
		}
		TypeOfError GiveError ()
		{
			return Error;
		}
	      private:
		  TypeOfError Error;
	};

      public:
	Node ();

	void BindSocket ();
	void BuildSelectList (fd_set * ArgSocksFd);
	void ManageSocks (fd_set * ArgSocksFd);
};
