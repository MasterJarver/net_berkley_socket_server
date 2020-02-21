/*
* Created by Oleh Korshun
* email: oleh.korshun@gmail.com
*/
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"

const int kPort = 12345;

int main()
{
    Server server(kPort);
    server.acceptMetaData();
    if(!server.createFile())
    {
        std::cout << "can't create file" << std::endl;
        return -1;
    }
    server.acceptFile();
    return 0;
}
