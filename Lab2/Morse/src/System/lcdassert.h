#ifndef LCD_ASSERT_H
#define LCD_ASSERT_H

#include "GLCD.h"

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

/*
 * Turn A into a string literal after macro-expanding it.
 */
#define STRINGIZE(A) STRINGIZE_NX(A)

#define LCD_ASSERT(cond) \
    do \
    { \
        if (!(cond)) \
        { \
            GLCD_DisplayString(0, 0, 1, __FILE__ ":" STRINGIZE(__LINE__) ); \
					  __disable_irq(); \
            while(1); \
        } \
    } while(0)

#endif
