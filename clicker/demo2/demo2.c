/*
 * Copyright (c) 2016, Imagination Technologies Limited and/or its
 * affiliated group companies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 *  	Demo 2 for RIOT OS Summit 2016.
 *
 */

#include <contiki.h>
#include <contiki-lib.h>
#include <contiki-net.h>
#include <stdio.h>
#include <lib/sensors.h>
#include "dev/common-clicks.h"
#include "dev/leds.h"
#include "sys/clock.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define UDP_SERVER_ADDRESS      "fe80::e05f:77ff:fe03:7a13"
#define UDP_SERVER_PORT         (45032)

/*---------------------------------------------------------------------------*/
PROCESS(demo2, "Demo 2");
AUTOSTART_PROCESSES(&demo2);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo2, ev, data)
{
    PROCESS_EXITHANDLER(goto exit;)
    PROCESS_BEGIN();
    static char buffer[256];
    static int event_ID = 1;
    static struct etimer et;
    uip_ipaddr_t ipaddr;
    static struct uip_udp_conn *client_conn;

    uiplib_ipaddrconv(UDP_SERVER_ADDRESS, &ipaddr);
    client_conn = udp_new(&ipaddr, UIP_HTONS(UDP_SERVER_PORT), NULL);
    udp_bind(client_conn, UIP_HTONS(3001));

    printf("Created a connection with the server ");
    PRINT6ADDR(&client_conn->ripaddr);
    printf(" local/remote port %u/%u\n",
    UIP_HTONS(client_conn->lport), UIP_HTONS(client_conn->rport));

    GPIO_CONFIGURE_AS_INPUT(D, 0);
    GPIO_CONFIGURE_AS_OUTPUT(D, 6);
    GPIO_SET(D, 6);
    static int prev = 0;
    static int val = 0;
    while(1) {
        leds_off(LEDS_ALL);
        prev = val;
        val = GPIO_VALUE(D, 0);
        if (val == 1 && prev == 0) {
            sprintf(buffer, "Motion event %u", event_ID);
            ++event_ID;
            etimer_set(&et, CLOCK_SECOND);
            printf("%s\n", buffer);
            uip_udp_packet_send(client_conn, buffer, strlen(buffer));
            leds_on(LEDS_ALL);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        }
    }
    exit:
    ;
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
