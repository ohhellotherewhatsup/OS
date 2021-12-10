set -e
	#statements
as --32 boot.s -o boot.o

#compile kernel.c file
gcc -m32 -c src/kernel.c -o kernel.o -std=gnu99 -g -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
gcc -m32 -c src/util.c -o util.o -std=gnu99 -g -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
gcc -m32 -c src/ascii.c -o ascii.o -std=gnu99 -g -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
gcc -m32 -c src/command.c -o command.o -std=gnu99 -g -ffreestanding -O2 -Wall -Wextra -fno-stack-protector
gcc -m32 -c src/interrupt.c -o interrupt.o -std=gnu99 -g -ffreestanding -O2 -Wall -Wextra -fno-stack-protector

#linking the kernel with kernel.o and boot.o files
ld -m elf_i386 -T linker.ld kernel.o interrupt.o command.o util.o ascii.o boot.o -o os.bin -nostdlib
addr2line -e os.bin 0x1bf

#check os.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot os.bin

#building the iso file
mkdir -p isodir/boot/grub
cp os.bin isodir/boot/os.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o os.iso isodir
