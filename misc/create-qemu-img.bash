#! /usr/bin/env bash
# SPDX-License-Identifier: zlib-acknowledgement

# IMPORTANT(Ryan): As created with qemu as opposed to dd, we don't
# have to create GPT+ESP and install a bootloader like grub.
# This saves us having to deal with grub-install altering efibootmgr
# entry UUIDs.
qemu-img create ker.img 1G
mkfs.ext4 ker.img

mkdir mount-point
sudo mount ker.img mount-point -t ext4 -o loop
pushd mount-point

# TODO(Ryan): Are all of these necessary?
sudo mkdir lib64 lib sbin bin etc var dev proc sys run tmp

# TODO(Ryan): Are these necessary if running under qemu?
sudo mknod -m 600 dev/console c 5 1
sudo mknod -m 666 dev/null c 1 3

popd

sudo umount mount-point
