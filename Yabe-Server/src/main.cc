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

#include <iostream>
#include "include/node.h"
using namespace std;

void
ManageNode (Node * pNodeArg)
{
	int Num = 0;
	fd_set FdSet;

	pNodeArg->BuildSelectList (&FdSet);
	Num = select (FD_SETSIZE, &FdSet, NULL, NULL, NULL);	//fd setsize veranderen
	if (Num > 0)
		pNodeArg->ManageSocks (&FdSet);
}

int
main ()
{
	Node MainNode;
	MainNode.BindSocket ();

	while (true)
	{
		ManageNode (&MainNode);
	}
	return 0;
}
