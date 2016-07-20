# riot-demo
Demos prepared for RIOT summit 2016.

### Demo 1: Transmitting temperature measure over 6lowpan

Hardware:

  * 6lowpan Clicker has a Thermo3 Click
  * Ci40 displays temperature using two led matrices (8x8R Click).

The Clicker measures temperature from sensor every second and send a UDP packet to port 45032. The destination address is hardcoded to fe80::e05f:77ff:fe03:7a13.

Ci40 runs Openwrt and opens a IPv6 socket on port 45032 and is waiting for incoming packets. It uses LetMeCreate library to display temperature (integral part on Mikrobus 1 and 2 decimal digits on Mikrobus 2).


### Demo 2: Sending motion event over 6lowpan


Hardware:

  * 6lowpan Clicker has a Motion Click
  * Ci40 turns on/off a USB powered LED lamp via a Relay Click


The Clicker does not use the driver for the Motion Click because it did not work well with Cascoda-Contiki. Instead, it polls the status of GPIO RD0. When a low-to-high edge is detected, it sends a UDP packet. It also includes an event ID to eliminate duplicate packets.

Ci40 runs Openwrt and opens a IPv6 socket on port 45032 and is waiting for incoming packets. It uses LetMeCreate library to control a Relay Click. When an event is received, it turns on the lamp during two seconds.
