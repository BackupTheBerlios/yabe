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
	bool DeleteCurrentPtr ();
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

template < class Type > bool LinkedList < Type >::DeleteCurrentPtr ()
{
	Type *pTemp;

	if (pCurrent == NULL)
		return 0;
	else
	{ //indien Ptail = Phead dan blijft tail wel nog op oude waarde staan !
		if (pCurrent == pHead)	//Case 1 delete = Head
		{
			pTemp = pHead;
			pHead = pHead->Next;
			delete pTemp;
			pCurrent = pHead;
			return 1;
		}
		else if (pCurrent == pTail)	//Case 2 delete is at the end
		{
			pTemp = pTail;
			pTail = Rewind ();
			pTail->Next = NULL;
			delete pTemp;
			pCurrent = pTail;
			return 1;
		}
		else		//Case 3 delete is in middle somewhere
		{
			pTemp = pCurrent;
			pCurrent = Rewind ();
			pCurrent->Next = pTemp->Next;
			delete pTemp;
			return 1;
		}
	}
}

template < class Type > Type * LinkedList < Type >::GetCurrentPtr ()
{
	return pCurrent;
}
