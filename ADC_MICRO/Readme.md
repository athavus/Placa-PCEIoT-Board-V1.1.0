
# 📟 mic_adc - Biblioteca de Leitura de Áudio via ADC

Biblioteca C para leitura de sinais analógicos de um microfone (ou sensor de áudio) usando o ADC da Placa **PCEIoT-Board-V1.1.0**, baseada no Raspberry Pi Pico W (RP2040). Também pode ser usada para leituras de bateria.


## 🧩 Estrutura do Projeto

```
PCEIoT_ADC_MICRO/
│
├── inc/
│   ├── mic_adc.h          # Header principal da biblioteca
│   └── mic_adc.c          # Implementação da biblioteca
│
├── tests/
│   ├── tests_mic_adc.h    # Header dos testes
│   └── tests_mic_adc.c    # Implementação dos testes
│
├── ADC_MICRO.c            # Aplicação principal de exemplo
└── README.md              # (Este arquivo)
```

---

## ⚙️ Requisitos

- Placa **PCEIoT-Board-V1.1.0** com **RP2040 (Pico W)**
- SDK do Raspberry Pi Pico (Pico SDK)
- GCC para ARM (arm-none-eabi)
- Make ou CMake

---

## 🚀 Inicialização

### Função

```c
bool mic_adc_init();
```

Inicializa o ADC na **GPIO 27** (conectada ao microfone).  
Retorna `true` se a inicialização for bem-sucedida.

---

## 📈 Funções de Leitura

### Leitura Bruta

```c
uint16_t mic_adc_read_raw();
```

Lê o valor bruto de 12 bits do ADC (0–4095).

---

### Conversão para milivolts

```c
float mic_adc_raw_to_mv(uint16_t raw_value);
```

Converte um valor bruto do ADC para milivolts (com base em 3.3V de referência).

---

### Leitura Média

```c
float mic_adc_read_avg_mv(uint32_t sample_count, uint32_t delay_ms);
```

Realiza múltiplas amostragens e retorna a média em milivolts.

---

### Pico de Amplitude

```c
float mic_adc_read_peak_mv(uint32_t duration_ms, uint32_t sample_interval_ms);
```

Lê o pico de sinal (valor máximo) durante um intervalo de tempo.

---

### Limiar de Disparo (Threshold)

```c
void mic_adc_set_threshold_mv(float threshold_mv);
bool mic_adc_check_threshold_exceeded();
```

Define e verifica um limiar de disparo em milivolts.  
Útil para detectar eventos sonoros, como palmas.

---

### Leitura em Buffer

```c
void mic_adc_read_buffer(uint16_t *buffer, uint32_t buffer_size, uint32_t sample_delay_us);
```

Coleta um conjunto de amostras em um buffer fornecido.

---

## 🧪 Testes

A biblioteca acompanha testes automatizados para verificar as funcionalidades.

### Teste Único

```c
void run_mic_adc_test();
```

Executa testes de:

- Leitura básica
- Média
- Pico de amplitude
- Threshold
- Buffer

Incluído no arquivo: `tests/tests_mic_adc.c`

---

## 💻 Exemplo de Uso

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/mic_adc.h"
#include "tests/tests_mic_adc.h"

int main() {
    stdio_init_all();
    printf("Iniciando aplicacao principal...\n");

    run_mic_adc_test(); // Executa todos os testes

    while (true) {
        // Código principal (ex: detectar palmas)
        sleep_ms(1000);
    }

    return 0;
}
```

---

## 🔧 Compilação

Adicione os arquivos `.c` ao seu `CMakeLists.txt`:

```cmake
add_executable(adc_micro
    ADC_MICRO.c
    inc/mic_adc.c
    tests/tests_mic_adc.c
)

target_link_libraries(adc_micro pico_stdlib hardware_adc)
pico_add_extra_outputs(adc_micro)
```

---

