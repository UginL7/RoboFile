#include "StdAfx.h"
#include "TranzitFiles.h"

CTranzitFiles::CTranzitFiles(void)
{
	pPath = new CInitPath;
	pLog = new CLogs;
}

CTranzitFiles::~CTranzitFiles(void)
{
	if (pPath != NULL)
	{
		delete pPath;
	}
	if (pLog != NULL)
	{
		delete pLog;
	}
}

// Определение транзитного файла и отправка по назначению
void CTranzitFiles::WorkingTranzitFiles(char* szFileName, char* szFromFolder, char* szToFolder, bool bKiev)
{
	strFromFolder = szFromFolder;
	strToFolder = szToFolder;
	strFileName = szFileName;
	bKievRec = bKiev;
//	strFileName = strFileName.MakeLower();

	if(bKievRec)
	{
		if(strFileName.Mid(2,3) == "801" || strFileName.Mid(2,3) == "000")
		{
			// только на область 801 или 000
			return;
		}
		else if(strFileName.Mid(2,1) == "8")
		{
			CopyToRajFromKiev();
		}
		else if(strFileName.Mid(2,1) == "z")
		{
			// на все районы
			CopyToAllRajFromKiev();
		}
	}
	else
	{
		// Копирует ТР с района на район
		if(strFileName.Mid(2,1) == "8")
		{
			CopyToRajFromRaj();
		}
		else
		{
			CopyToKievFromRaj();
		}
	}
}


// Копирование принятных от Киева транзитных файлов на соответствующий район(ы)
bool CTranzitFiles::CopyToRajFromKiev()
{
	CString strRajID = strFileName.Mid(3,2);
	CString strTmpSrcPath = strFromFolder;
	CString strTmpDstPath;
	CString sLog;

	strTmpSrcPath += strFileName;

	if (strRajID == "01")
	{
		//Транзитка была на область
		return true;
	}

	// Проверяем на какой район должен попасть TR и формируем соответсвующий путь
	for (it_TRRajonID = pPath->m_mapTRRajonID.begin(); it_TRRajonID != pPath->m_mapTRRajonID.end(); ++it_TRRajonID)
	{
		if(!strRajID.Compare(it_TRRajonID->first))
		{
			strTmpDstPath = it_TRRajonID->second;
			strTmpDstPath += strFileName;
			// формирование пути 
			// копирование из KievIn в FileBox
			if(CopyFile(strTmpSrcPath, strTmpDstPath, FALSE))
			{
				sLog.Format("'%s' + удачно скопирован в папку '%s'.", strFileName,  it_TRRajonID->second);

				pLog->WriteInFileLog(&pPath->m_LogInTranzit, sLog.GetBuffer());
				sLog.ReleaseBuffer();
				return true;
			}
			else
			{
				// Ошибка при копировании
				LPTSTR msg;
				DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
				if (res != 0)
				{
					sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - %s.",strFileName, it_TRRajonID->second, msg);
					pLog->WriteInFileLog(&pPath->m_LogInTranzit, sLog.GetBuffer());
					sLog.ReleaseBuffer();
					LocalFree( msg );
				}
				else
				{
					sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - неизвестно.",strFileName, it_TRRajonID->second);
					pLog->WriteInFileLog(&pPath->m_LogInTranzit, sLog.GetBuffer());
					sLog.ReleaseBuffer();
				}
				return false;
			}	
		}
	}
	return true;
}

// Копирование принятных от Киева транзитных файлов на соответствующий район(ы)
bool CTranzitFiles::CopyToAllRajFromKiev()
{
	CString strRajID = strFileName.Mid(3,2);
	CString strTmpSrcPath = strFromFolder;
	CString strTmpDstPath;
	CString sLog;

	strTmpSrcPath += strFileName;

	// Проверяем на какой район должен попасть TR и формируем соответсвующий путь
	for (it_TRRajonID = pPath->m_mapTRRajonID.begin(); it_TRRajonID != pPath->m_mapTRRajonID.end(); ++it_TRRajonID)
	{
		strTmpDstPath = it_TRRajonID->second;
		strTmpDstPath += strFileName;
		// формирование пути 
		// копирование с KievIn в FileBox
		if(CopyFile(strTmpSrcPath, strTmpDstPath, FALSE))
		{
			sLog.Format("'%s' + удачно скопирован в папку '%s'.", strFileName,  it_TRRajonID->second);

			pLog->WriteInFileLog(&pPath->m_LogInTranzit, sLog.GetBuffer());
			sLog.ReleaseBuffer();
		}
		else
		{
			// Ошибка при копировании
			LPTSTR msg;
			DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
			if (res != 0)
			{
				sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - %s.",strFileName, it_TRRajonID->second, msg);
				pLog->WriteInFileLog(&pPath->m_LogInTranzit, sLog.GetBuffer());
				sLog.ReleaseBuffer();
				LocalFree( msg );
			}
			else
			{
				sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - неизвестно.",strFileName, it_TRRajonID->second);
				pLog->WriteInFileLog(&pPath->m_LogInTranzit, sLog.GetBuffer());
				sLog.ReleaseBuffer();
			}
		}	
	}
	return true;
}

