/**
 * @file main.c
 * @brief Função principal da demonstração OLED
 * @author TechFlow Solutions
 * @version 1.0
 * @date 2025-08-07
 * 
 * Demonstração interativa para display OLED 128x64 usando Raspberry Pi Pico W.
 * Implementa múltiplas telas com animações e efeitos visuais usando biblioteca
 * OLED customizada.
 */

#include "lib-display-ssd1306.h"

// ============================================================================
// DEFINIÇÃO DAS VARIÁVEIS GLOBAIS
// ============================================================================

/// Instância principal do display OLED
oled_device_t display;

/// Tela atualmente sendo exibida
screen_type_t current_screen = SCREEN_SPLASH;

/// Contador de frames para animações e controle de timing
uint32_t frame_counter = 0;

/// Valores simulados dos sensores
float sensor_temp = 23.5f;
float sensor_humidity = 65.2f;
uint16_t sensor_pressure = 1013;

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

int main(void)
{
    // Inicialização básica do sistema
    stdio_init_all();
    
    printf("===========================================\n");
    printf("  Demo OLED - Raspberry Pi Pico W\n");
    printf("  Biblioteca OLED Customizada v3.1\n");
    printf("===========================================\n");
    
    // Inicialização de todos os periféricos
    if (!init_all_peripherals()) {
        printf("ERRO CRÍTICO: Falha na inicialização dos periféricos!\n");
        return -1;
    }
    
    printf("Demo iniciada! Alternando entre telas a cada %d segundos...\n", SCREEN_SWITCH_TIME_S);
    
    // Efeito de fade in inicial
    oled_fade_effect(&display, true, 1000);
    
    // Loop principal da aplicação
    while (true) {
        // Controle de mudança de telas
        if (IS_SCREEN_SWITCH_TIME(frame_counter) && frame_counter > 0) {
            // Avança para próxima tela (inclui tela de gráficos como 5ª tela)
            current_screen = (current_screen + 1) % (SCREEN_COUNT + 1);
            printf("Mudando para tela: %d\n", current_screen);
            
            // Efeito de transição suave entre telas
            oled_fade_effect(&display, false, 200);
            sleep_ms(100);
            oled_fade_effect(&display, true, 200);
        }
        
        // Renderização da tela atual
        switch (current_screen) {
            case SCREEN_SPLASH:
                draw_splash_screen(&display, frame_counter);
                break;
                
            case SCREEN_SENSORS:
                draw_sensors_screen(&display, frame_counter);
                break;
                
            case SCREEN_GRAPH:
                draw_graph_screen(&display, frame_counter);
                break;
                
            case SCREEN_MENU:
                draw_menu_screen(&display, frame_counter);
                break;
                
            case SCREEN_COUNT: // Tela de demonstração gráfica (5ª tela)
                draw_graphics_demo(&display, frame_counter);
                break;
                
            default:
                // Tela de erro - não deveria acontecer
                oled_clear_screen(&display);
                oled_render_text_string(&display, 30, 25, "ERRO: Tela");
                oled_render_text_string(&display, 25, 35, "Desconhecida");
                oled_refresh_screen(&display);
                printf("AVISO: Tela desconhecida detectada!\n");
                break;
        }
        
        // Envio periódico de dados via UART
        if (IS_UART_REPORT_TIME(frame_counter) && frame_counter > 0) {
            char uart_msg[64];
            snprintf(uart_msg, sizeof(uart_msg), 
                    "Frame: %lu, Temp: %.1fC, Hum: %.1f%%, Press: %dhPa\n", 
                    frame_counter, sensor_temp, sensor_humidity, sensor_pressure);
            uart_puts(UART_ID, uart_msg);
        }
        
        // Demonstrações de recursos avançados do display
        if (frame_counter == COLOR_INVERT_TEST_FRAME) {
            printf("Testando inversão de cores...\n");
            oled_toggle_color_inversion(&display, true);
            sleep_ms(1000);
            oled_toggle_color_inversion(&display, false);
            printf("Inversão de cores concluída.\n");
        }
        
        if (frame_counter == ROTATION_TEST_FRAME) {
            printf("Testando rotação de display...\n");
            oled_set_display_rotation(&display, true, false);
            sleep_ms(2000);
            oled_set_display_rotation(&display, false, false);
            printf("Rotação de display concluída.\n");
        }
        
        // Demonstração de controle de brilho gradual
        if (frame_counter == 600) { // Após 60 segundos
            printf("Demonstrando controle de brilho...\n");
            
            // Diminui brilho gradualmente
            for (int brightness = 255; brightness >= 50; brightness -= 20) {
                oled_adjust_brightness(&display, brightness);
                sleep_ms(200);
            }
            
            // Aumenta brilho gradualmente
            for (int brightness = 50; brightness <= 255; brightness += 20) {
                oled_adjust_brightness(&display, brightness);
                sleep_ms(200);
            }
            
            printf("Demonstração de brilho concluída.\n");
        }
        
        // Incrementa contador de frames
        frame_counter++;
        
        // Previne overflow do contador (reinicia após ~49 dias a 10 FPS)
        if (frame_counter == UINT32_MAX - 1) {
            printf("Reiniciando contador de frames...\n");
            frame_counter = 1000; // Evita testes iniciais novamente
        }
        
        // Controle de taxa de quadros
        sleep_ms(DEMO_FRAME_DELAY_MS);
    }
    
    // Código nunca deve chegar aqui, mas incluído para completude
    printf("Encerrando demonstração...\n");
    
    // Desliga display antes de sair
    oled_set_power_mode(&display, false);
    
    return 0;
}