#include <cstring>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "../cmd.h"
#include "process.h"
#include "../stack.h"

static const double THRESHOLD = 1e-6;

static bool
are_equal(double value1, double value2)
{
        return (fabs(value1 - value2) < THRESHOLD);
}

static void
cpu_dump(double *cmd_buf, size_t cmd_count, size_t ip)
{
        printf("CPU dump:\n");

        for (size_t i = 0; i < cmd_count; i++)
                printf("%02zu ", i);
        printf("\n");

        for (size_t i = 0; i < cmd_count; i++)
                printf("%02lg ", cmd_buf[i]);
        printf("\n");

        for (size_t i = 0; i < ip; i++)
                printf("   ");
        printf("^\n");
}

static void
draw_vram(double *vram)
{
        for (int i = 0; i < RESOL_Y; i++) {
                for (int j = 0; j < RESOL_X; j++) {
                        if (are_equal(vram[i*RESOL_X + j], 1))
                                printf("*");
                        else
                                printf(" ");
                }
                printf("\n");
        }
        printf("\n");
}

#define POP(val) stack_pop(&stack, &(val)); reg[REG_RSP] = (double) stack.size;
#define PUSH(val) stack_push(&stack, (val)); reg[REG_RSP] = (double) stack.size;
#define IF_PUSH(msk, dst) if ((cmd & (msk)) == (msk)) {  \
                                PUSH(dst)                \
                                ip++;                    \
                                if ((msk) == (IMMED_MASK | REG_MASK | RAM_MASK)) \
                                        ip++;            \
                                }                        \
                        else
#define IF_POP(msk, dst, ...) if ((cmd & (msk)) == (msk)) {  \
                                POP(dst)                 \
                                __VA_ARGS__              \
                                ip++;                    \
                                if ((msk) == (IMMED_MASK | REG_MASK | RAM_MASK)) \
                                        ip++;            \
                                }                        \
                        else
#define DEF_CMD(name, ...) case CMD_##name: ip++; __VA_ARGS__ break;
#define DEF_JMP(name, ...) case CMD_##name: ip++; __VA_ARGS__ break;

void
execute(double *cmd_buf, size_t size)
{
        double val1 = 0;
        double val2 = 0;
        stack_t stack {};
        size_t ip = 0;

        double reg[N_REG] = {};
        double ram[N_RAM] = {};
        double vram[RESOL_X*RESOL_Y] = {};

        stack_ctor(&stack, DEF_STACK_CAPACITY, VAR_INFO(stack));

        bool halt = false;
        while (ip < size && !halt) {
                /*
                 * usleep(10000);
                 */
                int cmd = (int) cmd_buf[ip];
                switch (cmd & CMD_MASK) {
                        DEF_CMD(PUSH,
                                IF_PUSH(RAM_MASK | REG_MASK | IMMED_MASK,
                                        ram[(int) cmd_buf[ip] + (int) reg[(int) cmd_buf[ip + 1]]])
                                IF_PUSH(RAM_MASK | IMMED_MASK, ram[(int) cmd_buf[ip]])
                                IF_PUSH(REG_MASK | RAM_MASK, ram[(int) reg[(int) cmd_buf[ip]]])
                                IF_PUSH(IMMED_MASK, cmd_buf[ip])
                                IF_PUSH(REG_MASK, reg[(int) cmd_buf[ip]])
                                {
                                assert(0 && "Invalid PUSH command.\n");
                                }
                               )
                        DEF_CMD(POP,
                                IF_POP(RAM_MASK | REG_MASK | IMMED_MASK,
                                        ram[(int) cmd_buf[ip] + (int) reg[(int) cmd_buf[ip + 1]]])
                                IF_POP((RAM_MASK | IMMED_MASK), ram[(int) cmd_buf[ip]], )
                                IF_POP(REG_MASK | RAM_MASK, ram[(int) reg[(int) cmd_buf[ip]]])
                                IF_POP((REG_MASK), val1,
                                        reg[(int) cmd_buf[ip]] = val1;
                                        if ((int) cmd_buf[ip] == REG_RSP) {
                                        stack.size = (size_t) reg[REG_RSP];
                                        })
                                {
                                assert(0 && "Invalid POP command.\n");
                                }
                               )

                        #include "../cmds.inc"

                        default:
                                assert(0 && "Invalid command.\n");

                }

                cmd = (int) cmd_buf[ip];
        }

        stack_dtor(&stack);
}

#undef POP
#undef PUSH
#undef IF_PUSH
#undef IF_POP
#undef DEF_CMD
#undef DEF_JMP

