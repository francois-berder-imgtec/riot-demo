/**
 * @file demo1.c
 *
 * @author Imagination Technologies
 *
 * @copyright Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated group
 * companies and/or licensors.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <letmecreate/letmecreate.h>

#define MAXBUF 65536

volatile static bool running = true;
static int sock;
static void cleanup(int signo)
{
    running = false;
    shutdown(sock, 2);
    close(sock);
    spi_select_bus(MIKROBUS_1);
    led_matrix_click_disable();
    spi_select_bus(MIKROBUS_2);
    led_matrix_click_disable();
    spi_release();
}

int main()
{
    int status;
    struct sockaddr_in6 sin6;
    int sin6len;
    char buffer[MAXBUF];
    struct sigaction sa;

    /* setup signal handler */
    sa.sa_handler = &cleanup;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) != 0) {
        fprintf(stderr, "Could not set signal handler\n");
        return -1;
    }

    /* Initialise UDP connection */
    sock = socket(PF_INET6, SOCK_DGRAM,0);
    sin6len = sizeof(struct sockaddr_in6);
    memset(&sin6, 0, sin6len);

    /* just use the first address returned in the structure */
    sin6.sin6_port = htons(45032);
    sin6.sin6_family = AF_INET6;
    sin6.sin6_addr = in6addr_any;
    status = bind(sock, (struct sockaddr *)&sin6, sin6len);
    if(-1 == status)
     perror("bind"), exit(1);
    getsockname(sock, (struct sockaddr *)&sin6, &sin6len);

    /* Initialise led matrix */
    spi_init();
    spi_select_bus(MIKROBUS_1);
    led_matrix_click_enable();
    led_matrix_click_set_intensity(7);
    led_matrix_click_display_number(0);
    spi_select_bus(MIKROBUS_2);
    led_matrix_click_enable();
    led_matrix_click_set_intensity(7);
    led_matrix_click_display_number(0);
    while (running) {
        float temperature = 0.f;
        uint8_t temperature_integral = 0, temperature_decimal = 0;
        recvfrom(sock, buffer, MAXBUF, 0, (struct sockaddr *)&sin6, &sin6len);

        /* extract number */
        sscanf(buffer, "Temperature is %f degree celsius", &temperature);
        temperature_integral = (uint8_t)temperature;
        temperature -= temperature_integral;
        temperature *= 100.f;
        temperature_decimal = (uint8_t)temperature;

        spi_select_bus(MIKROBUS_1);
        led_matrix_click_display_number(temperature_integral);
        spi_select_bus(MIKROBUS_2);
        led_matrix_click_display_number(temperature_decimal);
    }

    return 0;
}
