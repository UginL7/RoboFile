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
	// Разбор файлов полученых из Районов
	list<CString> GetPostFromRajon();
	// Разбор файлов полученых из Киева
	list<CString> GetPostFromKiev();
private:
	// Информация о файле
	CGetFileInfo *pFileInfo;
	// Запись в лог-файл
	CLogs *pLog;
	// Пути и настройки
	CInitPath *pPath;
	// Транзитные файлы
	CTranzitFiles *pTranzit;
	// Пути к папкам
	CString strSrcFile;
	CString strArcDir;
	// Путь к файлу архива
	CString sDstFullPath;
	// Список файлов в папке на отправку
	list<CString> vFileName;
	list<CString>::iterator itFileName;
	// Получаем список принятых файлов
	void GetNameFileMailFromFolder(char* pFolderPath);
	// Разбор принятых файлов переносом в архив
	bool SpreadOutFiles(bool bKiev, char* szFileName);
	// Файл относится к файлам со спецмаской
	bool bSpecFile;
	// Файл назван ошибочно
	bool bCorrectName;
public:
	// Перемещение почты в архив ОГНС
	bool CopyFileToDirectory(char* szFileName, char* szDstFullPath, char* szDirPah, bool bKiev);
	// Определение кода управления для киевской почты
	CString GetKievDepartNumber(char* szFileName);
	// Копирование в папку канцелярии
	bool CopyToKanceljarija(char* szFileName);
	// Проверка файлов перенаправленнх по служебным для Районов
	CString CheckDedicatedFile(char* szFileName);
};
