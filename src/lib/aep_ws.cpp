#include "aep_ws.hpp"

aep_ws_server::aep_ws_server() {

        m_server.init_asio();       
        m_server.set_open_handler(bind(&aep_ws_server::on_open,this, std::placeholders::_1));
        m_server.set_close_handler(bind(&aep_ws_server::on_close,this, std::placeholders::_1));

}

aep_ws_server::~aep_ws_server() {

}

void aep_ws_server::on_open(connection_hdl hdl) {

  std::lock_guard<std::mutex> lock(m_mutex);
  m_connections.insert(hdl);

}
    
void aep_ws_server::on_close(connection_hdl hdl) {

  std::lock_guard<std::mutex> lock(m_mutex);
  m_connections.erase(hdl);

}
    
void aep_ws_server::broadcast() {

  //std::lock_guard<std::mutex> lock(m_mutex);    
  for (auto it : m_connections) {

    m_server.send(it,input.str(),websocketpp::frame::opcode::text);

  }

}

void aep_ws_server::broadcast(std::string inp) {

  //std::lock_guard<std::mutex> lock(m_mutex);    
  for (auto it : m_connections) {

    m_server.send(it,inp,websocketpp::frame::opcode::text);

  }

}




void aep_ws_server::run(uint16_t port) {

	m_server.listen(port);
	m_server.start_accept();
	m_server.run();

}