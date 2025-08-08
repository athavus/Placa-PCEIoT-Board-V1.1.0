/**
 * @file main.h
 * @brief Definições globais e declarações para demo OLED
 * @author TechFlow Solutions
 * @version 1.0
 * @date 2025-08-07
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "ssd1306/ssd1306.h"
#include <math.h>

// ============================================================================
// DEFINIÇÕES DE HARDWARE - SPI
// ============================================================================
#define SPI_PORT        spi0
#define PIN_MISO        16
#define PIN_CS          17
#define PIN_SCK         18
#define PIN_MOSI        19

// ============================================================================
// DEFINIÇÕES DE HARDWARE - I2C
// ============================================================================
#define I2C_PORT        i2c0
#define I2C_SDA         4
#define I2C_SCL         5

// ============================================================================
// DEFINIÇÕES DE HARDWARE - UART
// ============================================================================
#define UART_ID         uart1
#define BAUD_RATE       115200
#define UART_TX_PIN     8
#define UART_RX_PIN     9

// ============================================================================
// CONFIGURAÇÕES DA DEMONSTRAÇÃO
// ============================================================================
#define DEMO_FPS                10          // Taxa de quadros por segundo
#define DEMO_FRAME_DELAY_MS     (1000/DEMO_FPS)  // Delay entre frames em ms
#define SCREEN_SWITCH_TIME_S    5           // Tempo para trocar tela em segundos
#define SCREEN_SWITCH_FRAMES    (SCREEN_SWITCH_TIME_S * DEMO_FPS)  // Frames para trocar
#define UART_REPORT_INTERVAL    100         // Intervalo para envio UART (frames)
#define COLOR_INVERT_TEST_FRAME 200         // Frame para testar inversão
#define ROTATION_TEST_FRAME     400         // Frame para testar rotação

// ============================================================================
// ENUMERAÇÕES DE TIPOS
// ============================================================================

/**
 * @brief Tipos de tela disponíveis na demonstração
 */
typedef enum {
    SCREEN_SPLASH,      ///< Tela inicial com splash screen
    SCREEN_SENSORS,     ///< Tela de dados dos sensores
    SCREEN_GRAPH,       ///< Tela com gráfico em tempo real
    SCREEN_MENU,        ///< Tela de menu interativo
    SCREEN_GRAPHICS,    ///< Tela de demonstração gráfica
    SCREEN_COUNT        ///< Contador total de telas (usado internamente)
} screen_type_t;

// ============================================================================
// VARIÁVEIS GLOBAIS EXTERNAS
// ============================================================================
extern oled_device_t display;              // Instância do display OLED
extern screen_type_t current_screen;       // Tela atual sendo exibida
extern uint32_t frame_counter;             // Contador de frames
extern float sensor_temp;                  // Temperatura simulada do sensor
extern float sensor_humidity;              // Umidade simulada do sensor
extern uint16_t sensor_pressure;           // Pressão simulada do sensor

// ============================================================================
// DECLARAÇÕES DE FUNÇÕES - DEMO
// ============================================================================

/**
 * @brief Função auxiliar para desenhar string com destaque (fundo invertido)
 * 
 * @param device Ponteiro para estrutura do display
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial  
 * @param text Texto a ser renderizado com destaque
 */
void draw_inverted_string(oled_device_t *device, uint8_t x, uint8_t y, const char *text);

/**
 * @brief Desenha tela de splash com animação de carregamento
 * 
 * @param disp Ponteiro para estrutura do display
 * @param frame Número do frame atual para animação
 */
void draw_splash_screen(oled_device_t *disp, uint32_t frame);

/**
 * @brief Desenha tela de sensores com valores dinâmicos
 * 
 * @param disp Ponteiro para estrutura do display
 * @param frame Número do frame atual para animação
 */
void draw_sensors_screen(oled_device_t *disp, uint32_t frame);

/**
 * @brief Desenha tela com gráfico em tempo real
 * 
 * @param disp Ponteiro para estrutura do display
 * @param frame Número do frame atual para animação
 */
void draw_graph_screen(oled_device_t *disp, uint32_t frame);

/**
 * @brief Desenha tela de menu com seleção animada
 * 
 * @param disp Ponteiro para estrutura do display
 * @param frame Número do frame atual para animação
 */
void draw_menu_screen(oled_device_t *disp, uint32_t frame);

/**
 * @brief Desenha tela de demonstração de primitivas gráficas
 * 
 * @param disp Ponteiro para estrutura do display
 * @param frame Número do frame atual para animação
 */
void draw_graphics_demo(oled_device_t *disp, uint32_t frame);

// ============================================================================
// DECLARAÇÕES DE FUNÇÕES - INICIALIZAÇÃO
// ============================================================================

/**
 * @brief Inicializa interface SPI
 * 
 * @return true se inicialização bem-sucedida, false caso contrário
 */
bool init_spi_interface(void);

/**
 * @brief Inicializa interface I2C
 * 
 * @return true se inicialização bem-sucedida, false caso contrário
 */
bool init_i2c_interface(void);

/**
 * @brief Inicializa interface UART
 * 
 * @return true se inicialização bem-sucedida, false caso contrário
 */
bool init_uart_interface(void);

/**
 * @brief Inicializa display OLED
 * 
 * @return true se inicialização bem-sucedida, false caso contrário
 */
bool init_oled_display(void);

/**
 * @brief Inicializa todos os periféricos
 * 
 * @return true se todas inicializações bem-sucedidas, false caso contrário
 */
bool init_all_peripherals(void);

// ============================================================================
// MACROS DE CONVENIÊNCIA
// ============================================================================

/// Converte segundos em número de frames
#define SECONDS_TO_FRAMES(seconds) ((seconds) * DEMO_FPS)

/// Verifica se é hora de trocar de tela
#define IS_SCREEN_SWITCH_TIME(frame) ((frame) % SCREEN_SWITCH_FRAMES == 0)

/// Verifica se é hora de enviar dados por UART
#define IS_UART_REPORT_TIME(frame) ((frame) % UART_REPORT_INTERVAL == 0)

/// Calcula progresso em porcentagem baseado no frame
#define FRAME_TO_PROGRESS_PERCENT(frame, max_frames) (((frame) * 100) / (max_frames))

#endif // MAIN_H