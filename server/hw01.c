#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")


int merror(int redata, int error,char* showinfo) {
	if (redata == error) {
		perror(showinfo);
		getchar();
		return -1;
	}
	return 0;
}
void sendhtml(SOCKET s, char* filename);
int main() {
	printf("�w��ϥ�webser\n");
	WSADATA wsdata;
	int isok=WSAStartup(MAKEWORD(2,2),&wsdata);//�T�wsocket����
	//short 2 character 2.2
	merror(isok, WSAEINVAL, "�ӽ�socket����");


	//�Ĥ@��:��ĳ�ڡA�M�wsocket�a�}����
	//�ĤG��:�ǿ�����,SOC_STREAM�ǿ�
	//�ĤT��:���w�ǿ��ĳ,tcp
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	merror(server, INVALID_SOCKET, "�Ы�socket����");

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(80);
	seraddr.sin_addr.s_addr = INADDR_ANY;//��ť���N�a�}

	isok=bind(server, &seraddr, sizeof(seraddr));
	merror(isok, SOCKET_ERROR, "�j�w����...\n");

	isok = listen(server, 5);
	merror(isok, SOCKET_ERROR, "��ť����...\n");

	struct sockaddr_in claddr;
	int cllen = sizeof(claddr);
	while (1) {
		SOCKET client = accept(server, &claddr, &cllen);
		merror(client, INVALID_SOCKET, "�s������...\n");


		char revdata[1024] = "";
		recv(client, revdata, 1024, 0);
		printf("%s �@������%d�r�`�ƾ�\n",revdata,strlen(revdata));

		char sendata[1024] = "<h1 style=\	color:red;\">�A�n�A�ڦ��\�F!</h1>";
		send(client, sendata, strlen(sendata), 0);
		//char *filename = "./index.html";
		//sendhtml(client, filename);

		closesocket(client);
	}

	closesocket(server);
	WSACleanup();

	getchar();
	return 0;
}

void sendhtml(SOCKET s, char* filename) {
	FILE *pfile = fopen(filename,"r");
	if (pfile == NULL) {
		printf("�}�Ҥ�󥢱�\n");
		return;
	}
	char temp[1024] = "new";
	do {
		fgets(temp, 1024, pfile);
		send(s, temp, strlen(temp), 0);

	} while (!feof(pfile));
}
