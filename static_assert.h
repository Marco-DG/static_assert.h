#ifndef _DGLIB_STATIC_ASSERT_H
#define _DGLIB_STATIC_ASSERT_H

/*
    from: https://stackoverflow.com/a/76023098/8411453

    Macros
    ###################################################################################################################
    
    dglib_static_assert(expr)   |   (function macro)
                                |   Generates a compilation-time error if the user-specified condition is not true.
                                |   The allowed scope in which it can be called depends on the presence of the
                                |   __COUNTER__ macro.
*/

#include "macro.h"
/*
    DGLIB_CONCAT_HELPER
    DGLIB_CONCAT
*/

/* define dglib_static_assert */
#if defined __COUNTER__
    
    #define dglib_static_assert(expr)                               \
        struct DGLIB_CONCAT(__dglib_static_assert_, __COUNTER__)    \
        {                                                           \
            char                                                    \
            dglib_static_assert                                     \
            [2*(expr)-1];                                           \
                                                                    \
        }  DGLIB_CONCAT(__dglib_static_assert_, __COUNTER__)


#else /* !defined __COUNTER__ */ /* fall back to inscope_assert */

    #define dglib_static_assert(expr)                               \
        {                                                           \
            char                                                    \
            dglib_static_assert                                     \
            [2*(expr)-1];                                           \
            (void)dglib_static_assert;                              \
        }


#endif


#endif /* _DGLIB_STATIC_ASSERT_H */