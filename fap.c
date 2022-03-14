#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fap.h"
// testing -a option of the git

#define ARR_SIZE 51
#define CABLE_CONSTANT 0.18

int test_arr[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 250, 200, 150, 50, 1, 0, 0, 0, -101, -250, -500, -400, -99, 2, 0, 0, 0, 0, 0, 0, 102, 150, 200, 95, 3, 0, 0, 0, 150, 200, 300, 500, 490, 400, 4, 0, 0};
int peak_index[100] = {};
int peak_end[100] = {};

double TDRLength(int peaks[100], double cable_constant, int point1, int point2)
{
	// (c*v*Td) / 2 = TDRLength
	// c = Velocity of light i.e 2.9979 x 10^8 m/s
	// υ = Velocity factor of transmission line
	// Td = Round trip time (RTT) between onset pulse and first reflection (in µs) 	(10^-6)
	
	double one_distance = (9.523 * cable_constant) / 2;
	double total_distance = one_distance * (peaks[point2] - peaks[point1]);
	
	return total_distance;
}

int *find_peak(int *arr,int size, int peak_threshold)
{
	int temp=0,peak_counter=0, peak_end_counter=0, peak_switch=0, diff=0;
	int peak_start_positive = 0;
	int peak_start_negative = 0;
	temp = arr[0];
	
	for(int i=1;i<size;i++)
	{
		diff = arr[i] - temp;
		if(diff > peak_threshold && peak_switch == 0) {
			peak_index[peak_counter] = i;
			peak_counter++;
			peak_start_positive = arr[i];
			peak_switch=1;
		} else if(diff < peak_threshold*-1 && peak_switch == 0) {
			peak_index[peak_counter] = i;
			peak_counter++;
			peak_start_negative = arr[i];
			peak_switch=2;
		} else if(peak_switch==1){
			if(arr[i] < peak_start_positive && (diff > -10 && diff < 10)) {
				peak_switch = 0;
				peak_end[peak_end_counter] = i-1;
				printf("peak_end_index = %d, peak_end_val = %d\n", i-1, arr[i-1]);
				peak_end_counter++;
			}
		} else if(peak_switch==2){
			if(arr[i] > peak_start_negative && (diff > -10 && diff < 10)) {
				peak_switch = 0;
				peak_end[peak_end_counter] = i-1;
				printf("peak_end_index = %d, peak_end_val = %d\n", i-1, arr[i-1]);
				peak_end_counter++;
			}
		}
		
		temp=arr[i];
	}
	return peak_index;
}

int main(int argc, char *argv[])
{
	int peaks[100] = {};
	int peak_ends[100] = {};
	memcpy(peaks,find_peak(test_arr,ARR_SIZE,50),400);
	printf("\n\n");
	for(int i=0; i<100;i++)
		if(peaks[i]!=0)
			printf("%d. element => %d\ndistance => %.2fm\n\n",peaks[i],test_arr[peaks[i]],TDRLength(peaks,CABLE_CONSTANT,0,i));

	printf("\n\n distance between %d.")
}
