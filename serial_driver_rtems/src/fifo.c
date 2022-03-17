/*
 * FIFO management functions. This file belongs to the Serial Driver project.
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

#include <fifo.h>


void apbuart_fifo_initialize(apbuart_fifo *fifo, unsigned char * buf, int size)
{
	fifo->size = size;
    fifo->buffer = buf + 1;
    fifo->tail = fifo->buffer;
    fifo->head = fifo->buffer;
    fifo->max = &fifo->buffer[size-1];
    fifo->is_full=0;

}

int apbuart_fifo_is_full(apbuart_fifo *fifo)
{
    return fifo->is_full;
}

int apbuart_fifo_is_empty(apbuart_fifo *fifo)
{
    if ((fifo->head == fifo->tail) && !fifo->is_full)
    {
        return -1;
    }
    return 0;
}

int apbuart_fifo_putchar(apbuart_fifo *fifo, unsigned char c)
{
    if (!fifo->is_full){
        *fifo->head = c;
        fifo->head = (fifo->head >= fifo->max ) ? fifo->buffer : fifo->head+1;
        if (fifo->head == fifo->tail)
        {
            fifo->is_full = -1;
        }
        return 0;
    }
    return -1;
}

int apbuart_fifo_getchar(apbuart_fifo *fifo, unsigned char *c)
{
    if (apbuart_fifo_is_empty(fifo))
    {
        return -1;
    }
    if (c)
    {
        *c = *fifo->tail;
    }
    fifo->tail = (fifo->tail >= fifo->max) ? fifo->buffer : fifo->tail+1;
    fifo->is_full = 0;
    return 0;
}


