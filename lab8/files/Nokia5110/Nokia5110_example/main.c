/*
 * main.c
 *
 *  Created on: 16/7/2015
 *  Author: Javier Mart�nez Arrieta
 *  Version: 1.0
 *  This is part of the Nokia5110 library, with functions that will allow you to work with the nokia5110 screen, known as well as PCD8544.
 *  You can find a copy of the Datasheet in the parent folder of the project.
 */
#include "Nokia5110.h"

 /*  Copyright (C) 2015 Javier Mart�nez Arrieta
 *
 *  This project is licensed under Creative Commons Attribution-Non Commercial-Share Alike 4.0 International (CC BY-NC-SA 4.0). According to this license you are free to:
 *  Share � copy and redistribute the material in any medium or format.
 *  Adapt � remix, transform, and build upon the material.
 *  The licensor cannot revoke these freedoms as long as you follow the license terms.
 *	Complete information about this license can be found at: https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
 *
 */


int main(void)
{
	SysTick_Init();
	startSSI0();
	initialize_screen(BACKLIGHT_ON,SSI0);
	int i;
	int max=11,current_pos=0;
	set_buttons_up_down();
	unsigned char menu_elements[12][25];
	menu_elements[0][0]='1';
	menu_elements[0][1]=0x00;
	menu_elements[1][0]='2';
	menu_elements[1][1]=0x00;
	menu_elements[2][0]='3';
	menu_elements[2][1]=0x00;
	menu_elements[3][0]='4';
	menu_elements[3][1]=0x00;
	menu_elements[4][0]='5';
	menu_elements[4][1]=0x00;
	menu_elements[5][0]='6';
	menu_elements[5][1]=0x00;
	menu_elements[6][0]='7';
	menu_elements[6][1]=0x00;
	menu_elements[7][0]='8';
	menu_elements[7][1]=0x00;
	menu_elements[8][0]='9';
	menu_elements[8][1]=0x00;
	menu_elements[9][0]='1';
	menu_elements[9][1]='0';
	menu_elements[9][2]=0x00;
	menu_elements[10][0]='1';
	menu_elements[10][1]='1';
	menu_elements[10][2]=0x00;
	menu_elements[11][0]='1';
	menu_elements[11][1]='2';
	menu_elements[11][2]=0x00;
	set_menu(menu_elements);
	clear_screen(SSI0);
	screen_write("CPE\n403!",ALIGN_CENTRE_CENTRE,SSI0);
	SysTick_Wait50ms(100);
	while(1)
	{
		clear_screen(SSI0);
		char data=GPIO_PORTB_DATA_R&0x03;
		if((data==0x01) && current_pos<max)
		{
			current_pos++;
		}
		else
		{
			if((data==0x02) && current_pos>0)
			{
				current_pos--;
			}
		}
		show_menu(current_pos,SSI0);
		SysTick_Wait50ms(20);
	}
	return 0;
}


// The delay parameter is in units of the 16 MHz core clock.
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}


void SysTick_Wait50ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 50ms
  }
}


void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
