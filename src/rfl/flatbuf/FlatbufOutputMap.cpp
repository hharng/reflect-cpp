/*

MIT License

Copyright (c) 2023-2024 Code17 GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "rfl/flatbuf/FlatbufOutputMap.hpp"

#include <type_traits>

#include "rfl/flatbuf/calc_vtable_offset.hpp"

namespace rfl::flatbuf {

void FlatbufOutputMap::add_key(const std::string_view& _key) {
  keys_.emplace_back(fbb_->CreateString(_key.data(), _key.size()));
}

void FlatbufOutputMap::add_offset(const flatbuffers::uoffset_t _offset) {
  auto offset = flatbuffers::Offset<>(_offset);
  const auto ptr = internal::ptr_cast<const uint8_t*>(&offset);
  values_.insert(values_.end(), ptr, ptr + sizeof(flatbuffers::Offset<>));
}

void FlatbufOutputMap::end() {
  const auto offset_keys =
      fbb_->CreateVector<flatbuffers::String>(keys_.data(), keys_.size()).o;
  const auto offset_values = build_vector(value_schema(), values_, fbb_);

  const auto start = fbb_->StartTable();
  fbb_->AddOffset<>(calc_vtable_offset(0), flatbuffers::Offset<>(offset_keys));
  fbb_->AddOffset<>(calc_vtable_offset(1),
                    flatbuffers::Offset<>(offset_values));
  const auto offset = fbb_->EndTable(start);

  if (parent_) {
    parent_->add_offset(offset);
  }
}

}  // namespace rfl::flatbuf
