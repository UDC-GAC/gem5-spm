/**
 * 2mm.h: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#ifndef _2MM_H
# define _2MM_H

/* Default to STANDARD_DATASET. */
# if !defined(MINI_DATASET) && !defined(SMALL_DATASET) && !defined(LARGE_DATASET) && !defined(EXTRALARGE_DATASET)
#  define MINI_DATASET
# endif

/* Do not define anything if the user manually defines the size. */
# if !defined(NI) && !defined(NJ) && !defined(NK)
/* Define the possible dataset sizes. */
#  ifdef MINI_DATASET
#   define NI 32
#   define NJ 32
#   define NK 32
#   define NL 32
#   define N  32*32
#  endif

#  ifdef TINY_DATASET
#   define NI 64
#   define NJ 64
#   define NK 64
#   define NL 64
#   define N  64*64
#  endif

#  ifdef SMALL_DATASET
#   define NI 128
#   define NJ 128
#   define NK 128
#   define NL 128
#   define N  128*128
#  endif

#  ifdef STANDARD_DATASET /* Default if unspecified. */
#   define NI 1024
#   define NJ 1024
#   define NK 1024
#   define NL 1024
#   define N  1024*1024
#  endif

#  ifdef LARGE_DATASET
#   define NI 2000
#   define NJ 2000
#   define NK 2000
#   define NL 2000
#   define N  2000*2000
#  endif

#  ifdef EXTRALARGE_DATASET
#   define NI 4000
#   define NJ 4000
#   define NK 4000
#   define NL 4000
#   define N  4000*4000
#  endif
# endif /* !N */

#endif /* !_2MM */
