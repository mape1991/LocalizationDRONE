

#include "trilateration.h"

#ifdef TEST_FULL

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
* Function computes distance between 2 points
*/
double compute_norm (double *p_coord_1, double *p_coord_2){	
	double norm;
	double x,y,z;
	x=*(p_coord_1+0)-*(p_coord_2+0);
	y=*(p_coord_1+1)-*(p_coord_2+1);
	z=*(p_coord_1+2)-*(p_coord_2+2);
	norm=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	//printf("value of norm is: %f\n",norm);
	return norm;	
}

/**
* Function computes the distances mesured for simulation
*/
void simulate_mesu_distance (double **p_beacon, double *p_target, double *p_mesu_dist, int number_beacon){
	int i;
	srand(time(NULL));
	for (i=0;i<number_beacon;i++){
		//*(p_mesu_dist+i) = compute_norm(*(p_beacon+i),p_target);
		*(p_mesu_dist+i) = compute_norm(*(p_beacon+i),p_target) + 1.2*((double)rand() / (double)RAND_MAX -0.5);
		//printf("dist %i: %f\n",i+1,*(p_mesu_dist+i));	
		//printf("rand : %f\n",(double)rand() / (double)RAND_MAX);
	}	
}
/**
* Function computes vector fi (difference between true distance and distance measured) 
*/
void compute_fi (double *x, double *y, double *z, double *p_xi, double *p_yi, double *p_zi, double *p_ri, double *p_fi,int number_beacon){
	int i;
	for (i=0;i<number_beacon;i++){
		*(p_fi + i)=sqrt(pow((*x - *(p_xi + i)),2) + pow((*y - *(p_yi + i)),2) + pow((*z - *(p_zi + i)),2)) - *(p_ri+ i);	
		//printf("value of fi%d  %f\n",i,*(p_fi + i));
	}
}

/**
* Function computes vector di (distance between target and beacon i)
*/
void compute_di (double *x, double *y, double *z, double *p_xi, double *p_yi, double *p_zi, double *p_di,double *p_fi, int number_beacon){
	int i;
	//printf("value of fi0  %f\n",*(p_fi + 0));
	for (i=0;i<number_beacon;i++){
		*(p_di + i)=sqrt(pow((*x - *(p_xi + i)),2) + pow((*y - *(p_yi + i)),2) + pow((*z - *(p_zi + i)),2));	
		//printf("value of di%d  %f\n",i,*(p_di + i));
		//printf("value of fi0 %d %f\n",i,*(p_fi + 0));
	}
	
}
/**
* Function computes matrix Jtf (size 3x1)
*/
void compute_JTf (double *x, double *y, double *z, double *p_xi, double *p_yi, double *p_zi, double *p_fi, double *p_di, double *p_JTf,int number_beacon){
	int i;
	*(p_JTf+0)=0;
	*(p_JTf+1)=0;
	*(p_JTf+2)=0;
	for (i=0;i<number_beacon;i++){
		*(p_JTf+0)=*(p_JTf+0) + (*x - *(p_xi +i))*(*(p_fi +i))/(*(p_di +i));
		*(p_JTf+1)=*(p_JTf+1) + (*y - *(p_yi +i))*(*(p_fi +i))/(*(p_di +i));
		*(p_JTf+2)=*(p_JTf+2) + (*z - *(p_zi +i))*(*(p_fi +i))/(*(p_di +i));
		/*printf("value of i=  %d\n",i+1);
		printf("value of fi=  %f\n",*(p_fi +i));
		printf("value of di=  %f\n",*(p_di +i));*/
		/*printf("value of JTf 1  %f\n",*(p_JTf+0)); 
		printf("value of JTf 2  %f\n",*(p_JTf+1));
		printf("value of JTf 3  %f\n\n",*(p_JTf+2));*/
	}
	/*printf("value of JTf 1  %f\n",*(p_JTf+0)); 
	printf("value of JTf 2  %f\n",*(p_JTf+1));
	printf("value of JTf 3  %f\n\n",*(p_JTf+2));*/
}

