#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;
#define BUFF 120
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
class Trik
{
private:
    SOCKET s_out;
    SOCKET s_in;
    sockaddr_in addr_out;
    sockaddr_in addr_in;
    char *buf;

public:
    bool active;
    //�����������
    Trik(string s_IP)
    {
        active=false;
        static bool flag = true;
        int err=0;
        buf = new char[BUFF];
        //��������� ������ ���� ���
        if(flag){
            WSADATA wsaData;
            err|=WSAStartup(MAKEWORD(2, 2), &wsaData);;
        }
        flag=false;
        if(err)
            return;
        //������������ ��������
        s_out = socket(AF_INET, SOCK_STREAM, 0);
        addr_out.sin_family = AF_INET;
        addr_out.sin_port = htons(4444);
        addr_out.sin_addr.s_addr = inet_addr(s_IP.c_str());
        err|=connect(s_out,(SOCKADDR *) & addr_out, sizeof (addr_out));
        if(err)
            return;
        //������������ �����
        s_in=socket(AF_INET,SOCK_STREAM,0);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(8888);
        addr_in.sin_addr.s_addr = inet_addr(s_IP.c_str());
        err|=connect(s_in,(SOCKADDR *) &addr_in, sizeof(addr_in));
        //�������� ������ ���� ��� ������
        if(err==0)
            active=true;
    }
    //����������
    ~Trik()
    {
        delete [] buf;
        //��������� �����
        active=false;
        static bool flag = true;
        closesocket(s_out);
        closesocket(s_in);
        if(flag)
            WSACleanup();
        flag=false;
    }
    //��������� ��� ������
    void sendmsg(unsigned int* val)
    {
        string pad0="pad 0 "+SSTR(val[0])+" "+SSTR(val[1])+"\n";
        string pad1="pad 1 "+SSTR(val[2])+" "+SSTR(val[3])+"\n";
        if(active)
        {
            sendto(s_out,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
            sendto(s_out,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
        }
    }
    int recievemsg(unsigned int len)
    {
        //��������� ����������
        if(!active||len>100)
            return -1;
        //�������� ������
        int n=recv(s_in, buf, (size_t)(len+((len+7)>9?10:9)), 0),l=0,v=0;
        //��������� �� ������������
        if(buf[1]==':'){
            if(n!=buf[0]-'0'+2||len!=buf[0]-'0'-7)
                return -2;
        }
        else if(buf[2]==':')
            if(n!=buf[0]*10+buf[1]-'0'*11+3||len!=buf[0]*10+buf[1]-'0'*11-7)
                return -2;
        else; else return -2;
        //������
        if(string(buf).substr(n-len-8,8)!=":print: ")
            return -1;
        for(int i=0;i<n&&buf[i]!=':';i++)
            l=l*10+buf[i]-'0';
        for(int i=(n-l)+7;i<n;i++)
            v=v*10+buf[i]-'0';
        //���������� ��������
		return v;
    }
    //������������� ��� ��������
    void sendmsg(unsigned int a, unsigned int b=0, unsigned int c=0, unsigned int d=0)
    {
        unsigned int vals[4]={a,b,c,d};
        sendmsg(vals);
        delete [] vals;
    }
};
