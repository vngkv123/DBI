#include "pin.H"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <unistd.h>
 
int new_usleep(useconds_t usec)
{
    return 0;
}
 
VOID Image(IMG img, VOID *v)
{
    for (SYM sym = IMG_RegsymHead(img); SYM_Valid(sym); sym = SYM_Next(sym)) {
        string funcname = PIN_UndecorateSymbolName(SYM_Name(sym), 
            UNDECORATION_NAME_ONLY);
        if(funcname == "usleep") {
            RTN rtn = RTN_FindByAddress(IMG_LowAddress(img) + SYM_Value(sym));
            if(RTN_Valid(rtn)) {
                RTN_Open(rtn);
                RTN_Replace(rtn, (AFUNPTR) &new_usleep);
                RTN_Close(rtn);
            }
        }
    }
}
 
void xor_rax(CONTEXT *ctx)
{
    PIN_SetContextReg(ctx, REG_RAX, 0);
}
 
void mov_rax_rdx(CONTEXT *ctx)
{
    ADDRINT rdx = PIN_GetContextReg(ctx, REG_RDX);
    PIN_SetContextReg(ctx, REG_RAX, rdx);
}
 
void delete_insns(BBL bbl)
{
    for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins)) {
        INS_Delete(ins);
    }
}
 
VOID Trace(TRACE trace, VOID *v)
{
    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl)) {
        if(BBL_NumIns(bbl) == 2) {
            INS ins1 = BBL_InsHead(bbl);
            INS ins2 = INS_Next(ins1);
            if(INS_Disassemble(ins1) == "sub rax, 0x1" && INS_IsBranch(ins2) &&
                    INS_DirectBranchOrCallTargetAddress(ins2) == BBL_Address(bbl)) {
                BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR), &xor_rax,
                    IARG_CONTEXT, IARG_END);
                delete_insns(bbl);
            }
        }
        else if(BBL_NumIns(bbl) == 3) {
            INS ins1 = BBL_InsHead(bbl);
            INS ins2 = INS_Next(ins1);
            INS ins3 = INS_Next(ins2);
            if(INS_Disassemble(ins1) == "sub rax, 0x1" && INS_IsNop(ins2) &&
                    INS_DirectBranchOrCallTargetAddress(ins3) == BBL_Address(bbl)) {
                BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR) &xor_rax, IARG_CONTEXT,
                    IARG_END);
                delete_insns(bbl);
            }
            else if(INS_Disassemble(ins1) == "add rax, 0x1" &&
                    INS_Disassemble(ins2) == "cmp rax, rdx" &&
                    INS_DirectBranchOrCallTargetAddress(ins3) == BBL_Address(bbl)) {
                BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR) &mov_rax_rdx, 
                    IARG_CONTEXT,IARG_END);
                delete_insns(bbl);
            }
        }
    }
}
 
int main(int argc, char *argv[])
{
    if(PIN_Init(argc,argv)) {
        return 0;
    }
 
    PIN_InitSymbols();
 
    IMG_AddInstrumentFunction(Image, 0);
    TRACE_AddInstrumentFunction(Trace, 0);
 
    PIN_StartProgram();
    return 0;
}
