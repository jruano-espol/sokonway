#pragma once

#include <cstddef>
#include <cassert>

template <typename T, size_t N>
struct Fixed_Array {
    T *data = nullptr;
    size_t count = 0;

    const T& operator[](size_t i) const { return data[i]; }
          T& operator[](size_t i)       { return data[i]; }
    
    void append(const T &element)
    {
        assert(count + 1 <= N);
        data[count++] = element;
    }
};
