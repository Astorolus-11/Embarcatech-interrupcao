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
//Pino da matriz de leds
#define pin_matrix 7
static volatile uint32_t last_time=0;

//Desenho dos números da matriz de leds:
double numeros[10][25]={
    {0.0, 0.2, 0.2, 0.2, 0.0,  // 0, normal ok
     0.0, 0.2, 0.0, 0.2, 0.0,  // invertido
     0.0, 0.2, 0.0, 0.2, 0.0,  // normal
     0.0, 0.2, 0.0, 0.2, 0.0,  // invertido
     0.0, 0.2, 0.2, 0.2, 0.0},// normal

    {0.0, 0.0, 0.2, 0.0, 0.0, // 1 ok
     0.0, 0.0, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0,
     0.0, 0.0, 0.2, 0.0, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 2 ok
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 3 ok
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.0, 0.2, 0.0, // 4 ok
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 5 ok
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 6 ok
     0.0, 0.0, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 7 ok
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 8 ok
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0},

    {0.0, 0.2, 0.2, 0.2, 0.0, // 9 ok
     0.0, 0.2, 0.0, 0.2, 0.0,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.2, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.2, 0.0}

};


//Quantidade de pixels/leds
#define pixels 25

// Protótipos das funções
void setup(); 
static void gpio_irq_handler(uint gpio, uint32_t events);
//static void gpio_irq_handler_decrement(uint gpio, uint32_t events);
uint32_t intensidade(double b, double r, double g);
static volatile uint a=0;
void desenhar(uint num);

//PIO
static PIO pio;
static uint sm;
static uint offset;
static uint32_t valor_led;



int main() // Função principal
{  
    pio=pio0;
    bool ok;
    
    ok=set_sys_clock_khz(128000,false);// definindo o clock do sistema

    setup();
    stdio_init_all();

    printf("Iniciando a transmissao PIO:\n");
    if(ok){ // Se o clock de 128000 foi habilitado com sucesso
        printf("Clock definido: %ld\n",clock_get_hz(clk_sys));
    }

    //CONFIGURAÇÂO DA PIO --------------------------------------------------------------------------------------------------
    offset = pio_add_program(pio,&matriz_leds_program); 
    sm = pio_claim_unused_sm(pio,true); // Utiliza uma máquina de estado que não está ocupada 
    matriz_leds_program_init(pio, sm, offset, pin_matrix);
    //-----------------------------------------------------------------------------------------------------------------------

    //Interrupções: 
     gpio_set_irq_enabled_with_callback(botao_a,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);
     gpio_set_irq_enabled_with_callback(botao_b,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);
     desenhar(0); // Inicia já com o zero

//Loop infinito -------------------------------------------------------------------------------------------------------------
    while (true) {
        // A cada 200ms o led vermelho pisca, 1000ms = 1s, 200ms*5= = 1000s, logo piscará 5 vezes por segundo
        gpio_put(led_vermelho,true);
        sleep_ms(intervalo_led_vermelho); // LED ligado por 100ms 
        gpio_put(led_vermelho,false);
        sleep_ms(intervalo_led_vermelho); //LED desligado por 100ms

        
        
    }
}
//---------------------------------------------------------------------------------------------------------------------------

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
uint32_t intensidade(double b, double r, double g){
    unsigned char R,G,B;
    R=r*255;
    G=g*255;
    B=b*255;
    return(G<<24) | (R<<16) | (B<<8); // para cada 8 bits recebe uma cor
}


void gpio_irq_handler(uint gpio, uint32_t events){ // Função para incrementar

    uint32_t tempo_atual=to_us_since_boot(get_absolute_time()); // pega o tempo absoluto desde o inicio do boot,
    // ou seja o tempo desde o inicio do sistema

    if(tempo_atual-last_time>200000){//Botão A incrementa os números com debounce

    if(gpio_get(botao_a)==0){
        if(a<9){ //Se a for até 9
            a++;
        }
        else{
            a=9;
        }
        desenhar(a);

    }
    else if(gpio_get(botao_b)==0){ //Botão B decrementa os números
        if(a>0){
            a--;
            
        }
        else{
            a=0;
        }
        desenhar(a);

    }
    last_time=tempo_atual; //Atualiza o ultimo tempo
    
    }
}
void desenhar(uint num){

for(uint i=0;i<pixels;i++){ // Descobre qual o digito e desenha
            valor_led=intensidade(0.0,numeros[num][24-i],0.0);
            pio_sm_put_blocking(pio,sm,valor_led);
           }
}
    


