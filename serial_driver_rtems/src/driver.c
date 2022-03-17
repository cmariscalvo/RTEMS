/*
 * Serial Driver functions. This file belongs to the Serial Driver project.
 * Copyright (C) 2017-2020 University of Alcalá
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <rtems.h>
#include <bsp.h>
#include <rtems/libio.h>

#include <driver.h>
#include <fifo.h>

typedef struct {

	/** Mapping of the UART's registers */
	volatile LEON3_UART_Regs_Map * regs;

	/** The FIFO buffer that will store the received bytes */
	apbuart_fifo rx_fifo;

	/** The waiting semaphore */
	rtems_id rx_sem;

	/** The IRQ number of the UART */
	int irq;

} apbuart_info;

#define SERIAL_DRIVER_RX_FIFO_SIZE 64

/** Data array to store the received bytes */
static unsigned char rx_buffer[LEON3_APBUARTS][SERIAL_DRIVER_RX_FIFO_SIZE];

static apbuart_info uarts[LEON3_APBUARTS];

/** Number of installed UARTs. */
static int nb_uarts = 0;

/** Flag that indicates that the serial driver has been initialized */
static int _isinit = 0;

#define UART_DEV_NAME "/dev/ttyS0"

rtems_device_driver serial_driver_read (
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg)
{
    rtems_libio_rw_args_t *rw_args;
    unsigned int count = 0, oldLevel;
    unsigned char *buf;
    apbuart_info *uart = &uarts[minor];

    rw_args = (rtems_libio_rw_args_t *) arg;

    buf = (unsigned char *)rw_args->buffer;

    // Protect access to the software FIFO
    rtems_interrupt_disable(oldLevel);

    do
    {
    	/* Read from SW fifo */
    	if (apbuart_fifo_getchar(&uart->rx_fifo,&buf[count]) != 0 )
    	{
    		// Protect access to the software FIFO
    		rtems_interrupt_enable(oldLevel);

    		// TODO: Block thread until a char is received

    		// Protect access to the software FIFO
    		rtems_interrupt_disable(oldLevel);
    		continue;
    	}

        /* Got char from SW FIFO */
        count++;

    } while (count < rw_args->count);

    rtems_interrupt_enable(oldLevel);

    rw_args->bytes_moved = count;

    return RTEMS_SUCCESSFUL;
}

static void serial_driver_interrupt (apbuart_info * uart)
{
    unsigned int status;
    unsigned char c;

    /* Clear any error */
    status = uart->regs->status;
    if (status & (LEON_REG_UART_STATUS_OE | LEON_REG_UART_STATUS_PE | LEON_REG_UART_STATUS_FE))
    {
        uart->regs->status = status & ~(LEON_REG_UART_STATUS_OE | LEON_REG_UART_STATUS_PE | LEON_REG_UART_STATUS_FE);
    }

    /* Empty RX fifo into software fifo */
    while (uart->regs->status & LEON_REG_UART_STATUS_DR)
    {
        c = uart->regs->data;
        if (apbuart_fifo_is_full(&uart->rx_fifo))
        {
            break;
        }
        /* put into fifo */
        apbuart_fifo_putchar(&uart->rx_fifo,c);
    }

    // TODO: Wake up any waiting task

}

static void serial_driver_interrupt_handler (rtems_vector_number v)
{
	int minor;

	/* We have to check for the minor number corresponding to the vector */
	for(minor = 0; minor < nb_uarts; minor++)
	{
		if ( v == (uarts[minor].irq+0x10) )
		{
			serial_driver_interrupt(&uarts[minor]);
            return;
        }
    }
}


rtems_device_driver serial_driver_initialize (
			rtems_device_major_number major,
			rtems_device_minor_number dummy,
			void *arg)
{
    int minor;
    amba_apb_device apbuarts[LEON3_APBUARTS];
    rtems_status_code status;
    char fs_name[11];

    minor = 0;
    nb_uarts = 0;

    strcpy(fs_name, UART_DEV_NAME);

    /* if not already initialized */
    if(_isinit == 0)
    {

        /* find number of UART devices */
        nb_uarts = amba_find_apbslvs(&amba_conf ,
                                     VENDOR_GAISLER ,
                                     GAISLER_APBUART ,
                                     apbuarts ,
                                     LEON3_APBUARTS);

        /* cycle for each UART device */
        for(minor = 0; minor < nb_uarts; minor++)
        {
            uint32_t aux;

            /* and initialize it */
            uarts[minor].regs = (volatile LEON3_UART_Regs_Map *)apbuarts[minor].start;
            uarts[minor].irq = apbuarts[minor].irq;
            apbuart_fifo_initialize(&uarts[minor].rx_fifo, rx_buffer[minor], SERIAL_DRIVER_RX_FIFO_SIZE);

            /* get the control register */
            aux = uarts[minor].regs->ctrl;

            /* update the name of the device */
            fs_name[10] += minor;
            status = rtems_io_register_name(fs_name, major, minor);

            // If the name could not be registered, we output an
            // error message and exit
            if (RTEMS_SUCCESSFUL != status)
            {
            	return RTEMS_INTERNAL_ERROR;
            }

            /* Setup interrupt handler */
            set_vector(serial_driver_interrupt_handler, uarts[minor].irq + 0x10, 2);

            // TODO: Create a simple binary semaphore with count = 0

            /* only choose the previous parity select, parity enable,
             * flow control and loopback mode */
            aux &= LEON_REG_UART_CTRL_PS | LEON_REG_UART_CTRL_PE |
            	   LEON_REG_UART_CTRL_FL | LEON_REG_UART_CTRL_LB;

            uarts[minor].regs->ctrl |= aux;
            uarts[minor].regs->status = 0;

            /* Everything is deactivated until we open the device */

        }

        /* declared that console is initialized */
        _isinit = 1;

    }
    return RTEMS_SUCCESSFUL;
}

rtems_device_driver serial_driver_close (
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg)
{
    apbuart_info *uart = &uarts[minor];

    uart->regs->ctrl = 0;

    /* State will be reset when open is called again */
    rtems_semaphore_release(uart->rx_sem);

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver serial_driver_write (
		rtems_device_major_number major,
		rtems_device_minor_number minor,
		void *arg)
{
    rtems_libio_rw_args_t *rw_args;
    apbuart_info *uart = &uarts[minor];
    int i = 0;

    rw_args = (rtems_libio_rw_args_t *) arg;

    for (i = 0; i < rw_args->count; i++)
    {
    	/* wait until is possible to write */
    	while((uart->regs->status &
    	   		LEON_REG_UART_STATUS_THE ) == 0);

    	/* then write it */
    	uart->regs->data = (unsigned int) rw_args->buffer[i];
    }
    return RTEMS_SUCCESSFUL;
}

rtems_device_driver serial_driver_open(
		rtems_device_major_number major,
		rtems_device_minor_number minor, void *arg)
{
    apbuart_info *uart;

    if ((minor < 0) || (minor >= nb_uarts)) {
        return RTEMS_INVALID_NAME;
    }

    uart = &uarts[minor];

    uart->regs->ctrl |= LEON_REG_UART_CTRL_RE | LEON_REG_UART_CTRL_RI | LEON_REG_UART_CTRL_TE;

    return RTEMS_SUCCESSFUL;
}


