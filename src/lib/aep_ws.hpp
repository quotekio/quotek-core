#ifndef AEP_WS_H
#define AEP_WS_H

#include <string>
#include <iostream>
#include <functional>
#include <mutex>
#include <set>
#include <thread>

#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;

class aep_ws_server {
  public:

    aep_ws_server(uint16_t port);
    ~aep_ws_server();
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void broadcast(std::string buffname, std::string inp);
    void run();
    void stop();

  private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    int m_count;
    uint16_t port;
    server m_server;
    con_list m_connections;
    std::mutex m_mutex;
    std::map<std::string, std::string> buffmap;
};


#endif
