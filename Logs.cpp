#include "StdAfx.h"
#include "Logs.h"

CLogs::CLogs(void)
{
}

CLogs::~CLogs(void)
{
}

// Запись данных об оперциях в лог
void CLogs::WriteInFileLog(CString* szLogFileName, char* szLogString)
{
	CString strFileLog = szLogFileName->GetBuffer();
	CString strLogString;
	CString strTimeStamp;
	CFile pFile;
	SYSTEMTIME pTime;

	szLogFileName->ReleaseBuffer();
	GetLocalTime(&pTime);
	strFileLog = strFileLog.Mid(0, strFileLog.GetLength() - 4);
	strTimeStamp.Format("_%d_%02d_%02d.txt", pTime.wYear, pTime.wMonth,pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);
	strFileLog += strTimeStamp;

	strTimeStamp.Empty();
	strTimeStamp.Format("[%d.%02d.%02d %02d:%02d:%02d] ", pTime.wYear, pTime.wMonth,pTime.wDay, pTime.wHour, pTime.wMinute, pTime.wSecond);

	strLogString = strTimeStamp;
	strLogString += "\n";
	strLogString += szLogString;
	strLogString += "\n";

	pFile.Open(strFileLog, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareExclusive, NULL);

	pFile.SeekToEnd();
	pFile.Write(strLogString, strLogString.GetLength());
	pFile.Flush();
	pFile.Close();
	/*if(pFile.GetLength() > 2097152) //2MB
	{	
		strTimeStamp.Format(".%02d-%02d-%02d.%d-%02d-%02d", pTime.wHour,pTime.wMinute,pTime.wSecond,pTime.wYear,pTime.wMonth,pTime.wDay);
		pFile.Close();
		CString strNewFileName = strFileLog;
		strNewFileName += strTimeStamp;
		
		CFile::Rename(strFileLog.GetBuffer(), strNewFileName.GetBuffer());
		strFileLog.ReleaseBuffer();
		strNewFileName.ReleaseBuffer();
	}
	else
	{
		pFile.Close();	
	}*/
}
