#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <pthread.h>
#include <queue>

using namespace std;

template <class T>
class Queue {

  public:
    Queue();
    void push(T value);
    bool pop(T& value, bool block);

  private:

    queue<T>   m_queue;
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_condition;

};

template<class T> Queue<T>::Queue() {
  pthread_mutex_init( &m_mutex, NULL );
  pthread_cond_init( &m_condition, NULL );
}



template<class T> void Queue<T>::push(T value) {

  pthread_mutex_lock( &m_mutex );
  m_queue.push(value);
  if( !m_queue.empty() ) {
     pthread_cond_signal( &m_condition );
  }
  pthread_mutex_unlock( &m_mutex );
}


template<class T> bool Queue<T>::pop(T& value, bool block) {
    
  bool rtn = false;
  pthread_mutex_lock( &m_mutex );
  if( block ) {
     while( m_queue.empty() ) {
       pthread_cond_wait( &m_condition, &m_mutex );
     }
  }
  if( !m_queue.empty() ) {
      value = m_queue.front();
      m_queue.pop();  
      rtn = true;
  }
  pthread_mutex_unlock( &m_mutex );
  return rtn;

}

#endif
