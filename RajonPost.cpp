#include "StdAfx.h"
#include "RajonPost.h"

CRajonPost::CRajonPost(void)
{
	//�������� ������ ����� � �����������
	pPath = new CInitPath;
	pFileInfo = new CGetFileInfo(pPath);
	pLog = new CLogs;
}

CRajonPost::~CRajonPost(void)
{
	if (pFileInfo != NULL)
	{
		delete pFileInfo;
	}
	if (pLog != NULL)
	{
		delete pLog;
	}
	if (pPath != NULL)
	{
		delete pPath;
	}
}

// �������� ����� �� ���������� �����
list<CString> CRajonPost::SendPostToRajon()
{
	list<CString> m_InfoAboutCopyFile;

	for(it_RajonSrcDst = pPath->m_mapSGNISrcDst.begin();it_RajonSrcDst != pPath->m_mapSGNISrcDst.end(); ++it_RajonSrcDst)
	{		
		this->bErrorDirMove = false;
		strSrcFile = it_RajonSrcDst->second;
		strDstFile = it_RajonSrcDst->first;

		// �������� ������ ���� ������ �� �����
		GetNameFileMailFromFolder(strSrcFile.GetBuffer());
		strSrcFile.ReleaseBuffer();
		for(itFileName = vFileName.begin();itFileName != vFileName.end(); ++itFileName)
		{
			//�������� �� ������������ ����� �����
			m_strDstFolder = strDstFile;
			m_strDstFolder += *itFileName;

			m_strSrcFolder = strSrcFile;
			m_strSrcFolder += *itFileName;
			if (VerifyFileName(itFileName->GetBuffer()) == true)
			{
				if(this->bErrorDirMove == true)
				{
					break;
				}
				// ����������� ����� � ����� ����
				if(MoveFile(m_strSrcFolder, sDstFullPath) == TRUE)
				{
					sLog.Format("'%s' + ������ ���������� � ����� '%s'.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
					pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
					sLog.ReleaseBuffer();
				}
				else
				{	
					sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
					pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
					sLog.ReleaseBuffer();
				}
				sLog.Format( "����������� ����� '%s' : ������", *itFileName);
				m_InfoAboutCopyFile.push_back(sLog);
			}
			else
			{
				sLog.Format("����������� ����� '%s' : ��������", *itFileName);
				m_InfoAboutCopyFile.push_back(sLog);
			}
			itFileName->ReleaseBuffer();			
		}
	}
	vFileName.clear();
	return m_InfoAboutCopyFile;
}


// �������� ����� �� ��� ������ 
list<CString> CRajonPost::SendPostToAllODPI()
{
	bool bFileWork;
	list<CString> m_InfoAboutCopyFile;

	strSrcFile = pPath->m_PathOutAllODPI;
	// �������� ������ ���� ������ �� �����
	GetNameFileMailFromFolder(strSrcFile.GetBuffer());
	strSrcFile.ReleaseBuffer();

	for(itFileName = vFileName.begin();itFileName != vFileName.end(); ++itFileName)
	{
		bFileWork = false;
		this->bErrorDirMove = false;

		for(it_RajonSrcDst = pPath->m_mapODPISrcDst.begin();it_RajonSrcDst != pPath->m_mapODPISrcDst.end(); ++it_RajonSrcDst)
		{
			strDstFile = it_RajonSrcDst->first;
			//�������� �� ������������ ����� �����
			m_strDstFolder = strDstFile;
			m_strDstFolder += *itFileName;

			m_strSrcFolder = strSrcFile;
			m_strSrcFolder += *itFileName;

			bFileWork = VerifyFileName(itFileName->GetBuffer());
			if (this->bErrorDirMove == false)
			{	
				if ( bFileWork == true)
				{
					sLog.Format( "����������� ����� '%s' : ������", *itFileName);
					m_InfoAboutCopyFile.push_back(sLog);
				}
				else
				{
					sLog.Format("����������� ����� '%s' : ��������", *itFileName);
					m_InfoAboutCopyFile.push_back(sLog);
					break;
				}
				itFileName->ReleaseBuffer();
			}
			else
			{
				break;
			}
		}	

		if(bFileWork == true && bErrorDirMove == false)
		{
			// ����������� ����� � ����� ����
			if(MoveFile(m_strSrcFolder, sDstFullPath) == TRUE)
			{
				sLog.Format("'%s' + ������ ���������� � ����� '%s'.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
			else
			{	
				sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
		}
	}
	vFileName.clear();
	return m_InfoAboutCopyFile;
}

// �������� ����� �� ��� ������ 
list<CString> CRajonPost::SendPostToAllRajon()
{
	bool bFileWork;
	list<CString> m_InfoAboutCopyFile;

	strSrcFile = pPath->m_PathOutAllRajonPost;
	// �������� ������ ���� ������ �� �����
	GetNameFileMailFromFolder(strSrcFile.GetBuffer());
	strSrcFile.ReleaseBuffer();

	for(itFileName = vFileName.begin();itFileName != vFileName.end(); ++itFileName)
	{
		bFileWork = false;
		this->bErrorDirMove = false;

		for(it_RajonSrcDst = pPath->m_mapRajonSrcDst.begin();it_RajonSrcDst != pPath->m_mapRajonSrcDst.end(); ++it_RajonSrcDst)
		{
			strDstFile = it_RajonSrcDst->first;
			//�������� �� ������������ ����� �����
			m_strDstFolder = strDstFile;
			m_strDstFolder += *itFileName;

			m_strSrcFolder = strSrcFile;
			m_strSrcFolder += *itFileName;

			bFileWork = VerifyFileName(itFileName->GetBuffer());
			if (this->bErrorDirMove == false)
			{	
				if ( bFileWork == true)
				{
					sLog.Format( "����������� ����� '%s' : ������", *itFileName);
					m_InfoAboutCopyFile.push_back(sLog);
				}
				else
				{
					sLog.Format("����������� ����� '%s' : ��������", *itFileName);
					m_InfoAboutCopyFile.push_back(sLog);
					break;
				}
				itFileName->ReleaseBuffer();
			}
			else
			{
				break;
			}
		}	

		if(bFileWork == true && bErrorDirMove == false)
		{
			// ����������� ����� � ����� ����
			if(MoveFile(m_strSrcFolder, sDstFullPath) == TRUE)
			{
				sLog.Format("'%s' + ������ ���������� � ����� '%s'.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
			else
			{	
				sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
		}
	}
	vFileName.clear();
	return m_InfoAboutCopyFile;
}

// �������� ����� �� ��� ������ + ����
list<CString> CRajonPost::SendPostToAllRajonSGNI()
{
	bool bFileWork;
	list<CString> m_InfoAboutCopyFile;
	strSrcFile = pPath->m_PathOutAllRajonSGNIPost;
	// �������� ������ ���� ������ �� �����
	GetNameFileMailFromFolder(strSrcFile.GetBuffer());
	strSrcFile.ReleaseBuffer();

	for(itFileName = vFileName.begin();itFileName != vFileName.end(); ++itFileName)
	{	
		bFileWork = false;
		this->bErrorDirMove = false;
		for(it_RajonSrcDst = pPath->m_mapSGNISrcDst.begin();it_RajonSrcDst != pPath->m_mapSGNISrcDst.end(); ++it_RajonSrcDst)
		{	
			strDstFile = it_RajonSrcDst->first;
			//�������� �� ������������ ����� �����
			m_strDstFolder = strDstFile;
			m_strDstFolder += *itFileName;

			m_strSrcFolder = strSrcFile;
			m_strSrcFolder += *itFileName;

			bFileWork = VerifyFileName(itFileName->GetBuffer());
			if (this->bErrorDirMove == false)
			{
				if ( bFileWork == true)
				{
					sLog.Format( "����������� ����� '%s' : ������", *itFileName);
					m_InfoAboutCopyFile.push_back(sLog);
				}
				else
				{
					sLog.Format("����������� ����� '%s' : ��������", *itFileName);
					m_InfoAboutCopyFile.push_back(sLog);
				}
				itFileName->ReleaseBuffer();
			}
		}	

		if(bFileWork == true && bErrorDirMove == false)
		{
			// ����������� ����� � ����� ����
			if(MoveFile(m_strSrcFolder, sDstFullPath) == TRUE)
			{
				sLog.Format("'%s' + ������ ���������� � ����� '%s'.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
			else
			{	
				sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n�������� %s\\%s", *itFileName, strArcPath, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
		}
	}
	vFileName.clear();
	return m_InfoAboutCopyFile;
}

// �������� �� ������������ ����� ����� ��� ��������
bool CRajonPost::VerifyFileName(char* strFileName)
{
	CString sLog;
	CString pFileName = strFileName;
	int nDotPos = 0;
	char szBufMsg[1024] = {0};
	map<CString, CString>::iterator itStandFile;
	bool bStandFile = false;
	list<CString> m_ListSpecMask;
	list<CString>::iterator it_ListSpecMask;	
	//	pFileName = pFileName.MakeLower();
	m_ListSpecMask = pPath->m_listMask;

	// ���� ��������� ����� �����
	do 
	{
		memset(szDomainName, 0, 1024);
		memset(szFileOwner, 0, 1024);
		//��������� ���������� � ��������� �����
		pFileInfo->GetFileOwner(m_strSrcFolder.GetBuffer(), szFileOwner, szDomainName);

		// �������� ����� ����� �� ����������
		it_ListSpecMask = m_ListSpecMask.begin();
		for(; it_ListSpecMask != m_ListSpecMask.end(); ++it_ListSpecMask)
		{	
			if (pFileName.Left(2) == *it_ListSpecMask)
			{
				//�������� ����������� ����� � ����������� ����� � ����� ����
				if (CreateArcDir(strFileName, "������") == true)
				{					
					return true;
				}
				else
				{
					// ������ if (CreateArcDir(strFileName) == true)
					return false;						
				}
			}
		}	

		//�������� ����� ����� � ����������� ��������� �������
		for(itStandFile = pPath->m_mapRajonSpecFile.begin();itStandFile  != pPath->m_mapRajonSpecFile.end(); ++itStandFile )
		{
			CString sFirst = itStandFile->first;
			if (pFileName.Left(2) == sFirst)
			{
				bStandFile = true;
				break;
			}
		}
		// �������� ����� ����� �����
		if (pFileName.GetLength() < 13 || bStandFile == true)
		{
			// ����� ������� � ����� �����
			if(pFileName.Find(" ", 0) == -1)
			{
				if(pFileInfo->CheckFileNameSymbol(pFileName.GetBuffer()) == true)
				{
					// ��� ����� � �������
					pFileName.ReleaseBuffer();
					//�������� ������� �����
					if(pFileInfo->CheckFileSize(m_strSrcFolder.GetBuffer()) == true)
					{	
						//�������� ����������� ����� � ����������� ����� � ����� ����
						if (CreateArcDir(strFileName, "������") == true)
						{					
							return true;
						}
						else
						{
							// ������ if (CreateArcDir(strFileName) == true)
							return false;						
						}
					}
					else
					{	
						// if(pFileInfo->CheckFileSize(m_strSrcFolder.GetBuffer()) == true)
						if (this->bErrorDirMove == true)
						{
							return false;
						}
						SYSTEMTIME pTime;
						GetLocalTime(&pTime);
						if ((pPath->wMorningTime < pTime.wHour) && (pTime.wHour > pPath->wEveningTime))
						{
							// ������ �����
							m_strDstFolder = pPath->m_PathOutBigFile;
							m_strDstFolder += pFileName;
							MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
							sLog.Format("'%s' - �������� ������ �����. ���������� ������ ����� � %2d �� %2d - %d*1024*1024 ����, � %2d �� %2d - %d*1024*1024 ����. ���� ��������� � ����� %s.\n�������� %s\\%s", pFileName, pPath->wMorningTime, pPath->wEveningTime, pPath->dwDayTimeFileSize, pPath->wEveningTime, pPath->wMorningTime, pPath->dwNightTimeFileSize, pPath->m_PathOutBigFile, szDomainName, szFileOwner);
							pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
							sLog.ReleaseBuffer();
							return false;
						}
						else
						{
							// ������� �����
							sLog.Format("'%s' - �������� ������ �����. ���������� ������ ����� � %2d �� %2d - %d*1024*1024 ����, � %2d �� %2d - %d*1024*1024 ����.\n�������� %s\\%s", pFileName, pPath->wMorningTime, pPath->wEveningTime, pPath->dwDayTimeFileSize, pPath->wEveningTime, pPath->wMorningTime, pPath->dwNightTimeFileSize, szDomainName, szFileOwner);
							pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
							sLog.ReleaseBuffer();
							return false;
						}
					}	
					m_strSrcFolder.ReleaseBuffer();
				}
				else 
				{
					// ������ if(CheckFileNameSymbol(pFileName.GetBuffer()) == true)
					m_strDstFolder = pPath->m_PathOutError;
					m_strDstFolder += pFileName;
					MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
					sprintf_s(szBufMsg, "'%s' - �������� ��� �����, ��������� � ����� '%s'\n>.\n�������� %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
					pLog->WriteInFileLog(&pPath->m_LogToRajon, szBufMsg);
					return false;
				}				
			}
			else
			{
				//������ if(pFileName.Find(" ", 0)!= -1)
				m_strDstFolder = pPath->m_PathOutError;
				m_strDstFolder += pFileName;
				MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
				sprintf_s(szBufMsg, "'%s' - �������� ��� �����, ��������� � ����� '%s'\n>������������ ������ � ����� �����.\n�������� %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToRajon, szBufMsg);
				return false;
			}
		}
		else
		{
			//������ if (pFileName.GetLength() != 11)
			m_strDstFolder = pPath->m_PathOutError;
			m_strDstFolder += pFileName;
			MoveFileEx(m_strSrcFolder, m_strDstFolder,MOVEFILE_REPLACE_EXISTING );
			sprintf_s(szBufMsg, "'%s' - �������� ��� �����, ��������� � ����� '%s'\n>��� ����� ������ ���� � ������� '8.3'.\n�������� %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToRajon, szBufMsg);
			return false;
		}
	} while (FALSE);
}
// �������� ������ ������ �������������� �� �������� � ������ �� ��� � ����������� �������
void CRajonPost::GetNameFileMailFromFolder(char* pFolderPath)
{
	HANDLE hFindFile;
	WIN32_FIND_DATA pFindData;
	CString pPath = pFolderPath;
	CString strFile;
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
				//��������� ������ ��� ����� � ������
				vFileName.push_back(strFile);
			}		
		} while (FindNextFile(hFindFile, &pFindData));		
	}
}

// �������� ���������� ��� ����������� ����������, � ������� ������� � ����
bool CRajonPost::CreateArcDir(char* szFileName, char *szDestination)
{
	map<CString, CString>::iterator itSpecFile;
	bool bSpecFile = false;
	bool bDedicatedFile = false;
	bool bRet = false;
	int nPos;
	CString sPostArcDirectory;
	CString sFileName = szFileName;
	CString sLog;
	CString sDirPath;
	CString sUprID;
	SYSTEMTIME pTime;
	// �������� ������� ����, ����� � ��� ��� ��������� �����
	GetLocalTime(&pTime);

	bErrorDirMove = false;

	//������������ ���� ��������, � ����������� �� ���� �����

	//�������� ���������
	for(itSpecFile = pPath->m_mapRajonSpecFile.begin();itSpecFile != pPath->m_mapRajonSpecFile.end(); ++itSpecFile)
	{
		if (sFileName.Left(2) == itSpecFile->first)
		{
			sDirPath.Format("%s\\%d\\%s\\���-%s\\%02d\\����������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, itSpecFile->second, pTime.wMonth, pTime.wDay);
			bSpecFile = true;
			break;
		}
	}

	//�������� ������ ��������������� �� ��������� ��� �������
	sPostArcDirectory = CheckDedicatedFile(szFileName);
	if (!sPostArcDirectory.IsEmpty())
	{
		sDirPath.Format("%s\\%d\\%s\\%s\\%02d\\����������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, sPostArcDirectory, pTime.wMonth, pTime.wDay);
	}

	//������ �� ������������� �������
	if (bSpecFile == false && sPostArcDirectory.IsEmpty())
	{
		nPos = sFileName.Find(".",0);
		if(nPos == -1)
		{
			bErrorDirMove = true;
		}
		else
		{
			sUprID = sFileName.Mid(nPos+1,2);
			if (sFileName.Left(2) == "tr")
			{
				//����: �������
				if (sUprID.Left(1) == "8" || sUprID.Left(1) == "v")
				{
					sDirPath.Format("%s\\%d\\%s\\�������\\%02d\\����������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth,  pTime.wDay);
				}
				else
				{
					bErrorDirMove = true;
				}
			}
			else if ((sFileName.Left(2) < "99") && (sFileName.Left(2) > "00"))
			{
				//����: �������
				if ((sUprID.Left(2) < "99") && (sUprID.Left(2) > "00"))
				{
					sDirPath.Format("%s\\%d\\%s\\���-%s\\%02d\\����������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, sFileName.Left(2),pTime.wMonth, pTime.wDay);
				}
				else
				{
					bErrorDirMove = true;
				}
			}
			else if ((sFileName.Left(1) == "t") || (sFileName.Left(1) == "r"))
			{
				//����: ������ ��������� ��������� T-�������, R-�������
				//��������� ���� ���������� �� ����� �����

				if ((sUprID.Left(2) < "99") && (sUprID.Left(2) > "00"))
				{
					sDirPath.Format("%s\\%d\\%s\\���-%s\\%02d\\����������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, sUprID, pTime.wMonth, pTime.wDay);
				}
				else
				{
					bErrorDirMove = true;
				}
			}
			else if ((sFileName.Left(4) == "cor_"))
			{
				sDirPath.Format("%s\\%d\\%s\\Nostand\\%02d\\����������\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth, pTime.wDay);
			}
			else
			{
				bErrorDirMove = true;
			}
		}
	}

	if (this->bErrorDirMove == false)
	{
		// ������ ������� � ������ ����
		for(int i = 0; i < sDirPath.GetLength(); i++)
		{
			if (sDirPath.Mid(i, 1) == "\\")
			{
				CreateDirectory(sDirPath.Left(i), NULL);
				if (GetLastError()!= 0)
				{
					SetLastError(0);
				}
			}			
		}
		CreateDirectory(sDirPath, NULL);

		// ������������ ���� ����� ��� �����������
		sDstFullPath = sDirPath;
		sDstFullPath += "\\";
		strArcPath = sDstFullPath;
		sDstFullPath += sFileName;

		// �������� �� ������������� ����� � ������, ���� ����� ����, �� ���������� � ����� ������� ����� � ����
		sDstFullPath = pFileInfo->CheckExistingFile(sDstFullPath.GetBuffer());
		sDstFullPath.ReleaseBuffer();

		//����������� ����� � ����� � � ����� ��������
		bRet = CopyFileToDirectory(sFileName.GetBuffer());
	}
	else
	{
		// ����������� ����� � ����� ������ ��� ������
		bRet = MoveFileToErrorDirectoty(sFileName.GetBuffer());
	}

	sFileName.ReleaseBuffer();

	return bRet;
}


// ����������� ����� � ����� ������ ��� ������, ��� �������� �� �����
bool CRajonPost::MoveFileToErrorDirectoty(char *szFileName)
{
	CString pFileName = szFileName;
	char szBufMsg[1024] = {0};

	m_strDstFolder = pPath->m_PathOutError;
	m_strDstFolder += pFileName;
	MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
	sprintf_s(szBufMsg, "'%s' - �������� ��� �����, ��������� � ����� '%s'\n>.\n�������� %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
	pLog->WriteInFileLog(&pPath->m_LogToRajon, szBufMsg);

	return false;
}

// ����������� ����� � ����� ���� � �� ��������
bool CRajonPost::CopyFileToDirectory(char* szFileName)
{
	CString sFileName;
	CString sLog;

	sFileName = szFileName;

	// ����������� ����� � ����� ��� FossMail
	if(CopyFile(m_strSrcFolder, m_strDstFolder, FALSE))
	{
		sLog.Format("'%s' + ������ ���������� � ����� '%s'.\n�������� %s\\%s", sFileName, strDstFile, szDomainName, szFileOwner);
		pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
		sLog.ReleaseBuffer();
		return true;
	}
	else
	{
		LPTSTR msg;
		DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
		if (res != 0)
		{
			sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n������� - %s\n�������� %s\\%s",sFileName, strDstFile, msg, szDomainName, szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
			sLog.ReleaseBuffer();
			LocalFree( msg );
		}
		else
		{
			sLog.Format("'%s' - � ����� '%s' ����������� �� �������.\n������� - ����������!\n�������� %s\\%s",sFileName, strDstFile, szDomainName, szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToRajon, sLog.GetBuffer());
			sLog.ReleaseBuffer();
		}
		return false;
	}
}
// �������� ������ ��������������� �� ��������� ��� �������
CString CRajonPost::CheckDedicatedFile(char* szFileName)
{
	CString strFileName = szFileName;
	CString strDedicatedFile;
	CString strFileNameMask;
	CString strFileExtMask;
	map<CString, CString>::iterator itDedicatedFile;

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
