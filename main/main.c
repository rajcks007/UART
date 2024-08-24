#include <stdio.h>
#include <string.h>  // For snprintf()
#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_NUM             UART_NUM_0  // Use UART0
#define UART_TX_PIN          1           // GPIO number for UART TX (default pin)
#define UART_RX_PIN          3           // GPIO number for UART RX (default pin)
#define UART_BAUD_RATE       115200      // Baud rate for UART communication

void uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 2048, 0, 0, NULL, 0);
}

void send_data_via_uart(const char *data) {
    size_t len = strlen(data);  // Calculate the length of the string
    uart_write_bytes(UART_NUM, data, len);  // Send the string via UART
}

void app_main(void) {

    uart_init();

    // Example numerical data, including negative values
        int16_t X = -134;
        int16_t Y = 321;

    while (1) {

        X ++ ;
        Y -- ;

        int16_t Z = X ^ Y;

        // Prepare data string
        char data_str[100];
        snprintf(data_str, sizeof(data_str), " %d", Z);

        // Send the data string over UART0
        send_data_via_uart(data_str);

        // Optional: Print the data to the console for debugging
        // ESP_LOGI(TAG, "Sent data: %s", data_str);

        // Delay to control the sending frequency
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second
    }
}
