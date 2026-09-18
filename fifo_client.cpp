#include"comm.h"

int main()
{
    NamePipe name_pipe;
    name_pipe.WriteForFifo();
    string buffer="i am process A";
    for(int i=0;;i++)
    {
        name_pipe.Write(buffer+to_string(i));
        sleep(1);
        cout<<"正在向读端写入信息..."<<endl;
    }
}