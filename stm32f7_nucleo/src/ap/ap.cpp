/*
 * ap.cpp
 *
 *  Created on: 2018. 4. 14.
 *      Author: baram
 */




#include "ap.h"

extern "C"
{
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernet/ethernetif.h"
#include "ethernet/app_ethernet.h"
#include "ethernet/udp_echoserver.h"
}

static void threadMain(const void *argument);
static void threadUDP(const void *argument);



void apInit(void)
{
  cmdifBegin(_DEF_UART1, 115200);
}

void apMain(void)
{
  osThreadDef(threadMain, threadMain, osPriorityBelowNormal, 0, 6*1024/4);
  osThreadCreate(osThread(threadMain), NULL);

  osKernelStart ();
}

static void threadMain(const void *argument)
{
  uint32_t pre_time;


  osThreadDef(threadUDP, threadUDP, osPriorityBelowNormal, 0, 6*1024/4);
  osThreadCreate(osThread(threadUDP), NULL);


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(0);
    }

    cmdifMain();
    osThreadYield();
  }
}


struct netif gnetif;
static void Netif_Config(void);


static void threadUDP(const void *argument)
{
  /* Initialize the LwIP stack */
  lwip_init();

  /* Configure the Network interface */
  Netif_Config();

  /* tcp echo server Init */
  udp_echoserver_init();

  /* Notify user about the network interface config */
  User_notification(&gnetif);


  /* Infinite loop */
  while (1)
  {
    /* Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling */
    ethernetif_input(&gnetif);

    /* Handle timeouts */
    sys_check_timeouts();

#ifdef USE_DHCP
    /* handle periodic timers for DHCP */
    DHCP_Periodic_Handle(&gnetif);
#endif
  }
}

/**
  * @brief  Setup the network interface
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

#ifdef USE_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */

  /* add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
}

