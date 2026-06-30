#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "drivers/logging/logging.h"
#include "drivers/hardware_def.h"

#define SAMPLE_RATE 44100.0f
#define CLK_DIV ((ADC_CLK / SAMPLE_RATE) - 1.0f)

void init_mic()
{
    adc_init();
    adc_gpio_init(MIC_OUT_PIN);
    // two adc inputs are available on the RP2040, ADC0 and ADC1, pick appropriate one
    adc_select_input(MIC_ADC_INPUT);
    adc_set_clkdiv(CLK_DIV);
    adc_fifo_setup(true, false, 1, false, false);
    adc_fifo_drain();

    log(LogLevel::INFORMATION, "MICROPHONE SETUP", "completed initialisation");
}

void read_mic(uint16_t *data, size_t number_of_samples)
{
    adc_run(true);
    for (size_t i = 0; i < number_of_samples; i++)
    {
        data[i] = adc_fifo_get_blocking();
    }
    adc_run(false);
    adc_fifo_drain();
}