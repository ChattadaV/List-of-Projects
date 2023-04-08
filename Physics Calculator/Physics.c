#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Physics.h"

/* Name: projectile_1D_v1
 * Desc: Collects info, calculates 1D motion, displays results
 */
void projectile_1D_v1(void) {
    double t_init;
    double t_final;
    double v_y_init;
    double v_y_final;
    double y_init;
    double y_final;
    
    t_init = 0.0;
    t_final = 0.0;
    v_y_init = 0.0;
    v_y_final = 0.0;
    y_init = 0.0;
    y_final = 0.0;
    
    printf("Please enter the y_init:\n");
    scanf("%lf", &y_init);
    
    t_final = sqrt(( -2 * y_init ) / a_y ) + t_init;
    y_final = y_init + ( v_y_init * t_final ) + ( 0.5 * a_y * pow(t_final, 2.0));
    v_y_final = v_y_init + ( a_y * t_final );
    
    // result table
    printf("Name                    Value\n");
    printf("==================================\n");
    printf("y_init (ft):            %8.4f\n", y_init);
    printf("y_final (ft):           %8.4f\n", y_final);
    printf("t_init (s):             %8.4f\n", t_init);
    printf("t_final (s):            %8.4f\n", t_final);
    printf("v_y_init (ft/s):        %8.4f\n", v_y_init);
    printf("v_y_final (ft/s):       %8.4f\n", v_y_final);
    printf("==================================\n");
}

/* 
 * Name: projectile_1D_v2(double y_init)
 * Desc: Calculate the amount of time taken from an object to drop from an 
 * initial height as well as the final velocity.
 * Args: double y_init - initial height
 */
int projectile_1D_v2(double y_init) {
    double t_init;
    double t_final;
    double v_y_init;
    double v_y_final;
    double y_final;
    
    t_init = 0.0;
    t_final = 0.0;
    v_y_init = 0.0;
    v_y_final = 0.0;
    y_final = 0.0;
    
    t_final = sqrt(( -2 * y_init ) / a_y ) + t_init;
    v_y_final = v_y_init + ( a_y * t_final );
    
    // result table
    printf("Name                    Value\n");
    printf("==================================\n");
    printf("y_init (ft):            %8.4f\n", y_init);
    printf("y_final (ft):           %8.4f\n", y_final);
    printf("t_init (s):             %8.4f\n", t_init);
    printf("t_final (s):            %8.4f\n", t_final);
    printf("v_y_final (ft/s):       %8.4f\n", v_y_final);
    printf("==================================\n");
    
    return(EXIT_SUCCESS);
}

/* Name: projectile_1D_v3
 * Desc: Calculates 1D motion, no display
 */
int projectile_1D_v3(double y_init, double *t_final, double *y_final, double *v_y_final) {
    double t_init;
    double v_y_init;
    
    t_init = 0.0;
    v_y_init = 0.0;
    
    *t_final = sqrt(( -2 * y_init ) / a_y ) + t_init;
    *y_final = y_init + ( v_y_init * *t_final ) + ( 0.5 * a_y * pow(*t_final, 2.0));
    *v_y_final = v_y_init + ( a_y * *t_final );       
    
    return (EXIT_SUCCESS);
}

/* Name: projectile_1D_v4
 * Desc: Calculates 1D motion as parallel vectors, no display
 */
int projectile_1D_v4(double y_init, double time[], double y_time[], double v_y_time[], int n) {
    double y_final;
    double t_init;
    double t_delta;
    double t_final;
    double v_y_init;
    int index;
    
    y_final = 0.0;
    t_init = 0.0;
    t_final = 0.0;
    v_y_init = 0.0;
    
    t_final = sqrt(( -2 * y_init ) / a_y ) + t_init;
    t_delta = ( t_final - t_init ) / ( n - 1 );
    
    for (index = 0; index < n; index++) {
        time[index] = t_init + ( index * t_delta );
        // y-direction
        y_time[index] = y_init + ( v_y_init * time[index] ) + ( 0.5 * a_y * pow( time[index], 2.0 ) );
        v_y_time[index] = v_y_init + ( a_y * time[index] );        
    }   
    return (EXIT_SUCCESS);
}

/* Name: projectile_2D_v1
 * Desc: Collects info, calculates 2D motion, displays results
 */
