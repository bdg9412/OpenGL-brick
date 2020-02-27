#pragma once
//This is for menu with color choice
#include<stdio.h>
#include"SoundEngine.h"

using namespace jm;

SoundEngine sound_engine_color;

double color_x, color_y, color_z;
void sub_menu_function(int option)

{
	//false로 설정하여 loop현상 방지
	sound_engine_color.createSound("guitar.wav", "colorsound", false);
	sound_engine_color.playSound("colorsound");
	switch (option)
	{
	case 1:
		color_x = 1.0, color_y = 0.0, color_z = 0.0;
		break;
	case 2:
		color_x = 0.0, color_y = 0.0, color_z = 1;
		break;
	case 3:
		color_x = 0.0, color_y = 1, color_z = 0.0;
		break;
	case 4:
		color_x = 0.0, color_y = 0, color_z = 0.0;
		break;
	case 5:
		color_x = 1, color_y = 1, color_z = 1;
		break;
	case 6:
		color_x = 1.02, color_y = 0.51, color_z = 0;
		break;
	case 7:
		color_x = 1, color_y = 1, color_z = 0;
		break;
	case 8:
		color_x = 2.2, color_y = 0, color_z = 2.1;
		break;
	case 10:
		printf("choose color on the menu! new color comming soon\n");
	case 11:
		exit(-1);
	}
}

void main_menu_function(int option)

{

	printf("Main menu %d has been selected\n", option);

	if (option == 999) {

		exit(0);

	}

}