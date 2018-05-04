#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")

#define SZ 7 // 쓰레드 개수(회사 5, 종합주가지수 2)
#define BUFSIZE 10000
#define ARRSIZE 1000
#define MINUTE 60000
#define SECOND 1000


using namespace std;

const char *fname1[SZ] = { "Samsung(html).txt", "Samsung_Bio(html).txt", "Celtrion(html).txt", "KODEX_Leverage(html).txt",
"KODEX_Invers(html).txt", "KOSPI(html).txt", "KOSDAQ(html).txt"};
const char *fname2[SZ] = { "Samsung(update).csv", "Samsung_Bio(update).csv", "Celtrion(update).csv", "KODEX_Leverage(update).csv",
"KODEX_Invers(update).csv", "KOSPI(update).csv", "KOSDAQ(update).csv" };
const char *get1[SZ] = { "GET /item/main.nhn?code=005930 HTTP/1.1\r\n", "GET /item/main.nhn?code=207940 HTTP/1.1\r\n",
"GET /item/main.nhn?code=068270 HTTP/1.1\r\n", "GET /item/main.nhn?code=122630 HTTP/1.1\r\n",
"GET /item/main.nhn?code=114800 HTTP/1.1\r\n", "GET /sise/sise_index.nhn?code=KOSPI HTTP/1.1\r\n",
"GET /sise/sise_index.nhn?code=KOSDAQ HTTP/1.1\r\n" };

