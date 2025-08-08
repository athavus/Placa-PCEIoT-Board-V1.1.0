/**
 * @file demo.c
 * @brief Implementação das funções de demonstração para display OLED
 * @author TechFlow Solutions
 * @version 1.0
 * @date 2025-08-07
 */

#include "lib-display-ssd1306.h"

// ============================================================================
// DEFINIÇÕES LOCAIS DA DEMONSTRAÇÃO
// ============================================================================

/// Número máximo de itens no menu
#define MENU_ITEMS_COUNT        4

/// Velocidade da animação da barra de progresso
#define PROGRESS_ANIMATION_SPEED 2

/// Amplitude da variação dos sensores
#define SENSOR_TEMP_AMPLITUDE    2.0f
#define SENSOR_HUM_AMPLITUDE     5.0f
#define SENSOR_PRESS_AMPLITUDE   10

/// Parâmetros do gráfico
#define GRAPH_ORIGIN_X          10
#define GRAPH_ORIGIN_Y          15
#define GRAPH_WIDTH             110
#define GRAPH_HEIGHT            34
#define GRAPH_SAMPLES           100

/// Parâmetros de animação gráfica
#define CIRCLE_MIN_RADIUS       5
#define CIRCLE_MAX_RADIUS       11
#define GRAPHICS_PATTERN_SIZE   8

// ============================================================================
// VARIÁVEIS LOCAIS ESTÁTICAS
// ============================================================================

/// Itens do menu principal
static const char* menu_items[MENU_ITEMS_COUNT] = {
    "> Configuracoes",
    "> Diagnosticos", 
    "> Sobre",
    "> Sair"
};

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES
// ============================================================================

