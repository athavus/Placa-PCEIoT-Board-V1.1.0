# Display OLED

<p align="center">
  <img src="images/VirtusCC.png" alt="Virtus">
</p>


## Atividade: Configurando e Usando o Display OLED da Placa-PCEIoT-Board-V1.1.0 com Linguagem C

A seguir, apresentaremos um passo a passo para configurar e programar o Display OLED da Placa-PCEIoT-Board-V1.1.0 utilizando a linguagem C. Vamos criar um programa que escreve uma mensagem no display, explorando as funções básicas de inicialização, configuração e manipulação de texto.

### Objetivo:

Desenvolver um programa que configure o Display OLED da Placa-PCEIoT-Board-V1.1.0 e exiba uma mensagem personalizada. O programa deve inicializar a comunicação com o display, configurar os parâmetros de operação e atualizar a tela com a mensagem desejada.

### Desenvolvimento:

**Passo 1:** Configuração do Ambiente

Antes de iniciar a programação, certifique-se de que o ambiente de desenvolvimento está configurado:

- Conecte a Placa-PCEIoT-Board-V1.1.0 ao computador utilizando um cabo USB.
- Abra o VS Code e carregue o projeto com a estrutura de pastas para compilar e executar código em C na Placa-PCEIoT-Board-V1.1.0.
- Verifique se as bibliotecas necessárias estão instaladas:
  - Certifique-se de incluir os arquivos ssd1306.h, ssd1306_font.h, ssd1306_i2c.h e ssd1306_i2c.c numa pasta separada (de nome “inc”) dentro do projeto.
  - Drivers para comunicação I2C.

**Passo 2:** Entendimento da Configuração do Hardware

- O Display OLED está conectado ao barramento I2C da Placa-PCEIoT-Board-V1.1.0 através dos seguintes pinos:
  - SDA: GPIO6
  - SCL: GPIO7
- O endereço do Display OLED é 0x3C.

**Passo 3:** Escrevendo o Código

- Inicialização do I2C e do Display OLED – Inclua as bibliotecas necessárias e inicialize a comunicação com o display:

```c
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
```

**Passo 4:** Compilação e Execução

- Compile o código no VS Code.
- Carregue o programa na Placa-PCEIoT-Board-V1.1.0.
- Observe a mensagem "Bem-vindos!" e "Embarcatech" no Display OLED.

### Exploração Adicional:

- Modifique a mensagem para exibir seu nome ou outro texto personalizado.
- Experimente mudar a posição do texto no display ajustando os valores de x e y.
- Tente adicionar animações simples, como rolagem de texto ou texto piscando.

### Dica Importante:

Sempre chame a função render_on_display após modificar o buffer do display para garantir que as alterações sejam exibidas.

## Atividade: Desenhando Linhas no Display OLED da Placa-PCEIoT-Board-V1.1.0 com o Algoritmo de Bresenham

Nesta atividade, exploraremos como desenhar linhas no Display OLED da Placa-PCEIoT-Board-V1.1.0 utilizando a linguagem C. Para isso, aplicaremos o algoritmo de Bresenham, amplamente usado em sistemas gráficos por sua eficiência no cálculo de pontos de uma linha reta.

### Objetivo:

- Compreender como um algoritmo pode ser implementado para trabalhar com gráficos baseados em pixels.
- Aprender a manipular pixels no Display OLED.
- Estimular o raciocínio matemático aplicado à programação.

Esta atividade é ideal para introduzir conceitos de gráficos computacionais e controle de hardware gráfico.

### Desenvolvimento:

**Passo 1:** Função do Algoritmo de Bresenham

O algoritmo de Bresenham calcula os pontos de uma linha entre dois pontos cartesianos (x_0, y_0) e (x_1, y_1) e acende os pixels correspondentes no display.

```c
// Função do Algoritmo de Bresenham
void ssd1306_draw_line(uint8_t *ssd, int x_0, int y_0, int x_1, int y_1, bool set) {
    int dx = abs(x_1 - x_0); // Deslocamentos
    int dy = -abs(y_1 - y_0);
    int sx = x_0 < x_1 ? 1 : -1; // Direção de avanço
    int sy = y_0 < y_1 ? 1 : -1;
    int error = dx + dy; // Erro acumulado
    int error_2;

    while (true) {
        ssd1306_set_pixel(ssd, x_0, y_0, set); // Acende pixel no ponto atual
        if (x_0 == x_1 && y_0 == y_1) {
            break; // Verifica se o ponto final foi alcançado
        }

        error_2 = 2 * error; // Ajusta o erro acumulado

        if (error_2 >= dy) {
            error += dy;
            x_0 += sx; // Avança na direção x
        }
        if (error_2 <= dx) {
            error += dx;
            y_0 += sy; // Avança na direção y
        }
    }
}
```

<details>

<summary>Comentários do Código</summary>
  
### Explicando as variáveis:

- dx e dy: Distâncias absolutas em cada eixo, definindo a inclinação da linha.
- sx e sy: Direção de movimento nos eixos x e y, dependendo do ponto inicial e final.
- Erro acumulado (error): Usado para decidir se o próximo ponto avança no eixo principal ou secundário.
- Chamada de ssd1306_set_pixel: Marca cada ponto da linha no buffer do display.
</details>

**Passo 2:** Programa Principal

Este programa utiliza a função de Bresenham para desenhar uma linha no display, conectando os pontos (10, 10) e (100, 50).

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 6;
const uint I2C_SCL = 7;

