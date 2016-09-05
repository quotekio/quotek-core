Using the Strategy Class
========================

In Quotek, the strategy class can be seen as a framework, a skulleton on which you can build your trading algorithm. The strategy class mainly consists of a set of virtual members, which need to be reimplemented by the user to effectively create an algotrading system.


How to use it ?
---------------



Example
-------

Now let's talk about an effective implementation. As explained, the first step is 
to declare a new class inheriting from the strategy class.

.. code-block:: c++

  #include <quotek.hpp>
  #include <strategy.hpp>

  //Here, we declare a new class inheriting from strategy
  class mystrat : public strategy {

Next, the goal will be to override the two main members of the class: init() and evaluate(). evaluate will contain your effetive algorithm.

.. code-block:: c++

  //
  void init() {

  }

  void evaluate() {

    if ( value  > 10000 ) 

  }

};

Now that we have our inherited class that is implemented, we will want to interface it with the data and run it.

.. code-block:: c++

  int main(int argc, char** argv) {

    //We create a new instance of our custom class
    mystrat s1;

    //
    s1.setEnv();

    //We call the init() member
    s1.init();

    while() {
      s1.evaluate();
    }

  }




