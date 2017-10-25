#include<iostream>
#include<fstream>
#include "pin.H"
#define START 0x8048202
#define END 0x80A3778
using namespace std;
ofstream out("flag");
long long int iXor,iCmp;
void Instruction(INS ins, void *v)
{
        // INS_Address 현제 주소를 구해온다.
        ADDRINT Address = INS_Address(ins);
        // 원하는 범위 한정
        if(START<=Address && Address<=END)
        {
                char cAns;
                // INS_Opcode 현제 주소의 Opcode를 구해온다.
                switch(INS_Opcode(ins))
                {
                // 만약 XOR이라면
                case XED_ICLASS_XOR:
                        // Immediate를 가지고 온다.
                        iXor = INS_OperandImmediate(ins,1);
                        break;
                // 만약 CMP라면
                case XED_ICLASS_CMP:
                        // Immediate를 가지고 온다.
                        iCmp = INS_OperandImmediate(ins,1);
                        cAns = iXor^iCmp;
                        // 출력
                        out<<cAns;
                        iXor=0;
                        break;
                }
                // 명령어 삭제로 다음명령어 강제 실행
                INS_Delete(ins);
        }
}
int main(int argc, char* argv[])
{
        // 핀툴 초기화
        if(PIN_Init(argc,argv)) return -1;
        // CallBack 함수 추가   
        INS_AddInstrumentFunction(Instruction, 0);
        // 프로그램 시작
        PIN_StartProgram();
        return 0;
}
 
