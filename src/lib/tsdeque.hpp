#ifndef TSDEQUE_HPP
#define TSDEQUE_HPP

  #include <deque>
  #include <mutex>

  template <class T>

  class tsdeque: public std::deque<T> {

  public:

    


  private:
    std::mutex mtx;


  }; 

#endif
