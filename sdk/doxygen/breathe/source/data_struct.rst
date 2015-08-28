Data Structures
===============

This section explains how to import and store structured financial data with with Quotek SDK.

.. toctree::
   :hidden:

   record
   records
   history
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

.. image:: /img/recs_datastruct.png





quotek::data::history
^^^^^^^^^^^^^^^^^^^^^

The history container was created to store **(open,close,high,low)** time-series data, 
which is the typical data structure returned by financial databases like yahoo! finance, quandl, etc..

If you need to store full history time-series, you can vectorize the class by declaring an ``std::vector<quotek::data::history>`` 

Extracting and Down-Sampling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to analyze time-series data, you will often have to use a subset sample or reduce the amount of ticks in the sample.
This is why these functions were implemented in Quotek SDK, and they work as follows:


Text Data
---------

Along with time series, text information can be vital to assess market contexts, especially financial news.
That's why the quotek::data::news class was created, and every news datasource class in the SDK uses it to return news content.

Namespace Documentation
-----------------------

.. doxygennamespace:: quotek::data
