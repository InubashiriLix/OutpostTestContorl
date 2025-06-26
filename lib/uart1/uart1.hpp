#pragma once

extern char send_buffer[64];
void serial1_init();
void serial1_task(void *args);