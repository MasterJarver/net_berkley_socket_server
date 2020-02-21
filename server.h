#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

const int kSize = 1024;
const int kWhence = 1024;

class Server
{
public:
    explicit Server(int port);
    void acceptMetaData();
    void acceptFile();
    bool createFile();
private:
    int m_size;
    int m_port;
    char m_fileName[kSize];
    unsigned long long fileNameSize;
    unsigned long long m_waitSizeFile = 0; // waiting file size
    unsigned long long m_fileWriteBytes = 0;
    long long m_recvSize = 0; // read bytes from file
    unsigned long long m_writeFileBytes; // write bytes into file
    char* buffer[kSize];

    int m_serverSocket;
    int m_clientSocket;
    struct sockaddr_in m_sockAddr;

    FILE* m_file;
};

#endif // SERVER_H
