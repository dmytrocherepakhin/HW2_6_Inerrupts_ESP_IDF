
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>
#include <stdbool.h>
#include "esp_timer.h"

#define BTN_PIN GPIO_NUM_1

// -------------- Завдання 1: Базова реалізація ----------------

// static volatile uint32_t button_counter = 0;

// static void button_isr(void *arg)
// {
//     button_counter++;
// }

// void app_main(void)
// {
//     gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
//     gpio_set_pull_mode(BTN_PIN, GPIO_PULLUP_ONLY);
//     gpio_set_intr_type(BTN_PIN, GPIO_INTR_NEGEDGE);

//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(BTN_PIN, button_isr, NULL);

//     uint32_t last_count = 0;

//     while (1) {
//         uint32_t count = button_counter;

//         if (count != last_count) {
//             last_count = count;
//             printf("Button Pressed! Count: %" PRIu32 "\n", count);
//         }

//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }


//-------------- Завдання 2: Software debounce через таймер ----------------

#define DEBOUNCE_MS 50

volatile bool BtnInterrupt = false;

uint32_t button_counter = 0;
uint32_t LastPressTime = 0;

void button_isr(void *arg)
{
    BtnInterrupt = true;
}

void onButton(void)
{
    const uint32_t now = (uint32_t)(esp_timer_get_time() / 1000);

    if (now - LastPressTime >= DEBOUNCE_MS)
    {
        LastPressTime = now;
        button_counter++;
        printf("Button Pressed! Count: %" PRIu32 "\n", button_counter);
    }
}

void app_main(void)
{
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BTN_PIN, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_PIN, button_isr, NULL);

    printf("Setuped_2\n");

    while (1)
    {
        if (BtnInterrupt)
        {
            BtnInterrupt = false;
            onButton();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


// --------------- Завдання 3: Debounce через перевірку рівня ---------------