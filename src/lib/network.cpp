#include "network.h"

networkSession::networkSession(int snum) {
  socknum = snum;
  alive = true;
}

bool networkSession::isAlive() {
  return alive;
}


int networkSession::send_data(std::string msg) {

  int len=0;
  len = send(socknum,msg.c_str(),msg.length() ,0);

  if (len == -1) alive = false;
  return len;
}

std::string networkSession::recv_data() {

  int len;
  int bufflen = 512;
  char buff[bufflen];

  for (int i=0;i<bufflen;i++) buff[i] = 0x00;

  len = recv(socknum,buff,bufflen,0);
  if (len == -1) { 
    alive = false;
  }

  return std::string(buff);

}


void networkSession::close_connection() {
  close(socknum);
}


network::network() {

}

int network::server(std::string laddr,int port) {
  
  listen_addr = laddr;
  listen_port = port;

  struct sockaddr_in sin;
  int val=1;

  if((socknum=socket(AF_INET,SOCK_STREAM,0))<0) {
    std::cerr << "ERROR: CANNOT CREATE SOCKET\n" << std::endl;
    exit(1);
  }

  memset(&sin,0,sizeof(sin));

  if ( laddr == "0.0.0.0") {
     sin.sin_addr.s_addr=INADDR_ANY;
  }

  else {
      sin.sin_addr.s_addr = inet_addr(laddr.c_str());
  }

  sin.sin_family=AF_INET;
  sin.sin_port=htons(port);
  setsockopt(socknum,SOL_SOCKET,SO_REUSEADDR,
    &val,sizeof(val));

  
  if( bind(socknum,(struct sockaddr *) &sin,sizeof(sin)) < 0) {
    std::cerr << "ERROR: CANNOT BIND SOCKET ON " << laddr << ":" << port << std::endl;
    exit(1);
  }

  listen(socknum,5);
  return socknum;

}


networkSession* network::server_accept() {

  struct sockaddr_in peer_addr;
  socklen_t addr_len = sizeof(peer_addr);
  int sock_id = 0;

  if((sock_id = accept(socknum,(struct sockaddr*)&peer_addr,&addr_len))<0) {
      std::cerr << "ERROR: CANNOT ACCEPT NEW CONNECTION ON " << listen_addr << ":" <<  listen_port  << std::endl;
      return NULL;
  }

  else {
      std::cout << "NEW CONN ACCEPTED !" << std::endl;
      networkSession* nsession = new networkSession(sock_id);
      return nsession;
  }

}

int network::getSocket() {
  return socknum;
}