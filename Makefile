# Build + run the OS in QEMU. We assemble the boot sector into a flat binary
# (no ELF headers) because the BIOS expects raw machine code, not a program.
ASM := nasm

all: build/os.img

build/boot.bin: boot/boot.asm
	$(ASM) -f bin $< -o $@      # -f bin = raw binary, exactly what the BIOS loads

build/os.img: build/boot.bin
	cp $< $@                    # for now the disk image IS just the boot sector

run: build/os.img
	qemu-system-i386 -drive format=raw,file=build/os.img

clean:
	rm -f build/*.bin build/*.img
