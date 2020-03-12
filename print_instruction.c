// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"

#define sw 43
#define sh 41
#define sb 40
#define lw 35
#define lh 33
#define lb 32
#define mul 28
#define lui 15
#define xori 14
#define ori 13
#define andi 12
#define slti 10
#define addi 8
#define bgtz 7
#define blez 6
#define bne 5
#define beq 4
#define jal 3
#define j 2

#define syscall 12
#define add 32
#define sub 34
#define and 36
#define or 37
#define xor 38
#define sllv 4
#define srlv 6
#define jr 8
#define slt 42
#define sll 0
#define srl 2

#define bltz 0
#define bgez 1

/*
 * print a MIPS instruction (no newline)
 * run reference implementation if unsure what to print
 */
void set1_instruction(uint32_t instruction);
void set2_instruction(uint32_t instruction);

void print_instruction(uint32_t instruction) {
    unsigned int jumper = instruction >> 26;
    if (jumper == 0)
        set1_instruction(instruction);
    else if (jumper == 1)
        set2_instruction(instruction);
    else if (jumper == sw) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        printf("sw $%d, %d($%d)", t, o, b);
    }
    else if (jumper == sh) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        printf("sh $%d, %d($%d)", t, o, b);
    }
    else if (jumper == sb) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        printf("sb $%d, %d($%d)", t, o, b);
    }
    else if (jumper == lw) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        printf("lw $%d, %d($%d)", t, o, b);
    }
    else if (jumper == lh) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        printf("lh $%d, %d($%d)", t, o, b);
    }
    else if (jumper == lb) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        printf("lb $%d, %d($%d)", t, o, b);
    }
    
    else if (jumper == mul) {
        jumper = instruction >> 11;
        int d = jumper & 0x1F;
        jumper = jumper >> 5;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("mul $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == lui) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        printf("lui $%d, %d", t, i);
    }
    else if (jumper == xori) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("xori $%d, $%d, %d", t, s, i);
    }
    else if (jumper == ori) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("ori $%d, $%d, %d", t, s, i);
    }
    else if (jumper == andi) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("andi $%d, $%d, %d", t, s, i);
    }
    else if (jumper == slti) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("slti $%d, $%d, %d", t, s, i);
    }
    else if (jumper == addi) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("addi $%d, $%d, %d", t, s, i);
    }
    else if (jumper == bgtz) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        //int t = jumper | 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("bgtz $%d, %d", s, i);
    }
    else if (jumper == blez) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        //int t = jumper | 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("blez $%d, %d", s, i);
    }
    else if (jumper == bne) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("bne $%d, $%d, %d", s, t, i);
    }
    else if (jumper == beq) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("beq $%d, $%d, %d", s, t, i);
    }
    else if (jumper == jal) {
        int x = instruction & 0x3FFFFFF;
        printf("jal 0x%0x", x);
    }
    else if (jumper == j) {
        int x = instruction & 0x3FFFFFF;
        printf("j 0x%0x", x);
    }
    else
        //Things are very wrong if this happens
        printf("Error A: 0_o\n");
}

// PUT YOUR FUNCTIONS HERE
// There will be a lot of code. Sorry.

void set1_instruction(uint32_t instruction) {
    int jumper = instruction & 0x3F;

    if (jumper == syscall) {
        printf("syscall");
    }
    else if (jumper == add) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("add $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == sub) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("sub $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == and) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("and $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == or) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("or $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == xor) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("xor $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == sllv) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("sllv $%d, $%d, $%d", d, t, s);
    }
    else if (jumper == srlv) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("srlv $%d, $%d, $%d", d, t, s);
    }

    else if (jumper == jr) {
        jumper = instruction >> 21;
        int s = jumper & 0x1F;
        printf("jr $%d", s);
    }
    else if (jumper == slt) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        printf("slt $%d, $%d, $%d", d, s, t);
    }
    else if (jumper == sll) {
        jumper = instruction >> 6;
        unsigned int I = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        printf("sll $%d, $%d, %d", d, t, I);
    }
    else if (jumper == srl) {
        jumper = instruction >> 6;
        unsigned int I = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        printf("srl $%d, $%d, %d", d, t, I);
    }
    else
    //Things are very wrong if this happens
    printf("Error 0_o\n");
}

void set2_instruction(uint32_t instruction) {
    int jumper = instruction >> 16;
    jumper = jumper & 0x1F;
    if (jumper == bltz) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        //int t = jumper | 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("bltz $%d, %d", s, i);
    }
    else if (jumper == bgez) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        //int t = jumper | 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        printf("bgez $%d, %d", s, i);
    }
    else
    //Things are very wrong if this happens
    printf("Error 0_o\n");
}