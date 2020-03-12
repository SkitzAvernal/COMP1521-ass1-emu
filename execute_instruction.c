// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"



/**
 * execute a MIPS instruction
 *
 * This function should:
 *
 * get register values by calling `get_register(register_type register_number)`
 * change registers by calling `set_register(register_type register_number, uint32_t value)`
 *
 * get memory values by `calling get_byte(uint32_t address)`
 * changes memory by `set_byte(uint32_t address, uint8_t value)`
 *
 * updates program_counter to address of next instruction
 *
 * returns 1 if an exit syscall is executed, 0 otherwise
 */
void set1_instructionexec(uint32_t instruction);
void set2_instructionexec(uint32_t instruction);

int execute_instruction(uint32_t instruction, uint32_t *program_counter) {

    unsigned int jumper = instruction >> 26;
    if (jumper == 0)
        set1_instructionexec(instruction);
    else if (jumper == 1)
        set2_instructionexec(instruction);
    else if (jumper == 43) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        uint32_t r = get_register(t);
        for (int i = 0; i < 4; i++) {
            set_byte(b+o+i, r);
            r = r >> 8;
        }
        //sw
    }
    else if (jumper == 41) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        uint16_t r = get_register(t);
        set_byte(b+o, r);
        r = r >> 8;
        set_byte(b+o+1, r);
        //sh
    }
    else if (jumper == 40) {
        uint32_t o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        uint32_t b = jumper & 0x1F;
        uint8_t r = get_register(t);
        set_byte(r, b+o);
        //sb
    }
    else if (jumper == 35) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        int32_t res = 0;
        for (int i = 0; i < 4; i++) {
            uint32_t resm = get_byte(b+o+i);
            res = res | (resm << 8*i);
        }
        set_register(t, res);
        //printf("lw $%d, %d($%d)", t, o, b);
    }
    else if (jumper == 33) {
        int o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        int16_t res = get_byte(b+o);
        uint32_t resm = get_byte(b+o+1);
        res = res | (resm << 8);
        set_register(t, res);
        //printf("lh $%d, %d($%d)", t, o, b);
    }
    else if (jumper == 32) {
        int16_t o = instruction & 0xFFFF;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int b = jumper & 0x1F;
        int8_t res = get_byte(b+o);
        set_register(t, res);
        //printf("lb $%d, %d($%d)", t, o, b);
    }
    
    else if (jumper == 28) {
        jumper = instruction >> 11;
        int d = jumper & 0x1F;
        jumper = jumper >> 5;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t mul = x * y;
        set_register(d, mul);
    }
    else if (jumper == 15) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        uint32_t lui = i << 16;
        set_register(t, lui);
        //printf("lui $%d, %d", t, i);
    }
    else if (jumper == 14) {
        unsigned int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t xori = s ^ i;
        set_register(t, xori);
        //printf("xori $%d, $%d, %d", t, s, i);
    }
    else if (jumper == 13) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t ori = s | i;
        set_register(t, ori);
        //printf("ori $%d, $%d, %d", t, s, i);
    }
    else if (jumper == 12) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t andi = s & i;
        set_register(t, andi);
        //printf("andi $%d, $%d, %d", t, s, i);
    }
    else if (jumper == 10) {
        int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1)
            i = -i;
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t slti = (s < i);
        set_register(t, slti);
        //printf("slti $%d, $%d, %d", t, s, i);
    }
    else if (jumper == 8) {
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
        uint32_t addi = s + i;
        set_register(t, addi);
        //printf("addi $%d, $%d, %d", t, s, i);
    }
    else if (jumper == 7) {
        unsigned int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            //i = -i;
        }
        jumper = instruction >> 16;
        //int t = jumper | 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        if (x > 0) {
            if (i >= 0)
                    (*program_counter) += i << 2;
                else
                    (*program_counter) -= -i << 2;
            return 0;
        }
        //printf("bgtz $%d, %d", s, i);
    }
    else if (jumper == 6) {
        unsigned int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        //int t = jumper | 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        if (x <= 0) {
            if (i >= 0)
                    (*program_counter) += i << 2;
                else
                    (*program_counter) -= -i << 2;
            return 0;
        }
        //printf("blez $%d, %d", s, i);
    }
    else if (jumper == 5) {
        unsigned int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        if (x != y) {
            if (i >= 0)
                    (*program_counter) += i << 2;
                else
                    (*program_counter) -= -i << 2;
            return 0;
        }
        //printf("bne $%d, $%d, %d", s, t, i);
    }
    else if (jumper == 4) {
        unsigned int i = instruction & 0x7FFF;
        jumper = instruction >> 15;
        if (jumper & 1){
            i = 0x7FFF-i+1;
            i = -i;
        }
        jumper = instruction >> 16;
        int t = jumper & 0x1F;
        jumper = jumper >> 5;
        int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        if (x == y) {
            if (i >= 0)
                    (*program_counter) += i << 2;
                else
                    (*program_counter) -= -i << 2;
            return 0;
        }
        //printf("beq $%d, $%d, %d", s, t, i);
    }
    else if (jumper == 3) {
        unsigned int x = instruction & 0x3FFFFFF;
        set_register(ra, (*program_counter) +4);
        (*program_counter) = ((*program_counter) & 0xF0000000) | (x << 2);
        //printf("jal 0x%0x", x);
        return 0;
    }
    else if (jumper == 2) {
        unsigned int x = instruction & 0x3FFFFFF;
        (*program_counter) = ((*program_counter) & 0xF0000000) | (x << 2);
        //printf("j 0x%0x", x);
        return 0;
    }
    else
        //Things are very wrong if this happens
        printf("Error: 0_o\n");
    if (jumper == 0) {
        jumper = instruction & 0x3F;
        if (jumper == 8){
            jumper = instruction >> 21;
            int s = jumper & 0x1F;
            (*program_counter) = s;
            return 0;
        }
        if (jumper == 12) {
            int caller = get_register(v0);
            if (caller == 1) {
                int res = get_register(a0);
                printf("%d", res);
            }
            if (caller == 4) {
                uint32_t res = get_register(a0);
                uint8_t character = get_byte(res); 
                while (character != '\0') {
                    putchar(character);
                    character = get_byte(res);
                }
            }
            if (caller == 5) {
                int res;
                scanf("%d", &res);
                set_register(v0, res);
            }
            if (caller == 8) {
                int size = get_register(a0);
                //char result[size];
                for (int i = 0; i < size; i++) {
                    int res = getchar();
                    if (res == '\n')
                        break;
                    set_byte(a0, res);
                }
            }
            if (caller == 10) {
                return 1;
            }
            if (caller == 11) {
                uint32_t reg = get_register(a0);
                uint8_t res = get_byte(reg);
                putchar(res);
            }
            if (caller == 12) {
                uint8_t res = getchar();
                set_byte(v0, res);
            }
        }
    }
    if (jumper == 1) {
        jumper = instruction >> 16;
        jumper = jumper & 0x1F;
        if (jumper == 0) {
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
            uint32_t x = get_register(s);
            if (x < 0) {
                if (i >= 0)
                    (*program_counter) += i << 2;
                else
                    (*program_counter) -= -i << 2;
                return 0;
            }
        }
        else if (jumper == 1) {
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
            uint32_t x = get_register(s);
            if (x >= 0) {
                if (i >= 0)
                    (*program_counter) += i << 2;
                else
                    (*program_counter) -= -i << 2;
                return 0;
            }
        } 
    }
    
    (*program_counter) += 4;
    return 0;
}

