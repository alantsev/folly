
#include <thread>
#include <iostream>
#include <stdio.h>

#include <folly/ProducerConsumerQueue.h>
#include <chrono>

namespace {

using namespace folly;

typedef unsigned int ThroughputType;
typedef ProducerConsumerQueue<ThroughputType> ThroughputQueueType;

typedef unsigned long LatencyType;
typedef ProducerConsumerQueue<LatencyType> LatencyQueueType;

template<class QueueType>
struct ThroughputTest {
  explicit ThroughputTest(size_t size, int iters)
  : queue_(size),
    iters_(iters)
    { }

  void producer() {
    for (int i = 0; i < iters_; ++i) {
      ThroughputType item = i;
      while (!queue_.write((ThroughputType) item)) {
      }
    }
  }

  void consumer() {
    for (int i = 0; i < iters_; ++i) {
      ThroughputType item = 0;
      while (!queue_.read(item)) {
      }
    }
  }

  QueueType queue_;
  const int iters_;
};


void BM_ProducerConsumer(int iters, int size) {
  ThroughputTest<ThroughputQueueType> *test =
    new ThroughputTest<ThroughputQueueType>(size, iters);
  
  std::thread producer( [test] { test->producer(); } );
  std::thread consumer( [test] { test->consumer(); } );

  producer.join();
  consumer.join();
  delete test;
}


}

int main(int argc, char** argv) {

  for (int i = 0; i < 100; ++i)
  {
    auto start = std::chrono::system_clock::now();
    BM_ProducerConsumer (100000000, 1048574);
    auto stop = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed = stop - start;
    std::cout << "elapsed time: " << elapsed.count() << "s\n";
  }
  return 0;
}
