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

#include "include/linkedlist.h"

template <class Type>
class Node : private LinkedList<Type>
{
	private:
	int MainSock;
	int gHighSock;
    fd_set gSocks;
	struct sockaddr_in ServerAddress;
	
	public:
	Node (int Port, int MaxConnections);
	void SetNonBlocking (int Sock);
	void BuildSelectList ();
	void ReadSocks ();
	
	int& NewSocket();
	int& GetCurrentSocket();
	int& GetPreviousSocket();
	int& GetNextSocket();
};

template <class Type>
int& Node<Type>::NewSocket()
{
	Type *Temp = AddItem();
	return Temp->Socket;
}

template <class Type>
int& Node<Type>::GetNextSocket()
{
	Type *Temp = Advance();
	return Temp->Socket;
}

template <class Type>
int& Node<Type>::GetPreviousSocket()
{
	Type *Temp = Rewind();
	return Temp->Socket;
}

template <class Type>
int& Node<Type>::GetCurrentSocket()
{
	Type *Temp = Current();
	return Temp->Socket;
}
