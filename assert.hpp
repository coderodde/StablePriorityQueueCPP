#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>

#define ASSERT(CONDITION) assert(CONDITION, #CONDITION, __FILE__, __LINE__);
#define REPORT            assert.report();
#define TOTAL_ASSERTIONS  assert.get_total_number_of_assertions()
#define FAILED_ASSERTIONS assert.get_number_of_failed_assertions()

class Assert {
public:
    bool operator()(const bool condition,
                    const char *const condition_text,
                    const char *const file_name,
                    const int line_number);
    
    size_t get_total_number_of_assertions() const;
    size_t get_number_of_failed_assertions() const;
    void report() const;
    
private:
    size_t m_total_assertions;
    size_t m_failed_assertions;
};

// Can't think of anything better than a global.
extern Assert assert;

#endif  // ASSERT_H
