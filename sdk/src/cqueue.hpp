/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef CQUEUE_HPP
#define CQUEUE_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace quotek {

  namespace data {

    template <class T>

    /**
     * cqueue class is a thread-safe implementation of std::queue
     */
    class cqueue {

      public:
        cqueue();
        void push(T value);
        bool pop(T& value, bool block);

      private:

        std::queue<T>   m_queue;
        std::mutex mtx;

        std::condition_variable m_condition;

    };

    template<class T> cqueue<T>::cqueue() {

    }

    template<class T> void cqueue<T>::push(T value) {
      std::lock_guard<std::mutex> lock(mtx);
      m_queue.push(value);
      if( !m_queue.empty() ) {
         m_condition.notify_all();
      }
      
    }


    template<class T> bool cqueue<T>::pop(T& value, bool block) {
        
      bool rtn = false;
      std::lock_guard<std::mutex> lock(mtx);
      if( block ) {
         while( m_queue.empty() ) {
           m_condition.wait(lock);
         }
      }
      if( !m_queue.empty() ) {
          value = m_queue.front();
          m_queue.pop();  
          rtn = true;
      }
      
      return rtn;

    }
  }
}

#endif
