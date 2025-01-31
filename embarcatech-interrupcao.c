#include <stdio.h>
#include "pico/stdlib.h"

const uint led_vermelho = 13;
const uint intervalo_led_vermelho = 100; 

// Protótipos das funções
void setup(); 


int main() // Função principal
{   
    setup();
    stdio_init_all();

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
    //Configuração entrada/saida
    gpio_set_dir(led_vermelho,GPIO_OUT);
    //Desligar os pinos inicialmente
    gpio_put(led_vermelho,false);


}
