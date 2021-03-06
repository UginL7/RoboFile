#include "StdAfx.h"
#include "RecivePost.h"

CRecivePost::CRecivePost(void)
{
	pPath = new CInitPath;
	pLog = new CLogs;
	pFileInfo = new CGetFileInfo;
	pTranzit = new CTranzitFiles;
}

CRecivePost::~CRecivePost(void)
{
	if (pPath != NULL)
	{
		delete pPath;
	}
	if (pLog != NULL)
	{
		delete pLog;
	}
	if (pTranzit != NULL)
	{
		delete pTranzit;
	}
}

// ������ ������ ��������� �� �����
list<CString> CRecivePost::GetPostFromKiev()
{
	CString strPrint;
	list<CString> m_InfoAboutCopyFile;

	strSrcFile = pPath->m_PathInPostKiev;

	// �������� ������ ���� ������ �� �����
	GetNameFileMailFromFolder(strSrcFile.GetBuffer());
	strSrcFile.ReleaseBuffer();

	itFileName = vFileName.begin();
	for(;itFileName != vFileName.end(); ++itFileName)
	{
		if (SpreadOutFiles(TRUE, itFileName->GetBuffer()) == true)
		{					
			strPrint.Format("����������� ����� '%s' : ������", *itFileName);
			m_InfoAboutCopyFile.push_back(strPrint);
		}
		else
		{
			strPrint.Format("����������� ����� '%s' : ��������", *itFileName);
			m_InfoAboutCopyFile.push_back(strPrint);
		}
		itFileName->ReleaseBuffer();
	}

	return m_InfoAboutCopyFile;
}

// ������ ������ ��������� �� �������
list<CString> CRecivePost::GetPostFromRajon()
{
	CString strPrint;
	list<CString> m_InfoAboutCopyFile;

	strSrcFile = pPath->m_PathInPostRajon;

	// �������� ������ ���� ������ �� �����
	GetNameFileMailFromFolder(strSrcFile.GetBuffer());
	strSrcFile.ReleaseBuffer();

	itFileName = vFileName.begin();
	for(;itFileName != vFileName.end(); ++itFileName)
	{
		if (SpreadOutFiles(FALSE, itFileName->GetBuffer()) == true)
		{					
			strPrint.Format("����������� ����� '%s' : ������", *itFileName);
			m_InfoAboutCopyFile.push_back(strPrint);
		}
		else
		{
			strPrint.Format("����������� ����� '%s' : ��������", *itFileName);
			m_InfoAboutCopyFile.push_back(strPrint);
		}
		itFileName->ReleaseBuffer();

	}
	return m_InfoAboutCopyFile;
}