void* stock(void* t_num)
{
	int t_id = *((int *)t_num);

	int retval;
	char buf[BUFSIZE];
	string str_start = "	        <dt>종목 시세 정보</dt>\n"; // 시작
	string str_no = "	        <dd>종목"; // 예외
	string str_price = "	        <dd>현재가 "; // 분리
	string str_fin = "        </dl>\n"; // 끝
	int flag1; // 출력 시작 플래그
	int flag2; // 일자 중복 출력 방지 플래그
	int flag3; // 분리 필요 플래그
			   //int flag4 = 0; // 장마감 플래그
	int cnt; // 분리중에 공백 개수 세기 위해
	char updown[100] = { 0 }; // +-

	char before[ARRSIZE] = { 0 }; // 가장 최근 체결가를 저장할 배열
	char current[ARRSIZE] = { 0 }; // 현재 체결가를 저장할 배열
	char past[ARRSIZE] = { 0 }; // 가장 최근 일자
	char present[ARRSIZE] = { 0 }; // 현재 일자
	int idx2; // ARRSIZE를 갖는 배열의 인덱스
	int tmp;

	while (1)
	{
		char msg[500] = { 0 };
		flag1 = 0; flag2 = 0; flag3 = 0;
		tmp = 0; cnt = 0; idx2 = 0;
		memset(buf, 0, BUFSIZE);

		FILE *fp1 = fopen(fname1[t_id - 1], "w+"); // 출력내용을 파일에 저장하기 위
		FILE *fp2 = fopen(fname2[t_id - 1], "a+"); // txt파일로 원하는 부분만 정리 
		if (fp1 != 0 && fp2 != 0)
		{
			WSADATA wsa; // 윈속 초기화
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;

			//socket()

			// 1. 소켓 생성(socket)
			// AF_INET: 물리적으로 서로 멀리 떨어진 컴퓨터 사이의 통신. 일반적으로 IPv4사용
			// SOCK_STREAM: TCP/IP 프로토콜
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) return 0;

			hostent* remoteHost = gethostbyname("finance.naver.com"); // 웹주소를 ip로 바꾸기 위해 

																	  //connect()

																	  // 2. 소켓 정의(bind): IP주소, PORT번호 등을 할당
			SOCKADDR_IN serveraddr;
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(80);
			serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list)); //
			retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) return 0;

			strcpy(msg, get1[t_id - 1]);
			strcat(msg, "Host: finance.naver.com\r\n\r\n");// 웹서버로 보낼 헤더 작성 

			send(sock, msg, strlen(msg), 0);// 웹서버로 보내기

			while (1)
			{
				if ((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break; // 서버에서 응답 받기

				fprintf(fp1, "%s", buf); // 파일에 저장
				memset(buf, 0, BUFSIZE); // 버퍼 지우기(초기화)
			}



			// 주식 정보 출력
			printf("★%d★ ", t_id);
			fseek(fp1, 0, SEEK_SET);
			while (fgets(buf, BUFSIZE, fp1))
			{
				if ((flag1 == 1) && (strcmp(buf,str_fin.c_str()) == 0)) // 끝
				{
					fprintf(fp2, "\n");
					printf("\n");
					break;
				}

				if (flag1 == 1)
				{
					if (flag2 == 3) // 그 이후 출력
					{
						for (int i = 0; i < BUFSIZE; i++)
						{
							if (flag3 == 2)
							{
								if (buf[i] != ',')
								{
									fprintf(fp2, "%c", buf[i]);
									printf("%c", buf[i]);
								}
							}
							if (flag3 == 0 && buf[i] == '>') flag3 = 1;
							if (flag3 == 1 && buf[i] == ' ') flag3 = 2;
							if (flag3 == 2 && buf[i + 1] == '<')
							{
								fprintf(fp2, ",");
								printf(" ");
								flag3 = 0;
								break;
							}
						}
					}
					if (flag2 == 0) // 일자 출력
					{
						for (int i = 0; i < BUFSIZE; i++)
						{
							if (flag3 == 1)
							{
								fprintf(fp2, "%c%c%c%c,", buf[i], buf[i + 1], buf[i + 2], buf[i + 3]); // 년
								printf("%c%c%c%c/", buf[i], buf[i + 1], buf[i + 2], buf[i + 3]);
								fprintf(fp2, "%c%c,", buf[i + 7], buf[i + 8]); // 월
								printf("%c%c/", buf[i + 7], buf[i + 8]);
								fprintf(fp2, "%c%c,", buf[i + 12], buf[i + 13]); // 일
								printf("%c%c/", buf[i + 12], buf[i + 13]);
								fprintf(fp2, "%c%c:%c%c,", buf[i + 17], buf[i + 18], buf[i + 22], buf[i + 23]); // 시
								printf("%c%c:%c%c/", buf[i + 17], buf[i + 18], buf[i + 22], buf[i + 23]);

								i = i + 31;
								if (buf[i + 7] == '<') // 장마감 또는 개장전
								{
									//flag4 = 1;
									fprintf(fp2, "%c%c%c%c%c%c,", buf[i + 1], buf[i + 2], buf[i + 3], buf[i + 4], buf[i + 5], buf[i + 6]);
									printf("%c%c%c%c%c%c/ ", buf[i + 1], buf[i + 2], buf[i + 3], buf[i + 4], buf[i + 5], buf[i + 6]);
									flag2 = 1;
									flag3 = 0;
									break;
								}
								else
								{
									//flag4 = 0;
									fprintf(fp2, "%c%c%c%c,", buf[i + 1], buf[i + 2], buf[i + 3], buf[i + 4]);
									printf("%c%c%c%c/", buf[i + 1], buf[i + 2], buf[i + 3], buf[i + 4]);
									flag2 = 1;
									flag3 = 0;
									break;
								}
							}
							if (flag3 == 0 && buf[i] == '>') flag3 = 1;
						}
					}

					str_no = "           <dd>종목";
					if (strncmp(buf,str_no.c_str(), str_no.length()) == 0)
						flag2 = 2;

					if (strncmp(buf, str_price.c_str(), strlen(str_price.c_str())) == 0)
					{
						for (int i = strlen(str_price.c_str()); i < BUFSIZE; i++)
						{
							if ((buf[i] != ',') && (buf[i] != ' '))
							{
								fprintf(fp2, "%c", buf[i]);
								printf("%c", buf[i]);
							}
							if (buf[i] == ' ')
							{
								if (flag3 == 2)
								{
									fprintf(fp2, "%%,");
									printf("%% ");
									flag2 = 3;
									flag3 = 0;
									break;
								}
								fprintf(fp2, ",");
								printf(" ");
								flag3++;
								if (flag3 == 1)
								{
									updown[0] = buf[i + 10];
									updown[1] = buf[i + 11];
									updown[2] = buf[i + 12];

									if (strncmp(updown, "하", strlen("하")) == 0) updown[0] = '-';
									else updown[0] = ' ';

									fprintf(fp2, "%c", updown[0]);
									printf("%c", updown[0]);

									i = i + 14;
								}
								if (flag3 == 2)
								{
									if (updown[0] == '-') i = i + 9;
									else i = i + 7;

									fprintf(fp2, "%c", updown[0]);
									printf("%c", updown[0]);
								}
							}
						}
					}
				}

				if ((flag1 == 0) && (strcmp(buf, str_start.c_str()) == 0)) // 시작
					flag1 = 1;

				memset(buf, 0, BUFSIZE); // 버퍼 지우기(초기화)
			}

			closesocket(sock);
			fclose(fp1);
			fclose(fp2);
			WSACleanup(); // 윈속 종료

			Sleep(MINUTE);
		}
	}

	return NULL;
}

