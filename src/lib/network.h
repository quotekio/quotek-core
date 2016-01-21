#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <string>
#include <iostream>


class networkSession {

  public:
  	networkSession(int);
    int send_data(std::string);
    std::string recv_data();
    void close_connection();
    bool isAlive();
  private:
    int socknum;
    bool alive;

};

class network {

  public:
    network();
    int server(std::string,int);
    networkSession* server_accept();
    int getSocket();
    

  private:
    int socknum;
    std::string listen_addr;
    int listen_port;

};

#endif
