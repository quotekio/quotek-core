#include "aep_ws.hpp"

aep_ws_server::aep_ws_server(uint16_t port) {

        this->port = port;
        m_server.init_asio();
        m_server.set_reuse_addr(true);

        m_server.set_open_handler(bind(&aep_ws_server::on_open,this, std::placeholders::_1));
        m_server.set_close_handler(bind(&aep_ws_server::on_close,this, std::placeholders::_1));


}

aep_ws_server::~aep_ws_server() {

}

void aep_ws_server::on_open(connection_hdl hdl) {

  std::lock_guard<std::mutex> lock(m_mutex);
  m_connections.insert(hdl);

  for(auto iterator = buffmap.begin(); iterator != buffmap.end(); iterator++) {
  	
  	std::stringstream to_send;
    to_send << "{\"" << iterator->first << "\":" << iterator->second << "}";

    m_server.send(hdl, to_send.str(), websocketpp::frame::opcode::text );
  }



}
    
void aep_ws_server::on_close(connection_hdl hdl) {

  std::lock_guard<std::mutex> lock(m_mutex);
  m_connections.erase(hdl);

}
    

void aep_ws_server::broadcast(std::string buffname, std::string inp) {

  buffmap[buffname] = inp;
  //std::lock_guard<std::mutex> lock(m_mutex);
  std::stringstream to_send;
  to_send << "{\"" << buffname << "\":" << inp << "}";

  for (auto it : m_connections) {
    m_server.send(it,to_send.str(),websocketpp::frame::opcode::text);
  }

}


void aep_ws_server::run() {

    std::cout << "Starting AEP Websocket" << std::endl;

	m_server.listen(this->port);
	m_server.start_accept();
	m_server.run();

}

void aep_ws_server::stop() {
  m_server.stop();
}