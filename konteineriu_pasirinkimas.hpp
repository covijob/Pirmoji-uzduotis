#pragma once
#include <vector>
#include <list>
#include <type_traits>
#include <memory>
#include "studentai.hpp"

struct VectorTag {};
struct ListTag {};

template <typename Tag, typename T>
using ContainerT = std::conditional_t<std::is_same_v<Tag, VectorTag>,
    std::vector<T>,
    std::list<T>>;

template <typename Tag>
constexpr const char* container_name_v =
std::is_same_v<Tag, VectorTag> ? "vector" : "list";

template <typename Tag, typename T>
inline void reserve_if_possible(ContainerT<Tag, T>& c, std::size_t n) {
    if constexpr (std::is_same_v<ContainerT<Tag, T>, std::vector<T>>) {
        c.reserve(n);
    }
    else {
        (void)c; (void)n; 
    }
}

inline const void* ptr_of(const Studentas& s) {
    return static_cast<const void*>(std::addressof(s));
}