// PUT YOUR FUNCTIONS HERE
// There will be a lot of code. Sorry.

void set1_instructionexec(uint32_t instruction) {
    int jumper = instruction & 0x3F;

    if (jumper == 12) {
        //printf("syscall");
    }
    else if (jumper == 32) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t sum = x + y;
        set_register(d, sum);
    }
    else if (jumper == 34) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t sub = x - y;
        set_register(d, sub);
    }
    else if (jumper == 36) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t and = x & y;
        set_register(d, and);
    }
    else if (jumper == 37) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t or = x | y;
        set_register(d, or);
    }
    else if (jumper == 38) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t xor = x ^ y;
        set_register(d, xor);
    }
    else if (jumper == 4) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t sllv = y << x;
        set_register(d, sllv);
    }
    else if (jumper == 6) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t srlv = y >> x;
        set_register(d, srlv);
    }

    else if (jumper == 8) {
        //jumper = instruction >> 21;
        //int s = jumper & 0x1F;
        //printf("jr $%d", s);
        //return 0;
    }
    else if (jumper == 42) {
        jumper = instruction >> 11;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int s = jumper & 0x1F;
        uint32_t x = get_register(s);
        uint32_t y = get_register(t);
        uint32_t slt = (x < y);
        set_register(d, slt);
    }
    else if (jumper == 0) {
        jumper = instruction >> 6;
        unsigned int I = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        uint32_t y = get_register(t);
        uint32_t sll = y << I;
        set_register(d, sll);
    }
    else if (jumper == 2) {
        jumper = instruction >> 6;
        unsigned int I = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int d = jumper & 0x1F;
        jumper = jumper >> 5;
        unsigned int t = jumper & 0x1F;
        uint32_t y = get_register(t);
        uint32_t srl = y >> I;
        set_register(d, srl);
    }
    else
    //Things are very wrong if this happens
    printf("Error 0_o\n");
}

void set2_instructionexec(uint32_t instruction) {
    /*int jumper = instruction >> 16;
    jumper = jumper & 0x1F;
    if (jumper == 0) {
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
    else if (jumper == 1) {
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
    printf("Error 0_o\n")*/
}
