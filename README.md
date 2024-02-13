Generic `static_assert` ( C89, C++98 or later ):
================================================

[Link Github][1]

    #define CONCAT_(prefix, suffix)     prefix##suffix
    #define CONCAT(prefix, suffix)      CONCAT_(prefix, suffix)
    
    
    #define outscope_assert(expr)                           \
        struct CONCAT(__outscope_assert_, __COUNTER__)   \
        {                                                   \
            char                                            \
            outscope_assert                                 \
            [2*(expr)-1];                                   \
                                                            \
        } CONCAT(__outscope_assert_, __COUNTER__)

call it like this:

    outscope_assert( 0 > 5 );

Compatible with **C89 :**

    gcc main.c -o main.exe -std=c89

or **C++98 :**

    g++ main.c -o main.exe -std=c++98

The error that it produces looks like this:

    main.c:32:9: error: size of array 'outscope_assert' is negative
       32 |         outscope_assert                                 \
          |         ^~~~~~~~~~~~~~~
    main.c:50:1: note: in expansion of macro 'outscope_assert'
       50 | outscope_assert(2 > 5);
          | ^~~~~~~~~~~~~~~

**It does not break** so easily as most of the other solutions that have been proposed,
you can test it!

    /* Some test */

    /* Global */
    outscope_assert(1 < 2);
    
    /* Within Struct */
    struct A
    {
        int a;
    
        outscope_assert(1 < 2);
        outscope_assert(2 > 1); outscope_assert(2 > 1); /* Same Line */
    };
    
    
    int main (void)
    {
        /* Within Function */
        outscope_assert(2 > 1);
        return 0;
    }

----------
## About `__COUNTER__` : ##

`__COUNTER__` is a macro that monotonically increments is value during compilation (starting from 0) and it is commonly used to generate ids.

The macro may or may not be defined depending on the compiler and/or is version,
altough basically all modern compilers support it.

If your are stuck on using some dinosaur or unicorn like compiler, you will have to use an alternative:

 1. replace `__COUNTER__` with `__LINE__`, the catch is that you will no be able anymore to have static asserts on the same line, and you may also have problems having them on the same line in different files.
 2. *(prob the best option)* replace `__COUNTER__` with `my_id` and add it as parameter in the macro function definition like this: `#define outscope_assert(expr, myid)` and then call the macro like `outscope_assert(0 < 1 , this_is_myid );`
 3. Read the next paragraph we will use an `inscope_assert`


----------
## *(you can skip this)* Why `inscope_assert` ? ##

    #define inscope_assert(expr)                            \
        {                                                   \
            char                                            \
            inscope_assert                                  \
            [2*(expr)-1];                                   \
            (void)inscope_assert;                           \
        }

As you may have noticed, `inscope_assert` is pretty the same of `outscope_assert` just without the `struct` and the `id` thing.

This one has obvious disadvantages: it cannot be used neither *globally* nor *within a struct*, but well on the flip side if you cannot use the `__COUNTER__` you can rest assured this will work without any side-effects and it guarantees the strictest C89 possible.

You may have noticed *(prob not)* that if we use more strict flags on compilation:

    gcc main.c -o main.exe -std=c89 -Wall -Wextra -ansi -pedantic

 our `outscope_assert` declared in `main` get us a `warning`:

    warning: unused variable '__outscope_assert__9' [-Wunused-variable]

Simply telling us that we have declared a variable of type struct but we not have used it, not a big deal really, but still our `inscope_assert` will not produce it.


----------

## C11, C23 and GCC Specific ##

>     C11 standard adds the _Static_assert keyword.
>         _Static_assert ( expression , message )


>     C23 standard adds the static_assert keyword
>         static_assert ( expression , message )
>     or  static_assert ( expression )


>     gcc-4.6 adds the _Static_assert keyword for all versions of C,
>     including c90, c99, c11, c17, etc.

So simply doing some before-checking:

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
        #endif
    
    #endif


----------

Quick Technical Explanation:
----------------------------

Unwrapped the macro will look like this:

    struct __outscope_assert_24 { char outscope_assert [2*(2 > 1)-1]; } __outscope_assert_25

Our objective is to throw an error if the expression (in this case `2 > 1`) is false:

    error: size of array 'outscope_assert' is negative

The expr `(2 > 1)` evaluates to **1** because **true** so `2*1 - 1 = 1` we are declaring an array of `char` of **size = 1**.

The expr `(2 > 9)` evaluates to **0** because **false** so `2*0 - 1 = -1` we are declaring an array of `char` of **size = -1**. Wich trows an error at compile time.

`__outscope_assert_24` and `__outscope_assert_25` are the struct name and the variable name, omitting the first will cause a warning in g++, Omitting the second will cause a duplicate member error if two assert are declared in the same scope.

The macro `CONCAT` and `CONCAT_` are here to create the struct and variable names.


  [1]: https://github.com/Marco-DG/static_assert.h
