/*
 * This code is distributed under the terms of the GNU General Public License.
 * For more information, please refer to the LICENSE file in the root directory.
 * -------------------------------------------------
 * Copyright (C) 2025 Rodrigo R.
 * This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions; type `show c' for details.
*/

#ifndef FLUENT_LIBC_ATOMIC_LIBRARY_H
#define FLUENT_LIBC_ATOMIC_LIBRARY_H

// ============= FLUENT LIB C =============
// atomic_size_t API
// ----------------------------------------
// A cross-platform wrapper for atomic operations on `size_t`.
// Provides portable atomic manipulation across:
// - C11 standard atomics
// - Windows Interlocked APIs
// - GCC built-ins (when atomics are unavailable)
//
// Features:
// - atomic_size_init(p, v):        Initialize an atomic with value `v`
// - atomic_size_load(p):           Load the current atomic value
// - atomic_size_store(p, v):       Store a new value atomically
// - atomic_size_fetch_add(p, v):   Atomically add `v` and return the previous value
// - atomic_size_fetch_sub(p, v):   Atomically subtract `v` and return the previous value
//
// Typical Use:
// ----------------------------------------
// atomic_size_t counter;
// atomic_size_init(&counter, 0);
// size_t old = atomic_size_fetch_add(&counter, 1);
// size_t current = atomic_size_load(&counter);
//
// ----------------------------------------
// Initial revision: 2025-05-26
// ----------------------------------------
// Depends on: stdatomic.h, windows.h, or GCC __sync built-ins
// ----------------------------------------

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
extern "C"
{
#endif

#include <stddef.h> // size_t

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
    #include <stdatomic.h>
    typedef _Atomic size_t atomic_size_t;

    #define atomic_size_init(p, v) atomic_init(p, v)
    #define atomic_size_load(p) atomic_load(p)
    #define atomic_size_store(p, v) atomic_store(p, v)
    #define atomic_size_fetch_add(p, v) atomic_fetch_add(p, v)
    #define atomic_size_fetch_sub(p, v) atomic_fetch_sub(p, v)

#elif defined(_WIN32)
    // Pending Windows implementation review
    /*
    #include <windows.h>
    typedef struct {
        volatile size_t value;
    } atomic_size_t;

    static inline void atomic_size_init(atomic_size_t* p, size_t v)
    {
        p->value = v;
    }

    static inline size_t atomic_size_load(atomic_size_t* p)
    {
        MemoryBarrier(); // full barrier
        return p->value;
    }

    static inline void atomic_size_store(atomic_size_t* p, size_t v)
    {
        MemoryBarrier();
        p->value = v;
    }

    static inline size_t atomic_size_fetch_add(atomic_size_t* p, size_t v)
    {
    #if defined(_WIN64)
        return InterlockedExchangeAdd64((volatile LONGLONG*)&p->value, (LONGLONG)v);
    #else
        return InterlockedExchangeAdd((volatile LONG*)&p->value, (LONG)v);
    #endif
    }

    static inline size_t atomic_size_fetch_sub(atomic_size_t* p, size_t v)
    {
    #if defined(_WIN64)
        return InterlockedExchangeAdd64((volatile LONGLONG*)&p->value, -(LONGLONG)v);
    #else
        return InterlockedExchangeAdd((volatile LONG*)&p->value, -(LONG)v);
    #endif
    }*/
#else
    typedef struct
    {
        volatile size_t value;
    } atomic_size_t;

    static inline void atomic_size_init(atomic_size_t* p, size_t v)
    {
        __sync_synchronize();
        p->value = v;
    }

    static inline size_t atomic_size_load(atomic_size_t* p)
    {
        __sync_synchronize();
        return p->value;
    }

    static inline void atomic_size_store(atomic_size_t* p, size_t v)
    {
        __sync_synchronize();
        p->value = v;
    }

    static inline size_t atomic_size_fetch_add(atomic_size_t* p, size_t v)
    {
        return __sync_fetch_and_add(&p->value, v);
    }

    static inline size_t atomic_size_fetch_sub(atomic_size_t* p, size_t v)
    {
        return __sync_fetch_and_sub(&p->value, v);
    }
#endif

// ============= FLUENT LIB C++ =============
#if defined(__cplusplus)
}
#endif
#endif //FLUENT_LIBC_ATOMIC_LIBRARY_H