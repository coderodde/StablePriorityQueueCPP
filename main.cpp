#include "net/coderodde/util/StablePriorityQueue.hpp"
#include "assert.hpp"
#include <iostream>
#include <stdexcept>

using net::coderodde::util::StablePriorityQueue;

void test1() {
    StablePriorityQueue<int, int> q;
    q.add(2, 2);
    q.add(3, 3);
    q.add(1, 1);
    
    ASSERT(q.extractMinimum() == 1);
    ASSERT(q.extractMinimum() == 2);
    ASSERT(q.extractMinimum() == 3);
}

void test2() {
    StablePriorityQueue<int, int> q;
    q.add(1, 10);
    q.add(2, 10);
    q.add(3, 1);
    
    ASSERT(q.extractMinimum() == 3);
    ASSERT(q.extractMinimum() == 1);
    ASSERT(q.extractMinimum() == 2);
}

void test3() {
    StablePriorityQueue<char*, int> q;
    char* X = "X";
    char* Y = "Y";
    char* Z = "Z";
    
    q.add(X, 10);
    q.add(Y, 10);
    q.add(Z, 10);
    q.add(Y, 9);
    q.add(X, 11);
    
    ASSERT(q.extractMinimum() == Y);
    ASSERT(q.extractMinimum() == Z);
    ASSERT(q.extractMinimum() == X);
}

void testThrowsOnEmptyQueueWhenTop() {
    StablePriorityQueue<int, int> q;
    
    try {
        q.top();
        ASSERT(false);
    } catch (std::underflow_error& err) {
        ASSERT(true);
    } catch (...) {
        ASSERT(false);
    }
}

void testThrowsOnEmptyQueueWhenExtractMinimum() {
    StablePriorityQueue<int, int> q;
    
    try {
        q.extractMinimum();
        ASSERT(false);
    } catch (std::underflow_error& err) {
        ASSERT(true);
    } catch (...) {
        ASSERT(false);
    }
}

void testSize() {
    StablePriorityQueue<int, int> q;
    
    for (int i = 0; i < 10; ++i) {
        ASSERT(q.size() == i);
        q.add(i, i);
        ASSERT(q.size() == i + 1);
    }
    
    for (int i = 10; i > 0; --i) {
        ASSERT(q.size() == i);
        q.extractMinimum();
        ASSERT(q.size() == i - 1);
    }
}

void test() {
    test1();
    test2();
    test3();
    testThrowsOnEmptyQueueWhenTop();
    testThrowsOnEmptyQueueWhenExtractMinimum();
    testSize();
}

void testIsEmpty() {
    StablePriorityQueue<int, int> q;
    ASSERT(q.empty() == true);
    q.add(1, 1);
    ASSERT(q.empty() == false);
    q.add(2, 2);
    ASSERT(q.empty() == false);
    q.extractMinimum();
    ASSERT(q.empty() == false);
    q.extractMinimum();
    ASSERT(q.empty() == true);
}

int main(int argc, const char * argv[]) {
    test();
    REPORT;
}
