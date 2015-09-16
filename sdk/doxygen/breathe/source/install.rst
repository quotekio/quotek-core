Install
=======

Using Quotek SDK in your C++ code is fairy easy, since it only consists of a library and header files.
Please note that if you're using our Quotek SaaS solution, this section doesn't concern 
you because everything is already setup for your inside your server environment.

Windows Install
---------------

Download and unzip the quotek SDK archive file. Then right-click on the "installer_win64.bat" script
file which is located at the root of the decompressed directory, then finally click on "Run as Administrator". The SDK will be installed in 
**C:\\Program Files\\quotek\\sdk**, and ready to go.

Building & Linking
^^^^^^^^^^^^^^^^^^ 

When using Quotek SDK on windows, The first thing you have to do is to let your IDE / Compiling Toolchain know where the Quotek SDK headers and libraries are located.

For Visual studio 2015, the procedure is the following:

* Start a new, empty C++ Project.
* go to Project => Properties.
* Select "VC++ Directories".
* Modify the Include Directories property to add **C:\\Program Files\\quotek\\sdk\\include;** as include search path.
* Modify the Library Directories property to add **C:\\Program Files\\quotek\\sdk\\lib;** as library search path.

|
.. image:: _static/img/vs15_screen1.png

|
* Go to Linker, then Inputs. 
* Modify the Additional Dependancies property to add **"libcurl.lib;quotek.lib"** as additional dependancies.
|

.. image:: _static/img/vs15_screen2.png
|

Then you are good to go, you can start your project and compile it without any trouble.

**Note:** You will have to perform this operation for evey new visual studio project you start with quotek SDK.

Linux Install
-------------

Download and unzip the quotek SDK archive file, and launch the "installer_linux.sh" script 
located at the root of the decompressed directory. All of the Quotek SDK will then be installed 
in /opt/quotek/sdk. The libraries are located in **/opt/quotek/sdk/lib** and headers are in **/opt/quotek/sdk/include**.

Dependancies
^^^^^^^^^^^^

În order to work properly on Linux, Quotek SDK needs libcurl as a dependancy. This is
a very common package which is probably already installed on your system. In any case,
the install procedure for libcurl is as follow:

**For Debian/Ubuntu**

``sudo apt-get install libcurl4``

**For CentOS/Fedora**

``sudo yum install libcurl``

Building & Linking
^^^^^^^^^^^^^^^^^^

Given that the headers and libraries are not installed at the usual places in the system, you will need to 
add a few arguments to your compilation line. Also, because libquotek needs them, you will need to link the programs you code with libcurl and pthread.

So the typical compilation line of a program using quotek SDK on Linux is the following:

``g++ -I/opt/quotek/sdk/include -L/opt/quotek/sdk/lib -o <YOUR_PROGRAM> <YOUR_C++_FILE> -lpthread -lcurl -lquotek``
