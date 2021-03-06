#include "stdafx.h"
#include "PostParser.h"

CPostParser::CPostParser(void)
{
}

CPostParser::~CPostParser(void)
{
}

void CPostParser::Run()
{
	CKievPost *pKiev;
	CRajonPost *pRajon;
	CRecivePost *pRecive;
	list<CString> m_ListFile;
	list<CString>::iterator itFileList;

	// �������� ��������/��������� ����� 
	//�������� ����� �� ����
	pKiev = new CKievPost();
	m_ListFile = pKiev->SendPostToKiev();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	m_ListFile.clear();
	//�������� ����� �� ���������� ������
	pRajon = new CRajonPost();
	m_ListFile = pRajon->SendPostToRajon();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	m_ListFile.clear();


	//�������� ����� �� ��� ����
	m_ListFile = pRajon->SendPostToAllODPI();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	m_ListFile.clear();

	//�������� ����� �� ��� ������
	m_ListFile = pRajon->SendPostToAllRajon();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	m_ListFile.clear();

	//�������� ����� �� ��� ������ + ����
	m_ListFile = pRajon->SendPostToAllRajonSGNI();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	pRecive = new CRecivePost;

	m_ListFile.clear();
	//��������� ����� �� �����
	m_ListFile = pRecive->GetPostFromKiev();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	m_ListFile.clear();
	//��������� ����� �� �������
	m_ListFile = pRecive->GetPostFromRajon();
	for (itFileList = m_ListFile.begin();itFileList != m_ListFile.end(); ++itFileList)
	{
		cout << *itFileList << endl;
	}

	if (pKiev != NULL)
	{
		delete pKiev;
	}
	if (pRajon != NULL)
	{
		delete pRajon;
	}
	if (pRecive != NULL)
	{
		delete pRecive;
	}
}
