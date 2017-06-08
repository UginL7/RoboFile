#include "StdAfx.h"
#include "KievPost.h"

CKievPost::CKievPost(void)
{
	//Получаем список путей к директориям
	pPath = new CInitPath;
	pFileInfo = new CGetFileInfo(pPath);
	pLog = new CLogs;	
}

CKievPost::~CKievPost(void)
{
	if (pLog != NULL)
	{
		delete pLog;
	}
	if (pPath != NULL)
	{
		delete pPath;
	}
	if (pFileInfo != NULL)
	{
		delete pFileInfo;
	}
}
// Перемещение файлов в папку отправки на Киев
list<CString> CKievPost::SendPostToKiev()
{
	char szInfoBuf[1024] = {0};
	list<CString> m_InfoAboutCopyFile;
	
	map<CString, CString>::iterator itKievSrcDst;
	itKievSrcDst = pPath->m_mapKievSrcDst.begin();
	strDstFile = itKievSrcDst->first;
	strSrcFile = itKievSrcDst->second;

	// Получаем список всех файлов из папки
	GetNameFileMailFromFolder(strSrcFile.GetBuffer());
	strSrcFile.ReleaseBuffer();

	for(itFileName = vFileName.begin();itFileName != vFileName.end(); ++itFileName)
	{
		//проверка на правильность имени файла
		m_strDstFolder = strDstFile;
		//формирование полного пути к файлу
		m_strDstFolder += *itFileName;

		m_strSrcFolder = strSrcFile;
		//формирование полного пути к файлу
		m_strSrcFolder += *itFileName;
		if (VerifyFileName(itFileName->GetBuffer()) == true)
		{
			if(bErrorDirMove == false)
			{	
				sprintf_s(szInfoBuf, "Копирование файла '%s' : удачно", *itFileName);
				m_InfoAboutCopyFile.push_back(szInfoBuf);
			}
		}
		else
		{
			sprintf_s(szInfoBuf, "Копирование файла '%s' : отложено", *itFileName);
			m_InfoAboutCopyFile.push_back(szInfoBuf);
		}
		itFileName->ReleaseBuffer();
		
	}	
	return m_InfoAboutCopyFile;
}

