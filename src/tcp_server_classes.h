/*  =========================================================================
    tcp_server_classes - private header file

    =========================================================================
*/

#ifndef _TCP_SERVER_CLASSES_H_INCLUDED_
#define _TCP_SERVER_CLASSES_H_INCLUDED_

//  Asserts check the invariants of methods. If they're not
//  fulfilled the program should fail fast. Therefore enforce them!
#ifdef NDEBUG
  #undef NDEBUG
  #include <assert.h>
  #define NDEBUG
#else
  #include <assert.h>
#endif

//  External API
#include "../include/tcp_server.h"

//  Private external dependencies

//  Opaque class structures to allow forward references

//  Extra headers

//  Internal API
// common definitions and idioms from czmq_prelude.h, which are used in generated code
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define streq(s1,s2)    (!strcmp ((s1), (s2)))
#define strneq(s1,s2)   (strcmp ((s1), (s2)))

//  Replacement for malloc() which asserts if we run out of heap, and
//  which zeroes the allocated block.
static inline void *
safe_malloc (size_t size, const char *file, unsigned line)
{
//     printf ("%s:%u %08d\n", file, line, (int) size);
    void *mem = calloc (1, size);
    if (mem == NULL) {
        fprintf (stderr, "FATAL ERROR at %s:%u\n", file, line);
        fprintf (stderr, "OUT OF MEMORY (malloc returned NULL)\n");
        fflush (stderr);
        abort ();
    }
    return mem;
}

static inline void
safe_free (void *mem_ptr)
{
    if (mem_ptr != NULL)
    {
        free(mem_ptr);
    }
    mem_ptr = NULL;
}

#define smalloc(size) safe_malloc((size), __FILE__, __LINE__)
#define sfree(mem_ptr) safe_free((mem_ptr))

#endif /* _TCP_SERVER_CLASSES_H_INCLUDED_ */
