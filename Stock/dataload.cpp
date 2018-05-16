
#include "dataload.h"
#include <fstream>
#include <algorithm>
#include <string>
dataload::dataload(std::string n, QObject *parent) :
	QThread(parent)
{
	htmlLoadName = n;
}

void dataload::htmlLoad(std::string _code)
{
	std::locale::global(std::locale("kor"));
	auto fileStream = std::make_shared<ostream>();
	std::string fileName = _code + ".html";
	pplx::task<void> requestTask =
		fstream::open_ostream(utility::conversions::to_string_t(fileName)).then([=](ostream outFile)
	{
		*fileStream = outFile;

		http_client_config conf;
		conf.set_timeout(seconds(5));

		if (_code == "KOSPI" || _code == "KOSDAQ")
		{
			http_client client(U("http://finance.naver.com/"));
			uri_builder builder(U("sise/sise_index.nhn"));
			builder.append_query(U("code"), _code.c_str()); // 주식 코드

			return client.request(methods::GET, builder.to_string());
		}
		else
		{
			http_client client(U("http://finance.naver.com/"));
			uri_builder builder(U("item/main.nhn"));
			builder.append_query(U("code"), _code.c_str()); // 주식 코드

			return client.request(methods::GET, builder.to_string());
		}

	}).then([=](http_response response)
	{
		printf("Receive response status code:%u\n", response.status_code());

		return response.body().read_to_end(fileStream->streambuf());
	}).then([=](size_t nVal)
	{
		printf("Size is %u\n", nVal);

		return fileStream->close();
	});

	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}
}

void dataload::wordPasing(std::string _str, std::string _info)
{
	int idx = _str.find_last_of(" ") + 1;
	int idx2 = _str.find_last_of("<");
	info[_info] = _str.substr(idx, idx2 - idx);
	//ofs << "최저가 : " + _str.substr(idx, idx2 - idx) + "\n";
}

void dataload::run()
{

	int cnt = 7;
	htmlLoad(stockName[htmlLoadName]);
	cur_time = time(NULL);
	cur_tm = localtime(&cur_time);
	
	if (htmlLoadName == "코스피 지수" || htmlLoadName == "코스닥 지수")
		KOSPI_KOSDAQ_Parser(htmlLoadName);
	else
		stock_Parser(htmlLoadName);

	//infoParser(htmlLoadName);

	emit FinishCount(idx);
	//emit FinishCount(info);

}
 
void dataload::stock_Parser(std::string _filename)
{
	std::locale::global(std::locale("kor")); // encoding
	std::ofstream ofs;
	std::string filename = _filename + ".txt";
	int idx = 0, idx2 = 0;
	std::string sign,line,rate;

	ofs.open(filename, std::ios::out | std::ios::app);
	//ofs.open(filename);

	std::ifstream ifs(stockName[htmlLoadName]+".html"); // 한글을 읽기 위해 wifstream 사용
	ifs.imbue(std::locale(std::locale::empty(), \
		new std::codecvt_utf8<wchar_t, 0x10ffff, \
		std::consume_header>));
	//ofs << filename + "\n";
	ofs << "\n\n2018" << "." << cur_tm->tm_mon+1 << "." << cur_tm->tm_mday << " ";
	ofs << cur_tm->tm_hour << ":" << cur_tm->tm_min << ":" << cur_tm->tm_sec << " ";

	while (getline(ifs, line))
	{
		if (line.find("<dl class=\"blind\">") != -1)
			break;
	}

	while (1)
	{
		getline(ifs, line);
		if (line.find("</dl>") != -1)	break;
	/*	if (line.find("년") != -1)
		{
			if (line.find("장중") != -1)
				flag = "장중";
			else
				flag = "장마감";

			ofs << flag + "\n";
		}*/
		if (line.find("현재가") != -1)
		{
			idx = line.find("현재가") + 7;
			idx2 = line.find(" ", idx);
			info["현재가"] = line.substr(idx, idx2 - idx);
			
			if (line.find("하락") != -1)
			{
				rate = "-";
				idx = line.find("하락") + 5;
				flag = -1;
			}
			else
			{
				rate = "+";
				idx = line.find("상승") + 5;
				flag = 1;
			}
				
			idx2 = line.find(" ", idx );
			rate += line.substr(idx, idx2 - idx);
			info["전일비"] = line.substr(idx, idx2 - idx);

			idx = line.find(" ", idx2 + 3);
			idx2 = line.find_last_of(" ");
			if (line.find("마이너스") != -1)
				sign = "▼  " + line.substr(idx+1, idx2 - idx-1) + "%";
			else
				sign = "▲  " + line.substr(idx+1, idx2 - idx-1) + "%";
			info["등락률"] = sign;

		}
		else if (line.find("거래량") != -1)
			wordPasing(line, "거래량");
	/*	else if (line.find("고가") != -1)
			wordPasing(line, "최고가");

		else if (line.find("저가") != -1)

			wordPasing(line, "최저가");

		

		else if (line.find("거래대금") != -1)
			wordPasing(line, "거래대금");*/

	}

	for (auto i = info.begin(); i != info.end(); ++i)
	{
		ofs << i->first << " : " << i->second << "\n";
		ofs << i->second << "\n";
	}
	ofs.close();
	ifs.close();

}

