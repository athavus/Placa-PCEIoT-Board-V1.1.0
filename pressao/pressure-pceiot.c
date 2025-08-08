#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "ms5637/ms5637.h"

// Cálculo da pressão normalizada ao nível do mar usando altitude conhecida
// Aplicação da fórmula barométrica: P_mar = P_local / (1 - h/44330)^5.255
float calculate_normalized_pressure(float local_pressure, float elevation) {
    const float standard_gradient = 44330.0f;
    const float barometric_exponent = 5.255f;
    float ratio = 1.0f - (elevation / standard_gradient);
    return local_pressure / powf(ratio, barometric_exponent);
}

int main() {
    stdio_init_all();
    
    // Configuração inicial do sensor barométrico
    barometric_sensor_setup();
    
    // Altitude de referência para Campina Grande, Paraíba
    const float reference_elevation = 555.0f;
    
    printf("=== Sistema de Monitoramento Barométrico ===\n");
    printf("Localização: Campina Grande, PB (%.0fm)\n\n", reference_elevation);
    
    while (true) {
        float pressure_mbar;
        
        sensor_result_t result = get_barometric_readings(&pressure_mbar);
        
        if (result == SENSOR_SUCCESS) {
            float normalized_pressure = calculate_normalized_pressure(pressure_mbar, reference_elevation);
            
            printf("P.Mar: %6.2f mbar | P.Local: %6.2f mbar\n",
                   normalized_pressure, pressure_mbar);
        } else {
            printf("ERRO: Falha na comunicação com o sensor\n");
        }
        
        sleep_ms(1000);
    }
    
    return 0;
}