#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/flatbuf.hpp>
#include <string>
#include <vector>

#include "person_generated.h"

namespace test_person_comparison {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  Age age;
  std::vector<Person> children;
};

flatbuffers::Offset<TestPersonComparisonPerson> person_to_flatbuffers(
    const Person& _person, flatbuffers::FlatBufferBuilder* _fbb) {
  std::vector<flatbuffers::Offset<TestPersonComparisonPerson>> children;
  for (const auto& child : _person.children) {
    children.push_back(person_to_flatbuffers(child, _fbb));
  }
  return CreateTestPersonComparisonPersonDirect(
      *_fbb, _person.first_name.c_str(), _person.last_name.c_str(),
      _person.age.value(), &children);
}

std::vector<char> serialize_using_flatbuffers(const Person& _person) {
  flatbuffers::FlatBufferBuilder fbb;
  const auto root = person_to_flatbuffers(_person, &fbb);
  fbb.Finish(root);
  const auto ptr = fbb.GetBufferPointer();
  return std::vector<char>(rfl::internal::ptr_cast<char*>(ptr),
                           rfl::internal::ptr_cast<char*>(ptr + fbb.GetSize()));
}

TEST(flatbuffers, test_person_comparison) {
  const auto bart = Person{.first_name = "Bart", .age = 10};
  const auto lisa = Person{.first_name = "Lisa", .age = 8};
  const auto maggie = Person{.first_name = "Maggie", .age = 0};
  const auto homer =
      Person{.first_name = "Homer",
             .age = 45,
             .children = std::vector<Person>({bart, lisa, maggie})};

  const auto serialized1 = rfl::flatbuf::write(homer);
  const auto serialized2 = serialize_using_flatbuffers(homer);

  ASSERT_EQ(serialized1, serialized2);
}
}  // namespace test_person_comparison