// Получаем список файлов подготовленных на отправку в Киев из ГНС в Запорожской области и
// принятых из Киева для ГНС в Запорожской области
void CKievPost::GetNameFileMailFromFolder(char* szFolderPath)
{
	HANDLE hFindFile;
	WIN32_FIND_DATA pFindData;
	CString pPath = szFolderPath;
	CString sDelFileName;
	CString strFile;
	CString sLog;
	pPath += "*";

	//ищем файлы в папке
	hFindFile = FindFirstFile(pPath, &pFindData);
	do 
	{
		if (!(pFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//приводим к нижнему регистру
			strFile = pFindData.cFileName;
			strFile = strFile.MakeLower();
			//добавляем только имя файла в список
			if(strFile.Left(2) == "tr")
			{
				// Запрет отправки транзитных файлов. 
				// Письмо из Киева № 39471/7/99-99-09-06-02-17 от 12.12.2016
				sDelFileName = pPath.Left(pPath.GetLength() - 1);
				sDelFileName += strFile;
				DeleteFile(sDelFileName);
				sLog.Format("'%s' - Удалён! Причина - Письмо из Киева № 39471/7/99-99-09-06-02-17 от 12.12.2016.",strFile);
				this->pLog->WriteInFileLog(&this->pPath->m_LogOutTranzit, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
			else
			{
				vFileName.push_back(strFile);
			}
		}		
	} while (FindNextFile(hFindFile, &pFindData));
}

// Проверка на правильность имени файла для отправки
bool CKievPost::VerifyFileName(char* strFileName)
{
	CString sLog;
	CString pFileName = strFileName;
	int nDotPos = 0;
	char szBufMsg[1024] = {0};
	list<CString> m_ListSpecMask;
	list<CString>::iterator it_ListSpecMask;	

//	pFileName = pFileName.MakeLower();

	m_ListSpecMask = pPath->m_listMask;
	
	// Цикл обработки имени файла
	do 
	{
		memset(szDomainName, 0, 1024);
		memset(szFileOwner, 0, 1024);
		//Получение информации о владельце файла
		pFileInfo->GetFileOwner(m_strSrcFolder.GetBuffer(), szFileOwner, szDomainName);

		// Проверка имени файла со спецмаской
		it_ListSpecMask = m_ListSpecMask.begin();
		for(; it_ListSpecMask != m_ListSpecMask.end(); ++it_ListSpecMask)
		{	
			if (pFileName.Left(2) == *it_ListSpecMask)
			{
				//Создание необходимой папки и копирование файла в архив ОГНС
				if (CreateArcDir(strFileName, "Киев") == true)
				{					
					return true;
				}
				else
				{
					// ошибка if (CreateArcDir(strFileName) == true)
					return false;						
				}
			}
		}	

		// проверка длины имени файла
		if (pFileName.GetLength() < 13)
		{
			// Поиск пробела в имени файла
			if(pFileName.Find(" ", 0) == -1)
			{
				if(pFileInfo->CheckFileNameSymbol(pFileName.GetBuffer()) == true)
				{
					// Имя файла в порядке
					pFileName.ReleaseBuffer();
					//Проверка размера файла
					if(pFileInfo->CheckFileSize(m_strSrcFolder.GetBuffer()) == true)
					{	
						//Создание необходимой папки и копирование файла в архив ОГНС
						if (CreateArcDir(strFileName, "Киев") == true)
						{					
							return true;
						}
						else
						{
							// ошибка if (CreateArcDir(strFileName) == true)
							return false;						
						}
					}
					else
					{	
						// if(pFileInfo->CheckFileSize(m_strSrcFolder.GetBuffer()) == true)
						SYSTEMTIME pTime;
						GetLocalTime(&pTime);
						if ((pPath->wMorningTime < pTime.wHour) && (pTime.wHour > pPath->wEveningTime))
						{
							// Ночное время
							m_strDstFolder = pPath->m_PathOutBigFile;
							m_strDstFolder += pFileName;
							MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
							sLog.Format("'%s' Привышен размер файла. Допустимый размер файла с %2d до %2d - %d*1024*1024 байт, с %2d до %2d - %d*1024*1024 байт. Файл перемещён в папку %s.\nВладелец %s\\%s", pFileName, pPath->wMorningTime, pPath->wEveningTime, pPath->dwDayTimeFileSize, pPath->wEveningTime, pPath->wMorningTime, pPath->dwNightTimeFileSize, pPath->m_PathOutBigFile, szDomainName, szFileOwner);
							pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
							sLog.ReleaseBuffer();
							return true;
						}
						else
						{
							// Рабочее время
							sLog.Format("'%s' Привышен размер файла. Допустимый размер файла с %2d до %2d - %d*1024*1024 байт, с %2d до %2d - %d*1024*1024 байт.\nВладелец %s\\%s", pFileName, pPath->wMorningTime, pPath->wEveningTime, pPath->dwDayTimeFileSize, pPath->wEveningTime, pPath->wMorningTime, pPath->dwNightTimeFileSize, szDomainName, szFileOwner);
							pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
							sLog.ReleaseBuffer();
							return false;
						}
					}	
					m_strSrcFolder.ReleaseBuffer();
				}
				else 
				{
					// ошибка if(CheckFileNameSymbol(pFileName.GetBuffer()) == true)
					m_strDstFolder = pPath->m_PathOutError;
					m_strDstFolder += pFileName;
					MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
					sprintf_s(szBufMsg, "'%s' - неверное имя файла, перемещён в папку '%s'.\nВладелец %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
					pLog->WriteInFileLog(&pPath->m_LogToKiev, szBufMsg);
					return false;
				}				
			}
			else
			{
				//ошибка if(pFileName.Find(" ", 0)!= -1)
				m_strDstFolder = pPath->m_PathOutError;
				m_strDstFolder += pFileName;
				MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
				sprintf_s(szBufMsg, "'%s' - неверное имя файла, перемещён в папку '%s'\n>присутствует пробел в имени файла.\nВладелец %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToKiev, szBufMsg);
				return false;
			}
		}
		else
		{
			//ошибка if (pFileName.GetLength() != 11)
			m_strDstFolder = pPath->m_PathOutError;
			m_strDstFolder += pFileName;
			MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
			sprintf_s(szBufMsg, "'%s' - неверное имя файла, перемещён в папку '%s'\n>имя файла должно быть в формате '8.3'.\nВладелец %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToKiev, szBufMsg);
			return false;
		}
	} while (FALSE);
}



// Создание директории для конкретного управления, с текущим месяцем и днём
bool CKievPost::CreateArcDir(char* szFileName, char *szDestination)
{
	map<CString, CString>::iterator itSpecFileKiev;
	bool bRet = false;
	bool bSpecFile = false;
	int nPos = 0;
	CString sFileName = szFileName;
	CString sLog;
	CString sOutInPost;
	CString sDstFullPath;
	CString sDirPath;
	CString sUprID;
	SYSTEMTIME pTime;
	// Получаем текущий день, месяц и год для структуры папок
	GetLocalTime(&pTime);
	
//	sFileName = sFileName.MakeLower();
	sOutInPost = "Отправлено";

	//Формирование пути каталога, в зависимости от типа файла

	bErrorDirMove = false;

	for (itSpecFileKiev = pPath->m_mapKievSpecFile.begin(); itSpecFileKiev != pPath->m_mapKievSpecFile.end(); ++itSpecFileKiev)
	{
		if (sFileName.Left(2) == itSpecFileKiev->first)
		{
			sDirPath.Format("%s\\%d\\%s\\Упр-%s\\%02d\\%s\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, itSpecFileKiev->second, pTime.wMonth, sOutInPost, pTime.wDay);
			bSpecFile = true;
			break;
		}
	}

	if (bSpecFile == false)
	{	
		nPos = sFileName.Find(".",0);
		if(nPos == -1)
		{
			this->bErrorDirMove = true;
		}
		else
		{
			sUprID = sFileName.Mid(nPos+1,2);
			if (sFileName.Left(2) == "tr")
			{
				if(sUprID.Left(1) == "8" || sUprID.Left(1) == "v")
				{
					sDirPath.Format("%s\\%d\\%s\\Транзит\\%02d\\%s\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth, sOutInPost, pTime.wDay);		
				}
				else
				{
					this->bErrorDirMove = true;
				}
			}
			else if ((sFileName.Left(2) < "99") && (sFileName.Left(2) > "00"))
			{
				if ((sUprID.Left(2) < "99") && (sUprID.Left(2) > "00"))
				{
					sDirPath.Format("%s\\%d\\%s\\Упр-%s\\%02d\\%s\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, sFileName.Left(2),pTime.wMonth, sOutInPost, pTime.wDay);
				}
				else
				{
					this->bErrorDirMove = true;
				}
			}
			else
			{
				if ((sUprID.Left(2) < "99") && (sUprID.Left(2) > "00"))
				{
					sDirPath.Format("%s\\%d\\%s\\Nostand\\%02d\\%s\\%02d", pPath->m_PathArcPost, pTime.wYear, szDestination, pTime.wMonth, sOutInPost, pTime.wDay);
				}
				else
				{
					this->bErrorDirMove = true;
				}
			}
		}
	}

	if (bErrorDirMove == false)
	{
		// Создаём каталог в Архиве ОГНС
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

		// Формирование пути файла для копирования
		sDstFullPath = sDirPath;
		sDstFullPath += "\\";
		sDstFullPath += sFileName;
		
		// Проверка на существование файла в архиве, если такой есть, то дописываем в конец текущее время и дату
		sDstFullPath = pFileInfo->CheckExistingFile(sDstFullPath.GetBuffer());
		sDstFullPath.ReleaseBuffer();
		
		//Перемещение файла в архив и в папку отправки
	
		bRet = CopyFileToDirectory(sFileName.GetBuffer(), sDstFullPath.GetBuffer(), sDirPath.GetBuffer());
		sDstFullPath.ReleaseBuffer();
		sDirPath.ReleaseBuffer();
	}
	else
	{
		bRet = MoveFileToErrorDirectoty(sFileName.GetBuffer());
	}

	sFileName.ReleaseBuffer();
	return bRet;
}

// Перемещение файла в папку Ошибки для района, без отправки на Киев
bool CKievPost::MoveFileToErrorDirectoty(char *szFileName)
{
	CString pFileName = szFileName;
	char szBufMsg[1024] = {0};

	m_strDstFolder = pPath->m_PathOutError;
	m_strDstFolder += pFileName;
	MoveFileEx(m_strSrcFolder, m_strDstFolder, MOVEFILE_REPLACE_EXISTING );
	sprintf_s(szBufMsg, "'%s' - неверное имя файла, перемещён в папку '%s'.\nВладелец %s\\%s", pFileName, pPath->m_PathOutError,szDomainName,szFileOwner);
	pLog->WriteInFileLog(&pPath->m_LogToKiev, szBufMsg);
	return false;
}

// Копирование файла в архив ОГНС и на отправку
bool CKievPost::CopyFileToDirectory(char* szFileName, char* szDstFullPath, char* szDirPah)
{
	CString sFileName;
	CString sDstFullPath;
	CString sDirPath;
	CString sLog;
	sDirPath = szDirPah;
	sFileName = szFileName;
	sDstFullPath = szDstFullPath;
	// Копирование файла в архив ОГНС
	if(CopyFile(m_strSrcFolder, sDstFullPath, FALSE) == TRUE)
	{
		sLog.Format("'%s' + удачно скопирован в папку '%s\\'.\nВладелец %s\\%s", sFileName, sDirPath,szDomainName, szFileOwner);
		pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
		sLog.ReleaseBuffer();

		// Копирование файла в папку для FossMail
		if(MoveFileEx(m_strSrcFolder, m_strDstFolder,MOVEFILE_REPLACE_EXISTING ))
		{
			sLog.Format("'%s' + удачно скопирован в папку '%s'.\nВладелец %s\\%s", sFileName, strDstFile, szDomainName, szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
			sLog.ReleaseBuffer();
			return true;
		}
		else
		{
			LPTSTR msg;
			DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
			if (res != 0)
			{
				sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - %s\nВладелец %s\\%s",sFileName, strDstFile, msg, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
				sLog.ReleaseBuffer();
				LocalFree( msg );
			}
			else
			{
				sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - неизвестно!\nВладелец %s\\%s",sFileName, strDstFile, szDomainName, szFileOwner);
				pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
			return false;
		}
	}
	else
	{	
		LPTSTR msg;
		DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
		if (res != 0)
		{
			sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - %s\nВладелец %s\\%s",sFileName, strDstFile, msg, szDomainName, szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
			sLog.ReleaseBuffer();
			LocalFree( msg );
		}
		else
		{
			sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - неизвестно!\nВладелец %s\\%s",sFileName, strDstFile, szDomainName, szFileOwner);
			pLog->WriteInFileLog(&pPath->m_LogToKiev, sLog.GetBuffer());
			sLog.ReleaseBuffer();
		}
		return false;
	}
}

