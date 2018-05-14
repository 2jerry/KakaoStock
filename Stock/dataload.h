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
	{ {"�ڽ���","KOSPI"},{"�ڽ���","KOSDAQ"},{"�Ｚ����","005930"},\
	{"�Ｚ���̿�", "207940"}, { "KODEX ��������","122630" },\
	{ "KODEX �ι���","114800" }, {"��Ʈ����","068270"} };
	std::map<std::string, std::string> info = \
	{ {"�ŷ���", ""}, { "���簡","" }, { "���Ϻ�","" }, \
	{"�����", ""}};
	std::map<std::string, std::string> tmp_info = \
	{ {"�ŷ���", ""}, { "���簡","" }, { "���Ϻ�","" }, \
	{"�����", ""}};

private:
	void run();

signals:
	void FinishCount(int cnt);
};


#endif // !__DATALOAD_H__
