#pragma once
#include "InitPath.h"
#include <accctrl.h>
#include <aclapi.h>
#include <clocale>

class CGetFileInfo
{
public:
	CGetFileInfo(CInitPath *pConfig = NULL);
	~CGetFileInfo(void);
	// Размер файла в байтах
	DWORD dwFileSize;
	// Получение имени создателя файла
	int GetFileOwner(char* szFilepath, char *szAccount, char *szDomainName);
	// Проверка на существование файла в архиве, если такой есть, то дописываем в конец текущее время и дату
	CString CheckExistingFile(char* szFullFuliPath);
	// Проверка имени файла на наличие кириллицы
	bool CheckFileNameSymbol(char *szFileName);
	// Проверка размера файла и временных интервалов
	bool CheckFileSize(char* szFileName);
private:
	CInitPath *pPath;

};
