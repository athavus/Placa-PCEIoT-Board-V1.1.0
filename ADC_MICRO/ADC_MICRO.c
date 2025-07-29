#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/mic_adc.h"
#include "tests/tests_mic_adc.h"

int main() {
    stdio_init_all();

    printf("Iniciando aplicacao principal...\n");

    run_mic_adc_test(); // Isso executará todos os testes que você adicionar

    while (true) {
        // Você pode adicionar seu código principal aqui,
        // como um sistema de detecção de palmas, etc.
        sleep_ms(1000);
    }

    return 0;
}


/* 
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define N_SAMPLES 1000
uint16_t sample_buf[N_SAMPLES];

// A função printhelp não será mais usada para leitura constante, mas mantida para referência
void printhelp() {
    puts("\nCommands:");
    puts("c0, ...\t: Select ADC channel n");
    puts("s\t: Sample once");
    puts("S\t: Sample many");
    puts("w\t: Wiggle pins");
}

void __not_in_flash_func(adc_capture)(uint16_t *buf, size_t count) {
    adc_fifo_setup(true, false, 0, false, false);
    adc_run(true);
    for (size_t i = 0; i < count; i = i + 1)
        buf[i] = adc_fifo_get_blocking();
    adc_run(false);
    adc_fifo_drain();
}

int main(void) {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);

    // Set all pins to input (as far as SIO is concerned)
    gpio_set_dir_all_bits(0);
    for (int i = 2; i < 30; ++i) {
        gpio_set_function(i, GPIO_FUNC_SIO);
        if (i >= 26) {
            gpio_disable_pulls(i);
            gpio_set_input_enabled(i, false);
        }
    }

    printf("\n===========================\n");
    printf("RP2040 ADC e Console de Teste\n");
    printf("===========================\n");

    // *** MODIFICAÇÃO AQUI: Seleciona o ADC1 (GPIO27) e inicia a leitura contínua ***
    adc_select_input(1); // Seleciona o canal ADC1 (GPIO27)
    printf("Lendo continuamente o ADC1 (GPIO27)...\n");

    while (1) {
        uint32_t result = adc_read();
        const float conversion_factor = 3.3f / (1 << 12);
        printf("Leitura ADC1: %d -> %f V\n", result, result * conversion_factor);

        sleep_ms(500); // Atraso de 500 milissegundos (0.5 segundos) entre as leituras
    }

    // O loop original de comandos foi removido para focar na leitura contínua do ADC1
    // switch (c) { ... }
}
*/