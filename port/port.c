 /*
  * FreeModbus Libary: RT-Thread Port
  * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * File: $Id: port.c,v 1.60 2015/02/01 9:18:05 Armink $
  */

/* ----------------------- System includes --------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
/* ----------------------- Variables ----------------------------------------*/
static struct rt_semaphore lock;
static int is_inited = 0;
/* ----------------------- Start implementation -----------------------------*/
void EnterCriticalSection(void)
{
    rt_err_t err;
    if(!is_inited)
    {
        err = rt_sem_init(&lock, "fmb_lock", 1, RT_IPC_FLAG_PRIO);
        if(err != RT_EOK)
        {
            rt_kprintf("Freemodbus Critical init failed!\n");
        }
        is_inited = 1;
    }
    rt_sem_take(&lock, RT_WAITING_FOREVER);
}

void ExitCriticalSection(void)
{
    rt_sem_release(&lock);
}