void* kospi_kosdaq(void* t_num)
{
	int t_id = *((int *)t_num);

	int retval;
	char buf[BUFSIZE];

	string str_time = "	                                <span id=\"time\">"; // 시간
	string str_price = "	                <em id=\"now_value\">"; // 체결가
	string str_gap = "	                <span class=\"fluc\" id=\"change_value_and_rate\"><span>"; // 전일대비

	int flag1; // ~등락률 구분 플래그
	int flag2; // 등락률~ 구분 플래그
	int flag3; // 주가 출력 여부 플래스
	int flag4; // 일자,체결가 중복 출력 방지 플래그
			   //int flag5 = 0; // 장마감 플래그

	int cnt[4] = { 12, 2, 4, 2 }; // 전일 대비 정보가 있는 라인으로부터 다음 정보까지의 라인 수
	int idx1; // cnt배열의 인덱스(한 나라 출력할 때마다 증가)
	int line; // 현재 나라로부터 다음 나라까지의 라인 수 카운터(cnt배열과 일치하면 다음 나라)

	char before[ARRSIZE] = { 0 }; // 가장 최근 체결가를 저장할 배열
	char current[ARRSIZE] = { 0 }; // 현재 체결가를 저장할 배열
	char past[ARRSIZE] = { 0 }; // 가장 최근 일자
	char present[ARRSIZE] = { 0 }; // 현재 일자
	int idx2; // ARRSIZE를 갖는 배열의 인덱스
	int tmp;

	while (1)
	{
		char msg[500];
		flag1 = 0; flag2 = 0; flag3 = 0; flag4 = 0;
		idx1 = 0; idx2 = 0;
		line = 0; tmp = 0;
		memset(buf, 0, BUFSIZE);

		FILE *fp1 = fopen(fname1[t_id - 1], "w+"); // 출력내용을 파일에 저장하기 위
		FILE *fp2 = fopen(fname2[t_id - 1], "a+"); // txt파일로 원하는 부분만 정리 
		if (fp1 != 0 && fp2 != 0)
		{
			WSADATA wsa; // 윈속 초기화
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;

			//socket()

			// 1. 소켓 생성(socket)
			// AF_INET: 물리적으로 서로 멀리 떨어진 컴퓨터 사이의 통신. 일반적으로 IPv4사용
			// SOCK_STREAM: TCP/IP 프로토콜
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) return 0;

			hostent* remoteHost = gethostbyname("finance.naver.com"); // 웹주소를 ip로 바꾸기 위해 

																	  //connect()

																	  // 2. 소켓 정의(bind): IP주소, PORT번호 등을 할당
			SOCKADDR_IN serveraddr;
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(80);
			serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list)); //
			retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) return 0;

			strcpy(msg, get1[t_id - 1]);
			strcat(msg, "Host: finance.naver.com\r\n\r\n");// 웹서버로 보낼 헤더 작성 

			send(sock, msg, strlen(msg), 0);// 웹서버로 보내기

			while (1)
			{
				if ((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break; // 서버에서 응답 받기

				fprintf(fp1, "%s", buf); // 파일에 저장
				memset(buf, 0, BUFSIZE); // 버퍼 지우기(초기화)
			}


			// 출력
			printf("☆%d☆ ", t_id);
			fseek(fp1, 0, SEEK_SET);
			while (fgets(buf, BUFSIZE, fp1))
			{
				// 3. 시간정보 가져오기
				if ((flag1 == 0) && (strncmp(buf, str_time.c_str(), str_time.length()) == 0))
				{
					for (int i = str_time.length(); i < BUFSIZE; i++)
					{
						if (buf[i] == '<')
						{
							fprintf(fp2, ",");
							printf(" ");
							flag1 = 1;
							break;
						}
						if (buf[i] == '.')
						{
							fprintf(fp2, ",");
							printf("/");
						}
						else if (buf[i] == ' ')
						{
							if (buf[i + 1] < 80 || buf[i + 1] > 89) // 시간이 아니면
							{
								//flag5 = 1;
								fprintf(fp2, ",,");
								printf("  ");
							}
							//else flag5 = 0;
						}
						else
						{
							fprintf(fp2, "%c", buf[i]);
							printf("%c", buf[i]);
						}
					}
				}

				// 4. 체결가 가져오기
				if ((flag1 == 1) && (strncmp(buf, str_price.c_str(), str_price.length()) == 0))
				{
					for (int i = str_price.length(); i < BUFSIZE; i++)
					{
						if (buf[i] == '<')
						{
							flag1 = 2;
							tmp = 0;
							fprintf(fp2, ",");
							printf(" ");
							break;
						}
						if (buf[i] != ',')
						{
							fprintf(fp2, "%c", buf[i]);
							printf("%c", buf[i]);
						}
					}
				}

				// 6. 전일대비/등락율 출력
				if (flag1 == 2 && strncmp(buf, str_gap.c_str(), str_gap.length()) == 0)
				{
					for (int i = str_gap.length(); i < BUFSIZE; i++)
					{
						if ((flag1 == 3) && (buf[i] == '<'))
						{
							fprintf(fp2, ",");
							printf(" ");
							break;
						}
						if (buf[i] == '<')
						{
							i = i + 6;
							fprintf(fp2, ",");
							printf(" ");
						}
						if (buf[i] == '>')
						{
							i = i + 2;
							flag1 = 3;
						}
						if (buf[i] != ',')
						{
							fprintf(fp2, "%c", buf[i]);
							printf("%c", buf[i]);
						}
					}
				}

				// 7. 등락률~ 정보들 출력
				if (flag1 == 3)
				{
					if (line == cnt[idx1])
					{
						for (int i = 0; i < BUFSIZE; i++)
						{
							if ((flag2 == 1) && (buf[i] != ','))
							{
								fprintf(fp2, "%c", buf[i]);
								printf("%c", buf[i]);
							}
							if ((flag2 == 0) && (buf[i] == '>')) flag2 = 1;
							if ((flag2 == 1) && (buf[i + 1] == '<'))
							{
								idx1++;
								line = 0;
								flag2 = 0;
								fprintf(fp2, ",");
								printf(" ");

								if (idx1 == 4)
								{
									fprintf(fp2, "\n");
									printf("\n");
								}
								break;
							}
						}
					}
					line++;
				}
			}

			memset(buf, 0, BUFSIZE); // 버퍼 지우기(초기화)

			closesocket(sock);
			fclose(fp1);
			fclose(fp2);
			WSACleanup(); // 윈속 종료

			Sleep(MINUTE);
		}
	}

	return NULL;
}

