#include "trikcomm.h"
int main()
{
    //���������
    Trik trik("192.168.77.1");
    while(1){
        //��������
        trik.sendmsg(228,239,1488,1337);
        //���������
        cout<<trik.recievemsg(4)<<endl;
    }
}
