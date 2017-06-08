#pragma once
#include "InitPath.h"
#include "Logs.h"

class CTranzitFiles
{
public:
	CTranzitFiles(void);
	virtual ~CTranzitFiles(void);
	// Определение транзитного файла и отправка по назначению
	void WorkingTranzitFiles(char* szFileName, char* szFromFolder, char* szToFolder, bool bKiev);
private:
	CLogs *pLog;
	CInitPath *pPath;
	CString strFileName;
	CString strFromFolder;
	CString strToFolder;
	bool bKievRec;
	// Пути к папкам и кодам инспекций, полученные из XML
	map<CString, CString>::iterator it_TRRajonID;
	// Индикатор того, что файл получен из папки входящей почты
	bool bIsReceive;
	// Копирование принятных от Киева транзитных файлов на соответствующий район(ы)
	bool CopyToRajFromKiev();
	// Копирование принятных от Киева транзитных файлов на все районы
	bool CopyToAllRajFromKiev();
	// Копирование принятных от района транзитных файлов на район
	bool CopyToRajFromRaj();
	// Копирование принятных от района транзитных файлов на Киев
	bool CopyToKievFromRaj();
};
