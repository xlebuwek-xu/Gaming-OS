#include <stdint.h>
#include <stddef.h>
#include "limine.h"
#include "gdt.h"

__attribute__((used, section(".limine_requests_start"), aligned(8)))
static volatile uint64_t requests_start_marker[] =
    LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests"), aligned(8)))
static volatile uint64_t base_revision[] =
    LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests"), aligned(8)))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests_end"), aligned(8)))
static volatile uint64_t requests_end_marker[] =
    LIMINE_REQUESTS_END_MARKER;

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_init(void)
{
    outb(0x3F9, 0x00);
    outb(0x3FB, 0x80);
    outb(0x3F8, 0x03);
    outb(0x3F9, 0x00);
    outb(0x3FB, 0x03);
    outb(0x3FA, 0xC7);
    outb(0x3FC, 0x0B);
}

static void serial_write_char(char c)
{
    while ((inb(0x3FD) & 0x20) == 0)
    {
    }

    outb(0x3F8, (uint8_t)c);
}

static void serial_write_string(const char *str)
{
    while (*str)
    {
        serial_write_char(*str++);
    }
}


static void serial_write_hex(uint64_t value)
{
    static const char digits[] = "0123456789ABCDEF";

    for (int i = 15; i >= 0; i--)
    {
        serial_write_char(digits[(value >> (i * 4)) & 0xF]);
    }
}

void kernel_main(void)
{
    serial_init();

    serial_write_string("\nGaming OS: kernel started!\n");

    if (!LIMINE_BASE_REVISION_SUPPORTED(base_revision))
    {
        serial_write_string("ERROR: Unsupported Limine base revision.\n");

        for (;;)
        {
            __asm__ volatile ("hlt");
        }
    }

    serial_write_string("Limine base revision: supported.\n");
    serial_write_string("GDT: loading custom GDT...\n");
    gdt_init();
    serial_write_string("GDT: custom GDT loaded successfully.\n");

    struct gdt_state gdt_state;
    gdt_read_state(&gdt_state);

    serial_write_string("GDT: GDTR base = 0x");
    serial_write_hex(gdt_state.base);
    serial_write_string("\n");

    serial_write_string("GDT: GDTR limit = 0x");
    serial_write_hex(gdt_state.limit);
    serial_write_string("\n");

    serial_write_string("GDT: CS = 0x");
    serial_write_hex(gdt_state.cs);
    serial_write_string("\n");

    serial_write_string("GDT: DS = 0x");
    serial_write_hex(gdt_state.ds);
    serial_write_string("\n");

    serial_write_string("GDT: ES = 0x");
    serial_write_hex(gdt_state.es);
    serial_write_string("\n");

    serial_write_string("GDT: SS = 0x");
    serial_write_hex(gdt_state.ss);
    serial_write_string("\n");

    serial_write_string("Gaming OS: first boot milestone reached!\n");

    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}
