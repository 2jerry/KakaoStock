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

#define SZ 7 // ������ ����(ȸ�� 5, �����ְ����� 2)
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
	string str_start = "	        <dt>���� �ü� ����</dt>\n"; // ����
	string str_no = "	        <dd>����"; // ����
	string str_price = "	        <dd>���簡 "; // �и�
	string str_fin = "        </dl>\n"; // ��
	int flag1; // ��� ���� �÷���
	int flag2; // ���� �ߺ� ��� ���� �÷���
	int flag3; // �и� �ʿ� �÷���
			   //int flag4 = 0; // �帶�� �÷���
	int cnt; // �и��߿� ���� ���� ���� ����
	char updown[100] = { 0 }; // +-

	char before[ARRSIZE] = { 0 }; // ���� �ֱ� ü�ᰡ�� ������ �迭
	char current[ARRSIZE] = { 0 }; // ���� ü�ᰡ�� ������ �迭
	char past[ARRSIZE] = { 0 }; // ���� �ֱ� ����
	char present[ARRSIZE] = { 0 }; // ���� ����
	int idx2; // ARRSIZE�� ���� �迭�� �ε���
	int tmp;

	while (1)
	{
		char msg[500] = { 0 };
		flag1 = 0; flag2 = 0; flag3 = 0;
		tmp = 0; cnt = 0; idx2 = 0;
		memset(buf, 0, BUFSIZE);

		FILE *fp1 = fopen(fname1[t_id - 1], "w+"); // ��³����� ���Ͽ� �����ϱ� ��
		FILE *fp2 = fopen(fname2[t_id - 1], "a+"); // txt���Ϸ� ���ϴ� �κи� ���� 
		if (fp1 != 0 && fp2 != 0)
		{
			WSADATA wsa; // ���� �ʱ�ȭ
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;

			//socket()

			// 1. ���� ����(socket)
			// AF_INET: ���������� ���� �ָ� ������ ��ǻ�� ������ ���. �Ϲ������� IPv4���
			// SOCK_STREAM: TCP/IP ��������
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) return 0;

			hostent* remoteHost = gethostbyname("finance.naver.com"); // ���ּҸ� ip�� �ٲٱ� ���� 

																	  //connect()

																	  // 2. ���� ����(bind): IP�ּ�, PORT��ȣ ���� �Ҵ�
			SOCKADDR_IN serveraddr;
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(80);
			serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list)); //
			retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) return 0;

			strcpy(msg, get1[t_id - 1]);
			strcat(msg, "Host: finance.naver.com\r\n\r\n");// �������� ���� ��� �ۼ� 

			send(sock, msg, strlen(msg), 0);// �������� ������

			while (1)
			{
				if ((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break; // �������� ���� �ޱ�

				fprintf(fp1, "%s", buf); // ���Ͽ� ����
				memset(buf, 0, BUFSIZE); // ���� �����(�ʱ�ȭ)
			}



			// �ֽ� ���� ���
			printf("��%d�� ", t_id);
			fseek(fp1, 0, SEEK_SET);
			while (fgets(buf, BUFSIZE, fp1))
			{
				if ((flag1 == 1) && (strcmp(buf,str_fin.c_str()) == 0)) // ��
				{
					fprintf(fp2, "\n");
					printf("\n");
					break;
				}

				if (flag1 == 1)
				{
					if (flag2 == 3) // �� ���� ���
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
					if (flag2 == 0) // ���� ���
					{
						for (int i = 0; i < BUFSIZE; i++)
						{
							if (flag3 == 1)
							{
								fprintf(fp2, "%c%c%c%c,", buf[i], buf[i + 1], buf[i + 2], buf[i + 3]); // ��
								printf("%c%c%c%c/", buf[i], buf[i + 1], buf[i + 2], buf[i + 3]);
								fprintf(fp2, "%c%c,", buf[i + 7], buf[i + 8]); // ��
								printf("%c%c/", buf[i + 7], buf[i + 8]);
								fprintf(fp2, "%c%c,", buf[i + 12], buf[i + 13]); // ��
								printf("%c%c/", buf[i + 12], buf[i + 13]);
								fprintf(fp2, "%c%c:%c%c,", buf[i + 17], buf[i + 18], buf[i + 22], buf[i + 23]); // ��
								printf("%c%c:%c%c/", buf[i + 17], buf[i + 18], buf[i + 22], buf[i + 23]);

								i = i + 31;
								if (buf[i + 7] == '<') // �帶�� �Ǵ� ������
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

					str_no = "           <dd>����";
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

									if (strncmp(updown, "��", strlen("��")) == 0) updown[0] = '-';
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

				if ((flag1 == 0) && (strcmp(buf, str_start.c_str()) == 0)) // ����
					flag1 = 1;

				memset(buf, 0, BUFSIZE); // ���� �����(�ʱ�ȭ)
			}

			closesocket(sock);
			fclose(fp1);
			fclose(fp2);
			WSACleanup(); // ���� ����

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

	string str_time = "	                                <span id=\"time\">"; // �ð�
	string str_price = "	                <em id=\"now_value\">"; // ü�ᰡ
	string str_gap = "	                <span class=\"fluc\" id=\"change_value_and_rate\"><span>"; // ���ϴ��

	int flag1; // ~����� ���� �÷���
	int flag2; // �����~ ���� �÷���
	int flag3; // �ְ� ��� ���� �÷���
	int flag4; // ����,ü�ᰡ �ߺ� ��� ���� �÷���
			   //int flag5 = 0; // �帶�� �÷���

	int cnt[4] = { 12, 2, 4, 2 }; // ���� ��� ������ �ִ� �������κ��� ���� ���������� ���� ��
	int idx1; // cnt�迭�� �ε���(�� ���� ����� ������ ����)
	int line; // ���� ����κ��� ���� ��������� ���� �� ī����(cnt�迭�� ��ġ�ϸ� ���� ����)

	char before[ARRSIZE] = { 0 }; // ���� �ֱ� ü�ᰡ�� ������ �迭
	char current[ARRSIZE] = { 0 }; // ���� ü�ᰡ�� ������ �迭
	char past[ARRSIZE] = { 0 }; // ���� �ֱ� ����
	char present[ARRSIZE] = { 0 }; // ���� ����
	int idx2; // ARRSIZE�� ���� �迭�� �ε���
	int tmp;

	while (1)
	{
		char msg[500];
		flag1 = 0; flag2 = 0; flag3 = 0; flag4 = 0;
		idx1 = 0; idx2 = 0;
		line = 0; tmp = 0;
		memset(buf, 0, BUFSIZE);

		FILE *fp1 = fopen(fname1[t_id - 1], "w+"); // ��³����� ���Ͽ� �����ϱ� ��
		FILE *fp2 = fopen(fname2[t_id - 1], "a+"); // txt���Ϸ� ���ϴ� �κи� ���� 
		if (fp1 != 0 && fp2 != 0)
		{
			WSADATA wsa; // ���� �ʱ�ȭ
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;

			//socket()

			// 1. ���� ����(socket)
			// AF_INET: ���������� ���� �ָ� ������ ��ǻ�� ������ ���. �Ϲ������� IPv4���
			// SOCK_STREAM: TCP/IP ��������
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) return 0;

			hostent* remoteHost = gethostbyname("finance.naver.com"); // ���ּҸ� ip�� �ٲٱ� ���� 

																	  //connect()

																	  // 2. ���� ����(bind): IP�ּ�, PORT��ȣ ���� �Ҵ�
			SOCKADDR_IN serveraddr;
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(80);
			serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list)); //
			retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) return 0;

			strcpy(msg, get1[t_id - 1]);
			strcat(msg, "Host: finance.naver.com\r\n\r\n");// �������� ���� ��� �ۼ� 

			send(sock, msg, strlen(msg), 0);// �������� ������

			while (1)
			{
				if ((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break; // �������� ���� �ޱ�

				fprintf(fp1, "%s", buf); // ���Ͽ� ����
				memset(buf, 0, BUFSIZE); // ���� �����(�ʱ�ȭ)
			}


			// ���
			printf("��%d�� ", t_id);
			fseek(fp1, 0, SEEK_SET);
			while (fgets(buf, BUFSIZE, fp1))
			{
				// 3. �ð����� ��������
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
							if (buf[i + 1] < 80 || buf[i + 1] > 89) // �ð��� �ƴϸ�
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

				// 4. ü�ᰡ ��������
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

				// 6. ���ϴ��/����� ���
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

				// 7. �����~ ������ ���
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

			memset(buf, 0, BUFSIZE); // ���� �����(�ʱ�ȭ)

			closesocket(sock);
			fclose(fp1);
			fclose(fp2);
			WSACleanup(); // ���� ����

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

	char *str_time = "                  <span class=\"time\">";                         // �ð�
	char *str_count = "                  <span class=\"count\">���ȸ��<span class=\"num\">"; // ���ȸ��
	char *str_rate = "   <h3 class=\"h_calculator\"><span>ȯ��";                            // ȯ��
	int flag1; // �ð� ���� ��� �ߺ� ���� �÷���
	int flag2; // ù��° ������ ȯ������ �÷���
	int flag3; // ȯ�� �ݾ��� ��ġ �÷���
	int flag4; // ȯ�� ��� ȸ�� �ߺ� �÷���
	int line; // ���� ����κ��� ���� ��������� ���� �� ī����(cnt�迭�� ��ġ�ϸ� ���� ����)

	int cnt[4] = { 10, 9, 3, 19 }; // �ð������� �ִ� �������κ��� ���� ��������� ���� ��
	char before[4][ARRSIZE]; // ���� ȯ�������� ������ �迭
	char current[4][ARRSIZE]; // ���� ȯ�������� ������ �迭
	double prev; // ���� ȯ�� ��
	double now; // ���� ȯ�� ��
	double gap; // ���� - ����
	double per; // ����� (%)
	int idx1; // cnt�迭�� �ε���(�� ���� ����� ������ ����)
	int idx2; // current�迭�� �ε���

	char present[5] = { 0 }; // ���� ���ȸ��
	char past[5] = { 0 }; // ���� ���ȸ��

	memset(buf, 0, BUFSIZE);
	memset(before, 0, ARRSIZE * 4);
	memset(current, 0, ARRSIZE * 4);

	while (1)
	{
		flag1 = 0; flag2 = 0; flag3 = 0; flag4 = 0;
		idx1 = 0; idx2 = 0; line = 0;

		// ȯ��
		FILE *fp1 = fopen(fname1[t_id - 1], "w+"); // ��³����� ���Ͽ� �����ϱ� ����
		FILE *fp2 = fopen(fname2[t_id - 1], "a+"); // csv���Ϸ� ���ϴ� �κи� ���� 
		if (fp1 != 0 && fp2 != 0)
		{
			WSADATA wsa; // ���� �ʱ�ȭ
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;

			//socket()

			// 1. ���� ����(socket)
			// AF_INET: ���������� ���� �ָ� ������ ��ǻ�� ������ ���. �Ϲ������� IPv4���
			// SOCK_STREAM: TCP/IP ��������
			SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET) return 0;

			hostent* remoteHost = gethostbyname("finance.naver.com"); // ���ּҸ� ip�� �ٲٱ� ���� 

																	  //connect()

																	  // 2. ���� ����(bind): IP�ּ�, PORT��ȣ ���� �Ҵ�
			SOCKADDR_IN serveraddr;
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_port = htons(80);
			serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list)); //
			retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

			if (retval == SOCKET_ERROR) return 0;

			// ȯ��
			char msg[500] = "GET /marketindex/ HTTP/1.1\r\n";
			strcat(msg, "Host: finance.naver.com\r\n\r\n");// �������� ���� ��� �ۼ� 

			send(sock, msg, strlen(msg), 0);// �������� ������

			while (1)
			{
				if ((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break; // �������� ���� �ޱ�

				fprintf(fp1, "%s", buf); // ���Ͽ� ����
				memset(buf, 0, BUFSIZE); // ���� �����(�ʱ�ȭ)
			}



			// ȯ�� ���� ��� 
			fseek(fp1, 0, SEEK_SET);

			// 1. csv���Ͽ��� ���� ���ȸ�� ã��
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

			// 2. ���� html���� ��� ȸ�� ã��
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

			// 3. ���ȸ�� ��ġ ���ϸ� ȯ�� ���
			if (strcmp(past, present) != 0)
			{
				fseek(fp1, 0, SEEK_SET);
				fseek(fp2, 0, SEEK_SET);

				// 4. �� ó���� csv���� ���� ȯ�� �� ã��
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
					// 5. �ð� ���
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

					// 6. ���ȸ�� ���
					if (flag1 == 2)
					{
						if (strncmp(buf, str_count, strlen(str_count)) == 0)
						{
							fprintf(fp2, "%s,", present);
							printf("%s\n", present);
							flag1 = 1;
						}
					}

					// 7. ȯ�� ��� ����
					// ȯ�� ������ ���۵Ǵ� ���� ã��
					if (strncmp(buf, str_rate, strlen(str_rate)) == 0) flag2 = 1;

					if ((flag2 == 1) && (line == cnt[idx1]))
					{
						// �ں��� �����鼭 ��ȭ�� ���� ���
						for (int i = BUFSIZE; i >= 0; i--)
						{
							if (buf[i] == '<')
							{
								fprintf(fp2, "%c%c%c,", buf[i - 3], buf[i - 2], buf[i - 1]);
								printf("%c%c%c ", buf[i - 3], buf[i - 2], buf[i - 1]);
								break;
							}
						}

						// ���� ȯ�� ���
						for (int i = 0; i < BUFSIZE; i++)
						{
							// �� ���� ȯ�� ��� �Ϸ�
							if ((flag3 == 1) && (buf[i] == '"'))
							{
								flag3 = 0;

								// ���� ��� ���
								prev = atof(before[idx1]);
								now = atof(current[idx1]);
								gap = now - prev;
								fprintf(fp2, ",%.2f", gap);
								printf(" [%.2f]", gap);

								// ����� ���
								per = (now - prev) / prev * 100;
								fprintf(fp2, ",%.2f%%", per);
								printf(" [%.2f(%)]", per);

								// ������ ������� �Ϸ��ϸ� �Ʒ� ���� �̵�
								// current �ʱ�ȭ
								if (idx1 == 3)
								{
									fprintf(fp2, "\n"); printf("\n");
									memset(current, 0, ARRSIZE);
								}
								else
								{
									// ���� ���� �ٷ� �Ʒ� ���� �̵�
									fprintf(fp2, "\n,,,,,"); printf("\n");
								}

								break;
							}

							// ���簪 ���� & ���
							if (flag3 == 1)
							{
								if (idx2 == 0) idx2 = i; // current�迭�� ó������ �ۼ�

								if (idx1 == 1) // �Ϻ��� 100�� �����̶� �ٸ��� ���
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
								else // �ٸ� ����� �״�� ���
								{
									current[idx1][i - idx2] = buf[i];
									fprintf(fp2, "%c", buf[i]);
									printf("%c", buf[i]);
								}
							}

							// ��ȭ�� �ִ� �κ� ã��
							if ((flag3 == 0) && (buf[i] == '"')) flag3 = 1;
						}

						idx1++; // ���� ����
						idx2 = 0;
						line = 0; // ���� �� �ʱ�ȭ
					}

					else if ((flag2 == 1) && (line != cnt[idx1])) line++;

					memset(buf, 0, BUFSIZE); // ���� �����(�ʱ�ȭ)
				}
			}

			memset(buf, 0, BUFSIZE);
			memset(past, 0, 5);
			memset(present, 0, 5);
			memset(current, 0, ARRSIZE * 4);

			closesocket(sock);
			fclose(fp1);
			fclose(fp2);
			WSACleanup(); // ���� ����

			Sleep(SECOND);
		}
	}

	return NULL;
}

*/

int main()
{
	pthread_t thread[SZ]; // ȸ�� 5, �����ְ����� 2, ȯ�� 1

	int t_return; // thread�� ����Ǹ� ����� 0 ��ȯ
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