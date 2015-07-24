#asset_match "FX_(.*)"

using namespace quotek;

class moving_average_crossing : public strategy {

  public:

    std::vector<float> sma20;
    std::vector<float> sma50;
    std::vector<float> ema50;

    broker b0;

    int initialize() {
      return 0;

      data::records closing_100; 

      sma20 = quant::SMA(closing_100,20);
      sma50 = quant::SMA(closing_100,50);
      ema50 = quant::EMA(closing_100,50);

      b0 = new broker(this);

    }

    void evaluate() {

      



    }

}