/*
 * note.c
 *
 * Created: 2022-05-25 15:29:32
 *  Author: admin
 */ 
#include <math.h>
int i, result;

int note(int a) {
	if ((31.785<a) && (a<63.575))									//0��Ÿ��
		i = 0;
	else if ((31.785*2<a) && (a<63.575*2))							//1��Ÿ��
		i = 1;
	else if ((31.785*pow(2,2)<a) && (a<63.575*pow(2,2)))           //2��Ÿ��
		i = 2;
	else if ((31.785*pow(2,3)<a) && (a<63.575*pow(2,3)))           //3��Ÿ��
		i = 3;
	else if ((31.785*pow(2,4)<a) && (a<63.575*pow(2,4)))           //4��Ÿ��
		i = 4;
	else if ((31.785*pow(2,5)<a) && (a<63.575*pow(2,5)))           //5��Ÿ��
		i = 5;
	else if ((31.785*pow(2,6)<a) && (a<63.575*pow(2,6)))           //6��Ÿ��
		i = 6;
	else if ((31.785*pow(2,7)<a) && (a<63.575*pow(2,7)))           //7��Ÿ��
		i = 7;
	



	if ((31.785*pow(2,i)<a) && (a<33.675*pow(2,i)))                       // ��
		result = 1;
	else if ((33.675*pow(2,i)<a) && (a<35.68*pow(2,i)))						// ��#
		result = 2;
	else if ((35.68*pow(2,i)<a) && (a<37.8*pow(2,i)))						// ��
		result = 3;
	else if ((37.8*pow(2,i)<a) && (a<40.045*pow(2,i)))						// ��#
		result = 4;								
	else if ((40.045*pow(2,i)<a) && (a<42.425*pow(2,i)))					// ��
		result = 5;
	else if ((42.425*pow(2,i)<a) && (a<44.95*pow(2,i)))                    // ��
		result = 6;
	else if ((44.95*pow(2,i)<a) && (a<47.625*pow(2,i)))                    // ��#
		result = 7;
	else if ((47.625*pow(2,i)<a) && (a<50.455*pow(2,i)))                   // ��
		result = 8;
	else if ((50.455*pow(2,i)<a) && (a<53.455*pow(2,i)))                   // ��#
		result = 9;
	else if ((53.455*pow(2,i)<a) && (a<56.635*pow(2,i)))                   // ��
		result = 10;
	else if ((56.635*pow(2,i)<a) && (a<60.005*pow(2,i)))                   // ��#
		result = 11;
	else if ((60.005*pow(2,i)<a) && (a<63.575*pow(2,i)))                   // ��
		result = 12;
	else
		result = 13;
	
	return result;
}