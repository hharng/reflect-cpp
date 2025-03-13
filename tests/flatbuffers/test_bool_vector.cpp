#include <cstdint>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_bool_vector {

struct TestStruct {
  std::vector<bool> bool_vector;
};

TEST(flatbuffers, test_bool_vector) {
  const auto test =
      TestStruct{.bool_vector = std::vector<bool>({true, false, true, false})};

  write_and_read(test);
}
}  // namespace test_bool_vector
