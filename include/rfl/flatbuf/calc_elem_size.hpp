#ifndef RFL_FLATBUF_CALCELEMSIZE_HPP_
#define RFL_FLATBUF_CALCELEMSIZE_HPP_

#include <cstddef>
#include <type_traits>

namespace rfl::flatbuf {

template <class T>
consteval size_t calc_elem_size() {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_same_v<Type, bool> || std::is_floating_point_v<Type> ||
                std::is_integral_v<Type> || std::is_same_v<Type, std::byte>) {
    return sizeof(Type);
  } else {
    return 4;
  }
}

}  // namespace rfl::flatbuf

#endif

