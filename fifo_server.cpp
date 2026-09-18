#include"comm.h"

int main()
{
    Fifo fifo;
    NamePipe name_pipe;
    string buffer;
    int fd=name_pipe.ReadForFifo();
    while(true)
    {
        buffer=name_pipe.Read();
        cout<<buffer<<endl;
    }
    
}