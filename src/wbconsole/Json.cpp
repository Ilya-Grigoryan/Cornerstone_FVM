// Json.cpp: implementation of the CJson class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "Json.h"

CJson::CJson()
{
	m_sJSON.GetBufferSetLength(BLOCK_LENGTH);
	m_bEmpty = true;
}

CJson::~CJson()
{

}

void CJson::Begin()
{
	m_sJSON = "{";
}

void CJson::End()
{
	m_sJSON += "}";
}

void CJson::BeginArray()
{
	if(!m_bEmpty)
		m_sJSON += ",";
	m_sJSON = "[";
}

void CJson::EndArray()
{
	m_sJSON += "]";
	m_bEmpty = false;
}

int CJson::Add(const char* szKey, const char* szVal)
{
	if(!m_bEmpty)
		m_sJSON += ",";

	CString sItem;
	sItem.Format(_T("\"%s\":%s"), szKey, szVal);

	m_sJSON += sItem;
	m_bEmpty = false;
	return 1;
}

int CJson::Add(const char* szVal)
{
	if(!m_bEmpty)
		m_sJSON += ",";

	CString sItem;
	sItem.Format(_T("%s"), szVal);

	m_sJSON += szVal;
	m_bEmpty = false;
	return 1;
}

int CJson::AddString(const char* szKey, const char* szVal)
{
	if(!m_bEmpty)
		m_sJSON += ",";

	CString sItem;
	if(szKey)
		sItem.Format(_T("\"%s\":\"%s\""), szKey, szVal);
	else
		sItem.Format(_T("\"%s\""), szVal);

	m_sJSON += sItem;
	m_bEmpty = false;
	return 1;
}

int CJson::AddInt(const char* szKey, const int nVal)
{
	if(!m_bEmpty)
		m_sJSON += ",";

	CString sItem;
	if(szKey)
		sItem.Format(_T("\"%s\":%i"), szKey, nVal);
	else
		sItem.Format(_T("%i"), nVal);

	m_sJSON += sItem;
	m_bEmpty = false;
	return 1;
}

int CJson::AddIntQ(const char* szKey, const int nVal)
{
	if(!m_bEmpty)
		m_sJSON += ",";

	CString sItem;
	if(szKey)
		sItem.Format(_T("\"%s\":\"%i\""), szKey, nVal);
	else
		sItem.Format(_T("\"%i\""), nVal);

	m_sJSON += sItem;
	m_bEmpty = false;
	return 1;
}

int CJson::AddDouble(const char* szKey, const double fVal)
{
	if(!m_bEmpty)
		m_sJSON += ",";

	CString sItem;
	if(fVal == (double)_INF_)
	{
		if(szKey)
			sItem.Format(_T("\"%s\":null"), szKey);
		else
			sItem = "null";
	}
	else
	{
		if(szKey)
			sItem.Format(_T("\"%s\":%f"), szKey, fVal);
		else
			sItem.Format(_T("%f"), fVal);
	}

	m_sJSON += sItem;
	m_bEmpty = false;
	return 1;
}
