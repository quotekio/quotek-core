Quantitative Analysis
=====================

The following section describes all the functions of the SDK which are meant to analyse your time-series from a quantitative perspective.


.. doxygennamespace:: quotek::quant
   :members:

Example
-------

In the following example we will see how to use the statistical analysis functions listed above.

.. code-block:: c++

    #include <quotek/record.hpp>
    #include <quotek/quant.hpp>
    using namespace quotek::data;
    using namespace quotek::quant;

    int main() {


      /* First we create fake data. */      

      std::vector<long> tstamps = { 1437143900, 
                                    1437143901, 
                                    1437143902, 
                                    1437143903, 
                                    1437143904, 
                                    1437143905, 
                                    1437143906, 
                                    1437143907, 
                                    1437143908 };

      std::vector<float> vals = { 4530, 4575, 4890, 4235, 4144, 4356, 4588, 4720, 4989 };


      /* Then we create a records container from these data. */
      records r1 = records(vals);
     
      for (int i=0;i< r1.size(); i++ ) {
        r1[i].timestamp = tstamps[i];
        
      }

      /* We find the minimum value inside the r1 time-series, value is 4144. */
      float minval = min(r1);
      
      /* Now we compute the 5 periods Exponential moving average for the r1 time-series.
         Result is the following: {4552.5, 4560, 4670, 4525, 4398, 4384, 4452, 4541.33 }. */

      records r3 = EMA(r1,5);

      /* Finally We compute the linear regression of the r1 time-series. Affine parameters are 
      (29.8, 4409.56) */

      affine af = linear_regression(r1);
    }

 
.. toctree::
   :includehidden:
   generators
   blackscholes