int main()
{
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

    ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
    render_on_display(ssd, &frame_area);

    while(true) {
        sleep_ms(1000);
    }

    return 0;
}
```

<details>

<summary>Comentários do Código</summary>

### Explicando cada parte:

- Inicialização do I2C:
  - Configura a comunicação I2C no barramento correto (SDA: GPIO14, SCL: GPIO15).
  - Ajusta a velocidade para 400 kHz.
- Inicialização do Display OLED:
  - Usa ssd1306_init para preparar o display.
- Chamada de ssd1306_draw_line:
  - Desenha a linha no buffer usando a função de Bresenham.
- Atualização do Display:
  - Chama render_on_display para exibir os dados do buffer no hardware.

</details>

### Exploração Adicional:

- Desenhe múltiplas linhas para criar figuras geométricas, como triângulos ou retângulos.
- Modifique os pontos de início e fim para explorar diferentes inclinações de linhas.
- Use botões ou um joystick para desenhar linhas interativamente no display.

### Dica Importante:

Lembre-se de sempre chamar a função render_on_display após modificar o buffer para que as alterações sejam exibidas no display.

## Atividade: Exibindo Bitmaps no Display OLED da Placa-PCEIoT-Board-V1.1.0

Nesta atividade, vamos aprender a exibir uma imagem monocromática no Display OLED presente na Placa-PCEIoT-Board-V1.1.0. As imagens serão importadas como bitmaps no formato 128x64, adequado para a resolução do display. Essa atividade é uma excelente oportunidade para entender como gráficos são manipulados em dispositivos embarcados.

Para representar um bitmap monocromático no Display OLED, precisamos traduzir a imagem para uma matriz de bytes. Cada byte da matriz representa uma coluna de 8 pixels (uma "página" vertical) no display, seguindo o modelo de mapeamento do controlador SSD1306. Este formato compacto e eficiente permite que gráficos sejam exibidos diretamente no hardware.

A seguir, apresentaremos um guia detalhado que inclui:

- Como criar ou converter imagens para o formato 128x64 monocromático.
- Como estruturar e integrar a matriz de bytes no código.
- Como exibir o bitmap no Display OLED utilizando linguagem C.

Ao final, você será capaz de importar e exibir imagens personalizadas no display, explorando novas possibilidades para suas aplicações gráficas embarcadas!

### Desenvolvimento:

**Passo 1:** Converter a Imagem para Bitmap

- Use uma ferramenta gráfica (como GIMP ou um conversor online) para criar ou editar uma imagem de 128x64 pixels.
- Exporte a imagem no formato monocromático (1-bit, preto e branco) e salve-a como uma matriz de bytes.
- O resultado será uma matriz de bytes em formato hexadecimal, onde cada bit representa um pixel (1 para ligado, 0 para desligado).

Exemplo de matriz de bitmap (imagem simples):

```c
const uint8_t bitmap_128x64[] = {
    0xFF, 0x81, 0x81, 0xFF,  // Padrão de quadrado (exemplo)
    0x81, 0x81, 0x81, 0xFF,  // Continuação do padrão
    // Adicione mais linhas para completar 128x64
};
```

![Figura](images/bitmap_info.png)

**Passo 2:** Código para Renderizar o Bitmap no Display OLED

- Função para Desenhar o Bitmap – Crie uma função que copie os dados do bitmap para o buffer do display.

```c
void ssd1306_draw_bitmap(ssd1306_t *ssd, const uint8_t *bitmap) {
    // Copia o bitmap para o buffer do display
    for (int i = 0; i < ssd->bufsize - 1; i++) {
        ssd->ram_buffer[i + 1] = bitmap[i]; // O buffer começa no índice 1
    }
    // Atualiza o display com os dados do buffer
    ssd1306_send_data(ssd);
}
```

- Programa Principal – Use a função ssd1306_draw_bitmap para carregar e exibir o bitmap no display.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 6;
const uint I2C_SCL = 7;

int main()
{
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

    const uint8_t bitmap_128x64[] = {
        0xFF, 0x81, 0x81, 0xFF,  // Padrão de quadrado (exemplo)
        0x81, 0x81, 0x81, 0xFF,  // Continuação do padrão
        // Adicione mais linhas para completar 128x64
        };

    ssd1306_t ssd_bm;
    ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);

    while(true) {
        sleep_ms(1000);
    }

    return 0;
}
```

**Passo 3:** Como Testar

- Carregue o Programa: Compile o código e envie para a Placa-PCEIoT-Board-V1.1.0 usando seu ambiente de desenvolvimento.
- Verifique a Imagem: A imagem definida no bitmap_128x64 será exibida no Display OLED.

### Dicas Adicionais:

- Ferramenta para Gerar Bitmaps:
  - Use o GIMP:
    - Crie uma imagem de 128x64 pixels.
    - Exporta como .xbm (C header file). O GIMP gera automaticamente um array de bytes para incluir no código.
- Formato de tamanho:
  - Certifique-se de que o tamanho do array seja exatamente 128x64 / 8 = 1024 bytes.

### Exploração Adicional:

- Combine a exibição de bitmaps com outras funções, como desenhar texto ou linhas, para criar interfaces gráficas dinâmicas.
- Com isso, você terá uma forma eficiente de exibir bitmaps no Display OLED da Placa-PCEIoT-Board-V1.1.0 usando C!
