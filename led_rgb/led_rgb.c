#include "pico/stdlib.h"

// Função para atualizar os estados dos LEDs
void set_led_color(uint red_pin, uint green_pin, uint blue_pin, bool R, bool G, bool B) {
    gpio_put(red_pin, R);   // Configura o estado do LED vermelho
    gpio_put(green_pin, G); // Configura o estado do LED verde
    gpio_put(blue_pin, B);  // Configura o estado do LED azul
}

int main() {
    // Configuração dos pinos GPIO
    const uint red_pin = 13;   // Pino para o LED vermelho
    const uint green_pin = 11; // Pino para o LED verde
    const uint blue_pin = 12;  // Pino para o LED azul

    gpio_init(red_pin);
    gpio_init(green_pin);
    gpio_init(blue_pin);

    gpio_set_dir(red_pin, GPIO_OUT);
    gpio_set_dir(green_pin, GPIO_OUT);
    gpio_set_dir(blue_pin, GPIO_OUT);

    // Variáveis binárias para os estados dos LEDs - começa com todos apagados
    bool R = 0; // Vermelho
    bool G = 0; // Verde
    bool B = 0; // Azul 

    while (true) {
        // Atualiza os LEDs conforme os estados de R, G e B
        set_led_color(red_pin, green_pin, blue_pin, R, G, B);

        // Inverte os estados conforme o padrão
        B = !B;         // Inverte B a cada ciclo
        if (B == 0) {   // Se B voltar a 0, inverte G
            G = !G;
        }
        if (B == 0 && G == 0) { // Se B e G voltarem a 0, inverte R
            R = !R;
        }

        sleep_ms(500); // Aguarda 500 ms antes do próximo ciclo
    }

    return 0;
}

