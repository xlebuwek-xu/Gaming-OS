#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_state
{
    uint16_t limit;
    uint64_t base;

    uint16_t cs;
    uint16_t ds;
    uint16_t es;
    uint16_t ss;
};

void gdt_init(void);
void gdt_read_state(struct gdt_state *state);

#endif
