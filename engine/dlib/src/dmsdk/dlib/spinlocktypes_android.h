#ifndef DM_SPINLOCKTYPES_H
#define DM_SPINLOCKTYPES_H

#include <stdint.h>

#if !defined(__aarch64__)
namespace dmSpinlock
{
    typedef uint32_t Spinlock;

    static inline void Init(Spinlock* lock)
    {
        *lock = 0;
    }

    static inline void Lock(Spinlock* lock)
    {
        uint32_t tmp;
         __asm__ __volatile__(
 "1:     ldrex   %0, [%1]\n"
 "       teq     %0, #0\n"
// "       wfene\n"  -- WFENE gives SIGILL for still unknown reasons on some 64-bit ARMs Aarch32 mode.
//                      Disassembly of libc.so and pthread_mutex_lock shows no use of the instruction.
 "       strexeq %0, %2, [%1]\n"
 "       teqeq   %0, #0\n"
 "       bne     1b\n"
"        dsb\n"
         : "=&r" (tmp)
         : "r" (lock), "r" (1)
         : "cc");

    }

    static inline void Unlock(Spinlock* lock)
    {
        __asm__ __volatile__(
"       dsb\n"
"       str     %1, [%0]\n"
        :
        : "r" (lock), "r" (0)
        : "cc");
    }
}

#elif defined(__aarch64__)
// Asm implementation from https://gitlab-beta.engr.illinois.edu/ejclark2/linux/blob/f668cd1673aa2e645ae98b65c4ffb9dae2c9ac17/arch/arm64/include/asm/spinlock.h
namespace dmSpinlock
{
    typedef uint32_t Spinlock;

    static inline void Init(Spinlock* lock)
    {
        *lock = 0;
    }

    static inline void Lock(Spinlock* lock)
    {

        uint32_t tmp;
         __asm__ __volatile__(
    "   sevl\n"
        "1: wfe\n"
        "2: ldaxr   %w0, [%1]\n"
        "   cbnz    %w0, 1b\n"
        "   stxr    %w0, %w2, [%1]\n"
        "   cbnz    %w0, 2b\n"
        : "=&r" (tmp)
        : "r" (lock), "r" (1)
        : "memory");

    }

    static inline void Unlock(Spinlock* lock)
    {
        __asm__ __volatile__(
        "   stlr    %w1, [%0]\n"
        : : "r" (lock), "r" (0) : "memory");

    }
}

#else
#error "Unsupported platform"
#endif

#endif