void projectile_2D_v1(void) {
    double t_init;
    double t_final;
    double y_init;
    double y_final;
    double x_init;
    double x_final;
    double v_y_init;
    double v_y_final;
    double v_x_init;
    double v_x_final;
    double theta;
    double v_init;
    double a_x;
    
    t_init = 0.0;
    y_init = 0.0; // assume level ground
    x_init = 0.0; // assumed origin
    y_final = y_init; // extra. y_delta
    a_x = 0.0;
    
    printf("Please enter the v_init:\n");
    scanf("%lf", &v_init);    
    printf("Please enter the theta:\n");
    scanf("%lf", &theta);    
    
    // split v_init into 2 components
    v_x_init = v_init * cos(theta);
    v_y_init = v_init * sin(theta);
    
    // calc t_final
    t_final = -2 * ( v_y_init / a_y );
    
    // calc the positions
    x_final = x_init + ( v_x_init * t_final ) + ( 0.5 * a_x * pow( t_final, 2.0));
    y_final = y_init + ( v_y_init * t_final ) + ( 0.5 * a_y * pow( t_final, 2.0));

    // calc the velocities
    v_x_final = v_x_init + ( a_x * t_final );
    v_y_final = v_y_init + ( a_y * t_final );
    
    // result table
    printf("Name                        Value\n");
    printf("==================================\n");
    printf("t_init (ft):             %8.4f\n", t_init);
    printf("t_final (s):             %8.4f\n", t_final);
    printf("theta (radians):         %8.4f\n", theta);    
    printf("y_init (ft):             %8.4f\n", y_init);
    printf("y_final (ft):            %8.4f\n", y_final);
    printf("v_y_init (ft/s):         %8.4f\n", v_y_init);
    printf("v_y_final (ft/s):        %8.4f\n", v_y_final);
    printf("x_init (ft):             %8.4f\n", x_init);
    printf("x_final (ft):            %8.4f\n", x_final);
    printf("v_x_init (ft/s):         %8.4f\n", v_x_init);
    printf("v_x_final (ft/s):        %8.4f\n", v_x_final);
    printf("==================================\n");
}

/* 
 * Name: projectile_2D_v2(double v_init, double theta)
 * Desc: Calculate the amount of time taken from an object launched from a 
 * cannon at a specific angle and velocity.
 * Args: 
 *      double v_init - inital velocity
 *      double theta - initial angle theta
 */
int projectile_2D_v2(double v_init, double theta){
    double t_init; // 0.0
    double t_final; // calc
    double y_init; // 0.0
    double y_final; // 0.0
    double x_init; // 0.0
    double x_final; // calc
    double v_y_init;
    double v_y_final;
    double v_x_init;
    double v_x_final;
    double a_x;
    
    t_init = 0.0;
    t_final = 0.0;
    y_init = 0.0; // assume level ground
    y_final = 0.0;
    x_init = 0.0; // assumed origin
    x_final = 0.0;
    v_y_init = 0.0;
    v_y_final = 0.0;
    v_x_init = 0.0;
    v_x_final = 0.0;
    a_x = 0.0;
    
    // split out v_init into its components
    v_x_init = v_init * cos(theta);
    v_y_init = v_init * sin(theta);
        
    // calc t_final
    t_final = -2 * ( v_y_init / a_y );
    
    // calc the positions
    x_final = x_init + ( v_x_init * t_final ) + ( 0.5 * a_x * pow( t_final, 2.0));
    y_final = y_init + ( v_y_init * t_final ) + ( 0.5 * a_y * pow( t_final, 2.0));

    // calc the velocities
    v_x_final = v_x_init + ( a_x * t_final );
    v_y_final = v_y_init + ( a_y * t_final );
    
    // result table
    printf("Name                        Value\n");
    printf("==================================\n");
    printf("t_init (ft):             %8.4f\n", t_init);
    printf("t_final (s):             %8.4f\n", t_final);
    printf("theta (radians):         %8.4f\n", theta);    
    printf("y_init (ft):             %8.4f\n", y_init);
    printf("y_final (ft):            %8.4f\n", y_final);
    printf("v_y_init (ft/s):         %8.4f\n", v_y_init);
    printf("v_y_final (ft/s):        %8.4f\n", v_y_final);
    printf("x_init (ft):             %8.4f\n", x_init);
    printf("x_final (ft):            %8.4f\n", x_final);
    printf("v_x_init (ft/s):         %8.4f\n", v_x_init);
    printf("v_x_final (ft/s):        %8.4f\n", v_x_final);
    printf("==================================\n");

    return (EXIT_SUCCESS);
}

/* Name: projectile_2D_v3
 * Desc: Calculates 2D motion, no display
 */
int projectile_2D_v3(double v_init, double theta, double *t_final, double *y_final, double *v_y_final, double *x_final, double *v_x_final) {
    double t_init; // 0.0
    double y_init; // 0.0
    double x_init; // 0.0
    double v_y_init;
    double v_x_init;
    double a_x;
    
    t_init = 0.0;
    y_init = 0.0; // assume level ground
    x_init = 0.0; // assumed origin
    *y_final = y_init;
    a_x = 0.0;
    
    // split out v_init into its components
    v_x_init = v_init * cos(theta);
    v_y_init = v_init * sin(theta);
        
    // calc t_final
    *t_final = -2 * ( v_y_init / a_y );
    
    // calc the positions
    *x_final = x_init + ( v_x_init * *t_final ) + ( 0.5 * a_x * pow( *t_final, 2.0));
    *y_final = y_init + ( v_y_init * *t_final ) + ( 0.5 * a_y * pow( *t_final, 2.0));

    // calc the velocities
    *v_x_final = v_x_init + ( a_x * *t_final );
    *v_y_final = v_y_init + ( a_y * *t_final );   
        
    return (EXIT_SUCCESS);
}

