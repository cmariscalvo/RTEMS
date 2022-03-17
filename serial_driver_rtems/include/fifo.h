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

#ifndef MAIN__FIFO_H
#define MAIN__FIFO_H


typedef struct {
    int size; // Size of the memory buffer
    unsigned char * buffer; // Pointer to the memory buffer
    unsigned char * tail; // Tail pointer to the last used position
    unsigned char * head; //
    unsigned char * max; //
    int is_full;
} apbuart_fifo;

void apbuart_fifo_initialize(apbuart_fifo *fifo, unsigned char * buf, int size);
int apbuart_fifo_is_full(apbuart_fifo *fifo);
int apbuart_fifo_is_empty(apbuart_fifo *fifo);
int apbuart_fifo_putchar(apbuart_fifo *fifo, unsigned char c);
int apbuart_fifo_getchar(apbuart_fifo *fifo, unsigned char *c);


#endif // MAIN__FIFO_H
