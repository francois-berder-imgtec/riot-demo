/*
 * Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated
 * group companies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file
 *  	Demo 1 for RIOT OS Summit 2016.
 */

#include <contiki.h>
#include <contiki-lib.h>
#include <contiki-net.h>
#include <stdio.h>
#include "dev/common-clicks.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define TEMP_REG	0x00   /*Temp. register Address*/
#define CONFIG_REG	0x01   /*Config. register Address*/
#define CONFIG_MSB	0x60   /*default value*/
#define CONFIG_LSB	0xE0   /*Conversion rate is set to 8Hz*/

#define UDP_SERVER_ADDRESS      "fe80::e05f:77ff:fe03:7a13"
#define UDP_SERVER_PORT         (45032)

/*---------------------------------------------------------------------------*/
PROCESS(demo1, "Demo 1");
AUTOSTART_PROCESSES (&demo1);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo1, ev, data)
{
    PROCESS_BEGIN();
    static char buffer[256];
    static double temp_c;
    uip_ipaddr_t ipaddr;
    static struct uip_udp_conn *client_conn;

    uiplib_ipaddrconv(UDP_SERVER_ADDRESS, &ipaddr);
    client_conn = udp_new(&ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
    udp_bind(client_conn, UIP_HTONS(0));

    printf("Created a connection with the server ");
    PRINT6ADDR(&client_conn->ripaddr);
    printf(" local/remote port %u/%u\n",
    UIP_HTONS(client_conn->lport), UIP_HTONS(client_conn->rport));

    tmp102_init();
    tmp102_reg_select(CONFIG_REG);
    tmp102_reg_write(CONFIG_MSB, CONFIG_LSB);
    tmp102_reg_select(TEMP_REG);

    while(1) {
        temp_c = tmp102_read_temp();
        sprintf(buffer, "Temperature is %.3f degree celsius", temp_c);
        printf("%s\n", buffer);
        uip_udp_packet_send(client_conn, buffer, strlen(buffer));
          /* Delay for 1s */
          for(i=0;i<=1000;++i) {
          clock_delay_usec(1000);
          }
    }
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