void draw_inverted_string(oled_device_t *device, uint8_t x, uint8_t y, const char *text) 
{
    if (!device || !text) return;
    
    uint16_t text_width = oled_calculate_text_width(text);
    
    // Desenha fundo branco para simular inversão
    oled_draw_filled_rectangle(device, x-1, y-1, text_width+2, 10, true);
    
    // Usa função de texto destacado da biblioteca
    oled_render_highlighted_text(device, x, y, text);
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DE TELA
// ============================================================================

void draw_splash_screen(oled_device_t *disp, uint32_t frame)
{
    if (!disp) return;
    
    oled_clear_screen(disp);
    
    // Título principal centralizado
    oled_render_text_string(disp, 35, 10, "PICO DEMO");
    
    // Subtítulo
    oled_render_text_string(disp, 25, 22, "Sistema OLED v3.1");
    
    // Cálculo da barra de progresso animada
    uint8_t progress = (frame / PROGRESS_ANIMATION_SPEED) % 100;
    uint8_t progress_width = (progress * 98) / 100; // 98 pixels de largura útil
    
    // Contorno da barra de progresso
    oled_draw_rectangle_outline(disp, 14, 35, 100, 8, true);
    
    // Preenchimento da barra de progresso
    if (progress_width > 0) {
        oled_draw_filled_rectangle(disp, 15, 36, progress_width, 6, true);
    }
    
    // Texto de progresso
    char loading_text[32];
    snprintf(loading_text, sizeof(loading_text), "Carregando... %d%%", progress);
    oled_render_text_string(disp, 25, 50, loading_text);
    
    oled_refresh_screen(disp);
}

void draw_sensors_screen(oled_device_t *disp, uint32_t frame)
{
    if (!disp) return;
    
    oled_clear_screen(disp);
    
    // Cabeçalho com indicador de tela
    oled_render_text_string(disp, 25, 0, "SENSORES [1/5]");
    oled_draw_line_segment(disp, 0, 9, 127, 9, true);
    
    // Simulação de variação nos sensores usando funções trigonométricas
    float temp_variation = sinf(frame * 0.05f) * SENSOR_TEMP_AMPLITUDE;
    float hum_variation = cosf(frame * 0.03f) * SENSOR_HUM_AMPLITUDE;
    int press_variation = (int)(sinf(frame * 0.02f) * SENSOR_PRESS_AMPLITUDE);
    
    // Atualiza valores globais dos sensores
    sensor_temp = 23.5f + temp_variation;
    sensor_humidity = 65.2f + hum_variation;
    sensor_pressure = 1013 + press_variation;
    
    // Formatação dos valores dos sensores
    char temp_str[16], hum_str[16], press_str[16];
    snprintf(temp_str, sizeof(temp_str), "%.1f C", sensor_temp);
    snprintf(hum_str, sizeof(hum_str), "%.1f %%", sensor_humidity);
    snprintf(press_str, sizeof(press_str), "%d hPa", sensor_pressure);
    
    // Exibição dos dados dos sensores
    oled_render_text_string(disp, 5, 16, "TEMP:");
    oled_render_text_string(disp, 85, 16, temp_str);
    
    oled_render_text_string(disp, 5, 28, "UMIDADE:");
    oled_render_text_string(disp, 85, 28, hum_str);
    
    oled_render_text_string(disp, 5, 40, "PRESSAO:");
    oled_render_text_string(disp, 85, 40, press_str);
    
    // Indicador de atividade piscante
    if ((frame / 10) % 2) {
        oled_draw_filled_rectangle(disp, 120, 16, 3, 3, true);
    }
    
    // Barra de status no rodapé
    oled_draw_line_segment(disp, 0, 55, 127, 55, true);
    oled_render_text_string(disp, 5, 57, "WiFi: ON  Bat: 85%");
    
    oled_refresh_screen(disp);
}

void draw_graph_screen(oled_device_t *disp, uint32_t frame)
{
    if (!disp) return;
    
    oled_clear_screen(disp);
    
    // Cabeçalho
    oled_render_text_string(disp, 25, 0, "GRAFICO [2/5]");
    oled_draw_line_segment(disp, 0, 9, 127, 9, true);
    
    // Desenho dos eixos do gráfico
    oled_draw_line_segment(disp, GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y, 
                          GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y + GRAPH_HEIGHT, true);  // Eixo Y
    oled_draw_line_segment(disp, GRAPH_ORIGIN_X, GRAPH_ORIGIN_Y + GRAPH_HEIGHT, 
                          GRAPH_ORIGIN_X + GRAPH_WIDTH, GRAPH_ORIGIN_Y + GRAPH_HEIGHT, true); // Eixo X
    
    // Desenho da curva do gráfico com linhas conectadas
    int prev_y = -1;
    for (int i = 0; i < GRAPH_SAMPLES && i < GRAPH_WIDTH; i++) {
        // Função senoidal para simular dados de temperatura
        float value = sinf((frame + i) * 0.1f) * 15.0f + 30.0f;
        int y = (GRAPH_ORIGIN_Y + GRAPH_HEIGHT) - (int)((value - 15.0f) / 30.0f * GRAPH_HEIGHT);
        
        // Clipping para manter dentro dos limites do gráfico
        if (y >= GRAPH_ORIGIN_Y && y <= GRAPH_ORIGIN_Y + GRAPH_HEIGHT) {
            int x = GRAPH_ORIGIN_X + 1 + i;
            
            // Desenha ponto
            OLED_PIXEL_ON(disp, x, y);
            
            // Conecta com linha se há ponto anterior válido
            if (prev_y != -1 && i > 0) {
                oled_draw_line_segment(disp, x-1, prev_y, x, y, true);
            }
            prev_y = y;
        }
    }
    
    // Labels dos eixos
    oled_render_text_string(disp, 2, GRAPH_ORIGIN_Y, "30");
    oled_render_text_string(disp, 2, GRAPH_ORIGIN_Y + GRAPH_HEIGHT/2, "22");
    oled_render_text_string(disp, 2, GRAPH_ORIGIN_Y + GRAPH_HEIGHT - 8, "15");
    oled_render_text_string(disp, 15, 52, "Tempo (s)");
    
    oled_refresh_screen(disp);
}

void draw_menu_screen(oled_device_t *disp, uint32_t frame)
{
    if (!disp) return;
    
    oled_clear_screen(disp);
    
    // Cabeçalho
    oled_render_text_string(disp, 40, 0, "MENU [3/5]");
    oled_draw_line_segment(disp, 0, 9, 127, 9, true);
    
    // Seleção animada do menu (cicla pelos itens)
    int selected_item = (frame / 30) % MENU_ITEMS_COUNT;
    
    // Renderização dos itens do menu
    for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
        uint8_t item_y = 17 + i * 10;
        
        if (i == selected_item) {
            // Item selecionado com destaque
            oled_render_highlighted_text(disp, 8, item_y, menu_items[i]);
        } else {
            // Item normal
            oled_render_text_string(disp, 8, item_y, menu_items[i]);
        }
    }
    
    oled_refresh_screen(disp);
}

