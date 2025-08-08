<p align="center">
  <img src="pressao/assets/VirtusCC.png" alt="Virtus">
</p>


# Bibliotecas para Placa PCEIoT-Board-V1.1.0

Este repositório contém três bibliotecas desenvolvidas para a placa **PCEIoT-Board-V1.1.0**, com o objetivo de facilitar o uso de seus principais periféricos. O projeto foi desenvolvido durante a **Capacitação em Sistemas Embarcados do VIRTUS-CC**, como parte do aprendizado prático em integração de hardware e software embarcado.

## 📦 Bibliotecas incluídas

1. **Display OLED**
   - Biblioteca para controle de display OLED via I2C.
   - Permite exibir textos, gráficos e outras informações úteis no display.
   - Compatível com displays baseados no controlador **SSD1306**.

2. **Sensor de Pressão**
   - Biblioteca para leitura de sensores de pressão atmosférica.
   - Ideal para aplicações como monitoramento climático ou altimetria.
   - Suporte a sensores como **BMP280** ou **BME280**.

3. **Leitura do ADC (Microfone + Bateria)**
   - Biblioteca que realiza a leitura dos sinais analógicos da placa.
   - Captura sinais do microfone embutido e monitora a tensão da bateria.
   - Facilita a conversão e interpretação dos dados via ADC.

## 📚 Como utilizar

1. Clone este repositório:
   ```bash
   git clone https://github.com/aryelson1/pceioboard-libs.git
   ```
2. Importe as bibliotecas desejadas no seu projeto Arduino ou PlatformIO.
3. Utilize os exemplos disponíveis em cada pasta para iniciar rapidamente.

## 🛠 Requisitos

- Placa: **PCEIoT-Board-V1.1.0**
- VsCode com extensão Raspberry Pi Pico
- Bibliotecas adicionais (verifique a pasta de cada lib para dependências)

## 📁 Estrutura do Repositório

```
/display_oled
    └── Biblioteca e exemplos para o display OLED
/SensorPressao
    └── Biblioteca e exemplos para o sensor de pressão
/ADC_MICRO
    └── Biblioteca e exemplos para microfone e bateria
```

## 🧠 Sobre o projeto

Este repositório foi desenvolvido durante a **Capacitação em Sistemas Embarcados** promovida pelo **VIRTUS-CC**, com foco no desenvolvimento de aplicações embarcadas utilizando a placa PCEIoT-Board-V1.1.0.

## 🧩 Contribuindo com o Projeto

Contribuições são sempre bem-vindas! Se você deseja colaborar com o desenvolvimento deste repositório, siga as orientações abaixo:

### ✅ Como contribuir

1. **Fork este repositório**  
   Crie uma cópia do projeto na sua conta GitHub.

2. **Crie uma branch**  
   Use uma branch com um nome descritivo para a sua contribuição:
   ```bash
   git checkout -b feature/nome-da-sua-feature


## Autores

- [Aryelson Gonçalves](https://github.com/aryelson1)  

- [Guilherme Santos](https://github.com/GuilhermexL)  

- [MIGUEL RYAN DANTAS DE FREITAS](https://github.com/athavus)  