/*
void* exchange_rate(void* t_num)
{
	int t_id = *((int *)t_num);

	int retval;
	char buf[BUFSIZE] = { 0 };

	char *str_time = "                  <span class=\"time\">";                         // 시간
	char *str_count = "                  <span class=\"count\">고시회차<span class=\"num\">"; // 고시회차
	char *str_rate = "   <h3 class=\"h_calculator\"><span>환율";                            // 환율
	int flag1; // 시간 정보 출력 중복 방지 플래그
	int flag2; // 첫번째 나라의 환율정보 플래그
	int flag3; // 환율 금액의 위치 플래그
	int flag4; // 환율 고시 회차 중복 플래그
	int line; // 현재 나라로부터 다음 나라까지의 라인 수 카운터(cnt배열과 일치하면 다음 나라)

	int cnt[4] = { 10, 9, 3, 19 }; // 시간정보가 있는 라인으로부터 다음 나라까지의 라인 수
	char before[4][ARRSIZE]; // 이전 환율정보를 저장할 배열
	char current[4][ARRSIZE]; // 현재 환율정보를 저장할 배열
	double prev; // 과거 환율 값
	double now; // 현재 환율 값
	double gap; // 과거 - 현재
	double per; // 등락율 (%)
	int idx1; // cnt배열의 인덱스(한 나라 출력할 때마다 증가)
	int idx2; // current배열의 인덱스

	char present[5] = { 0 }; // 현재 고시회차
	char past[5] = { 0 }; // 과거 고시회차

	memset(buf, 0, BUFSIZE);
	memset(before, 0, ARRSIZE * 4);
	memset(current, 0, ARRSIZE * 4);

	while (1)
	{
		flag1 = 0; flag2 = 0; flag3 = 0; flag4 = 0;
		idx1 = 0; idx2 = 0; line = 0;

		// 환율
		FILE *fp1 = fopen(fname1[t_id - 1], "w+"); // 출력내용을 파일에 저장하기 위해
		FILE *fp2 = fopen(fname2[t_id - 1], "a+"); // csv파일로 원하는 부분만 정리 
		if (fp1 != 0 && fp2 != 0)
		{
			WSADATA wsa; // 윈속 초기화
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;

			//socket()

			// 1. 소켓 생성(socket)
			// AF_INET: 물리적으로 서로 멀리 떨어진 컴퓨터 사이의 통신. 일반적으로 IPv4사용
			// SOCK_STREAM: TCP/IP 프로토콜
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) return 0;

			hostent* remoteHost = gethostbyname("finance.naver.com"); // 웹주소를 ip로 바꾸기 위해 

																	  //connect()

																	  // 2. 소켓 정의(bind): IP주소, PORT번호 등을 할당
			SOCKADDR_IN serveraddr;
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(80);
			serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list)); //
			retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) return 0;

			// 환율
			char msg[500] = "GET /marketindex/ HTTP/1.1\r\n";
			strcat(msg, "Host: finance.naver.com\r\n\r\n");// 웹서버로 보낼 헤더 작성 

			send(sock, msg, strlen(msg), 0);// 웹서버로 보내기

			while (1)
			{
				if ((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break; // 서버에서 응답 받기

				fprintf(fp1, "%s", buf); // 파일에 저장
				memset(buf, 0, BUFSIZE); // 버퍼 지우기(초기화)
			}



			// 환율 정보 출력 
			fseek(fp1, 0, SEEK_SET);

			// 1. csv파일에서 이전 고시회차 찾기
			Sleep(SECOND * 10);
			fgets(buf, BUFSIZE, fp2);
			while (fgets(buf, BUFSIZE, fp2))
			{
				if (buf[0] != ',')
				{
					memset(past, 0, 5);
					for (int i = 0; i < BUFSIZE; i++)
					{
						if ((flag4 != 4) && (buf[i] == ','))
						{
							i++;
							idx2 = i;
							flag4++;
						}
						if ((flag4 == 4) && (buf[i] == ','))
						{
							idx2 = 0;
							flag4 = 0;

							fgets(buf, BUFSIZE, fp2);
							fgets(buf, BUFSIZE, fp2);
							fgets(buf, BUFSIZE, fp2);
							break;
						}
						if (flag4 == 4)
							past[i - idx2] = buf[i];
					}
				}
				memset(buf, 0, BUFSIZE);
			}

			// 2. 현재 html에서 고시 회차 찾기
			while (fgets(buf, BUFSIZE, fp1))
			{
				if (strncmp(buf, str_count, strlen(str_count)) == 0)
				{
					for (int i = strlen(str_count); i < BUFSIZE; i++)
					{
						if (buf[i] == '<') break;
						present[i - strlen(str_count)] = buf[i];
					}
					break;
				}
				memset(buf, 0, BUFSIZE);
			}

			// 3. 고시회차 일치 안하면 환율 출력
			if (strcmp(past, present) != 0)
			{
				fseek(fp1, 0, SEEK_SET);
				fseek(fp2, 0, SEEK_SET);

				// 4. 맨 처음에 csv에서 전일 환율 값 찾기
				if (before[0][0] == 0)
				{
					fgets(buf, BUFSIZE, fp2);
					memset(buf, 0, BUFSIZE);
					while (fgets(buf, BUFSIZE, fp2))
					{
						for (int i = 0; i < BUFSIZE; i++)
						{
							if ((flag4 < 6) && (buf[i] == ','))
							{
								flag4++;
								if (flag4 == 6)
								{
									i++;
									idx2 = i;
									if (idx1 == 0) memset(before, 0, ARRSIZE * 4);
								}
							}
							if ((flag4 == 6) && (buf[i] == ','))
							{
								idx1++;
								idx2 = 0;
								flag4 = 0;
								if (idx1 == 4) idx1 = 0;
								break;
							}
							if (flag4 == 6)
								before[idx1][i - idx2] = buf[i];
						}
						memset(buf, 0, BUFSIZE);
					}
					idx1 = 0;
				}

				while (fgets(buf, BUFSIZE, fp1))
				{
					// 5. 시간 출력
					if ((flag1 == 0) && (strncmp(buf, str_time, strlen(str_time)) == 0))
					{
						for (int i = strlen(str_time); i < BUFSIZE; i++)
						{
							if (buf[i] == '<')
							{
								fprintf(fp2, ","); printf("\n");
								flag1 = 2;
								break;
							}
							if (buf[i] == '.' || buf[i] == ' ')
							{
								fprintf(fp2, ",");
								printf("/");
							}
							else
							{
								fprintf(fp2, "%c", buf[i]);
								printf("%c", buf[i]);
							}
						}
					}

					// 6. 고시회차 출력
					if (flag1 == 2)
					{
						if (strncmp(buf, str_count, strlen(str_count)) == 0)
						{
							fprintf(fp2, "%s,", present);
							printf("%s\n", present);
							flag1 = 1;
						}
					}

					// 7. 환율 출력 시작
					// 환율 정보가 시작되는 라인 찾기
					if (strncmp(buf, str_rate, strlen(str_rate)) == 0) flag2 = 1;

					if ((flag2 == 1) && (line == cnt[idx1]))
					{
						// 뒤부터 읽으면서 통화명 먼저 출력
						for (int i = BUFSIZE; i >= 0; i--)
						{
							if (buf[i] == '<')
							{
								fprintf(fp2, "%c%c%c,", buf[i - 3], buf[i - 2], buf[i - 1]);
								printf("%c%c%c ", buf[i - 3], buf[i - 2], buf[i - 1]);
								break;
							}
						}

						// 현재 환율 출력
						for (int i = 0; i < BUFSIZE; i++)
						{
							// 한 나라 환율 출력 완료
							if ((flag3 == 1) && (buf[i] == '"'))
							{
								flag3 = 0;

								// 이전 대비 계산
								prev = atof(before[idx1]);
								now = atof(current[idx1]);
								gap = now - prev;
								fprintf(fp2, ",%.2f", gap);
								printf(" [%.2f]", gap);

								// 등락율 계산
								per = (now - prev) / prev * 100;
								fprintf(fp2, ",%.2f%%", per);
								printf(" [%.2f(%)]", per);

								// 마지막 나라까지 완료하면 아래 셀로 이동
								// current 초기화
								if (idx1 == 3)
								{
									fprintf(fp2, "\n"); printf("\n");
									memset(current, 0, ARRSIZE);
								}
								else
								{
									// 다음 줄의 바로 아래 셀로 이동
									fprintf(fp2, "\n,,,,,"); printf("\n");
								}

								break;
							}

							// 현재값 저장 & 출력
							if (flag3 == 1)
							{
								if (idx2 == 0) idx2 = i; // current배열의 처음부터 작성

								if (idx1 == 1) // 일본은 100엔 기준이라 다르게 출력
								{
									if (buf[i] == '.')
									{
										current[idx1][i - idx2] = buf[i + 1];
										current[idx1][i - idx2 + 1] = buf[i + 2];
										current[idx1][i - idx2 + 2] = buf[i];
										fprintf(fp2, "%c%c%c", buf[i + 1], buf[i + 2], buf[i]);
										printf("%c%c%c", buf[i + 1], buf[i + 2], buf[i]);
										i = i + 3;
									}
									current[idx1][i - idx2] = buf[i];
									fprintf(fp2, "%c", buf[i]);
									printf("%c", buf[i]);
								}
								else // 다른 나라는 그대로 출력
								{
									current[idx1][i - idx2] = buf[i];
									fprintf(fp2, "%c", buf[i]);
									printf("%c", buf[i]);
								}
							}

							// 통화명 있는 부분 찾기
							if ((flag3 == 0) && (buf[i] == '"')) flag3 = 1;
						}

						idx1++; // 다음 나라
						idx2 = 0;
						line = 0; // 라인 수 초기화
					}

					else if ((flag2 == 1) && (line != cnt[idx1])) line++;

					memset(buf, 0, BUFSIZE); // 버퍼 지우기(초기화)
				}
			}

			memset(buf, 0, BUFSIZE);
			memset(past, 0, 5);
			memset(present, 0, 5);
			memset(current, 0, ARRSIZE * 4);

			closesocket(sock);
			fclose(fp1);
			fclose(fp2);
			WSACleanup(); // 윈속 종료

			Sleep(SECOND);
		}
	}

	return NULL;
}

*/

int main()
{
	pthread_t thread[SZ]; // 회사 5, 종합주가지수 2, 환율 1

	int t_return; // thread가 종료되면 여기로 0 반환
	int t[SZ] = { 1, 2, 3, 4, 5, 6, 7 };

	for (int i = 0; i < SZ; i++)
	{
		//if (i == 7)         pthread_create(&thread[i], NULL, exchange_rate, (void*)&t[i]);
		if (i == 7 || i == 6) pthread_create(&thread[i], NULL, kospi_kosdaq, (void*)&t[i]);
		else                     pthread_create(&thread[i], NULL, stock, (void*)&t[i]);
	}
	for (int i = 0; i < SZ; i++)
		pthread_join(thread[i], (void**)&t_return);

	return 0;
}