#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"


const uint I2C_SDA = 6;
const uint I2C_SCL = 7;

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
// SPI defines - CORRIGIDO para sua placa
#define SPI_PORT spi0
#define PIN_MISO 16 // GP16
#define PIN_CS 17   // GP17
#define PIN_SCK 18  // GP18
#define PIN_MOSI 19 // GP19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
// UART defines - MUDEI PARA OUTROS PINOS
#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 8 // GP8 (era GP4)
#define UART_RX_PIN 9 // GP9 (era GP5)

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART

    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");

    //Daqui para baixo já estava..

    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário
    sleep_ms(5000);
    // Inicialização do i2c
    i2c_init(i2c0, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    printf("Testando 1 \n"); 

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();
    printf("Testando 2 \n"); 
    
     // Preparar área de renderização para o display completo
    struct render_area full_screen_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&full_screen_area);

    // Preenche o buffer com 0xFF para acender TODOS os pixels
    uint8_t test_ssd_buffer[ssd1306_buffer_length];
    memset(test_ssd_buffer, 0x7F, ssd1306_buffer_length); // Preenche com branco (pixels acesos)

    printf("Testando: Tentando acender todos os pixels...\n");
    render_on_display(test_ssd_buffer, &full_screen_area);
    sleep_ms(3000); // Espera 3 segundos para você ver se o display acende

    
    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    printf("Testando 3 \n"); 

    while(true) {
        printf("Testando\n"); 
        sleep_ms(1000);
    }

    return 0;
}
