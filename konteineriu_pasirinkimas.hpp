#pragma once
#include <vector>
#include <list>

struct VectorTag {};
struct ListTag {};

template<typename Tag, typename T>
struct ContainerOf;

template<typename T>
struct ContainerOf<VectorTag, T> { using type = std::vector<T>; };

template<typename T>
struct ContainerOf<ListTag, T> { using type = std::list<T>; };

template<typename Tag, typename T>
using ContainerT = typename ContainerOf<Tag, T>::type;
