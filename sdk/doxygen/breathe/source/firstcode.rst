First Program
=============

The following section will get you started width the Quotek SDK with a very simple code, 
exactly like an hello world if you want.

Let's begin
-----------

First we start by generating some fake yet realistic prices data for an asset, say the DOW JONES index. We will assume a full day of quotation, meaning ~ 25000 values if we'd get 1 tick per second.

To do that with quotek there's nothing more simple, we'll just call the quotek::rand::generators::normal function:

.. code-block:: c++
  
  std::vector<quotek::data::record> dow_data = quotek::rand::generators::normal(25000,17000,200);

Once we have our fake data sample, what we'll do is to extract some properties from it. 
We will compute the minimum value, the maximum value, the mean and finally the standard deviation of the series. To do that, we'll make call to the functions located inside the quotek::quant namespace:


.. code-block:: c++

  float dow_min = quotek::quant::min(dow_data);
  float dow_max = quotek::quant::max(dow_data);
  float dow_mean = quotek::quant::AVG(dow_data);
  float dow_sigma = quotek::quant::SIGMA(dow_data);


Finally, we will display all the computed value of the series on the standard output:


.. code-block:: c++

  /* We display all the results */
  std::cout << "DOW MIN:" << dow_min << std::endl ;
  std::cout << "DOW MAX:" << dow_max << std::endl;
  std::cout << "DOW MEAN:" << dow_mean << std::endl;
  std::cout << "DOW SIGMA:" << dow_sigma << std::endl;


Full source code
----------------

You'll find below the full, merged source code which can be compiled as is:


.. code-block:: c++

  #include <quotek/generator.hpp>
  #include <iostream>

  int main(int argc, char** argv) {

    /* We generate a random sample of values to simulate our Dow Jones Data */
    std::vector<quotek::data::record> dow_data = quotek::rand::generators::normal(25000,17000,200);
 
    /* We compute all the properties we want from the asset data */
    float dow_min = quotek::quant::min(dow_data);
    float dow_max = quotek::quant::max(dow_data);
    float dow_mean = quotek::quant::AVG(dow_data);
    float dow_sigma = quotek::quant::SIGMA(dow_data);

    /* We display all the results */
    std::cout << "DOW MIN:" << dow_min << std::endl ;
    std::cout << "DOW MAX:" << dow_max << std::endl;
    std::cout << "DOW MEAN:" << dow_mean << std::endl;
    std::cout << "DOW SIGMA:" << dow_sigma << std::endl;

  }


