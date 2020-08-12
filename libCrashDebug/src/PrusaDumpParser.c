#include <stdio.h>
#include "PrusaDumpParser.h"

// dumped ram area (128kb)
#define DUMP_RAM_ADDR 0x20000000
#define DUMP_RAM_SIZE 0x00020000

// dumped ccram area (64kb), last 256 bytes used for register dump etc.
#define DUMP_CCRAM_ADDR 0x10000000
#define DUMP_CCRAM_SIZE 0x00010000

// general registers stored to ccram
// r0-r12, sp, lr, pc - 64 bytes
// xpsr, fpcsr, PRIMASK, BASEPRI, FAULTMASK, CONTROL, MSP, PSP - 32 bytes
#define DUMP_REGS_GEN_ADDR 0x1000ff00
#define DUMP_REGS_GEN_SIZE 0x00000060

static const int dump_register_map[] = {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    SP,
    LR,
    PC,
    XPSR,
    -1, // PRIMASK
    -1, // BASEPRI
    -1, // FAULTMASK
    -1, // CONTROL
    MSP,
    PSP,
    -1, // lrexc
};

#define LREXC_OFFSET 0x5c


__throws void PrusaDumpBin_Read(IMemory* pMem, RegisterContext* pContext, const char *pDumpFilename) {
    uint32_t addr;
    uint32_t reg_idx;
    FILE* file = fopen(pDumpFilename, "rb");
    if (!file)
        __throw_msg(fileException, "Failed to open the dump file.");

    // load RAM
    MemorySim_CreateRegion(pMem, DUMP_RAM_ADDR, DUMP_RAM_SIZE);
    for (addr = DUMP_RAM_ADDR; addr < DUMP_RAM_ADDR + DUMP_RAM_SIZE; addr++) {
        uint8_t byte;
        int read = fread(&byte, 1, 1, file);
        if (read == 1) {
            IMemory_Write8(pMem, addr, byte);
        } else {
            __throw(fileFormatException);
        }
    }

    // load CCRAM
    MemorySim_CreateRegion(pMem, DUMP_CCRAM_ADDR, DUMP_CCRAM_SIZE);
    for (addr = DUMP_CCRAM_ADDR; addr < DUMP_CCRAM_ADDR + DUMP_CCRAM_SIZE; addr++) {
        uint8_t byte;
        int read = fread(&byte, 1, 1, file);
        if (read == 1) {
            IMemory_Write8(pMem, addr, byte);
        } else {
            __throw(fileFormatException);
        }
    }

    // load basic registers
    for (reg_idx = 0; reg_idx < sizeof(dump_register_map) / sizeof(dump_register_map[0]); reg_idx ++) {
        int reg = dump_register_map[reg_idx];
        if (reg == -1) // unsupported register; skip
            continue;
        pContext->R[reg] = IMemory_Read32(pMem, DUMP_REGS_GEN_ADDR + (reg_idx * 4));
    }

    pContext->exceptionPSR = IMemory_Read8(pMem, DUMP_REGS_GEN_ADDR + LREXC_OFFSET);
    uint32_t sp = pContext->R[SP];
    sp += 8 * 4; // skip pushed integer registers
    sp += (16 + 1 + 1) * sizeof(uint32_t); // skip pushed floating point registers
    pContext->R[SP] = sp;
}
