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
#include "include/linkedlist.h"

struct Connection
{
	int Socket;
	Connection *Next;
	Connection()
	{Next = NULL;
	Socket = 0;
	}
};

class Node : private LinkedList<Connection>
{
	private:
	int MainSock;
	int HighSock;
	int MaxConnections;
	int Port;
	struct sockaddr_in ServerAddress;
	void SetNonBlocking (int Sock);
	
	public:
	Node (int Port, int MaxConnections);
	void BindSocket ();
	void BuildSelectList (fd_set *SocksFd);
	void ManageSocks ();
};
