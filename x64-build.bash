#! /usr/bin/env bash
# SPDX-License-Identifier: zlib-acknowledgement

# IMPORTANT(Ryan): $(sudo visudo /etc/sudoers) 
# ryan ALL=(ALL) NOPASSWD:ALL to avoid entering sudo password

[[ ! -d mount-point/sbin ]] && sudo mount ker.img mount-point -t ext4 -o loop

# -nolibc for linker
debug_flags="-g"
common_flags="-Wl,-eentry_point -ffreestanding -nostdlib -no-pie -masm=intel"

sudo gcc ${debug_flags} ${common_flags} code/x64-ker.c -o mount-point/sbin/x64-ker