/**
* Function computes matrix JTJ (size 3x3)
*/
void compute_JTJ (double *x, double *y, double *z, double *p_xi, double *p_yi, double *p_zi, double *p_di, double p_JTJ[3][3],int number_beacon){
	int i,j,k;
	for (j=0;j<3;j++)
		for (k=0;k<3;k++)
			*(*(p_JTJ+j)+k)=0;
	for (i=0;i<number_beacon;i++){
		/*printf("value of x %f\n",*x);
		printf("value of y %f\n",*y);
		printf("value of z %f\n",*z);*/

		/*for (j=0;j<number_beacon;j++){	
			printf("value of di%d  %f\n",j,*(p_di + j));
		}*/
		p_JTJ[0][0]= *(*(p_JTJ+0)+0) + pow((*x - *(p_xi +i)),2)/pow(*(p_di +i),2);
		p_JTJ[0][1]= *(*(p_JTJ+0)+1) + (*x - *(p_xi +i))*(*y - *(p_yi +i))/pow(*(p_di +i),2);
		p_JTJ[0][2]= *(*(p_JTJ+0)+2) + (*x - *(p_xi +i))*(*z - *(p_zi +i))/pow(*(p_di +i),2);
		p_JTJ[1][0]= *(*(p_JTJ+1)+0) + (*x - *(p_xi +i))*(*y - *(p_yi +i))/pow(*(p_di +i),2);
		p_JTJ[1][1]= *(*(p_JTJ+1)+1) + pow((*y - *(p_yi +i)),2)/pow(*(p_di +i),2);
		p_JTJ[1][2]= *(*(p_JTJ+1)+2) + (*y - *(p_yi +i))*(*z - *(p_zi +i))/pow(*(p_di +i),2);
		p_JTJ[2][0]= *(*(p_JTJ+2)+0) + (*x - *(p_xi +i))*(*z - *(p_zi +i))/pow(*(p_di +i),2);
		p_JTJ[2][1]= *(*(p_JTJ+2)+1) + (*y - *(p_yi +i))*(*z - *(p_zi +i))/pow(*(p_di +i),2);
		p_JTJ[2][2]= *(*(p_JTJ+2)+2) + pow((*z - *(p_zi +i)),2)/pow(*(p_di +i),2);
	}
	/*printf("value of JTf 11  %f\n",*(*(p_JTJ+0)+0)); 
	printf("value of JTf 12  %f\n",*(*(p_JTJ+0)+1));
	printf("value of JTf 13  %f\n",*(*(p_JTJ+0)+2));
	printf("value of JTf 21  %f\n",*(*(p_JTJ+1)+0)); 
	printf("value of JTf 22  %f\n",*(*(p_JTJ+1)+1));
	printf("value of JTf 23  %f\n",*(*(p_JTJ+1)+2));
	printf("value of JTf 31  %f\n",*(*(p_JTJ+2)+0)); 
	printf("value of JTf 32  %f\n",*(*(p_JTJ+2)+1));
	printf("value of JTf 33  %f\n\n",*(*(p_JTJ+2)+2));*/
	/*printf("\nThe matrix JTJ is\n");
  	for(i=0;i<3;i++){
  		printf("\n");
   	   	for(j=0;j<3;j++)
           		printf("%f\t",*(*(p_JTJ+i)+j));
 	}
	printf("\n");*/
}

