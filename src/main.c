#include <stdio.h>
#include "shell.h"

int main(void){
    shell_t sh; 
    shell_init(&sh); // Wypisze READY do bufora TX

    // 1. Sprawdzenie GET
    printf("--- TEST 1: Get ---\n");
    shell_rx_bytes(&sh, "get\r\n");
    shell_tick(&sh); // Przetworzenie

    // 2. Sprawdzenie SET
    printf("--- TEST 2: Set ---\n");
    shell_rx_bytes(&sh, "set 0.42\r\n");
    shell_tick(&sh);

    // 3. Sprawdzenie STAT
    printf("--- TEST 3: Stat ---\n");
    shell_rx_bytes(&sh, "stat\r\n");
    shell_tick(&sh);

    // 4. Sprawdzenie ECHO
    printf("--- TEST 4: Echo ---\n");
    shell_rx_bytes(&sh, "echo hello world\r\n");
    shell_tick(&sh);

    // 5. TEST PRZEPEŁNIENIA (Burst)
    // RB_SIZE = 128. Wysyłamy 200 komend "noop\r\n" (6 bajtów każda) = 1200 bajtów.
    // Oczekujemy dużego wzrostu licznika 'drop' wewnątrz RB.
    printf("--- TEST 5: Burst (Overflow) ---\n");
    for(int i=0;i<200;i++) {
        shell_rx_bytes(&sh, "noop\r\n");
    }
    // Teraz tickujemy, żeby przemielić to co weszło (i zobaczyć co przepadło)
    for(int i=0;i<20;i++) shell_tick(&sh);

// 6. Sprawdzenie statusu po awarii
    printf("--- TEST 6: Status post-burst ---\n");
    

    shell_rx_bytes(&sh, "\r\n");
    shell_tick(&sh); 

    shell_rx_bytes(&sh, "get\r\n");
    shell_tick(&sh);

    return 0;
}