/*
 * Serial Driver RTEMS Project
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

#include <rtems_config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

rtems_task Init(rtems_task_argument arg)
{
	int fd;
	char cadena[4];

	fd = open("/dev/ttyS0", O_RDWR);

	read(fd, cadena, 4);
	write(fd, cadena, 4);

	rtems_shutdown_executive(0);

}
