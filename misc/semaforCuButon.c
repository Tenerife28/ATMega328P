#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

enum {
    ROSU_MASINI,
    VERDE_MASINI,
    GALBEN_MASINI,
    VERDE_PIETONI_BLINK
} stare;

void semaforCuButon() {
    Timer0_Init();
    GPIO_Init(D11, GPIO_OUTPUT); 
    GPIO_Init(D6, GPIO_OUTPUT);  
    GPIO_Init(D2, GPIO_OUTPUT);  
    GPIO_Init(D8, GPIO_INPUT);   
    GPIO_Init(A0, GPIO_OUTPUT);  
    GPIO_Init(A4, GPIO_OUTPUT);  

    uint32_t timp_rosu_m = 10000;
    uint32_t timp_galben_m = 2000;
    uint32_t timp_verde_m = 8000;
    uint32_t timp_blink = 3000;

    uint32_t t0 = Millis();
    uint32_t t_blink = 0;
    stare = VERDE_MASINI;

    GPIO_Write(D2, GPIO_HIGH);
    GPIO_Write(A0, GPIO_HIGH);
    GPIO_Write(D11, GPIO_LOW);
    GPIO_Write(D6, GPIO_LOW);
    GPIO_Write(A4, GPIO_LOW);

    while (1) {
        uint32_t timp_curent = Millis();

        switch (stare) {
            case VERDE_MASINI:
                if (GPIO_Read(D8) && (timp_curent - t0 >= 5000)) {
                    t0 = timp_curent;
                    GPIO_Write(D2, GPIO_LOW);
                    GPIO_Write(D6, GPIO_HIGH);
                    stare = GALBEN_MASINI;
                }
                break;

            case GALBEN_MASINI:
                if (timp_curent - t0 >= timp_galben_m) {
                    t0 = timp_curent;
                    GPIO_Write(D6, GPIO_LOW);
                    GPIO_Write(D11, GPIO_HIGH);
                    GPIO_Write(A0, GPIO_LOW);
                    GPIO_Write(A4, GPIO_HIGH);
                    stare = ROSU_MASINI;
                }
                break;

            case ROSU_MASINI:
                if (timp_curent - t0 >= timp_rosu_m) {
                    t0 = timp_curent;
                    t_blink = timp_curent;
                    stare = VERDE_PIETONI_BLINK;
                }
                break;

            case VERDE_PIETONI_BLINK:
                if (timp_curent - t_blink >= 400) {
                    GPIO_Toggle(A4);
                    t_blink = timp_curent;
                }

                if (timp_curent - t0 >= timp_blink) {
                    t0 = timp_curent;
                    GPIO_Write(A4, GPIO_LOW);
                    GPIO_Write(A0, GPIO_HIGH);
                    GPIO_Write(D11, GPIO_LOW);
                    GPIO_Write(D2, GPIO_HIGH);
                    stare = VERDE_MASINI;
                }
                break;
        }
    }
}

int main(void) {
    semaforCuButon();
    return 0;
}