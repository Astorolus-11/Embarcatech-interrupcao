#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
//Bibliotecas para controlar a matriz de leds
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

#include "matriz_leds.pio.h" // arquivo .pio

//Definições dos pinos e tempo 
const uint led_vermelho = 13;
const uint intervalo_led_vermelho = 100; 
const uint botao_a = 5;
const uint botao_b = 6;


//Quantidade de pixels/leds
#define pixels 25

//Pino da matriz de leds
#define pin_matrix 7

// Protótipos das funções
void setup(); 


int main() // Função principal
{   PIO pio=pio0;
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r=0.0,g=0.0,b=0.0; // Variáveis para calcular a intensidade dos leds
    
    ok=set_sys_clock_khz(128000,false);// definindo o clock do sistema
    setup();
    stdio_init_all();
    printf("Iniciando a transmissao PIO:\n");
    if(ok){ // Se o clock de 128000 foi habilitado com sucesso
        printf("Clock definido: %ld\n",clock_get_hz(clk_sys));
    }

    //CONFIGURAÇÂO DA PIO
    uint offset = pio_add_program(pio,&matriz_leds_program); // local do programa
    uint sm = pio_claim_unused_sm(pio,true); // Utiliza uma máquina de estado que não está ocupada 
    matriz_leds_program_init(pio, sm, offset, pin_matrix);



    while (true) {
        // A cada 200ms o led vermelho pisca, 1000ms = 1s, 200ms*5= = 1000s, logo piscará 5 vezes por segundo
        gpio_put(led_vermelho,true);
        sleep_ms(intervalo_led_vermelho); // LED ligado por 100ms 
        gpio_put(led_vermelho,false);
        sleep_ms(intervalo_led_vermelho); //LED desligado por 100ms
        
    }
}

void setup(){ // Função para iniciar os pinos
    //Inicialização dos pinos
    gpio_init(led_vermelho);
    gpio_init(botao_a);
    gpio_init(botao_b);
    //Configuração entrada/saida
    gpio_set_dir(led_vermelho,GPIO_OUT);
    gpio_set_dir(botao_a,GPIO_IN);
    gpio_set_dir(botao_b,GPIO_IN);
    //Desligar o led vermelho inicialmente
    gpio_put(led_vermelho,false);
    //Habilitar o pull up para os botões
    gpio_pull_up(botao_a);
    gpio_pull_up(botao_b);


}