/**
* Function inverses a matrix 3x3
*/
void inverse_matrix (double mat1[3][3], double mat2[3][3]){
	double a[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	double b[3][3]={{0,0,0},{0,0,0},{0,0,0}};
  	int i,j;
 	float determinant=0;

	for(i=0;i<3;i++){
   	   	for(j=0;j<3;j++)
           		a[i][j]=*(*(mat1+i)+j);
			//a[i][j]=mat1[i][j];
 	}

  	/*printf("Enter the 9 elements of matrix: ");
 	 for(i=0;i<3;i++)
     	 for(j=0;j<3;j++)
           scanf("%lf",&a[i][j]);*/

  	/*printf("\nThe matrix is\n");
  	for(i=0;i<3;i++){
  		printf("\n");
   	   	for(j=0;j<3;j++)
           		printf("%f\t",a[i][j]);
 	}*/

  	for(i=0;i<3;i++)
      		determinant = determinant + (a[0][i]*(a[1][(i+1)%3]*a[2][(i+2)%3] - a[1][(i+2)%3]*a[2][(i+1)%3]));

   	for(i=0;i<3;i++){
      		for(j=0;j<3;j++){
		   //printf("%f\t",((a[(i+1)%3][(j+1)%3] * a[(i+2)%3][(j+2)%3]) - (a[(i+1)%3][(j+2)%3]*a[(i+2)%3][(j+1)%3]))/ determinant);
		   b[j][i]=((a[(i+1)%3][(j+1)%3] * a[(i+2)%3][(j+2)%3]) - (a[(i+1)%3][(j+2)%3]*a[(i+2)%3][(j+1)%3]))/ determinant;
	}
   }

	for(i=0;i<3;i++){
   	   	for(j=0;j<3;j++)
           		*(*(mat2+i)+j)=b[i][j];
 	}

	/*printf("\nMatrix inverse of a is : \n\n");
   	for(i=0;i<3;i++){
    		for(j=0;j<3;j++){
		printf("%f\t", *(*(mat2+i)+j));
		}
	printf("\n");
	}
	printf("\n");*/
   		
}

/**
* Function multiplies a matrix 3x3 with 3x1
*/
void multiple_matrix(double mat1[3][3], double *mat2, double *res){
	int i,j;
	for(i=0;i<3;i++){
      		for(j=0;j<3;j++){
			*(res+i)=0;	
		}
		//printf("%f\n", *(res+i));	
	}
	for(i=0;i<3;i++){
      		for(j=0;j<3;j++){
			//printf("c_mat1 = %f c_mat2= %f\n", mat1[i][j],*(mat2+j));
			*(res+i)=*(res+i) + mat1[i][j]*(*(mat2+j));	
		}
		//printf("%f\n", *(res+i));	
	}
	/*printf("x= %f\n", *(res+0));
	printf("y= %f\n", *(res+1));
	printf("z= %f\n\n", *(res+2));*/
}


/**
* Function prints the result of final coordinate
*/
void print_coordinate(double *p_coord){
	printf("The final position is \n");
	printf("x = %lf \n",*(p_coord+0));
	printf("y = %lf \n",*(p_coord+1));
	printf("z = %lf \n",*(p_coord+2));
}

/**
* Function computes the position of drone
* Inputs: *distances: vector NUM_BEACONSx1 measured distances between the beacons and the target
*	 *p_coord: vector 3x1 result of function
*	 *p_dim_room: vector 3x1 dimension of the room
*	 number_beacon: number of beacons will be used	
*	 *p_selected_beacons : vector number_beacon x 1 contains selected beacons
* Note: When need to use distance measure as input, replace p_target by vector distance
*/
void trilateration (double *mesu_distances, double *p_coord, double *p_dim_room, int number_beacon, int *p_selected_beacons ){
	double length_room = *(p_dim_room+0);
	double width_room = *(p_dim_room+1);
	double height_room = *(p_dim_room+2);
	double bea1[3] = {0, 		0, 		height_room};
	double bea2[3] = {length_room, 	0, 		height_room};
	double bea3[3] = {0, 		width_room, 	height_room};
	double bea4[3] = {length_room, 	width_room, 	height_room};
	double bea5[3] = {0, 		0, 		0};
	double bea6[3] = {length_room, 	0, 		0};
	double bea7[3] = {0,		width_room,	0};
	double bea8[3] = {length_room, 	width_room, 	0};
	double* beacon[number_beacon];
	double xi[number_beacon];
	double yi[number_beacon];
	double zi[number_beacon];
	//double mesu_distances[number_beacon];
	double fi[4]={0,0,0,0};
	double di[4]={0,0,0,0};
	double JTf[3]={0,0,0};
	double JTJ[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	double JTJinv[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	double aux[3]={0,0,0};
	double teta_prec[3]={length_room/2, width_room/2, height_room/2};
	//double teta_actu[3];
	int i;
	//int j,k;
	
	for (i=1;i<=number_beacon;i++){
		switch (*(p_selected_beacons+i-1)){
			case 1: beacon[i-1]=bea1;
				break;
			case 2: beacon[i-1]=bea2;
				break;
			case 3: beacon[i-1]=bea3;
				break;
			case 4: beacon[i-1]=bea4;
				break;
			case 5: beacon[i-1]=bea5;
				break;
			case 6: beacon[i-1]=bea6;
				break;
			case 7: beacon[i-1]=bea7;
				break;
			case 8: beacon[i-1]=bea8;
				break;	
		}		
	}

	for (i=1;i<=number_beacon;i++){
		xi[i-1]=*(beacon[i-1]+0);	
		//printf("value of xi %f\n", *(beacon[i-1]));
	}
	
	for (i=1;i<=number_beacon;i++){
		yi[i-1]=*(beacon[i-1]+1);	
		//printf("value of yi %f\n", *(beacon[i-1]+1));
	}
	
	for (i=1;i<=number_beacon;i++){
		zi[i-1]=*(beacon[i-1]+2);	
		//printf("value of zi %f\n", *(beacon[i-1]+2));
	}

	//simulate_mesu_distance (beacon, p_target, mesu_dist, number_beacon);
	
	for (i=0;i<100;i++){
		aux[0]=0;
		aux[1]=0;
		aux[2]=0;
		/*double aux[3]={0,0,0};
		double fi[4]={0,0,0,0};
		double di[4]={0,0,0,0};
		double JTf[3]={0,0,0};
		double JTJ[3][3]={{0,0,0},{0,0,0},{0,0,0}};
		double JTJinv[3][3]={{0,0,0},{0,0,0},{0,0,0}};*/
		compute_fi (&teta_prec[0], &teta_prec[1], &teta_prec[2], xi, yi, zi, mesu_distances, fi, number_beacon);
		/*for (j=0;j<number_beacon;j++){	
			printf("value of fi%d before  %f\n",j,fi[j]);
		}*/
		compute_di (&teta_prec[0], &teta_prec[1], &teta_prec[2], xi, yi, zi, di,fi, number_beacon);
		/*for (j=0;j<number_beacon;j++){	
			printf("value of fi%d after  %f\n",j,fi[j]);
		}*/
		/*for (j=0;j<number_beacon;j++){	
			printf("value of di%d after  %f\n",j,di[j]);
		}*/	
		compute_JTf (&teta_prec[0], &teta_prec[1], &teta_prec[2], xi, yi, zi, fi, di, JTf, number_beacon);
		compute_JTJ (&teta_prec[0], &teta_prec[1], &teta_prec[2], xi, yi, zi, di, JTJ, number_beacon);
		inverse_matrix (JTJ,JTJinv);
		/*printf("\nMatrix inverse of JTJ is : \n\n");
	   	for(k=0;k<3;k++){
	    		for(j=0;j<3;j++){
			printf("%f\t", JTJinv[k][j]);
			}
		printf("\n");
		}*/
		multiple_matrix(JTJinv, JTf, aux);
		/*printf("x= %f\n", aux[0]);
		printf("y= %f\n", aux[1]);
		printf("z= %f\n\n", aux[2]);*/
		teta_prec[0]=teta_prec[0]-aux[0];
		teta_prec[1]=teta_prec[1]-aux[1];
		teta_prec[2]=teta_prec[2]-aux[2];
		if (teta_prec[2] > height_room)
			teta_prec[2]-=0.2;
		else if (teta_prec[2] < 0)
			teta_prec[2]+=0.2;
		
	}
	/*printf("The final position is \n");
	printf("x= %f\n", fabs(teta_prec[0]));
	printf("y= %f\n", fabs(teta_prec[1]));
	printf("z= %f\n", fabs(teta_prec[2]));*/
	if (teta_prec[2] > height_room)
			teta_prec[2]=5;
	 *(p_coord+0)=fabs(teta_prec[0]);
	 *(p_coord+1)=fabs(teta_prec[1]);
	 *(p_coord+2)=fabs(teta_prec[2]);
	print_coordinate(p_coord);
	
}

#endif



