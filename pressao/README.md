<div align="center">
  <img src="./assets/virtus.png">
</div>

# Driver do Sensor Barométrico MS5637 para Raspberry Pi Pico

[![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico-purple.svg)](https://www.raspberrypi.com/products/raspberry-pi-pico/)
[![Licença](https://img.shields.io/badge/Licença-MIT-green.svg)](./LICENSE)

## Visão Geral

Este repositório contém um driver em C para o sensor de pressão e temperatura **MS5637-02BA03**. O driver foi desenvolvido para a plataforma **Raspberry Pi Pico**, utilizando o **Pico SDK** e comunicação I2C.

O projeto demonstra a integração do sensor, incluindo a leitura de coeficientes de calibração, validação de dados e a aplicação de fórmulas de compensação de temperatura para garantir medições de alta precisão, conforme especificado no datasheet do componente.

## Funcionalidades Principais

- **Comunicação I2C:** Interface robusta configurada para 400 kHz (Fast Mode).
- **Validação de Dados:** Implementação de checksum **CRC-4** para garantir a integridade dos coeficientes de calibração lidos do sensor.
- **Compensação de Temperatura:**
  - Cálculo da compensação de temperatura de **1ª ordem**.
  - Cálculo da compensação de temperatura de **2ª ordem** para medições em temperaturas abaixo de 20°C, aumentando a precisão em ambientes mais frios.
- **Resolução Configurável (OSR):** Suporte a múltiplos níveis para balancear velocidade de conversão e precisão.
- **API Abstrata:** Interface simples para inicialização e leitura dos dados já compensados.

## Configuração de Hardware

### Componentes Necessários
- Raspberry Pi Pico
- Módulo/Breakout com sensor MS5637

### Diagrama de Conexão

O driver utiliza a interface `i2c0` por padrão.

| Pino do MS5637 | Pino do Raspberry Pi Pico | Descrição        |
| :------------- | :------------------------ | :--------------- |
| VIN / VCC      | 3V3 (OUT) (Pino 36)       | Alimentação (3.3V) |
| GND            | GND (Pino 38)             | Terra            |
| SCL            | GP5 / I2C0 SCL (Pino 7)   | Clock I2C        |
| SDA            | GP4 / I2C0 SDA (Pino 6)   | Dados I2C        |

> **Nota:** Recomenda-se adicionar resistores de pull-up nas linhas SDA e SCL. O driver habilita os pull-ups internos por padrão.

## Compilação e Instalação

O projeto utiliza o sistema de build `CMake` e o SDK do Raspberry Pi Pico.

### Pré-requisitos
- Ambiente de desenvolvimento para o Pico SDK configurado.
- Variável de ambiente `PICO_SDK_PATH` definida.

### Passos para Compilação

1.  **Clone o repositório:**
    ```bash
    git clone https://github.com/seu-usuario/pressure-ms5637.git
    cd pressure-ms5637
    ```

2.  **Crie o diretório de build e execute o CMake/Make:**
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

3.  **Carregue o firmware:**
    O processo de compilação gerará o arquivo `pressure-pceiot.uf2` dentro do diretório `build/`. Para carregar:
    a. Conecte o Raspberry Pi Pico ao computador enquanto mantém o botão `BOOTSEL` pressionado.
    b. Arraste e solte o arquivo `.uf2` na unidade de armazenamento que aparece.

## Referência da API

A interface do driver está definida em `ms5637/ms5637.h`.

---

`void barometric_sensor_setup(void);`

-   **Descrição:** Inicializa a interface I2C, reseta o sensor para um estado conhecido e carrega os coeficientes de calibração da PROM, validando-os com o CRC-4.
-   **Contexto:** Deve ser chamada uma única vez na inicialização do sistema.

---

`sensor_result_t get_barometric_readings(float *pressure_output);`

-   **Descrição:** Realiza uma sequência completa de leitura, obtendo os valores brutos de temperatura e pressão, aplicando as compensações e retornando os dados finais.
-   **Parâmetros:**
    -   `float *pressure_output`: Ponteiro para uma variável `float` que será preenchida com o valor da pressão em millibars (mbar).
-   **Retorno:** Um `sensor_result_t` indicando o status da operação (`SENSOR_SUCCESS`, `SENSOR_COMM_ERROR`, etc.).

---

## Aplicação de Exemplo

A aplicação principal (`pressure-pceiot.c`) demonstra o uso do driver. Ela inicializa o sensor, lê os dados em um loop contínuo e os exibe no console serial/USB.

-   **Ajuste de Altitude:** Para calcular a pressão normalizada ao nível do mar (QNH), a altitude local deve ser configurada no arquivo `pressure-pceiot.c`:
    ```c
    // Altere este valor para a altitude da sua cidade em metros
    const float reference_elevation = 555.0f; // Ex: Campina Grande, PB
    ```

-   **Saída no Monitor Serial:**
    Conecte-se à porta serial do Pico (baud rate não é crítico com USB) para visualizar a saída.
    ```
    === Sistema de Monitoramento Barométrico ===
    Localização: Campina Grande, PB (555m)

    P.Mar: 1012.54 mbar | P.Local:  948.32 mbar
    P.Mar: 1012.58 mbar | P.Local:  948.36 mbar
    ...
    ```

## Colaboradores

- [Guilherme Santos](https://www.github.com/GuilhermexL)
- [Miguel Ryan](https://www.github.com/athavus)
- [Aryelson Messias](https://www.github.com/aryelson1)


## Licença

Este projeto é distribuído sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.
