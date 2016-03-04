#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "history.hpp"
#include "quant.hpp"
#include "techanalysis.hpp"
#include "option.hpp"
#include "blackscholes.hpp"
#include "datasource.hpp"
#include "datasources/quandl.hpp"


typedef quotek::quant::affine lreg_s1(quotek::data::records&);
typedef void lreg_s2(quotek::data::records&, std::vector<float>&);

typedef quotek::quant::pivot pivot_s1(quotek::data::records&);
typedef quotek::quant::pivot pivot_s2(float,float,float);

namespace py = pybind11;

PYBIND11_PLUGIN(pyquotek) {
    py::module m("pyquotek", "pyquotek python module.");

    pybind11::module quant_m = m.def_submodule("quant", "Quotek quantitative module");
    pybind11::module data_m = m.def_submodule("data", "Quotek data module");
    pybind11::module ta_m = m.def_submodule("ta", "Quotek Technical Analysis module");
    pybind11::module datasources_m = m.def_submodule("datasources", "Quotek datasources module");


    //RECORD

    py::class_<quotek::data::record>(data_m, "Record")
        .def(py::init())
        .def(py::init<long, float, float>())
        .def_readwrite("timestamp",&quotek::data::record::timestamp)
        .def_readwrite("value",&quotek::data::record::value)
        .def_readwrite("spread",&quotek::data::record::spread);


    py::class_<quotek::data::records>(data_m, "Records")
        .def(py::init())
        .def(py::init<std::vector<float>>())
        .def(py::init<std::vector<quotek::data::record> >())
        .def("__getitem__", &quotek::data::records::operator [])
        .def("append", (void (quotek::data::records::*)(float)) &quotek::data::records::append)
        .def("append", (void (quotek::data::records::*)(quotek::data::record&)) &quotek::data::records::append)
        .def("append", (void (quotek::data::records::*)(long, float)) &quotek::data::records::append)
        .def("append", (void (quotek::data::records::*)(long, float, float)) &quotek::data::records::append);

    //HISTORY

    py::class_<quotek::data::history>(data_m, "History")
        .def(py::init())
        .def(py::init<long,float,float,float,float>())
        .def_readwrite("timestamp",&quotek::data::history::timestamp)
        .def_readwrite("open",&quotek::data::history::open)
        .def_readwrite("high",&quotek::data::history::high)
        .def_readwrite("low",&quotek::data::history::low)
        .def_readwrite("close",&quotek::data::history::close);
        
    //QUANT

    py::class_<quotek::quant::affine>(quant_m,"Affine")
        .def_readwrite("a",&quotek::quant::affine::a)
        .def_readwrite("b",&quotek::quant::affine::b);

    py::class_<quotek::quant::trinom>(quant_m,"Trinom")
        .def_readwrite("a",&quotek::quant::trinom::a)
        .def_readwrite("b",&quotek::quant::trinom::b)
        .def_readwrite("c",&quotek::quant::trinom::c);    

    py::class_<quotek::quant::trend_p>(quant_m,"Trend_p")
        .def_readwrite("bull",&quotek::quant::trend_p::bull)
        .def_readwrite("bear",&quotek::quant::trend_p::bear)
        .def_readwrite("neutral",&quotek::quant::trend_p::neutral);
    
    py::class_<quotek::quant::fibo_ret>(quant_m,"FiboRetrace")
        .def_readwrite("p0",&quotek::quant::fibo_ret::p0)
        .def_readwrite("p23",&quotek::quant::fibo_ret::p23)
        .def_readwrite("p38",&quotek::quant::fibo_ret::p38)
        .def_readwrite("p50",&quotek::quant::fibo_ret::p50)
        .def_readwrite("p61",&quotek::quant::fibo_ret::p61)
        .def_readwrite("p100",&quotek::quant::fibo_ret::p100);

    py::class_<quotek::quant::fibo_ext>(quant_m,"FiboExt")
        .def_readwrite("p100",&quotek::quant::fibo_ext::p100)
        .def_readwrite("p127",&quotek::quant::fibo_ext::p127)
        .def_readwrite("p161",&quotek::quant::fibo_ext::p161)
        .def_readwrite("p200",&quotek::quant::fibo_ext::p200)
        .def_readwrite("p261",&quotek::quant::fibo_ext::p261);

    py::class_<quotek::quant::pivot>(quant_m,"Pivot")
        .def_readwrite("s1",&quotek::quant::pivot::s1)
        .def_readwrite("s2",&quotek::quant::pivot::s2)
        .def_readwrite("s3",&quotek::quant::pivot::s3)
        .def_readwrite("r1",&quotek::quant::pivot::r1)
        .def_readwrite("r2",&quotek::quant::pivot::r2)    
        .def_readwrite("r3",&quotek::quant::pivot::r3);

    py::class_<quotek::quant::_greeks>(quant_m,"Greeks")
        .def_readwrite("delta",&quotek::quant::_greeks::delta)
        .def_readwrite("gamma",&quotek::quant::_greeks::gamma)
        .def_readwrite("theta",&quotek::quant::_greeks::theta)
        .def_readwrite("vega",&quotek::quant::_greeks::vega)
        .def_readwrite("rho",&quotek::quant::_greeks::rho);
       

    quant_m.def("above", &quotek::quant::above, "tells if the values contained in the provided dataset are mostly above or below value");
    quant_m.def("cross", &quotek::quant::cross, "Tells if 2 dataset values cross.");
    quant_m.def("cross_ex", &quotek::quant::cross_ex, "Tells if 2 dataset values cross, with extra info as return.");
    quant_m.def("min", &quotek::quant::min, "get min value from dataset.");
    quant_m.def("max", &quotek::quant::max, "get max value from dataset.");
    quant_m.def("average", &quotek::quant::average, "compute dataset average value");
    quant_m.def("weighted_average", &quotek::quant::weighted_average, "compute dataset weighted average value");
    
    quant_m.def("variance",&quotek::quant::variance, "Computes dataset's variance.");
    quant_m.def("covariance",&quotek::quant::covariance, "Computes dataset's covariance.");

    quant_m.def("stddev", &quotek::quant::standard_deviation, "Computes dataset's standard deviation.");    
    quant_m.def("pdelta", &quotek::quant::percent_delta, "Computes dataset's variation percentage between first and last element.");

    quant_m.def("sma", &quotek::quant::moving_average, "Computes dataset's moving average.");
    quant_m.def("ema", &quotek::quant::exponential_moving_average, "Computes dataset's EMA." );
    quant_m.def("wma", &quotek::quant::weighted_moving_average, "Computes dataset's WMA." );

    quant_m.def("linear_regression", (lreg_s1*) &quotek::quant::linear_regression, "Computes dataset's Linear regression." );
    quant_m.def("linear_regression", (lreg_s2*) &quotek::quant::linear_regression, "Computes dataset's Linear regression." );

    quant_m.def("trend_percentages", &quotek::quant::trend_percentages, "Computes the trend percentage of given dataset." );
    quant_m.def("rrr", &quotek::quant::risk_reward_ratio, "Computes the risk reward ratio given some parameters." );

    quant_m.def("cdf", &quotek::quant::normal_cumulative_distribution, "Computes the normal cumulative distribution for given value." );
    quant_m.def("sndf", &quotek::quant::standard_normal_distribution, "Computes the normal standard distribution for given value." );

    //TECHANALYSIS
    
    ta_m.def("macd", &quotek::ta::moving_average_convergeance_divergeance, "Computes MACD for dataset.");
    ta_m.def("fibo_retrace", &quotek::ta::fibo_retrace, "Computes fibo retracement for given dataset.");
    ta_m.def("fibo_extension", &quotek::ta::fibo_extension, "Computes fibo extension for given dataset.");
    ta_m.def("pivots",(pivot_s1*) &quotek::ta::pivots, "Computes pivot points for given dataset.");
    ta_m.def("pivots",(pivot_s2*) &quotek::ta::pivots, "Computes pivot points for given dataset.");

    //OPTIONS

    py::class_<quotek::quant::option>(quant_m,"Option")
      .def(py::init())
      .def(py::init<int,std::string,float,float,float>())
      .def_readwrite("type",&quotek::quant::option::type)
      .def_readwrite("underlying",&quotek::quant::option::underlying)
      .def_readwrite("expires",&quotek::quant::option::expires)
      .def_readwrite("price",&quotek::quant::option::price)
      .def_readwrite("strike",&quotek::quant::option::strike)
      .def("is_in_money",&quotek::quant::option::is_in_money,"Tells if the option is in the money or not.");

    //BLACKSCHOLES
 
    py::class_<quotek::quant::blackscholes>(quant_m,"Blackscholes")
      .def(py::init<quotek::quant::option&,float,float,float>())
      .def_readwrite("safe_interest_rate",&quotek::quant::blackscholes::safe_interest_rate)
      .def_readwrite("implied_volatility",&quotek::quant::blackscholes::implied_volatility)
      .def_readwrite("underlying_price",&quotek::quant::blackscholes::underlying_price)
      .def("compute_greeks",&quotek::quant::blackscholes::compute_greeks,"Computes the Option's Greeks.")
      .def("price_option",&quotek::quant::blackscholes::price_option,"Computes the correct price of the option given the provided parameters.");


    //DATASOURCE
    py::class_<quotek::datasource::datasource> dsource(datasources_m, "Datasource");
    dsource.def(py::init());
    dsource.def_readwrite("source",&quotek::datasource::datasource::source);

    //QUANDL
      
    py::class_<quotek::datasource::quandl>(datasources_m,"Quandl", dsource)
      .def(py::init<std::string>())
      .def("get_databases",&quotek::datasource::quandl::get_databases,"")
      .def("get_metadata", &quotek::datasource::quandl::get_metadata,"")
      .def("query", &quotek::datasource::quandl::query,"")
      .def("query_prices", &quotek::datasource::quandl::query_prices,"")
      .def("query_fullprices", &quotek::datasource::quandl::query_fullprices,"")
      .def("get_column", &quotek::datasource::quandl::get_column,"");
      

    py::class_<quotek::datasource::quandl_meta>(datasources_m, "QuandlMeta")
        .def(py::init())
        .def_readwrite("id",&quotek::datasource::quandl_meta::id)
        .def_readwrite("name",&quotek::datasource::quandl_meta::name)
        .def_readwrite("code",&quotek::datasource::quandl_meta::code)
        .def_readwrite("type",&quotek::datasource::quandl_meta::type)
        .def_readwrite("description",&quotek::datasource::quandl_meta::description)
        .def_readwrite("columns",&quotek::datasource::quandl_meta::columns)
        .def_readwrite("frequency",&quotek::datasource::quandl_meta::frequency);


    py::class_<quotek::datasource::quandl_db>(datasources_m, "QuandlDB")
        .def(py::init())
        .def_readwrite("id",&quotek::datasource::quandl_db::id)
        .def_readwrite("name",&quotek::datasource::quandl_db::name)
        .def_readwrite("code",&quotek::datasource::quandl_db::code)
        .def_readwrite("description",&quotek::datasource::quandl_db::description)
        .def_readwrite("datasets",&quotek::datasource::quandl_db::datasets);
    
    return m.ptr();
}
