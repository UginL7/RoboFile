#pragma once
#include "Logs.h"
#include "InitPath.h"
#include "GetFileInfo.h"
#include "TranzitFiles.h"
#include <list>

using namespace std;

class CRecivePost
{
public:
	CRecivePost(void);
	~CRecivePost(void);
	// ������ ������ ��������� �� �������
	list<CString> GetPostFromRajon();
	// ������ ������ ��������� �� �����
	list<CString> GetPostFromKiev();
private:
	// ���������� � �����
	CGetFileInfo *pFileInfo;
	// ������ � ���-����
	CLogs *pLog;
	// ���� � ���������
	CInitPath *pPath;
	// ���������� �����
	CTranzitFiles *pTranzit;
	// ���� � ������
	CString strSrcFile;
	CString strArcDir;
	// ���� � ����� ������
	CString sDstFullPath;
	// ������ ������ � ����� �� ��������
	list<CString> vFileName;
	list<CString>::iterator itFileName;
	// �������� ������ �������� ������
	void GetNameFileMailFromFolder(char* pFolderPath);
	// ������ �������� ������ ��������� � �����
	bool SpreadOutFiles(bool bKiev, char* szFileName);
	// ���� ��������� � ������ �� ����������
	bool bSpecFile;
	// ���� ������ ��������
	bool bCorrectName;
public:
	// ����������� ����� � ����� ����
	bool CopyFileToDirectory(char* szFileName, char* szDstFullPath, char* szDirPah, bool bKiev);
	// ����������� ���� ���������� ��� �������� �����
	CString GetKievDepartNumber(char* szFileName);
	// ����������� � ����� ����������
	bool CopyToKanceljarija(char* szFileName);
	// �������� ������ ��������������� �� ��������� ��� �������
	CString CheckDedicatedFile(char* szFileName);
};
