# pintool
**Easy to use Instrumentation**
- Use dynamic instrumentation
- Provides rich APIs to write in C/C++ your own instrumentation tools.


# How to use?
**Launch**
- `pin -t pintool -- application`
- `pin` is instrumentation engine
- `pintool` is Intrumentation tool.( write your own, or use one provided in the kit )

**attach to and instrument an application**
- `pin -t pintool -pid [pid]`

# APIs
**Basic APIs**
- Architecture independent.
- Control-flow changes
- MEmory accesses

**Call-based APIs**
- Instrumentation routines
- Analysis routines

# sample code
```
  1 #include <stdio.h>
  2 #include <iostream>
  3 #include "pin.H"
  4 
  5 using namespace std;
  6 
  7 #define start_address 0x0000000000400618
  8 #define end_address 0x00000000004006ab
  9 
 10 INT32 Usage()
 11 {
 12     PIN_ERROR("this pintool print a trace of memory address\n");
 13     return -1;
 14 }
 15 void Instruction(INS ins, VOID* v)
 16 {
 17     ADDRINT a = INS_Address(ins);   /* get $pc address */
 18     if(start_address<=a && a<=end_address)
 19     {
 20         if(INS_Opcode(ins) == XED_ICLASS_CMP )
 21         {
 22             UINT64 value = INS_OperandImmediate(ins,1);
 23             printf("value is 0x%lx \n",value);
 24         }
 25     }
 26 }
 27 void Fini(INT32 code, VOID *v)
 28 {
 29     cout << "Fini code: " << code << endl;
 30 }
 31 int main(int argc,char* argv[])
 32 {
 33     // Pin initiate
 34     if(PIN_Init(argc,argv))
 35         return Usage();
 36     INS_AddInstrumentFunction(Instruction,0);
 37     PIN_AddFiniFunction(Fini,0);
 38     PIN_StartProgram();
 39     return 0;
 40 }
```
