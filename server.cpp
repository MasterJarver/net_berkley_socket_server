#include "server.h"

Server::Server(int port) : m_port(port)
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_sockAddr.sin_family = AF_INET;
    m_sockAddr.sin_port = htons(port);
    m_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(m_serverSocket, (struct sockaddr*)(&m_sockAddr), sizeof(m_sockAddr));
    listen(m_serverSocket, SOMAXCONN);
    m_clientSocket = accept(m_serverSocket, 0, 0);
    std::cout << "accept metadata" << std::endl;
}

void Server::acceptMetaData()
{
    while(m_recvSize == 0 || m_recvSize == -1)
    {
        m_recvSize = static_cast<unsigned long long>(recv(m_clientSocket, &m_waitSizeFile, sizeof(m_waitSizeFile), MSG_NOSIGNAL));
        std::cout << "wait size file: " <<m_waitSizeFile << std::endl;
        m_recvSize = static_cast<unsigned long long>(recv(m_clientSocket, &fileNameSize, sizeof(fileNameSize), MSG_NOSIGNAL));
        std::cout << "file name size: " << fileNameSize << std::endl;
        m_recvSize = recv(m_clientSocket, &m_fileName, fileNameSize, MSG_NOSIGNAL);
        std::cout << "file name: " << m_fileName << std::endl;
    }
}

void Server::acceptFile()
{
    unsigned long long writeBytes = 0;
    while(m_fileWriteBytes != m_waitSizeFile)
    {
        m_recvSize = static_cast<unsigned long long>(recv(m_clientSocket, buffer, kWhence, MSG_NOSIGNAL));
        if(m_recvSize != 0 || m_recvSize != -1) // if we have info for reading
        {
            writeBytes = static_cast<unsigned long long>(fwrite(buffer, 1, m_recvSize, m_file));
            m_fileWriteBytes += writeBytes;
            std::cout << "bytes: " << m_fileWriteBytes << std::endl;
        }
        else if(m_fileWriteBytes == m_waitSizeFile)
        {
            fclose(m_file);
            shutdown(m_clientSocket, SHUT_RDWR);
            close(m_clientSocket);
        }
    }
    shutdown(m_serverSocket, SHUT_RDWR);
    close(m_serverSocket);
    std::cout << "final bytes: " << m_fileWriteBytes << std::endl;
    std::cout << "end program." << std::endl;
}

bool Server::createFile()
{
    m_file = fopen(m_fileName, "wb");
    if(m_file == nullptr)
    {
        std::cout << "can't create file" << std::endl;
        return false;
    }
    else
        return true;
}
