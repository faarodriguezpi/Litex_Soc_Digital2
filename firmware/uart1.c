#include "uart1.h"
#include <irq.h>
#include <generated/csr.h>

#ifdef CSR_UART1_BASE

/*
 * Buffer sizes must be a power of 2 so that modulos can be computed
 * with logical AND.
 */

//#define UART_POLLING

#ifndef UART1_POLLING

#define UART1_RINGBUFFER_SIZE_RX 128
#define UART1_RINGBUFFER_MASK_RX (UART1_RINGBUFFER_SIZE_RX-1)

static char rx_buf[UART1_RINGBUFFER_SIZE_RX];
static volatile unsigned int rx_produce;
static unsigned int rx_consume;

#define UART1_RINGBUFFER_SIZE_TX 128
#define UART1_RINGBUFFER_MASK_TX (UART1_RINGBUFFER_SIZE_TX-1)

static char tx_buf[UART1_RINGBUFFER_SIZE_TX];
static unsigned int tx_produce;
static volatile unsigned int tx_consume;

void uart1_isr(void)
{
	unsigned int stat, rx_produce_next;

	stat = uart1_ev_pending_read();

	if(stat & UART1_EV_RX) {
		while(!uart1_rxempty_read()) {
			rx_produce_next = (rx_produce + 1) & UART1_RINGBUFFER_MASK_RX;
			if(rx_produce_next != rx_consume) {
				rx_buf[rx_produce] = uart1_rxtx_read();
				rx_produce = rx_produce_next;
			}
			uart1_ev_pending_write(UART1_EV_RX);
		}
	}

	if(stat & UART1_EV_TX) {
		uart1_ev_pending_write(UART1_EV_TX);
		while((tx_consume != tx_produce) && !uart1_txfull_read()) {
			uart1_rxtx_write(tx_buf[tx_consume]);
			tx_consume = (tx_consume + 1) & UART1_RINGBUFFER_MASK_TX;
		}
	}
}

/* Do not use in interrupt handlers! */
char uart1_read(void)
{
	char c;

	if(irq_getie()) {
		while(rx_consume == rx_produce);
	} else if (rx_consume == rx_produce) {
		return 0;
	}

	c = rx_buf[rx_consume];
	rx_consume = (rx_consume + 1) & UART1_RINGBUFFER_MASK_RX;
	return c;
}

int uart1_read_nonblock(void)
{
	return (rx_consume != rx_produce);
}

void uart1_write(char c)
{
	unsigned int oldmask;
	unsigned int tx_produce_next = (tx_produce + 1) & UART1_RINGBUFFER_MASK_TX;

	if(irq_getie()) {
		while(tx_produce_next == tx_consume);
	} else if(tx_produce_next == tx_consume) {
		return;
	}

	oldmask = irq_getmask();
	irq_setmask(oldmask & ~(1 << UART1_INTERRUPT));
	if((tx_consume != tx_produce) || uart1_txfull_read()) {
		tx_buf[tx_produce] = c;
		tx_produce = tx_produce_next;
	} else {
		uart1_rxtx_write(c);
	}
	irq_setmask(oldmask);
}

void uart1_init(void)
{
	rx_produce = 0;
	rx_consume = 0;

	tx_produce = 0;
	tx_consume = 0;

	uart1_ev_pending_write(uart1_ev_pending_read());
	uart1_ev_enable_write(UART1_EV_TX | UART1_EV_RX);
	irq_setmask(irq_getmask() | (1 << UART1_INTERRUPT));
}

void uart1_sync(void)
{
	while(tx_consume != tx_produce);
}

#else

void uart_isr(void)
{
}

char uart1_read(void)
{
	char c;
	while (uart1_rxempty_read());
	c = uart1_rxtx_read();
	uart1_ev_pending_write(UART1_EV_RX);
	return c;
}

int uart1_read_nonblock(void)
{
	return (uart1_rxempty_read() == 0);
}

void uart1_write(char c)
{
	while (uart1_txfull_read());
	uart1_rxtx_write(c);
	uart1_ev_pending_write(UART1_EV_TX);
}

void uart1_init(void)
{
	uart1_ev_pending_write(uart1_ev_pending_read());
	uart1_ev_enable_write(UART1_EV_TX | UART1_EV_RX);
}

void uart1_sync(void)
{
	while (uart1_txfull_read());
}

#endif

#endif
