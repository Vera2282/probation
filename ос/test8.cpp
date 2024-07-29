#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#define MSGMAX 512

using namespace std;

struct my_msgbuf
{
	long mtype;
	char mtext[MSGMAX];
};

int main(int argc, char* argv[])
{
	char com = argv[1][0];
	int num = 0;
	int key = 365;
	my_msgbuf mobj;
	if (argc == 3) num = atoi(argv[2]);
	if (com == 'c')
	{
		int fd = msgget(key, IPC_CREAT | IPC_EXCL | 0642);
		cout<<"Message Queue created"<<endl;
	}
	else if(com == 's')
	{
		int fd = msgget(key, 0);
		cout<<"Input your message: "<<endl;
		char mes[40];
		while(true)
		{
			cout<<"> ";
			cin.getline(mes, 40);
			if(strlen(mes) == 0) break;
			strcat(mobj.mtext, mes);
			strcat(mobj.mtext, "\n");
		}
		cout<<"Message sent"<<endl;	
		mobj.mtype = num;
		msgsnd(fd, &mobj, strlen(mobj.mtext)+1, IPC_NOWAIT);
		perror("msgsnd: ");
	}
	else if (com == 'r')
	{
		int fd = msgget(key, 0);
		msgrcv(fd, &mobj, MSGMAX, num, IPC_NOWAIT | MSG_NOERROR);
		cout <<"Message: "<<endl;
		cout<<mobj.mtext<<endl;
	}
	else if (com == 'd')
	{
		int fd = msgget(key, 0);
		msgctl(fd, IPC_RMID, 0);
		cout<<"Message Queue deleted"<<endl;
	}
	return 0;
}
