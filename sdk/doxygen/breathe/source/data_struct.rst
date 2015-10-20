Data Structures
===============

This section explains how to import and store structured financial data with with Quotek SDK.

.. toctree::
   :hidden:
   record
   records
   history
   marray
   news

Time Series Data
----------------

There are three main time-series formats usable with Quotek SDK functions:

quotek::data::record
^^^^^^^^^^^^^^^^^^^^

quotek::data::record is the main tick data container. Its purpose is to store raw, real-time data provided by your broker. It has 3 main attributes:
  
  * ``timestamp:`` epoch timestamp of the tick (in seconds).
  * ``value:`` value of asset at time 'timestamp'.
  * ``spread:`` spread offered by broker at time 'timestamp'.

**Note about spread:** This attribute is not mandatory to create record objects, so you can work with it only if you have it.

for further infos, please read the :doc:`quotek::data::record documentation <record>`

quotek::data::records
^^^^^^^^^^^^^^^^^^^^^

quotek::data::records is a container of quotek::data::record. It embeddeds an internal vector container and various methods for processing this container.

The scheme displayed below represents the description of the quotek::data::records data structure:

.. image:: _static/img/recs_datastruct.png



quotek::data::history
^^^^^^^^^^^^^^^^^^^^^

The history container was created to store **(open,close,high,low)** time-series data, 
which is the typical data structure returned by financial databases like yahoo! finance, quandl, etc..

If you need to store full history time-series, you can vectorize the class by declaring an ``std::vector<quotek::data::history>`` 

Extracting and Down-Sampling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to analyze time-series data, you will often have to use a subset sample or reduce the amount of ticks in the sample.
This is why these functions were implemented in Quotek SDK, and they work as follows:

.. code-block:: c++

  #include <quotek/record.hpp>

  int main() {

    quotek::data::records r1({4559,4557,4556.2,4558.4});

    //we extract the last 2 values of r1:
    quotek::data::records r2 = r1.extract( r1.size() -2 , 2 );
 
    //we down-sample r1 to reduce it to only 2 values: 
    quotek::data::records r3 = r1.down_sample(2,1,"typical");

  }


Multi-Dimentional Arrays
------------------------

Quotek SDK embedds a series of classes which were created to instanciate and manipulate multi-dimentional
arrays. This is the original work of Pramod Gupta from University of Washington, and it was designed for
maximum performance and ease of use.

quotek::data::array{n}d
^^^^^^^^^^^^^^^^^^^^^^^
This is the main set of classes for handling multi-dimentional arrays. the {n} parameter depends 
of the number of dimensions you want to create for your array.

.. code-block:: c++
  #include <quotek/marray.hpp>
  
  int main() {

    //instanciates a 3 dimensions array
    quotek::data::array3d<float> a1(3,3,3);
    //assigns a value to array coord(1,1,1)
    a1.at(1,1,1) = 1.618;

    //instanciates a 7 dimensions array
    quotek::data::array<float> a2(23,38,50,61,100,123,138);
    //assigns a value to array coord(7,7,7,7,7,7,7)
    a2.at(7,7,7,7,7,7,7) = 1.618;

    //retrieves value we just set
    float golden = a2.at(7,7,7,7,7,7,7);

  }


Text Data
---------

Along with time series, text information can be vital to assess market contexts, especially financial news.
That's why the quotek::data::news class was created, and every news datasource class in the SDK uses it to return news content.


Namespace Documentation
-----------------------

.. doxygennamespace:: quotek::data
