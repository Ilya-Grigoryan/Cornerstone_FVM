// ListEx.h: interface for the CListEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTEX_H__55CC8416_9628_4111_9640_E922C3CAF61D__INCLUDED_)
#define AFX_LISTEX_H__55CC8416_9628_4111_9640_E922C3CAF61D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////////////
template <typename T>
class CListExItem
{
public:
	T		m_Key;
	void*	m_pData;
};

//////////////////////////////////////////////////////////////////////////////
template <typename T>
class _declspec(dllexport) CListEx  
{
public:
	CListEx();
	~CListEx();

public:
	int		Add(const T& key, void* pData, BOOL* pbNew=NULL);
	int		Lookup(const T& key);
	int		LookupEx(const T& key);
	
	int		GetSize();
	CListExItem<T>*	GetItem(int nIndex);

	void* operator[](const T& key);
	
	void	RemoveAll();

public:
	CArray<CListExItem<T>*,CListExItem<T>*>		m_ItemData;
};

///////////////////////////////////////////////////////////////////////////////
template <typename T> CListEx<T>::CListEx()
{
}

template <typename T> CListEx<T>::~CListEx()
{
	RemoveAll();
}

template <typename T> int CListEx<T>::GetSize()
{
	return m_ItemData.GetSize();
}

template <typename T> void CListEx<T>::RemoveAll()
{
	for(int i=0; i<m_ItemData.GetSize(); i++)
		delete m_ItemData[i];
	m_ItemData.RemoveAll();
}

template <typename T> CListExItem<T>* CListEx<T>::GetItem(int nIndex)
{
	return m_ItemData[nIndex];
}

template <typename T> void* CListEx<T>::operator[](const T& key)
{
	int nIndex = Lookup(key);
	if(nIndex == -1)
		return NULL;

	return (void*)m_ItemData[nIndex]->m_pData;
}

template <typename T> int CListEx<T>::Add(const T& key, void* pData, BOOL* pbNew)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = m_ItemData.GetUpperBound();
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find the index to which the element to be inserted*/
	while(nFirst <= nLast)
	{
		T tmp = m_ItemData[nMiddle]->m_Key;

		/* check whether the element already exists*/ 
		if(key == tmp)
		{
			CListExItem<T>* pItem = new CListExItem<T>();
			pItem->m_Key = key;
			pItem->m_pData = pData;
			m_ItemData.InsertAt(nMiddle, pItem);

			if(pbNew)
				*pbNew = false;
			return nMiddle;
		}

		/* check for great or less*/
		if(key < tmp)
			nLast = nMiddle - 1;
		else
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}
	
	/* the nFirst actually indicates the index */
	ASSERT(nFirst >= 0 && nFirst <= m_ItemData.GetSize());

	/* create new list item and add to list */
	CListExItem<T>* pItem = new CListExItem<T>();
	pItem->m_Key = key;
	pItem->m_pData = pData;

	m_ItemData.InsertAt(nFirst, pItem);
	if(pbNew)
		*pbNew = true;
	return nFirst;
}

template <typename T> int CListEx<T>::Lookup(const T& key)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = m_ItemData.GetUpperBound();
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		T Temp = m_ItemData[nMiddle]->m_Key;

		/* check whether the element already exists*/ 
		if(key == Temp)
			return nMiddle;

		/* check for greater or less*/
		if(key < Temp)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	return -1;
}

template <typename T> int CListEx<T>::LookupEx(const T& key)
{
	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = m_ItemData.GetUpperBound();
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		T Temp = m_ItemData[nMiddle]->m_Key;

		/* check whether the element already exists*/ 
		if(key == Temp)
			return nMiddle;

		/* check for greater or less*/
		if(key < Temp)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	return nFirst;
}

#endif // !defined(AFX_LISTEX_H__55CC8416_9628_4111_9640_E922C3CAF61D__INCLUDED_)
