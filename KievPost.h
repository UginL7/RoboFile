#pragma once
#include <list>
#include "Logs.h"
#include "InitPath.h"
#include "GetFileInfo.h"

using namespace std;

class CKievPost
{

public:
	CKievPost(void);
	virtual ~CKievPost(void);	
	// ����������� ������ � ����� �������� �� ����
	list<CString> SendPostToKiev();

private:
	CInitPath *pPath;
	CLogs *pLog;
	CGetFileInfo *pFileInfo;
	//���� ��� ��������� � ����� ������
	bool bErrorDirMove;

	// �������� �����
	char szFileOwner[1024];
	char szDomainName[1024];

	//���� � ������
	//������ �������
	CString strSrcFile;
	//���� ��������
	CString strDstFile;
	// ������ ������ � ����� �� ��������
	list<CString> vFileName;
	list<CString>::iterator itFileName;
	// �������� �� ������������ ����� ����� ��� ��������
	bool VerifyFileName(char* strFileName);
	// �������� ������ ������ �������������� �� �������� � ���� �� ��� � ����������� �������
	void GetNameFileMailFromFolder(char* pFolderPath);
	// ������ ���� � ����� �������� �������� ���������
	CString m_strSrcFolder;
	// ������ ���� � ����� �������� �������� ���������
	CString m_strDstFolder;
	// �������� ���������� ��� ����������� ����������, � ������� ������� � ���
	bool CreateArcDir(char* szFileName, char *szDestination);
	// ����������� ����� � ����� ���� � �� ��������
	bool CopyFileToDirectory(char* szFileName, char* szDstFullPath, char* szDirPah);
	// ����������� ����� � ����� ������ ��� ������, ��� �������� �� ����
	bool MoveFileToErrorDirectoty(char *szFileName);
};
