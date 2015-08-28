Install
=======

Using Quotek SDK in your C++ code is fairy easy, since it only consists of a library and header files.
Please note that if you're using our Quotek SaaS solution, this section doesn't concern 
you because everything is already setup for your inside your server environment.

Windows Install
---------------

  Download and unzip the quotek SDK archive file, and launch the "installer.pwsh"
  file which is located at the root of the decompressed directory. The SDK will then be installed in 
  **C:\\Program Files\\quotek\\sdk**.

Building & Linking
^^^^^^^^^^^^^^^^^^ 

  When using Quotek SDK on windows, The first thing you have to do is to let your IDE / Compiling Toolchain know where the
  headers and libraries are located.

  For Visual studio 2013, the procedure is the following:

  
Linux Install
-------------

  Download and unzip the quotek SDK archive file, and launch the "installer_linux.sh" script 
  located at the root of the decompressed directory. All of the Quotek SDK will then be installed 
  in /opt/quotek/sdk. The libraries are located in **/opt/quotek/sdk/lib** and headers are in **/opt/quotek/sdk/include**.

Building & Linking
^^^^^^^^^^^^^^^^^^

  Given that the headers and libraries are not installed at the usual places in the system, you will need to 
  add a few arguments to your compilation line. Also, because libquotek needs them, you will need to link the programs you code with libcurl and pthread.

  So the typical compilation line of a program using quotek SDK on Linux is the following:

  ``g++ -I/opt/quotek/sdk/include -L/opt/quotek/sdk/lib -o <YOUR_PROGRAM> <YOUR_C++_FILE> -lpthread -lcurl -lquotek``










