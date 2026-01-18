#include "bsp_gps.h"
#include "main.h" // huart1 and hdma_usart1_rx are defined here

extern UART_HandleTypeDef huart1; // GPS UART Handle (matches CubeMX name)
extern DMA_HandleTypeDef hdma_usart1_rx;

// Main buffer written by the DMA
static uint8_t dma_rx_buffer[GPS_RX_BUFFER_SIZE];

// Read index (where we last read)
static uint32_t read_index = 0;

void bsp_gps_dma_init(void) {
    // 1. Start DMA reception in circular mode
    HAL_UART_Receive_DMA(&huart1, dma_rx_buffer, GPS_RX_BUFFER_SIZE);
    
    // 2. Enable IDLE interrupt manually (notify when reception idles)
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

// This function will be called from a C++ task
uint16_t bsp_gps_read_data(uint8_t* buffer, uint16_t max_len) {
    uint16_t bytes_read = 0;
    
    // Where is the DMA buffer currently writing to?
    // Note: CNDTR counts down (Size -> 0). Thus (Size - CNDTR) = write index.
    uint32_t write_index = GPS_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
    
    // If there is new data
    while (read_index != write_index && bytes_read < max_len) {
        buffer[bytes_read] = dma_rx_buffer[read_index];
        read_index++;
        bytes_read++;
        
        // Circular wrap-around
        if (read_index >= GPS_RX_BUFFER_SIZE) {
            read_index = 0;
        }
    }
    
    return bytes_read;
}

// Interrupt handler: only to clear the IDLE flag (or to notify a task)
// This function should be called from USART1_IRQHandler in stm32f0xx_it.c
// or you can use callbacks like HAL_UART_RxCpltCallback, but IDLE handling is special.
void HAL_UART_IDLE_Callback_Custom() {
    // Clear IDLE flag
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    
    // Optional: Give a semaphore to wake an RTOS task here ("data arrived").
    // xSemaphoreGiveFromISR(...)
}