/* Name: projectile_2D_v4
 * Desc: Calculates 2D motion as parallel arrays, no display
 */
int projectile_2D_v4(double v_init, double theta, double time[], double y_time[], double v_y_time[], double x_time[], double v_x_time[], int n ) {
    double t_init;
    double t_delta;
    double t_final;
    double y_init;
    double x_init;
    double v_x_init;
    double v_y_init;
    int index;
    double a_x;
    
    t_init = 0.0;
    x_init = 0.0;
    y_init = 0.0;
    a_x = 0.0;
    
    v_y_init = v_init * sin(theta);
    v_x_init = v_init * cos(theta);
    t_final = sqrt(( -2 * v_y_init ) / a_y ) + t_init;
    // t_final = (( -2 * v_y_init ) / a_y ) + t_init;
    t_delta = ( t_final - t_init ) / ( n - 1 );
    
    for (index = 0; index < n; index++) {
        time[index] = t_init + ( index * t_delta );
        // y-direction
        y_time[index] = y_init + ( v_y_init * time[index] ) + ( 0.5 * a_y * pow( time[index], 2.0 ) );
        v_y_time[index] = v_y_init + ( a_y * time[index] );
        // x-direction
        x_time[index] = x_init + ( v_x_init * time[index] ) + ( 0.5 * a_x * pow( time[index], 2.0 ) );
        v_x_time[index] = v_x_init + ( a_x * time[index] );        
    }                   
    return (EXIT_SUCCESS);
}

/* Name: simple_supported_beam_v1
 * Desc: Collects info, calculates reaction forces, displays results
 */
void simple_supported_beam_v1(void) {
    double W; // load
    double L; // length of the beam
    double a; // location of the load
    double R1; // reaction force at P1
    double R2; // reaction force at P2
    
    printf("Enter in the load (lbs):\n");
    scanf("%lf", &W);

    printf("Enter in the length of the beam (ft):\n");
    scanf("%lf", &L);   
    
    printf("Enter in the locatin of the load (ft):\n");
    scanf("%lf", &a);

    R1 = 0.0;
    R2 = 0.0;    
    
    // calc R1 and R2 weight
    R1 = ( W * ( L - a )) / L;
    R2 = ( W * a ) / L;    

    //result table
    printf("Name                    Value\n");
    printf("================================\n");
    printf("W (lbs):                %8.4f\n", W);
    printf("L (ft):                 %8.4f\n", L);
    printf("a (ft):                 %8.4f\n", a);
    printf("R1 (lbs):               %8.4f\n", R1);
    printf("R2 (lbs):               %8.4f\n", R2);
    printf("================================\n");    
}

/* 
 * Name: simple_supported_beam_v2(double W, double L, double a)
 * Desc: Calculate the reaction forces on a beam, given a load, length, and 
 * load location
 * Args: 
 *      double W - load
 *      double L - beam length
 *      double a - load location
 */
int simple_supported_beam_v2(double W, double L, double a){
    double R1;
    double R2;
    
    R1 = 0.0;
    R2 = 0.0;    
    
    // calc R1 and R2 weight
    R1 = ( W * ( L - a )) / L;
    R2 = ( W * a ) / L;
    
    //result table
    printf("Name                    Value\n");
    printf("================================\n");
    printf("W (lbs):                %8.4f\n", W);
    printf("L (ft):                 %8.4f\n", L);
    printf("a (ft):                 %8.4f\n", a);
    printf("R1 (lbs):               %8.4f\n", R1);
    printf("R2 (lbs):               %8.4f\n", R2);
    printf("================================\n");   
       
    return (EXIT_SUCCESS);
}

/* Name: simple_supported_beam_v3
 * Desc: Calculates reaction forces, no display
 */
int simple_supported_beam_v3(double W, double L, double a, double *R1, double *R2) {
    *R1 = ( W * ( L - a )) / L;
    *R2 = ( W * a ) / L;  
    
    return (EXIT_SUCCESS);
}

/* Name: simple_supported_beam_v4
 * Desc: Calculates reaction forces as parallel arrays, no display
 */
int simple_supported_beam_v4(double W[], double L, double a[], double R1[], double R2[], int n) {
    int index;
    
    for (index = 0; index < n; index++) {
        R1[index] = ( W[index] * ( L - a[index] )) / L;
        R2[index] = ( W[index] * a[index] ) / L;   
    }
    return (EXIT_SUCCESS);
}
