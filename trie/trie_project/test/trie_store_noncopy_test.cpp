#include <bitset>
#include <cassert> // Include assert header
#include <functional>
#include <memory>
#include <numeric>
#include <optional>
#include <random>
#include <thread>  // NOLINT
#include <utility>
#include "../trie/solution.hpp"


using Integer = std::unique_ptr<uint32_t>;

void TrieStoreTest_NonCopyableTest() {
  sjtu::TrieStore store;
  store.Put<Integer>("tes", std::make_unique<uint32_t>(233));
  store.Put<Integer>("te", std::make_unique<uint32_t>(23));
  store.Put<Integer>("test", std::make_unique<uint32_t>(2333));
  assert(***store.Get<Integer>("te") == 23);
  assert(***store.Get<Integer>("tes") == 233);
  assert(***store.Get<Integer>("test") == 2333);
  store.Remove("te");
  store.Remove("tes");
  store.Remove("test");
  assert(store.Get<Integer>("te") == std::nullopt);
  assert(store.Get<Integer>("tes") == std::nullopt);
  assert(store.Get<Integer>("test") == std::nullopt);
}

void TrieStoreTest_ReadWriteTest() {
  sjtu::TrieStore store;
  store.Put<uint32_t>("a", 1);
  store.Put<uint32_t>("b", 2);
  store.Put<uint32_t>("c", 3);
  std::promise<int> x;

  std::cerr << "[0] begin" << std::endl;

  std::thread t([&store, &x] { store.Put<sjtu::MoveBlocked>("d", sjtu::MoveBlocked(x.get_future())); });

  std::cerr << "[1] thread spawn" << std::endl;

  // Loop for enough time to ensure that the thread is blocked on the promise.
  for (int i = 0; i < 100000; i++) {
    std::cout << "i : " << i << std::endl;
    {
      auto guard = store.Get<uint32_t>("a");
      assert(**guard == 1);
    }
    {
      auto guard = store.Get<uint32_t>("b");
      assert(**guard == 2);
    }
    {
      auto guard = store.Get<uint32_t>("c");
      assert(**guard == 3);
    }
  }

  std::cerr << "[2] read done" << std::endl;

  x.set_value(233);

  t.join();

  std::cerr << "[3] write complete" << std::endl;

  assert(store.Get<sjtu::MoveBlocked>("d") != std::nullopt);
}


int main() {
  TrieStoreTest_NonCopyableTest();
  TrieStoreTest_ReadWriteTest();
  return 0;
}