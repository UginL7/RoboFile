#include "StdAfx.h"
#include "GetFileInfo.h"

CGetFileInfo::CGetFileInfo(CInitPath *pConfig )
{
	pPath = pConfig;
}

CGetFileInfo::~CGetFileInfo(void)
{
}

// Получение имени создателя файла
int CGetFileInfo::GetFileOwner(char* szFilepath, char *szAcc, char *szDomName)
{
	DWORD dwRtnCode = 0;
	PSID pSidOwner;
	BOOL bRtnBool = TRUE;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD;
	char* szAccount = 0;
	char* szDomainName = 0;

	// Get the handle of the file object.
	hFile = CreateFile(
		szFilepath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	dwFileSize = GetFileSize(hFile, NULL);

	// Check GetLastError for CreateFile error code.
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		TRACE(TEXT("CreateFile error = %d\n"), dwErrorCode);
		return -1;
	}

	// Allocate memory for the SID structure.
	pSidOwner = (PSID)GlobalAlloc(
		GMEM_FIXED,
		sizeof(PSID));

	// Allocate memory for the security descriptor structure.
	pSD = (PSECURITY_DESCRIPTOR)GlobalAlloc(
		GMEM_FIXED,
		sizeof(PSECURITY_DESCRIPTOR));

	// Get the owner SID of the file.
	dwRtnCode = GetSecurityInfo(
		hFile,
		SE_FILE_OBJECT,
		OWNER_SECURITY_INFORMATION,
		&pSidOwner,
		NULL,
		NULL,
		NULL,
		&pSD);

	// Check GetLastError for GetSecurityInfo error condition.
	if (dwRtnCode != ERROR_SUCCESS) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		TRACE(TEXT("GetSecurityInfo error = %d\n"), dwErrorCode);
		return -1;
	}

	// First call to LookupAccountSid to get the buffer sizes.
	bRtnBool = LookupAccountSid(
		NULL,           // local computer
		pSidOwner,
		szAccount,
		(LPDWORD)&dwAcctName,
		szDomainName,
		(LPDWORD)&dwDomainName,
		&eUse);

	// Reallocate memory for the buffers.
	szAccount = (char *)GlobalAlloc(
		GMEM_FIXED,
		dwAcctName);

	// Check GetLastError for GlobalAlloc error condition.
	if (szAccount == NULL) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		TRACE(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
		return -1;
	}

	szDomainName = (char *)GlobalAlloc(
		GMEM_FIXED,
		dwDomainName);

	// Check GetLastError for GlobalAlloc error condition.
	if (szDomainName == NULL) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		TRACE(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
		return -1;

	}

	// Second call to LookupAccountSid to get the account name.
	bRtnBool = LookupAccountSid(
		NULL,                          // name of local or remote computer
		pSidOwner,                     // security identifier
		szAccount,                      // account name buffer
		(LPDWORD)&dwAcctName,          // size of account name buffer 
		szDomainName,                    // domain name
		(LPDWORD)&dwDomainName,        // size of domain name buffer
		&eUse);                        // SID type

	CloseHandle(hFile);

	memcpy(szDomName, "Err", 1024);
	memcpy(szAcc, "Err", 1024);

	return 0;
	// Check GetLastError for LookupAccountSid error condition.
	if (bRtnBool == FALSE) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();

		if (dwErrorCode == ERROR_NONE_MAPPED)
			TRACE(TEXT("Account owner not found for specified SID.\n"));
		else 
			TRACE(TEXT("Error in LookupAccountSid.\n"));
		return -1;

	}
	else if (bRtnBool == TRUE) 
	{
		memcpy(szDomName, szDomainName, 1024);
		memcpy(szAcc, szAccount, 1024);
		return 0;
	}
	return 0;
}

// Проверка на существование файла в архиве, если такой есть, то дописываем в конец текущее время и дату
CString CGetFileInfo::CheckExistingFile(char* szFullFuliPath)
{
	CString sFilePath = szFullFuliPath;

	if(CFileFind().FindFile(szFullFuliPath) == TRUE)
	{
		CString sTimeStamp;
		SYSTEMTIME pTime;
		GetLocalTime(&pTime);
		sTimeStamp.Format(".%02d-%02d-%02d_%d-%02d-%02d", pTime.wHour,pTime.wMinute,pTime.wSecond,pTime.wYear,pTime.wMonth,pTime.wDay);
		sFilePath += sTimeStamp;
	}
	return sFilePath;
}

// Проверка имени файла на наличие кириллицы
bool CGetFileInfo::CheckFileNameSymbol(char *szFileName)
{
	//проверка есть ли "." в имени файла
	CString sFile = szFileName;
	int nPos = sFile.Find(".",0);
	if (nPos == -1)
	{
		return false;
	}


	/************************************************************************/
	/* русский язык лежит в отрицательном диапазоне, а именно				*/
	/*					-32 .... -1											*/
	/*					а_________я											*/
	/*					-64 ... -33											*/
	/*					А_________Я                                         */
	/************************************************************************/

	char *s;
	setlocale( LC_CTYPE, "rus" );
	for( s = szFileName; *s; ++s )
	{
		//только маленькие буквы, т.к. имя файла заранее преобразовано
		if(*s <= 'я' && *s >= 'а')
		{
			return false;
		}
	}
	return true;
}


// Проверка размера файла и временных интервалов
bool CGetFileInfo::CheckFileSize(char* szFileName)
{
	CString sFileName = szFileName;
	SYSTEMTIME pTime;
	HANDLE hFile;

	GetLocalTime(&pTime);
	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		//Рабочий день
		if ((pPath->wMorningTime < pTime.wHour) && (pTime.wHour < pPath->wEveningTime))
		{
			if(dwFileSize < (pPath->dwDayTimeFileSize*1024*1024))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		//ночное время
		else
		{
			if(dwFileSize < (pPath->dwNightTimeFileSize*1024*1024))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	CloseHandle(hFile);
	return false;
}