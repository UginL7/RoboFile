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
	// Перемещение файлов в папку отправки на Киев
	list<CString> SendPostToKiev();

private:
	CInitPath *pPath;
	CLogs *pLog;
	CGetFileInfo *pFileInfo;
	//Файл был перемещён в папку Ошибки
	bool bErrorDirMove;

	// Владелец файла
	char szFileOwner[1024];
	char szDomainName[1024];

	//Пути к папкам
	//откуда забрать
	CString strSrcFile;
	//куда положить
	CString strDstFile;
	// Список файлов в папке на отправку
	list<CString> vFileName;
	list<CString>::iterator itFileName;
	// Проверка на правильность имени файла для отправки
	bool VerifyFileName(char* strFileName);
	// Получаем список файлов подготовленных на отправку в Киев из ГНС в Запорожской области
	void GetNameFileMailFromFolder(char* pFolderPath);
	// Полный путь к файлу отправки исходное положение
	CString m_strSrcFolder;
	// Полный путь к файлу отправки конечное положение
	CString m_strDstFolder;
	// Создание директории для конкретного управления, с текущим месяцем и днём
	bool CreateArcDir(char* szFileName, char *szDestination);
	// Копирование файла в архив ОГНС и на отправку
	bool CopyFileToDirectory(char* szFileName, char* szDstFullPath, char* szDirPah);
	// Перемещение файла в папку Ошибки для района, без отправки на Киев
	bool MoveFileToErrorDirectoty(char *szFileName);
};
