#include "global.h"

void asmPrint (FILE* fp) {
    vector<string> outs;
    outs.push_back(".data");
    outs.push_back("_prompt: .asciiz \"Enter an integer:\"");
    outs.push_back(".globl main");
    outs.push_back(".text");
    outs.push_back("read:\n  li $v0, 4\n  la $a0, _prompt\n  syscall\n  li $v0, 5\n  syscall\n  jr $ra\n\nwrite:\n  li $v0, 1\n  syscall\n  li $v0, 4\n  la $a0, _ret\n  syscall\n  move $v0, $0\n  jr $ra\n");
    // for (int i = 0; i < irCount(); ++i) {
    //     InterRepresentation* ir = irGet(i);

    // }
    for (string out : outs) {
        fprintf(fp, "%s\n", out.c_str());
    }
}
