# CrashDebug

This is an utility meant for exploring crash dumps made by the Buddy firmware.
It is a fork of https://github.com/adamgreen/CrashDebug adding support for Buddy's Crash Dump files.

## How to use it

1. Clone this repository

    ```sh
    git clone git@github.com:prusa3d/CrashDebug.git
    cd CrashDebug
    git submodule update --init --recursive
    ```

2. Build the project

    ```sh
    make CrashDebug
    ```

3. Collect the dump file (`.bin`) and appropriate ELF file (build product with no extension; ELF files are archived by Holly - holly.prusa3d.com).

    ```sh
    ❯ ls another_bsod_investigation
    dump.bin firmware
    ```

4. Start the debugger with your ELF file and the crash dump.

    ```sh
    arm-none-eabi-gdb <ELF FILE> \
        -ex "set target-charset ASCII" \
        -ex "target remote | ./CrashDebug --elf <ELF FILE> --dump <DUMP FILE> "
    ```

    > In case you don't have arm-none-eabi-gdb globally installed, you can you the one automatically bootstrapped by Buddy's build system (within the .dependecies folder).

    Example:

    ```sh
    arm-none-eabi-gdb ./another_bsod_investigation/firmware \
        -ex "set target-charset ASCII" \
        -ex "target remote | ./CrashDebug --elf ./another_bsod_investigation/firmware --dump ./another_bsod_investigation/dump.bin"
    ```

5. Use GDB to investigate the problem! 🎉

    ![screenshot](screenshot.png)





