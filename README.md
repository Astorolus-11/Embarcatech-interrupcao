# Atividade com a matriz de LEDs WS2812, LED RGB, botões, interrupções e debounce

## Sumário

1. [Propósito da atividade](#1--Propósito)
2. [Vídeo de demonstração](#3--Vídeo--de--demonstração)
3. [Funcionalidades ](#4--Funcionalidades)
4. [Técnicas utilizadas ](#4--Técnicas--utilizadas)
5. [Pré-requisitos para executar](#5--pré-requisitos-para-executar)

## 1. Propósito
O objetivo desse projeto é demonstrar domínio sobre a linguagem C, manipular portas GPIO, tratar debouncing e interrupções. A atividade foi proposta como um meio de avaliação desses conhecimentos.


## 2. Vídeo de demonstração
https://www.dropbox.com/scl/fi/vqa5v6cjlrym9oahamex0/WhatsApp-Video-2025-02-02-at-01.45.31.mp4?rlkey=cq2ydw7n94opav9f7nkf4plop&st=0c0252oj&dl=0

## 3. Funcionalidades
- O LED vermelho do RGB pisca 5 vezes por segundo no loop da função principal;
- O botão A (esquerdo) incrementa o número exibido na matriz de LEDs cada vez que for pressionado;
- O botão B (direito) decrementa o número exibido na matriz de LEDs cada vez que for pressionado;
- Os LEDs WS2812 exibe os números de acordo com o que eu pressiono, os digitos vão de 0 a 9.

## 4. Técnicas utilizadas
- Uso de interrupções: Função utilizada fora do loop para incrementar o digito.
- Debouncing: Tratamento do bouncing dos botões A e B dentro da função de interrupção para evitar leituras equivocadas.
- Controle de LEDs: Controle dos leds através do GPIO 13 do RGB e da matriz WS2812 através de um PIO.

## 5. Pré-requisitos
1. Ter o [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado na máquina;
2. Ter o [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) instalado;
3. Ter o [Visual Studio Code](https://code.visualstudio.com/download) instalado;
4. Ter este repositório clonado na sua máquina;
5. Ter as seguintes extensões instaladas no seu VS Code para o correto funcionamento:
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools);
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake);
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools);
- [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico);

