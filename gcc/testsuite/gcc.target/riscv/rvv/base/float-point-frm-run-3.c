/* { dg-do run { target { riscv_vector } } } */
/* { dg-options "-O3 -Wno-psabi" } */

#include "riscv_vector.h"
#include <stdio.h>
#include <stdint-gcc.h>

static int
get_frm ()
{
  int frm = -1;

  __asm__ volatile (
    "frrm %0"
    :"=r"(frm)
    :
    :
  );

  return frm;
}

static void
set_frm (int frm)
{
  __asm__ volatile (
    "fsrm %0"
    :
    :"r"(frm)
    :
  );
}

static inline void
assert_equal (int a, int b, char *message)
{
  if (a != b)
    {
      fprintf (stdout, "%s, but get %d != %d\n", message, a, b);
      __builtin_abort ();
    }
}

vfloat32m1_t __attribute__ ((noinline))
test_float_point_frm_run (vfloat32m1_t op1, vfloat32m1_t op2, size_t vl)
{
  vfloat32m1_t result = {};

  result = __riscv_vfadd_vv_f32m1_rm (op1, result, 4, vl);

  assert_equal (4, get_frm (), "The value of frm register should be 4.");

  result = __riscv_vfadd_vv_f32m1 (op1, result, vl);
  result = __riscv_vfadd_vv_f32m1 (op1, result, vl);

  return result;
}

int
main ()
{
  size_t vl = 8;
  vfloat32m1_t op1 = {};
  vfloat32m1_t op2 = {};

  set_frm (0);
  test_float_point_frm_run (op1, op2, vl);

  assert_equal (0, get_frm (), "The value of frm register should be 0.");

  return 0;
}
