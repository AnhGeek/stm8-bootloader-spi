#include "spi.h"
/*
 * SPI pinout:
 * SCK  -> PC5
 * MOSI -> PC6
 * MISO -> PC7
 * CS   -> PC3
 * CE   -> PC4
 */

void SPI_init() {
    /* Initialize CS pin, default High */
    PC_DDR |= (1 << CS_PIN);
    PC_CR1 |= (1 << CS_PIN);
    PC_ODR |= (1 << CS_PIN);

    // CE pin, default is Low
    PC_DDR |= (1 << CE_PIN);
    PC_CR1 |= (1 << CE_PIN);
    PC_ODR &= ~(1 << CE_PIN);

    /* Initialize SPI master at 500kHz  */
    /* Set master mode */ 
    SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI);
    SPI_CR1 = (1 << SPI_CR1_MSTR) | (1 << SPI_CR1_SPE) | (1 << SPI_CR1_BR0);
}

// Bidirectional SPI transfer function
uint8_t SPI_write(uint8_t data) {
    while (!(SPI_SR & (1 << SPI_SR_TXE)));
    SPI_DR = data;
    while (!(SPI_SR & (1 << SPI_SR_RXNE)));
    return SPI_DR;
}

uint8_t SPI_read(uint8_t data) {
    SPI_write(data);
    while (!(SPI_SR & (1 << SPI_SR_RXNE)));
    return SPI_DR;
}

void chip_select() {
    PC_ODR &= ~(1 << CS_PIN);
}

void chip_deselect() {
    while ((SPI_SR & (1 << SPI_SR_BSY)));
    PC_ODR |= (1 << CS_PIN);
}

void ce_low() {
    PC_ODR &= ~(1 << CE_PIN);
}

void ce_high() {
    PC_ODR |= (1 << CE_PIN);
}


