#ifndef _DGLIB_ASSERT_H
#define _DGLIB_ASSERT_H

#include <assert.h>
/*
    from: https://cplusplus.com/reference/cassert/assert/
    and : https://en.cppreference.com/w/cpp/header/cassert



    Macros
    ###################################################################################################################
    
    assert(expr)                |   (function macro) aborts the program if the user-specified condition is not true.
                                |   This macro is disabled if, at the moment of including <assert.h>,
                                |   a macro with the name NDEBUG has already been defined.

    static_assert(expr)         |   (function macro)
                                |   Generates a compilation-time error if the user-specified condition is not true.
                                |   The allowed scope in which it can be called is compiler and language dependent.
                                |   The generated output error is implementation dependent.
                                |   Eventual warnings are implementation dependent.

    static_assert(expr, msg)    |   (function macro)(optional)
                                |   The definition of this macro is not guaranteed.

*/

/*
    C11 standard adds the _Static_assert keyword.
        _Static_assert ( expression , message )


    C23 standard adds the static_assert keyword
        static_assert ( expression , message )
    or  static_assert ( expression )

    
    gcc-4.6 adds the _Static_assert keyword for all versions of C, including c90, c99, c11, c17, etc.
*/
#if !defined static_assert

    /* C11 or later */
    #if defined _Static_assert
        #define static_assert _Static_assert

    /* GCC 4.6 or later */
    #elif defined __GNUC__ && ( __GNUC__ > 4 || __GNUC__ == 4 && defined __GNUC_MINOR__ && __GNUC_MINOR >= 6)
        /*  It will work but it will throw a warning:
            warning: ISO C90 does not support '_Static_assert' [-Wpedantic] 
        */
        #define static_assert _Static_assert


    #else /* !(defined __GNUC__ && ( __GNUC__ > 4 || __GNUC__ == 4 && defined __GNUC_MINOR__ && __GNUC_MINOR >= 6))  */
        /*
            from: https://stackoverflow.com/a/76023098/8411453

            Macros
            ###################################################################################################################
            
            dglib_static_assert(expr)   |   (function macro)
                                        |   Generates a compilation-time error if the user-specified condition is not true.
                                        |   The allowed scope in which it can be called depends on the presence of the
                                        |   __COUNTER__ macro.
        */

        /* define dglib_static_assert */
        #if defined __COUNTER__

            #define DGLIB_CONCAT_HELPER(prefix, suffix)     prefix##suffix
            #define DGLIB_CONCAT(prefix, suffix)            DGLIB_CONCAT_HELPER(prefix, suffix)


            #define static_assert(expr)                                     \
                struct DGLIB_CONCAT(__dglib_static_assert_, __COUNTER__)    \
                {                                                           \
                    char                                                    \
                    dglib_static_assert                                     \
                    [2*(expr)-1];                                           \
                                                                            \
                }  DGLIB_CONCAT(__dglib_static_assert_, __COUNTER__)


        #else /* !defined __COUNTER__ */ /* fall back to inscope_assert */

            #define static_assert(expr)                                     \
                {                                                           \
                    char                                                    \
                    dglib_static_assert                                     \
                    [2*(expr)-1];                                           \
                    (void)dglib_static_assert;                              \
                }


        #endif

    #endif

#endif


#endif /* _DGLIB_ASSERT_H */