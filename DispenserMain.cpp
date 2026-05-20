#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <stdio.h>

#define SERVO_PIN 2
#define BUTTON_PIN 9
#define LDR_PIN 27
#define LDR_ADC 1              // GPIO27 = ADC1
#define LDR_THRESHOLD 1500

bool lastButtonState = false;

// ----- Servo PWM setup -----
void servo_init(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(pin);

    // 50Hz PWM for servo
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f); // 1 MHz clock
    pwm_config_set_wrap(&config, 20000);    // 20 ms period

    pwm_init(slice_num, &config, true);
}

// Set servo angle
void servo_write(uint pin, float angle) {
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Convert angle to pulse width (500us - 2500us)
    float pulse_us = 500 + (angle / 180.0f) * 2000;

    pwm_set_gpio_level(pin, (uint16_t)pulse_us);
}


void dispense_brik() {

    uint16_t ldrValue = adc_read();

    printf("Starter dispenser - LDR vaerdi: %d\n", ldrValue);

    if (ldrValue < LDR_THRESHOLD) {

        printf("OK! Skubber brik\n");

        servo_write(SERVO_PIN, 155);
        sleep_ms(1000);

        servo_write(SERVO_PIN, 0);

    } else {

        printf("FEJL: Der mangler brikker!\n");
    }
}


int main() {
    stdio_init_all();

    // Button setup
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // ADC setup
    adc_init();
    adc_gpio_init(LDR_PIN);
    adc_select_input(LDR_ADC);

    // Servo setup
    servo_init(SERVO_PIN);
    servo_write(SERVO_PIN, 0);

    while (true) {
        bool buttonState = gpio_get(BUTTON_PIN);

        // Rising edge detection
        if (buttonState && !lastButtonState) 
        {
            printf("Knap trykket\n");
            dispense_brik();
            
        }

        lastButtonState = buttonState;


        int c = getchar_timeout_us(0);

        if (c != PICO_ERROR_TIMEOUT) {

            // If laptop sends character '2'
            if (c == '2') {

                printf("Kommando modtaget fra laptop\n");
                dispense_brik();
            }
          }
        sleep_ms(10);
    }
}
