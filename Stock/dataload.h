#pragma once
#ifndef __DATALOAD_H__
#define __DATALOAD_H__

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <qthread.h>
#include <string>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <map>
#include <ctime>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

class dataload : public QThread
{
	Q_OBJECT
public:
	explicit dataload(std::string n,QObject *parent = 0);
	void htmlLoad(std::string _code);
	void KOSPI_KOSDAQ_Parser(std::string _filename);
	void stock_Parser(std::string _filename);
	void wordPasing(std::string _str, std::string _info);
	time_t cur_time;
	struct tm *cur_tm;
	int flag;
	std::string htmlLoadName;
	std::map<std::string, std::string> stockName = \
	{ {"코스피","KOSPI"},{"코스닥","KOSDAQ"},{"삼성전자","005930"},\
	{"삼성바이오", "207940"}, { "KODEX 레버리지","122630" },\
	{ "KODEX 인버스","114800" }, {"셀트리온","068270"} };
	std::map<std::string, std::string> info = \
	{ {"거래량", ""}, { "현재가","" }, { "전일비","" }, \
	{"등락률", ""}};
	std::map<std::string, std::string> tmp_info = \
	{ {"거래량", ""}, { "현재가","" }, { "전일비","" }, \
	{"등락률", ""}};

private:
	void run();

signals:
	void FinishCount(int cnt);
};


#endif // !__DATALOAD_H__
