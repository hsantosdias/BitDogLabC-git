#include <stdio.h>        // Biblioteca padrão de entrada e saída
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
#include "hardware/pwm.h" // Biblioteca para controle de PWM no RP2040
#include "pico/stdlib.h"  // Biblioteca padrão do Raspberry Pi Pico

// Definição dos pinos usados para o joystick e LEDs
const int VRX = 26;          // Pino de leitura do eixo X do joystick (conectado ao ADC)
const int VRY = 27;          // Pino de leitura do eixo Y do joystick (conectado ao ADC)
const int ADC_CHANNEL_0 = 0; // Canal ADC para o eixo X do joystick
const int ADC_CHANNEL_1 = 1; // Canal ADC para o eixo Y do joystick
const int SW = 22;           // Pino de leitura do botão do joystick

const int LED_B = 13;                    // Pino para controle do LED azul via PWM
const int LED_R = 11;                    // Pino para controle do LED vermelho via PWM
const float DIVIDER_PWM = 16.0;          // Divisor fracional do clock para o PWM
const uint16_t PERIOD = 4096;            // Período do PWM (valor máximo do contador)
uint16_t led_b_level, led_r_level = 100; // Inicialização dos níveis de PWM para os LEDs
uint slice_led_b, slice_led_r;           // Variáveis para armazenar os slices de PWM correspondentes aos LEDs

// Função para configurar o joystick (pinos de leitura e ADC)
void setup_joystick()
{
  // Inicializa o ADC e os pinos de entrada analógica
  adc_init();         // Inicializa o módulo ADC
  adc_gpio_init(VRX); // Configura o pino VRX (eixo X) para entrada ADC
  adc_gpio_init(VRY); // Configura o pino VRY (eixo Y) para entrada ADC

  // Inicializa o pino do botão do joystick
  gpio_init(SW);             // Inicializa o pino do botão
  gpio_set_dir(SW, GPIO_IN); // Configura o pino do botão como entrada
  gpio_pull_up(SW);          // Ativa o pull-up no pino do botão para evitar flutuações
}

// Função para configurar o PWM de um LED (genérica para azul e vermelho)
void setup_pwm_led(uint led, uint *slice, uint16_t level)
{
  gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
  *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
  pwm_set_clkdiv(*slice, DIVIDER_PWM);   // Define o divisor de clock do PWM
  pwm_set_wrap(*slice, PERIOD);          // Configura o valor máximo do contador (período do PWM)
  pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
  pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

// Função de configuração geral
void setup()
{
  stdio_init_all();                                // Inicializa a porta serial para saída de dados
  setup_joystick();                                // Chama a função de configuração do joystick
  setup_pwm_led(LED_B, &slice_led_b, led_b_level); // Configura o PWM para o LED azul
  setup_pwm_led(LED_R, &slice_led_r, led_r_level); // Configura o PWM para o LED vermelho
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
  // Leitura do valor do eixo X do joystick
  adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)
}

// Função principal
int main()
{
  uint16_t vrx_value, vry_value, sw_value; // Variáveis para armazenar os valores do joystick (eixos X e Y) e botão
  setup();                                 // Chama a função de configuração
  printf("Joystick-PWM\n");                // Exibe uma mensagem inicial via porta serial
  // Loop principal
  while (1)
  {
    joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
    // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
    pwm_set_gpio_level(LED_B, vrx_value); // Ajusta o brilho do LED azul com o valor do eixo X
    pwm_set_gpio_level(LED_R, vry_value); // Ajusta o brilho do LED vermelho com o valor do eixo Y

    // Pequeno delay antes da próxima leitura
    sleep_ms(100); // Espera 100 ms antes de repetir o ciclo
  }
}
