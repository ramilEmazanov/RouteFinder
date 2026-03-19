// infrastructure.hpp
#pragma once
#include <fstream>

#include "business_logic.hpp"

struct DataStorage {
 public:
  DataStorage(const DataRequester& requester);

  void foo();

 private:
  const DataRequester& requester_;
  std::ifstream file_;
};