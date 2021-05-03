#! /usr/bin/env bash
# SPDX-License-Identifier: zlib-acknowledgement

# IMPORTANT(Ryan): Disable CONFIG_STRICT_KERNEL_RWX to enable the use of software breakpoints

# IMPORTANT(Ryan): The location of where the kernel was compiled must be maintained, i.e cannot copy it.
# This is because the path to files are hardcoded from this location.

[[ ! -d mount-point/sbin ]] && sudo mount ker.img mount-point -t ext4 -o loop

# may need to add -device virtio-gpu-pci (graphics) -soundhw hda (intel hd audio)
qemu-system-x86_64 -enable-kvm -m 512M -s -S \
  -drive format=raw,file=ker.img \
  -kernel ~/Downloads/linux-5.11.16/arch/x86_64/boot/bzImage \
  -append "root=/dev/sda init=/sbin/x64-ker nokaslr" 