void dataload::KOSPI_KOSDAQ_Parser(std::string _filename)
{	
	std::locale::global(std::locale("kor")); // encoding

	std::ofstream ofs;
	std::string filename = _filename + ".txt";
	ofs.open(filename, std::ios::out | std::ios::app);
	
	std::string line,rate;
	int cnt = 0;
	int idx = 0, idx2 = 0;
	int info_idx = 0;
	
	//ofs.open(filename);
	ofs << filename << "\n";
		std::ifstream ifs(stockName[htmlLoadName] + ".html"); // 한글을 읽기 위해 wifstream 사용
	ifs.imbue(std::locale(std::locale::empty(), \
		new std::codecvt_utf8<wchar_t, 0x10ffff, \
		std::consume_header>));
	ofs <<"\n\n2018" <<"."<< cur_tm->tm_mon << "." << cur_tm->tm_mday << " ";
	ofs << cur_tm->tm_hour << ":" << cur_tm->tm_min << ":" << cur_tm->tm_sec << " " ;


	while (getline(ifs, line))
	{
	/*	if (line.find("<span id=\"time\">") != -1)
		{
			if (line.find("장중") != -1)
				flag2 = "장중";
			else
				flag2 = "장마감";

			ofs << flag2 + "\n";
		}*/
		if (line.find("<div class=\"subtop_sise_detail\">") != -1)
			break;
	}
	while (cnt < 2)
	{
		getline(ifs, line);
		if (line.find("em id=\"now_value\">") != -1)
		{
			idx = line.find(">") + 1;
			idx2 = line.find_last_of("<");
			info["현재가"] = line.substr(idx, idx2 - idx); // O
		}
		if (line.find("change_value_and_rate") != -1)
		{
			if (line.find("-") != -1)
			{
				flag = -1;
				rate = "-";
				idx = line.find("-") + 5;
			}
			else
			{
				flag = 1;
				rate = "+";
				idx = line.find("+") + 5;
			}
			idx = line.find(">");
			idx2 = line.find(">", idx + 5) + 1;
			idx = line.find("<", idx2);
			//info["전일비"] = rate+line.substr(idx2, idx - idx2); //O
			info["전일비"] =line.substr(idx2, idx - idx2); //O


			idx = line.find(">", idx) + 3;
			idx2 = line.find("<", idx);
			//std::string st = line.substr(idx, idx2 - idx);
			
			if (rate == "-")
				info["등락률"] = "▼  " + line.substr(idx, idx2 - idx); // O
			else
				info["등락률"] = "▲  " + line.substr(idx, idx2 - idx); // 


		}
		//if (line.find("<caption>") != -1)
		//	ofs << line + "\n";
		if (line.find("<tr>") != -1)
		{
			while (getline(ifs, line))
			{
				if (line.find("</tr>") != -1) break;
				if (line.find("span") == -1)
				{
					info_idx++;
					idx = line.find(">") + 1;
					idx2 = line.find_last_of("<");
					switch (info_idx)
					{
					case 1:
						info["거래량"] = line.substr(idx, idx2 - idx); // O
						break;
					case 2:
						//info["거래대금"] = line.substr(idx, idx2 - idx);
						break;
					case 3:
						//info["최저가"] = line.substr(idx, idx2 - idx);
						break;
					case 4:
						//info["최고가"] = line.substr(idx, idx2 - idx);
						break;
					}
				}
			}
			cnt++;

		}
	}

	for (auto i = info.begin(); i != info.end(); ++i)
	{
		ofs << i->first << " : " << i->second << "\n";
		ofs << i->second << "\n";
	}
	ofs << "\n\n";
	ofs.close();
	ifs.close();

}
