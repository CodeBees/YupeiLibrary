#pragma once

#define CONTAINER_DEFINE(T)                     \
using value_type = T;                           \
using size_type = std::size_t;                  \
using reference = value_type&;                  \
using const_reference = const value_type&;      \
using pointer = value_type*;                    \
using const_pointer = const value_type*;
