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

template < class Type > class LinkedList
{
      private:
	int Size;
	Type *pHead;
	Type *pTail;
	Type *pCurrent;
	Type *Previous (Type * pIndex);

      public:

	LinkedList ();
	Type *GetCurrentPtr ();
	Type *AddItem ();
	Type *Advance ();
	Type *Rewind ();
	Type *SetToHead ();
	bool DeleteItem (Type * pDelItem);
};

template < class Type > LinkedList < Type >::LinkedList ():Size (0)
{
	pHead = NULL;
	pCurrent = NULL;
	pTail = NULL;
}

template < class Type > Type * LinkedList < Type >::AddItem ()
{
	if (pHead == NULL)
	{
		pHead = new Type;
		pTail = pHead;
		pCurrent = pTail;
		return pCurrent;
	}
	pTail->Next = new Type;
	pTail = pTail->Next;
	pCurrent = pTail;
	return pCurrent;
}

template < class Type > Type * LinkedList < Type >::Previous (Type * pIndex)
{
	Type *pTemp = pHead;
	if (pIndex == pHead)
	{
		return pHead;
	}
	while (pTemp->Next != pIndex)
	{
		pTemp = pTemp->Next;
	}
	return pTemp;
}

template < class Type > Type * LinkedList < Type >::Rewind ()
{
	if (pCurrent != pHead)
	{
		pCurrent = Previous (pCurrent);
		return pCurrent;
	}
	pCurrent = pTail;
	return pCurrent;
}

template < class Type > Type * LinkedList < Type >::Advance ()
{
	if (pCurrent->Next != NULL)
	{
		pCurrent = pCurrent->Next;
		return pCurrent;
	}
	pCurrent = pHead;
	return pCurrent;
}

template < class Type > Type * LinkedList < Type >::GetCurrentPtr ()
{
	return pCurrent;
}
