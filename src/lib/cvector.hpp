#ifndef CVECTOR_HPP
#define CVECTOR_HPP

  #include <vector>
  #include <mutex>

  template <class T>

  /**
   * Concurrent Vector class, to avoid race condition for vectors shared across many threads
   * 
   */

  class cvector {
  public:

   long size() {
     std::lock_guard<std::mutex> lock(mtx);
     return this->data.size();
   }

   T& operator [] (const int&  i) {
     std::lock_guard<std::mutex> lock(mtx);
     return this->data[i];
   }


    T& at(const int& i) {
      std::lock_guard<std::mutex> lock(mtx);
      return this->data.at(i);
    }

    void push_back(T element) {

      std::lock_guard<std::mutex> lock(mtx);
      this->data.push_back(element);

    } 

    std::mutex mtx;
 
    typename std::vector<T>::iterator begin() {
      std::lock_guard<std::mutex> lock(mtx);
      return this->data.begin();
    }

    typename std::vector<T>::iterator end() {
      std::lock_guard<std::mutex> lock(mtx);
      return this->data.end();
    }
 
    void erase(const typename std::vector<T>::iterator& first, const typename std::vector<T>::iterator& last) {
      std::lock_guard<std::mutex> lock(mtx);
      this->data.erase(first, last);
    }


    const typename std::vector<T>::iterator erase(const typename std::vector<T>::iterator& pos) {
      std::lock_guard<std::mutex> lock(mtx);
      return this->data.erase(pos);
    }    

    void clear() {
      std::lock_guard<std::mutex> lock(mtx);
      this->data.clear();
    }

  private:

  	std::vector<T> data;

  }; 

#endif
