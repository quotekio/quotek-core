//#asset_match (.*)

//#include <thread>

using namespace quotek;
using namespace quotek::core;

class moving_average_crossing : public strategy {

  public:

    broker* b0;

    int initialize() {
      std::cout << asset_name << ":INIT!" << std::endl;
      b0 = new broker(this);
      
      //#ex_eval 10 this->eval2();
      //#ex_eval 60 this->eval3();

      return 0;
    }

    void eval2() {
        std::cout << asset_name << ":EVAL2" << std::endl;
    }

    void eval3() {
        std::cout << asset_name << ":EVAL3" << std::endl;
    }

    void evaluate() {
      std::cout << asset_name << ":EVAL" << std::endl;
    }




};

