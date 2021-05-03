// SPDX-License-Identifier: zlib-acknowledgement

// IMPORTANT(Ryan): Majority of the content of linux headers
// are only usable in kernel space, e.g. kernel modules
#include <asm/errno.h>
#include <linux/sysinfo.h> // uapi/...
#include <sys/mman.h>

#include <linux/version.h>

// IMPORTANT(Ryan): This is involved to replace due to sysv calling convention with registers (floating) and stack 
// gcc implements it as a builtin, so no linking
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end

#define INTERNAL static
#define NULL (void *)0

typedef long int i64;
typedef unsigned long int u64;
typedef int i32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;

// IMPORTANT(Ryan): Specify size for ease with struct paddings
typedef b32 u32;
#define true 1
#define false 0

// IMPORTANT(Ryan): Across different architectures, the contents of syscalls same named return struct may be different, e.g. stat struct
// Always refer to kernel headers of libc headers
i32
x64_syscall_write(u32 fd, char const *data, u64 data_size)
{
  i32 result = 0;
  // syscall arguments must be pointers or integers as specified in abi

  // use inputs instead of explicitly moving into register
  __asm__ __volatile__("syscall"
              : "=a" (result) // = clobbers eax
              : "a" (1), // eax
                "D" (fd), // edi
                "S" (data), // rsi
                "d" (data_size) // rdx
              : "r11", "rcx", "memory"); // memory is to specify argument value can be used

  if ((u32)result >= (u32)(-4095)) {
    // error
  }

  return result;
}

//void *
//x64_syscall_mmap(struct sysinfo *sys_info)
//{
//  i32 sys_result = 0;
//  __asm__ __volatile__("mov r10, %5\n",
//             "mov r8, %6\n",
//             "mov r9, %7\n",
//             "syscall"
//              : "=a" (sys_result)
//              : "a" (9), 
//                "D" (NULL),
//                "S" (size),
//                "d" (read|write),
//                "r" ((u64)),
//                "r" ((u64)),
//                "r" ((u64)),
//              : "r10", "r8", "r9", "r11", "rcx", "memory");
//
//  void *result = NULL;
//  if ((u64)sys_result < (u64)(-4095)) {
//    result = (void *)(sys_result);
//  }
//
//  return result;
//}

INTERNAL i32
x64_syscall_sysinfo(struct sysinfo *sys_info)
{
  i32 result = 0;
  __asm__ __volatile__("syscall"
              : "=a" (result)
              : "a" (99), 
                "D" (sys_info)
              : "r11", "rcx", "memory");
  return result;
}

// amd64 abi docs gives stack growth direction, setup of registers, etc.
// in reality it is System V ABI that is adopted by linux kernel
// we are concerned with process initialisation and syscalls
// IMPORTANT(Ryan): Use standard asm to avoid function prologue 
__asm__(".global entry_point\n"
  "entry_point:\n"
  "xor rbp, rbp\n"
  "pop rdi\n"  // argc
  "mov rsi, rsp\n" // argv
  "and rsp, 0xfffffffffffffff0\n"
  "call main\n"
  "mov rdi, rax\n"
  "mov rax, 60\n"
  "syscall\n"
  "ret");

  // xor rbp, rbp
  // pop rdi (argc into first argument)
  // mov rsi, rsp (rest of stack for argv)
  // and rsp, 0xfffffffffffffff0 (ensure stack is aligned to 16 bytes)
  // call here
  //
  // mov rdi, rax (ret value for here)
  // mov rax, 60 (if don't do this will get segfault as ret will try go to last address)
  // syscall
  // ret
  // uselib() syscall for shared libraries

// IMPORTANT(Ryan): modesetting only when drmSetMaster(). 
// probably unescessary for us as will be implicit by being the first to access card

#if defined(KER_SLOW) || defined(KER_DEBUG)
INTERNAL void assert(void) { (void *)0; }
#else
INTERNAL void assert(void) { return; }
#endif

INTERNAL void debug_breakpoint(void) { return; }

INTERNAL u64 
kilobytes(u32 num)
{
  return num * 1024ULL;
}

INTERNAL u64 
megabytes(u32 num)
{
  return kilobytes(num) * 1024ULL;
}

  // memory overcommitment is default and involves allocating to virtual address space and failing on usage.
  // mlock() more efficient for our purposes,
  // however when multiple processes better to just memset() to allow swapping

int
main(int argc, char *argv[])
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 3, 48)
#error (Structure Content Differs): Included for our edification.
#endif
  struct sysinfo sys_info = {0};
  if (x64_syscall_sysinfo(&sys_info) < 0) {
    // TODO(Ryan): Error logging
    breakpoint();
  }

  // TODO(Ryan): Parse sys_info output
  X64MemoryArena mem_arena = {0};
  if (x64_initialize_memory_arena(&mem_arena, megabytes(256)) < 0) {
    // TODO(Ryan): Error logging
    breakpoint();
  }

  u32 *pixel_buffer = x64_create_pixel_buffer(&mem_arena, 720, 480);

  b32 want_to_run = true;
  while (want_to_run) {
    // display buffer 
  } 


  return 0;
}
