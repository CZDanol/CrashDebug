#ifndef _PRUSA_DUMP_PARSER_H
#define _PRUSA_DUMP_PARSER_H

#include "try_catch.h"
#include "MemorySim.h"
#include "mriPlatform.h"

__throws void PrusaDumpBin_Read(IMemory* pMem, RegisterContext* pContext, const char *pDumpFilename);

#endif /* _PRUSA_DUMP_PARSER_H */