// �������� ������ ������ �������� ��� � ����������� �������
void CRecivePost::GetNameFileMailFromFolder(char* pFolderPath)
{
	HANDLE hFindFile;
	WIN32_FIND_DATA pFindData;
	CString pPath = pFolderPath;
	CString sLog;
	CString strFile;
	CString sDelFileName;
	pPath += "*";

	//������� ������� ���������� ������
	vFileName.clear();

	//���� ����� � �����
	hFindFile = FindFirstFile(pPath, &pFindData);
	if (hFindFile != INVALID_HANDLE_VALUE )
	{
		do 
		{
			if (!(pFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//�������� � ������� ��������
				strFile = pFindData.cFileName;
				strFile = strFile.MakeLower();
				if(strFile.Left(2) == "tr" && strFile.Left(3) != "tr8")
				{
					// ������ �������� ���������� ������. 
					// ������ �� ����� � 39471/7/99-99-09-06-02-17 �� 12.12.2016
					sDelFileName = pPath.Left(pPath.GetLength() - 1);
					sDelFileName += strFile;
					DeleteFile(sDelFileName);
					sLog.Format("'%s' - �����! ������� - ������ �� ����� � 39471/7/99-99-09-06-02-17 �� 12.12.2016.",strFile);
					this->pLog->WriteInFileLog(&this->pPath->m_LogInTranzit, sLog.GetBuffer());
					sLog.ReleaseBuffer();
				}
				else
				{
					//��������� ������ ��� ����� � ������
					vFileName.push_back(strFile);
				}
			}		
		} while (FindNextFile(hFindFile, &pFindData));		
	}
}

// ������ �������� ������ ��������� � �����
bool CRecivePost::SpreadOutFiles(bool bKiev, char* szFileName)
{
	CString szSources = ""; //���� ��� ������
	CString szDestination = ""; // ���� ��� ������
	map<CString, CString>::iterator itSpecFile;
	bool bRet = false;
	bool bTrToRaj = false;
	CString sFileName = szFileName;
	CString sLog = "";
	CString sDirPath = "";
	CString sMaskFile = "";
	CString sPostArcDirectory = "";
	SYSTEMTIME pTime;
	bool bDedicatedFile = false;
	bCorrectName = false;
	bSpecFile = false;
	// �������� ������� ����, ����� � ��� ��� ��������� �����
	GetLocalTime(&pTime);


//	sFileName = sFileName.MakeLower();

	bCorrectName = pFileInfo->CheckFileNameSymbol(szFileName);
	if (bKiev == TRUE)
	{
		szSources =  pPath->m_PathInPostKiev;
		szDestination = "����";
		if (bCorrectName == true)
		{
			sMaskFile = GetKievDepartNumber(szFileName);
		}		
	}
	else
	{
		szSources =  pPath->m_PathInPostRajon;
		szDestination = "������";
		if (bCorrectName == true)
		{
			sMaskFile = sFileName.Left(2);
		}
	}
	
	if (bCorrectName == true)
	{
		if (bKiev == TRUE)
		{
			//������������ ���� ��������, � ����������� �� ���� �����
			for(itSpecFile = pPath->m_mapKievSpecFile.begin();itSpecFile != pPath->m_mapKievSpecFile.end(); ++itSpecFile)
			{
				if (sMaskFile == itSpecFile->first)
				{
					sDirPath.Format("%s\\%d\\%s\\���-%s\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, itSpecFile->second, pTime.wMonth, pTime.wDay);
					bSpecFile = true;
					break;
				}
			}
		}
		else
		{
			//������������ ���� ��������, � ����������� �� ���� �����
			for(itSpecFile = pPath->m_mapRajonSpecFile.begin();itSpecFile != pPath->m_mapRajonSpecFile.end(); ++itSpecFile)
			{
				if (sMaskFile == itSpecFile->first)
				{
					sDirPath.Format("%s\\%d\\%s\\���-%s\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, itSpecFile->second, pTime.wMonth, pTime.wDay);
					bSpecFile = true;
					break;
				}
			}
		}	

		//�������� ������ ��������������� �� ��������� ��� �������
		sPostArcDirectory = CheckDedicatedFile(szFileName);
		if (!sPostArcDirectory.IsEmpty())
		{
			sDirPath.Format("%s\\%d\\%s\\%s\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, sPostArcDirectory, pTime.wMonth, pTime.wDay);
		}

		if (bSpecFile == false && sPostArcDirectory.IsEmpty())
		{
			if (sMaskFile == "tr")
			{
				// � ����� ������ ���������� ����� ��� ��� ������� ������������
				if (sFileName.Left(5) == "tr801")
				{
					sDirPath.Format("%s\\%d\\%s\\�������\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth, pTime.wDay);
				}
				else
				{
					bTrToRaj = true;
					bRet = true;						 
				}

			}
			else if ((sMaskFile < "99") && (sMaskFile > "00"))
			{
				sDirPath.Format("%s\\%d\\%s\\���-%s\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, sMaskFile, pTime.wMonth, pTime.wDay);
			}
			else
			{
				sDirPath.Format("%s\\%d\\%s\\Nostand\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth, pTime.wDay);
			}
		}
	}
	else
	{
		sDirPath.Format("%s\\%d\\%s\\������\\%02d\\��������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth, pTime.wDay);
	}


	// ������������ ���� ����� ��� �����������
	sDstFullPath = sDirPath;
	sDstFullPath += "\\";
	strArcDir = sDstFullPath;
	sDstFullPath += sFileName;

	if (bTrToRaj == false)
	{
		// ������ ������� � ������ ����
		for (int i = 0; i < sDirPath.GetLength(); i++)
		{
			if (sDirPath.Mid(i, 1) == "\\")
			{
				CreateDirectory(sDirPath.Left(i), NULL);
				if (GetLastError() != 0)
				{
					SetLastError(0);
				}
			}
		}
		CreateDirectory(sDirPath, NULL);

		// �������� �� ������������� ����� � ������, ���� ����� ����, �� ���������� � ����� ������� ����� � ����
		sDstFullPath = pFileInfo->CheckExistingFile(sDstFullPath.GetBuffer());
		sDstFullPath.ReleaseBuffer();
	
		//����������� ����� � �����
		bRet = CopyFileToDirectory(sFileName.GetBuffer(), sDstFullPath.GetBuffer(), strArcDir.GetBuffer(), bKiev);
	}
	else
	{
		CString m_strSrcFolder = pPath->m_PathInPostRajon;
		// �������� �� ������������� ����� � ������, ���� ����� ����, �� ���������� � ����� ������� ����� � ����
		sDstFullPath = pFileInfo->CheckExistingFile(sDstFullPath.GetBuffer());
		sDstFullPath.ReleaseBuffer();

		pTranzit->WorkingTranzitFiles(sFileName.GetBuffer(), m_strSrcFolder.GetBuffer(), strArcDir.GetBuffer(), bKiev);
		m_strSrcFolder.ReleaseBuffer();

		m_strSrcFolder.Append(sFileName);
		DeleteFile(m_strSrcFolder);
	}
	sFileName.ReleaseBuffer();
	sDstFullPath.ReleaseBuffer();
	strArcDir.ReleaseBuffer();

	return bRet;
}

// ����������� ����� � ����� ����
bool CRecivePost::CopyFileToDirectory(char* szFileName, char* szDstFullPath, char* szDirPah, bool bKiev)
{
	CString sFileName;
	CString sDstFullPath;
	CString sDirPath;
	CString m_strSrcFolder;
	CString sLog;
	sDirPath = szDirPah;
	sFileName = szFileName;
	sDstFullPath = szDstFullPath;
//	sFileName =	sFileName.MakeLower();

	//����� �������� �� �����
	if (bKiev)
	{
		//��������� �������� ������ �� �����
		m_strSrcFolder = pPath->m_PathInPostKiev;
		if(bSpecFile == false || bCorrectName == false)
		{
			if (sFileName.Left(1) == "a" || (sFileName.Left(1) == "e" && sFileName.Left(2) != "er"))
			{
				//����������� ������ a*, e* � ����������
				CopyToKanceljarija(szFileName);
			}
		}
	}
	//����� �������� �� ������
	else
	{
		//��������� �������� ������ �� �������
		m_strSrcFolder = pPath->m_PathInPostRajon;
	}	
	
	if (sFileName.Left(2) == "tr")
	{
		pTranzit->WorkingTranzitFiles(szFileName, m_strSrcFolder.GetBuffer(), szDirPah, bKiev);
	}
	m_strSrcFolder.ReleaseBuffer();
	m_strSrcFolder += sFileName;

	// ����������� ����� � ����� ����
//	if(MoveFileEx(m_strSrcFolder, sDstFullPath, MOVEFILE_REPLACE_EXISTING ))
	if(CopyFile(m_strSrcFolder, sDstFullPath, false ))
	{
		sLog.Format("'%s' + ������ ���������� � ����� '%s'.", sFileName, sDirPath);
		if (bKiev == TRUE)
		{
			pLog->WriteInFileLog(&pPath->m_LogFromKiev, sLog.GetBuffer());
		}
		else
		{
			pLog->WriteInFileLog(&pPath->m_LogFromRajon, sLog.GetBuffer());
		}
		
		sLog.ReleaseBuffer();
		DeleteFile(m_strSrcFolder);
		return true;
	}
	else
	{
		// ������ ��� ����������� � ����� ����
		LPTSTR msg;
		DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
		if (res != 0)
		{
			sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n������� - %s.",sFileName, sDirPath, msg);
			
		}
		else
		{
			sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n������� - ����������.",sFileName, sDirPath);
		}

		if (bKiev == TRUE)
		{
			pLog->WriteInFileLog(&pPath->m_LogFromKiev, sLog.GetBuffer());
		}
		else
		{
			pLog->WriteInFileLog(&pPath->m_LogFromRajon, sLog.GetBuffer());
		}
		sLog.ReleaseBuffer();
		LocalFree( msg );
		return false;
	}
}

// ����������� � ����� ����������
bool CRecivePost::CopyToKanceljarija(char* szFileName)
{
	CString sLog;
	CString strSrcFile = pPath->m_PathInPostKiev;
	CString strDstKanc = pPath->m_PathInKanceljarija;
	strDstKanc += szFileName;
	strSrcFile += szFileName;

	if(CopyFile(strSrcFile, strDstKanc, FALSE))
	{
		sLog.Format("'%s' + ������ ���������� � ����� '%s'.", szFileName,  pPath->m_PathInKanceljarija);

		pLog->WriteInFileLog(&pPath->m_LogFromKiev, sLog.GetBuffer());
		sLog.ReleaseBuffer();
		return true;
	}
	else
	{
		// ������ ��� �����������
		LPTSTR msg;
		DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
		if (res != 0)
		{
			sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n������� - %s.",szFileName, pPath->m_PathInPostKiev, msg);
			pLog->WriteInFileLog(&pPath->m_LogFromKiev, sLog.GetBuffer());
			sLog.ReleaseBuffer();
			LocalFree( msg );
		}
		else
		{
			sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n������� - ����������.",szFileName, pPath->m_PathInPostKiev);
			pLog->WriteInFileLog(&pPath->m_LogFromKiev, sLog.GetBuffer());
			sLog.ReleaseBuffer();
		}
		return false;
	}	
}

// ����������� ���� ���������� ��� �������� �����
CString CRecivePost::GetKievDepartNumber(char* szFileName)
{
	CString strFileName = szFileName;
	CString strDepartNumber;
	int nFirstDotPos = 0;

//	strFileName = strFileName.MakeLower();

	// ������ ����� �������� ����� �� ����� ������ ���� ����� E�-�.��.00
	// � - ��������� ���� �� �����, ����� ���� � - ������� ����������
	// �-� - ��������������� ����� ��������� ����� � �����
	// �� - ������ ��� ����� ������� ���������������� �������������

	//������� ������ ����� � ����� �����
	nFirstDotPos = strFileName.Find(".", nFirstDotPos)+1;
	// ���������, ���� �� ������ �����
	if(strFileName.Mid(nFirstDotPos+2,1) == ".")
	{
		//�������� ��� ������� ���������������� �������������
		strDepartNumber = strFileName.Mid(nFirstDotPos,2);
	}
	else
	{
		// �������� ���������� ���� �� �����
		if (strFileName.Left(2) == "tr")
		{
			strDepartNumber = "tr";
		}
		else
		{
			// ��� �������� �� ���������
			strDepartNumber = strFileName.Left(2);
		}
	}

	return strDepartNumber;
}

// �������� ������ ��������������� �� ��������� ��� �������
CString CRecivePost::CheckDedicatedFile(char* szFileName)
{
	CString strFileName = szFileName;
	CString strDedicatedFile;
	CString strFileNameMask;
	CString strFileExtMask;
	map<CString, CString>::iterator itDedicatedFile;

//	strFileName = strFileName.MakeLower();

	for (itDedicatedFile = pPath->m_mapRajonDedicatedFile.begin(); itDedicatedFile != pPath->m_mapRajonDedicatedFile.end(); ++itDedicatedFile)
	{
		strDedicatedFile = itDedicatedFile->first;
		//��������� ����� � ����� �����
		int nSeparate = strDedicatedFile.Find("|");
		if (nSeparate == -1)
		{
			return "";
		}
		strFileNameMask = strDedicatedFile.Left(nSeparate);
		nSeparate++;
		//��������� ����� � ���������� �����
		strFileExtMask = strDedicatedFile.Right(strDedicatedFile.GetLength()-nSeparate);
		if (strFileNameMask == strFileName.Left(strFileNameMask.GetLength()))
		{
			//��������, ��� ����� ���������� ������ ���� ����������
			if (strFileExtMask != "*")
			{
				if(strFileExtMask == strFileName.Right(strFileExtMask.GetLength()))
				{
					//����� ����� ����� � ���������� - �������
					return itDedicatedFile->second;
				}
			}
			else
			{
				//���������� ����� ���� �����, � ����� �� ����� ����� - �������
				return itDedicatedFile->second;
			}
		}
	}

	return "";
}