// Копирование принятных от района транзитных файлов на соответствующий район
bool CTranzitFiles::CopyToRajFromRaj()
{
	CString strRajID = strFileName.Mid(3,2);
	CString strTmpSrcPath = strFromFolder;
	CString strTmpDstPath;
	CString sLog;

	strTmpSrcPath += strFileName;

	if (strRajID == "01")
	{
		//Транзитка была на область
		return true;
	}

	// Проверяем на какой район должен попасть TR и формируем соответсвующий путь
	for (it_TRRajonID = pPath->m_mapTRRajonID.begin(); it_TRRajonID != pPath->m_mapTRRajonID.end(); ++it_TRRajonID)
	{
		if(!strRajID.Compare(it_TRRajonID->first))
		{
			strTmpDstPath = it_TRRajonID->second;
			strTmpDstPath += strFileName;
			// формирование пути 
			// копирование с Receive в FileBox
			if(CopyFile(strTmpSrcPath, strTmpDstPath, FALSE))
			{
				sLog.Format("'%s' + удачно скопирован в папку '%s'.", strFileName,  it_TRRajonID->second);

				pLog->WriteInFileLog(&pPath->m_LogOutTranzit, sLog.GetBuffer());
				sLog.ReleaseBuffer();
				return true;
			}
			else
			{
				// Ошибка при копировании
				LPTSTR msg;
				DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
				if (res != 0)
				{
					sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - %s.",strFileName, it_TRRajonID->second, msg);
					pLog->WriteInFileLog(&pPath->m_LogOutTranzit, sLog.GetBuffer());
					sLog.ReleaseBuffer();
					LocalFree( msg );
				}
				else
				{
					sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - неизвестно.",strFileName, it_TRRajonID->second);
					pLog->WriteInFileLog(&pPath->m_LogOutTranzit, sLog.GetBuffer());
					sLog.ReleaseBuffer();
				}
				return false;
			}	
		}
	}
	return true;
}

// Копирование принятных от района транзитных файлов на Киев
bool CTranzitFiles::CopyToKievFromRaj()
{
	CString strTmpSrcPath = strFromFolder;
	CString strTmpDstPath;
	CString sLog;
	map<CString, CString>::iterator itKievSrcDst;
	itKievSrcDst = pPath->m_mapKievSrcDst.begin();
	strTmpDstPath = itKievSrcDst->first;
	

	strTmpSrcPath += strFileName;
	strTmpDstPath += strFileName;

	// формирование пути 
	// копирование с Receive в KievBox
	if(CopyFile(strTmpSrcPath, strTmpDstPath, FALSE))
	{
		sLog.Format("'%s' + удачно скопирован в папку '%s'.", strFileName,  itKievSrcDst->first);

		pLog->WriteInFileLog(&pPath->m_LogOutTranzit, sLog.GetBuffer());
		sLog.ReleaseBuffer();
		return true;
	}
	else
	{
		// Ошибка при копировании
		LPTSTR msg;
		DWORD res= ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPTSTR)&msg, 0, NULL );
		if (res != 0)
		{
			sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - %s.",strFileName, itKievSrcDst->first, msg);
			pLog->WriteInFileLog(&pPath->m_LogOutTranzit, sLog.GetBuffer());
			sLog.ReleaseBuffer();
			LocalFree( msg );
		}
		else
		{
			sLog.Format("'%s' - в папку '%s' скопировать не удалось.\nПричина - неизвестно.",strFileName, itKievSrcDst->first);
			pLog->WriteInFileLog(&pPath->m_LogOutTranzit, sLog.GetBuffer());
			sLog.ReleaseBuffer();
		}
		return false;
	}	
}