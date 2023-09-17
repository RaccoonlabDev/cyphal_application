/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef CYPHAL_RING_BUFFER_HPP_
#define CYPHAL_RING_BUFFER_HPP_

#include <stddef.h>

template<class ElementType, size_t max_size>
class RingBuffer {
public:
    RingBuffer() {
        head = 0;
        tail = 0;
        size = 0;
    }

    // Overwrite head element if there is no free space
    void push(ElementType element) {
        buffer[tail] = element;
        tail = (tail + 1) % max_size;
        size++;
        if (size > max_size) {
            head = (head + 1) % max_size;
        }
    }

    // You must call get_size() before pop()
    // Return one of the allocated element if empty
    ElementType pop() {
        ElementType element = buffer[head];
        if (size > 0) {
            head = (head + 1) % max_size;
            size--;
        }

        return element;
    }

    size_t get_size() const {
        return size;
    }

private:
    ElementType buffer[max_size];
    size_t head;
    size_t tail;
    size_t size;
};

#endif  // CYPHAL_RING_BUFFER_HPP_
