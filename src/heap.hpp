#ifndef ___DST_HEAP_H
#define ___DST_HEAP_H

#include "config.hpp"
#include "dst_heap.cc"

#ifdef __DST_USE_NAMESPACES

using __VDSA::make_heap;
using __VDSA::pop_heap;
using __VDSA::push_heap;
using __VDSA::sort_heap;

#endif /* __DST_USE_NAMESPACES */

#endif /* ___DST_HEAP_H */