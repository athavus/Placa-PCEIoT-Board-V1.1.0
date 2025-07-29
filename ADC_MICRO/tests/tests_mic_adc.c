#include "inc/mic_adc.h" // Se mic_adc.h estiver em 'inc'
#include "tests_mic_adc.h" // O cabeçalho deste próprio arquivo

#include <stdio.h>
#include "pico/stdlib.h"
#include <time.h> // Para uso futuro, se precisar de tempo real

void run_mic_adc_test() {
    printf("====================================================\n");
    printf("Iniciando Testes Interativos da Biblioteca MIC ADC!\n");
    printf("Por favor, acompanhe as instrucoes no terminal.\n");
    printf("====================================================\n\n");
    sleep_ms(2000); // Pausa inicial para o usuário ler

    // --- Inicialização ---
    printf("--- Etapa 1: Inicializando o ADC do Microfone ---\n");
    printf("Tentando inicializar o ADC na GPIO27...\n");
    sleep_ms(1000); // Pausa para simular processamento

    if (!mic_adc_init()) {
        printf("ERRO: Nao foi possivel inicializar o ADC do microfone!\n");
        printf("Verifique as conexoes e configuracao do hardware.\n");
        return; // Sai do teste se a inicialização falhar
    }
    printf("SUCESSO: ADC do microfone inicializado com sucesso na GPIO27.\n");
    printf("Prossiga para a proxima etapa...\n");
    sleep_ms(2000);

    // --- Teste de Leitura Básica ---
    printf("\n--- Etapa 2: Teste de Leitura Basica do Sinal --- \n");
    printf("Irei realizar 5 leituras rapidas do valor bruto e em mV.\n");
    printf("Apenas observe os valores. Eles devem variar um pouco.\n");
    sleep_ms(2000);

    for (int i = 0; i < 5; ++i) {
        uint16_t raw_value = mic_adc_read_raw();
        float mv_value = mic_adc_raw_to_mv(raw_value);
        printf("  Leitura %d: Bruto = %u, mV = %.2f\n", i + 1, raw_value, mv_value);
        sleep_ms(500); // Pequena pausa entre cada leitura
    }
    printf("Teste de leitura basica concluido. Valores observados.\n");
    sleep_ms(2000);

    // --- Teste de Leitura de Média ---
    printf("\n--- Etapa 3: Teste de Leitura Media (Suavizada) ---\n");
    printf("Vou coletar 10 amostras com um pequeno atraso e calcular a media.\n");
    printf("Isso ajuda a ter um valor mais estavel do sinal de fundo.\n");
    sleep_ms(2000);

    printf("  Coletando amostras para media...\n");
    float avg_mv = mic_adc_read_avg_mv(10, 50); // 10 amostras com 50ms de atraso
    printf("  Media de 10 amostras: %.2f mV\n", avg_mv);
    printf("Teste de media concluido.\n");
    sleep_ms(2000);

    // --- Teste de Leitura de Pico ---
    printf("\n--- Etapa 4: Teste de Leitura de Pico de Amplitude ---\n");
    printf("Agora, farei um monitoramento por 3 segundos para detectar o maior barulho.\n");
    printf("***** POR FAVOR, FACA UM BARULHO ALTO (ex: um som curto) *****\n");
    printf("***** APOS O INICIO DA CONTAGEM REGRESSIVA.              *****\n");
    sleep_ms(3000); // Dê tempo para o usuário se preparar

    printf("  Iniciando monitoramento de pico em 3...\n");
    sleep_ms(1000);
    printf("  2...\n");
    sleep_ms(1000);
    printf("  1...\n");
    sleep_ms(1000);

    float peak_mv = mic_adc_read_peak_mv(3000, 100); // Monitora por 3 segundos, a cada 100ms
    printf("  Monitoramento de pico finalizado.\n");
    printf("  Pico de amplitude detectado: %.2f mV\n", peak_mv);
    printf("Este e o valor maximo de sinal que o microfone captou no periodo.\n");
    sleep_ms(3000);

    // --- Teste de Detecção de Limiar ---
    printf("\n--- Etapa 5: Teste de Deteccao de Limiar (Threshold) ---\n");
    mic_adc_set_threshold_mv(150.0f); // Define um limiar de 150mV de variação
    printf("Configurei um limiar de 150 mV de variacao de sinal.\n");
    printf("***** FACA UM BARULHO ALTO QUANDO SOLICITADO *****\n");
    printf("***** PARA DISPARAR O ALERTA.                     *****\n");
    sleep_ms(3000);

    printf("  Monitorando para disparo do limiar. Faca um barulho...\n");
    bool triggered = false;
    for (int i = 0; i < 15; ++i) { // Tenta por 15 * 500ms = 7.5 segundos
        if (mic_adc_check_threshold_exceeded()) {
            printf("  !!! LIMIAR EXCEDIDO !!! Leitura atual: %.2f mV\n", mic_adc_raw_to_mv(mic_adc_read_raw()));
            triggered = true;
            break;
        }
        printf("  Aguardando disparo... Leitura atual: %.2f mV\n", mic_adc_raw_to_mv(mic_adc_read_raw()));
        sleep_ms(500); // Pausa para nova leitura e verificação
    }
    if (!triggered) {
        printf("  Limiar nao disparado. O barulho pode nao ter sido alto o suficiente ou o limiar esta muito alto.\n");
    }
    printf("Teste de detecao de limiar concluido.\n");
    sleep_ms(3000);

    // --- Teste de Leitura de Buffer ---
    printf("\n--- Etapa 6: Teste de Leitura de Buffer de Amostras ---\n");
    printf("Vou coletar 100 amostras do ADC rapidamente em um buffer.\n");
    printf("Isso e util para analise de audio mais avancada (ex: FFT).\n");
    printf("Nao e necessario fazer barulho agora, apenas observe.\n");
    sleep_ms(3000);

    const uint32_t BUFFER_SIZE = 100;
    uint16_t adc_buffer[BUFFER_SIZE];
    printf("  Coletando %u amostras no buffer...\n", BUFFER_SIZE);
    mic_adc_read_buffer(adc_buffer, BUFFER_SIZE, 1000); // 100 amostras com 1ms de atraso (1kHz)

    printf("  Primeiras 10 amostras do buffer (bruto e mV):\n");
    for (int i = 0; i < 10 && i < BUFFER_SIZE; ++i) {
        printf("    Buffer[%d]: Bruto = %u, mV = %.2f\n", i, adc_buffer[i], mic_adc_raw_to_mv(adc_buffer[i]));
    }
    printf("Leitura de buffer concluida. Amostras armazenadas.\n");
    sleep_ms(3000);


    printf("\n====================================================\n");
    printf("Todos os testes da biblioteca MIC ADC foram concluidos!\n");
    printf("====================================================\n\n");
    sleep_ms(2000); // Pausa final
}