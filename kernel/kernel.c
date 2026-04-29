#include "../kernel/kernel.h"
#include "../kernel/drivers/graphics.h"
#include "../kernel/drivers/screen.h"
#include "../kernel/drivers/keyboard.h"
#include "../kernel/drivers/timer.h"
#include "../kernel/mm/memory.h"
#include "../kernel/arch/x86/idt.h"
#include "../kernel/arch/x86/irq.h"
#include "../kernel/cpu/cpu.h"
#include "../kernel/fs/fs.h"
#include "../kernel/sched/sched.h"
#include "../shell/shell.h"

static void enable_interrupts(void) {
    __asm__ volatile("sti");
}

static void splash_screen(void) {
    /* Clear screen */
    graphics_clear();

    /* Define colors */
    color_t white = {255, 255, 255};
    color_t blue = {0, 100, 200};
    color_t gray = {128, 128, 128};

    /* Get screen dimensions */
    uint32_t width = graphics_get_width();
    uint32_t height = graphics_get_height();

    /* Draw NyxOS logo - centered */
    uint32_t logo_x = (width - 200) / 2;
    uint32_t logo_y = height / 2 - 100;

    /* Simple ASCII art logo */
    graphics_draw_string(logo_x, logo_y, "    _   __           __", white, COLOR_BLACK);
    graphics_draw_string(logo_x, logo_y + 15, "   / | / /___  _____/ /_", white, COLOR_BLACK);
    graphics_draw_string(logo_x, logo_y + 30, "  /  |/ / __ \\/ ___/ __/", white, COLOR_BLACK);
    graphics_draw_string(logo_x, logo_y + 45, " / /|  / /_/ / /  / /_", white, COLOR_BLACK);
    graphics_draw_string(logo_x, logo_y + 60, "/_/ |_/_\\____/_/   \\__/", white, COLOR_BLACK);

    /* Version info */
    graphics_draw_string(logo_x + 50, logo_y + 90, "NyxOS v1-STABLE", blue, COLOR_BLACK);

    /* CPU info */
    cpu_info_t *cpu_info = cpu_get_info();
    graphics_draw_string(logo_x, logo_y + 120, "CPU: ", gray, COLOR_BLACK);
    graphics_draw_string(logo_x + 40, logo_y + 120, cpu_info->vendor, white, COLOR_BLACK);
    graphics_draw_string(logo_x + 40, logo_y + 135, cpu_info->brand, white, COLOR_BLACK);

    /* Loading messages */
    graphics_draw_string(logo_x, logo_y + 165, "Initializing kernel...", gray, COLOR_BLACK);
    timer_wait(500);  // Wait 500ms

    graphics_draw_string(logo_x, logo_y + 180, "Loading memory manager...", gray, COLOR_BLACK);
    timer_wait(500);

    graphics_draw_string(logo_x, logo_y + 195, "Starting shell...", gray, COLOR_BLACK);
    timer_wait(1000);

    /* Clear screen for shell */
    graphics_clear();
}

void kernel_main(uint32_t magic, multiboot_info_t *mboot_info) {
    /* Phase 1: Initialize core systems */
    graphics_init(mboot_info);

    /* Initialize timer early for splash screen timing */
    idt_init();
    irq_init();
    timer_init(100);
    irq_set_handler(0, timer_handler);
    irq_enable(0);

    /* Initialize CPU detection for splash screen */
    cpu_init();

    /* Show splash screen */
    splash_screen();

    /* Continue with normal initialization */
    screen_init();  // Fallback text mode if needed

    mem_init();
    screen_write_string("[OK] Memory manager initialized\n");

    keyboard_init();
    irq_set_handler(1, keyboard_handler);
    irq_enable(1);
    screen_write_string("[OK] Keyboard initialized\n");

    screen_write_string("[OK] Timer initialized\n");

    fs_init();
    screen_write_string("[OK] Filesystem initialized\n");

    sched_init();
    screen_write_string("[OK] Scheduler initialized\n");

    screen_write_string("\nKernel initialization complete!\n");
    screen_write_string("CPU: ");
    cpu_info_t *info = cpu_get_info();
    screen_write_string(info->vendor);
    screen_write_string(" ");
    screen_write_string(info->brand);
    screen_write_string("\n\n");

    /* Phase 2: Enable interrupts and start shell */
    enable_interrupts();

    shell_init();
    shell_run();

    /* Should never reach here */
    screen_write_string("ERROR: Shell exited!\n");
    __asm__ volatile("hlt");
}
