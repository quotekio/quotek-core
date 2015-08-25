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

quotek::data::records
^^^^^^^^^^^^^^^^^^^^^

quotek::data::history
^^^^^^^^^^^^^^^^^^^^^

Extracting and Down-Sampling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to analyze time-series data, you will often have to use a subset sample or reduce the amount of ticks in the sample.
This is why these functions were implemented in Quotek SDK, and they work as follows:


Text Data
---------

Along with time series, text information can be vital to assess market contexts, especially financial news.
That's why the quotek::data::news class was created, and every news datasource class in the SDK uses it to return news content.

Namespace Doc
-------------

.. doxygennamespace:: quotek::data
