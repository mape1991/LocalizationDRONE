#ifndef TRILATERATION_H
#define TRILATERATION_H


/*double coordinate[3]={0,0,0};
double *p_coordinate;
double dim_room[3]={15,10,5};
double *p_dim_room;
//int number_beacon = 4;
int chose_beacon[4]={1,2,3,4};
int number_beacon = sizeof(chose_beacon)/sizeof(*chose_beacon);
double target[3]={10,7,2.5};*/

void trilateration (double *mesu_distances, double *p_coord, double *p_dim_room, int number_beacon, int *p_chose_beacon);
#endif
