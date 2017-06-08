#pragma once
#pragma warning (disable : 4996)
#include <list>
#include <map>
#include "tinyxml.h"

using namespace std;

class CInitPath
{
public:
	CInitPath(void);
	virtual ~CInitPath(void);

	// Путь к папке архивов почты 
	CString m_PathArcPost;
	// Путь к папке входящей районной почты
	CString m_PathInPostRajon;
	// Путь к папке входящей киевской почты 
	CString m_PathInPostKiev;
	// Путь к папке входящей почты, для канцелярии
	CString m_PathInKanceljarija;
	// Путь к папке исходящей почты на Все ОДПИ
	CString m_PathOutAllODPI;
	// Путь к папке исходящей почты на Все РАЙОНЫ
	CString m_PathOutAllRajonPost;
	// Путь к папке исходящей почты на Все РАЙОНЫ + СГНИ
	CString m_PathOutAllRajonSGNIPost;
	// Путь к папке ошибок исходящей почты
	CString m_PathOutError;
	// Путь к папке Больших файлов исходящей почты
	CString m_PathOutBigFile;
	// Путь к лог файлу для почты полученной из Киева
	CString m_LogFromKiev;
	// Путь к лог файлу для почты отпрвеленной на Киев
	CString m_LogToKiev;
	// Путь к лог файлу для почты полученной из Районов
	CString m_LogFromRajon;
	// Путь к лог файлу для почты отпрвеленной на Районы
	CString m_LogToRajon;
	// Путь к лог файлу для входящих Транзит-файлов
	CString m_LogInTranzit;
	// Путь к лог файлу для исходящих Транзит-файлов
	CString m_LogOutTranzit;
	// Путь к папкам СГНИ
	map<CString, CString> m_mapSGNISrcDst;
	// Путь к папкам ОДПИ
	map<CString, CString> m_mapODPISrcDst;
	// Соответсвия направлений отправки на конкретный район и папки для Fossmail
	map<CString, CString> m_mapRajonSrcDst;
	// Соответсвия номера района и папки для копирования транзитного файла
	map<CString, CString> m_mapTRRajonID;
	// Соответсвия направлений отправки на Киев и папки для Fossmail
	map<CString, CString> m_mapKievSrcDst;
	// Соответсвия спец масок и управлений для Киева
	map<CString, CString> m_mapKievSpecFile;
	// Соответсвия спец масок и управлений для Районов
	map<CString, CString> m_mapRajonSpecFile;
	// Соответсвия файлов перенаправленнх по служебным для Районов
	map<CString, CString> m_mapRajonDedicatedFile;
	// Список масок файлов, которые необходимо отправить, независимо от их размера
	list<CString> m_listMask;
	// Размер файла в рабочее время между UpTime и DownTime
	DWORD dwDayTimeFileSize;
	// Размер файла в ночное время между DownTime и UpTime
	DWORD dwNightTimeFileSize;
	// Время начала дневного трафика
	WORD wMorningTime;
	// Время начала ночного трафика
	WORD wEveningTime;

private:
	// Путь к файлу конфигурации
	char szConfigFile[MAX_PATH];
	// Временное хранение пути
	CString strPathFromXml;
	// Для получения доступа к нодам файла
	TiXmlElement *pMainConf; 
	// Для получения доступа к аттрибутам и их значениям
	TiXmlElement *pPathElement;
	TiXmlElement *pPath;
	// Инициализация переменных для работы с XML
	void InitXml();
	// Получение списка путей
	void GetPathFromCfg(void);
	// Получение списка кодов инспекций и путей для входящих транзитных файлов
	void GetPathTrFromKiev(void);
	// Получение списка масок файлов, которые необходимо отправить, независимо от их размера
	void GetSpecMaskFromCfg(void);
	// Получение интервалов времени и размеров файлов допустимых для отправки
	void GetTimeAndSizeFromCfg(void);
	// Получение путей к лог-файлам
	void GetLogFilePathFromCfg(void);
};
