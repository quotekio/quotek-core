//#asset_match (.*)

using namespace quotek;
using namespace quotek::core;

class moving_average_crossing : public strategy {

  public:
    //we inherit constructor
    using strategy::strategy;

    broker* b0;

    int initialize() {
      std::cout << "INIT!" << std::endl;
      b0 = new broker(this);
      return 0;
    }

    void evaluate() {

      std::cout << "EVAL" << std::endl;    


    }

};

