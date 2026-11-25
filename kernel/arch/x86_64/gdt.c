#include <stdint.h>
#include "gdt.h"

struct gdt_pointer
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static uint64_t gdt[3] __attribute__((aligned(8))) =
{
    0x0000000000000000ULL,
    0x00AF9A000000FFFFULL,
    0x00CF92000000FFFFULL
};

extern void gdt_load(const struct gdt_pointer *gdt_ptr);

void gdt_init(void)
{
    struct gdt_pointer gdt_ptr;

    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint64_t)&gdt[0];

    gdt_load(&gdt_ptr);
}

void gdt_read_state(struct gdt_state *state)
{
    struct gdt_pointer current_gdt;

    __asm__ volatile ("sgdt %0" : "=m"(current_gdt));

    state->limit = current_gdt.limit;
    state->base = current_gdt.base;

    __asm__ volatile ("mov %%cs, %0" : "=r"(state->cs));
    __asm__ volatile ("mov %%ds, %0" : "=r"(state->ds));
    __asm__ volatile ("mov %%es, %0" : "=r"(state->es));
    __asm__ volatile ("mov %%ss, %0" : "=r"(state->ss));
}
