#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <uart.h>
#include <console.h>
#include <generated/csr.h>

/*
 *     #include "file"
 *  This form is used for header files of your own program. It searches for a file named 'file' in the directory containing the current file. You can prepend directories to this list with the -I option while compiling your source code.
 *
 */

#include "uart1.h"

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("RUNTIME>");
}

static void help(void)
{
	puts("Available commands:");
	puts("help                            - this command");
	puts("reboot                          - reboot CPU");
	puts("display                         - display test");
	puts("led                             - led test");
	puts("uart1                           - uart1 test");
}

static void reboot(void)
{
	ctrl_reset_write(1);
}

/*static void display_test(void)
{
	int i;
	printf("display_test...\n");
	for(i=0; i<6; i++) {
		display_sel_write(i);
		display_value_write(i);
		display_write_write(1);
	}
}

static void led_test(void)
{
	int i;
	printf("led_test...\n");
	for(i=0; i<32; i++) {
		leds_out_write(i);
		busy_wait(1);
	}
}

static void ultrasonido(void)
{
    trigger_out_write(1);
    busy_wait(10);
    trigger_out_write(0);
    busy_wait(2);
    
}*/



static void uart1_test(void)
{   
    uint8_t count = 0;
    for (int w = 0; w < 200; w++){
        char c = 'j';
	    printf("\nSending a char '%c' through uart1 - \n", c);
	    uart1_write(c);
	    puts("\n\tchar sent \n");
	    for(int i = 0; i < 10000; i++) {
	        if(i == 9999) {
	            puts("----------------------------------------------------------------\n");
	        }
	    }
	    puts("\nReceiving a char through uart1 - \n");
	    c = uart1_read();//uart1_read_nonblock();//uart1_read();
	    printf("\n\tchar read: %c \n", c);
	    count += 1;
	    printf("Conteo: %d", count);
	}
}

static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot();
	else if(strcmp(token, "display") == 0)
		puts("\nDisplay - \n");//display_test();
	else if(strcmp(token, "led") == 0)
		puts("\nLEDSsss\n");//led_test();
	else if(strcmp(token, "uart1") == 0)
		uart1_test();
	prompt();
}

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
	uart_init();
	
	uart1_init();

	puts("\n\t\t Lab Digital 2 \n\t - CPU testing software built "__DATE__" "__TIME__"\n");
	help();
	prompt();

	while(1) {
		console_service();
	}

	return 0;
}
