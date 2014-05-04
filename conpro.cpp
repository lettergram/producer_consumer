/**
 * Producer-Comsumer example, written in C++ May 4, 2014
 * Compiled on OSX 10.9, using:
 * g++ -std=c++11 semaphore.cpp
 **/

#include <iostream>           
#include <thread>             
#include <mutex>              
#include <condition_variable> 

std::mutex mtx;
std::condition_variable cv;

int meal = 0;

/* consumer */
void customer(int ordernumber){
  std::unique_lock<std::mutex> lck(mtx);
  while(meal == 0) cv.wait(lck);
  std::cout << "Order: ";
  std::cout << ordernumber + 1 << " being taken care of with ";
  std::cout << meal - 1 << " meals also ready." << std::endl;
  meal--;
}

/* Producer */
void chief(int ordernumber){
  std::unique_lock<std::mutex> lck(mtx);
  meal++;
  cv.notify_one();
}

int main (){

  std::thread customers[10];
  std::thread chiefs[10];

  /* Initialize customers and cheifs */
  for (int order = 0; order < 10; order++){
    customers[order] = std::thread(customer, order);   // spawn first
    chiefs[order] = std::thread(chief, order); // spawn second
  }

  /* Join the threads to the main threads */
  for (int order = 0; order < 10; order++) {
    chiefs[order].join();     // must be first
    customers[order].join();  // must be second
  }

  return 0;
}
