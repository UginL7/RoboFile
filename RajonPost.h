#pragma once
#include "InitPath.h"
#include "Logs.h"
#include "GetFileInfo.h"
#include <list>

using namespace std;

class CRajonPost
{
public:
	CRajonPost(void);
	virtual ~CRajonPost(void);
	// Отправка почты на конкретный район
	list<CString>SendPostToRajon();
	// Отправка почты на все ОДПИ
	list<CString>SendPostToAllODPI();
	// Отправка почты на все районы 
	list<CString>SendPostToAllRajon();
	// Отправка почты на все районы + СГНИ
	list<CString>SendPostToAllRajonSGNI();

private:
	// Запись в лог-файл
	CLogs *pLog;
	// Пути и настройки
	CInitPath *pPath;
	// Информация по файлу
	CGetFileInfo *pFileInfo;
	// Владелец файла
	char szFileOwner[1024];
	char szDomainName[1024];
	//Файл был перемещён в папку Ошибки
	bool bErrorDirMove;

	// Пути к папкам и файлам, полученные из XML
	map<CString, CString>::iterator it_RajonSrcDst;
	// Путь к файлу архива
	CString sDstFullPath;
	// Пути к папкам
	CString strArcPath;
	CString strSrcFile;
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
	// Строка для записи в лог-файл
	CString sLog;
	// Создание директории для конкретного управления, с текущим месяцем и днём
	bool CreateArcDir(char* szFileName, char *szDestination);
	// Копирование файла в архив ОГНС и на отправку
	bool CopyFileToDirectory(char* szFileName);
	// Проверка файлов перенаправленнх по служебным для Районов
	CString CheckDedicatedFile(char* szFileName);
	// Перемещение файла в папку Ошибки для района, без отправки на район
	bool MoveFileToErrorDirectoty(char *szFileName);
};