void draw_graphics_demo(oled_device_t *disp, uint32_t frame)
{
    if (!disp) return;
    
    oled_clear_screen(disp);
    
    // Cabeçalho
    oled_render_text_string(disp, 25, 0, "GRAFICOS [4/5]");
    oled_draw_line_segment(disp, 0, 9, 127, 9, true);
    
    // Círculo com raio animado
    int radius = CIRCLE_MIN_RADIUS + (int)(sinf(frame * 0.1f) * (CIRCLE_MAX_RADIUS - CIRCLE_MIN_RADIUS));
    oled_draw_circle_outline(disp, 30, 30, radius, true);
    
    // Retângulo preenchido com contorno
    oled_draw_filled_rectangle(disp, 60, 20, 20, 15, true);
    oled_draw_rectangle_outline(disp, 59, 19, 22, 17, true);
    
    // Linha diagonal animada
    int offset = (frame / 5) % 20;
    oled_draw_line_segment(disp, 90, 15 + offset, 120, 25 + offset, true);
    
    // Círculo preenchido pequeno
    oled_draw_filled_circle(disp, 20, 50, 4, true);
    
    // Padrão de pixels animado
    for (int i = 0; i < GRAPHICS_PATTERN_SIZE; i++) {
        for (int j = 0; j < 4; j++) {
            if ((i + j + frame/10) % 3 == 0) {
                OLED_PIXEL_ON(disp, 60 + i * 2, 45 + j * 2);
            }
        }
    }
    
    // Demonstração de primitivas extras
    // Triângulo usando linhas
    oled_draw_line_segment(disp, 100, 45, 110, 55, true);
    oled_draw_line_segment(disp, 110, 55, 105, 35, true);
    oled_draw_line_segment(disp, 105, 35, 100, 45, true);
    
    oled_refresh_screen(disp);
}

// ============================================================================
// FUNÇÕES DE INICIALIZAÇÃO DE PERIFÉRICOS
// ============================================================================

bool init_spi_interface(void)
{
    // Inicialização do SPI
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    
    printf("SPI inicializado com sucesso\n");
    return true;
}

bool init_i2c_interface(void)
{
    // Inicialização do I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    printf("I2C inicializado com sucesso\n");
    return true;
}

bool init_uart_interface(void)
{
    // Inicialização do UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_puts(UART_ID, "Sistema iniciado - Demo Interativa com biblioteca OLED customizada!\n");
    
    printf("UART inicializado com sucesso\n");
    return true;
}

bool init_oled_display(void)
{
    // Inicialização do display OLED
    if (!oled_initialize_display(&display, I2C_PORT, OLED_I2C_PRIMARY_ADDR)) {
        printf("Erro na inicialização do display OLED!\n");
        return false;
    }
    
    // Configurações iniciais do display
    oled_adjust_brightness(&display, OLED_DEFAULT_BRIGHTNESS);
    oled_set_power_mode(&display, true);
    
    printf("Display OLED inicializado com sucesso\n");
    printf("Usando biblioteca OLED customizada v3.1\n");
    
    return true;
}

bool init_all_peripherals(void)
{
    bool success = true;
    
    success &= init_spi_interface();
    success &= init_i2c_interface();
    success &= init_uart_interface();
    success &= init_oled_display();
    
    if (success) {
        printf("Todos os periféricos inicializados com sucesso!\n");
    } else {
        printf("Erro na inicialização de um ou mais periféricos!\n");
    }
    
    return success;
}