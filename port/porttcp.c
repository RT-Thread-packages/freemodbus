/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2019 flybreak <guozhanxin@rt-thread.com>
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
 * File: $Id: portserial.c,v 1.60 2019/07/11 17:04:32 flybreak $
 */

#include "port.h"

#ifdef PKG_MODBUS_SLAVE_TCP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "tcpserver.h"

/* ----------------------- Defines  -----------------------------------------*/
#define MB_TCP_DEFAULT_PORT     502
#define MB_TCP_BUF_SIZE     ( 256 + 7 )

/* ----------------------- Static variables ---------------------------------*/
static tcpclient_t mb_client;
static UCHAR    prvvTCPBuf[MB_TCP_BUF_SIZE];
static USHORT   prvvTCPLength;

static void tcpserver_event_notify(tcpclient_t client, rt_uint8_t event)
{
    switch (event)
    {
    case TCPSERVER_EVENT_CONNECT:
        if (mb_client == RT_NULL)
            mb_client = client;
        else
            rt_kprintf("Multi-host is not supported, please disconnect the current host first!");
        break;
    case TCPSERVER_EVENT_RECV:
        prvvTCPLength = tcpserver_recv(mb_client, &prvvTCPBuf, MB_TCP_BUF_SIZE, 100);
        if (prvvTCPLength)
        {
            xMBPortEventPost(EV_FRAME_RECEIVED);
        }
        break;
    case TCPSERVER_EVENT_DISCONNECT:
        mb_client = RT_NULL;
        break;
    default:
        break;
    }
}

BOOL
xMBTCPPortInit(USHORT usTCPPort)
{
    struct tcpserver *serv;
    
    if (usTCPPort == 0)
        usTCPPort = MB_TCP_DEFAULT_PORT;
    
    serv = tcpserver_create(0, usTCPPort);

    tcpserver_set_notify_callback(serv, tcpserver_event_notify);

    return TRUE;
}

void
vMBTCPPortClose(void)
{
    tcpserver_destroy(mb_client->server);
}

void
vMBTCPPortDisable(void)
{
    tcpserver_close(mb_client);
}

BOOL
xMBTCPPortGetRequest(UCHAR **ppucMBTCPFrame, USHORT *usTCPLength)
{
    *ppucMBTCPFrame = &prvvTCPBuf[0];
    *usTCPLength = prvvTCPLength;

    return TRUE;
}

BOOL
xMBTCPPortSendResponse(const UCHAR *pucMBTCPFrame, USHORT usTCPLength)
{
    rt_int16_t ret;
    BOOL bFrameSent = FALSE;

    if (mb_client)
    {
        ret = tcpserver_send(mb_client, (void *)pucMBTCPFrame, usTCPLength, 0);
        if (ret == usTCPLength)
            bFrameSent = TRUE;
    }
    return bFrameSent;
}
#endif
