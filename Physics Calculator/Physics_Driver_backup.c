/* 
 * File: Physics_Driver.c
 * Copy: Copyright (c) 2021 Chattada Viriyaphap
 * BlazerID: chattada
 * Vers: 1.0.0 12/10/2021
 * Desc: Driver for testing concepts 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define a_y -32.2 // gravity acceleration

void projectile_1D_v1(void); // 3 /1D /v1
int projectile_1D_v2(double y_init); // 4 /1D /v2 /i | 5 /1D /v2 /p 100.0
int projectile_1D_v3(double y_init, double *t_final, double *y_final, double *v_y_final); // 4 /1D /v3 /i | 5 /1D /v3 /p 100.0
int projectile_1D_v4(double y_init, double time[], double y_time[], double v_y_time[], int n); // 4 /1D /v4 /i | 6 /1D /v3 /p 100.0 11

void projectile_2D_v1(void); // 3 /2D /v1
int projectile_2D_v2(double v_init, double theta); // 4 /2D /v2 /i | 6 /2D /v2 /p 100.0 0.785
int projectile_2D_v3(double v_init, double theta, double *t_final, double *y_final, double *v_y_final, double *x_final, double *v_x_final); // 4 /2D /v3 /i | 6 /2D /v3 /p 100.0 0.785
int projectile_2D_v4(double v_init, double theta, double time[], double y_time[], double v_y_time[], double x_time[], double v_x_time[], int n ); // 4 /2D /v4 /i | 7 /2D /v4 /p 100.0 0.785 11

void simple_supported_beam_v1(void); // 3 /beam /v1
int simple_supported_beam_v2(double W, double L, double a); // 4 /beam /v2 /i | 7 /beam /v2 /p 100.0 10.0 6.0
int simple_supported_beam_v3(double W, double L, double a, double *R1, double *R2); // 4 /beam /v3 /i | 7 /beam /v3 /p 100.0 10.0 6.0
int simple_supported_beam_v4(double W[], double L, double a[], double R1[], double R2[], int n); // 4 /beam /v4 /i | 7 /beam /v4 /p 100.0 10.0 11

int help(void);

/* 
 * Name: int main(int argc, char** argv)
 * Desc: Used to drive other functions for the purpose to demo features in
 * class.  Function calls are commented/uncommented depending on which features 
 * are to be tested.  
 */
int main(int argc, char** argv) {    
    int index;
    double y_init;
    double y_final;
    double x_final;
    double t_final;
    double v_y_final;
    double v_x_final;
    double v_init;
    double theta;
    
    int n; // n = 11
    double time[11];
    double y_time[11];
    double v_y_time[11];
    double x_time[11];
    double v_x_time[11];
    
    double W; // load
    double L; // length of the beam
    double a; // location of the load
    double R1;
    double R2;
    double W_array[11];
    double a_array[11];
    double R1_array[11];
    double R2_array[11];    
    double a_delta;
                
    printf("argv[ 0 ]: %s\n", argv[ 0 ]); // name of the executable
    
    for (index = 0; index < argc; index++) {
        printf("argv[ %d ]: %s\n", index, argv[ index ]);
    }
 
    if (argc < 2) {
        help();
    } else {
        // handle scenarios where we have 2+ arguments
        if (strcmpi(argv[ 1 ], "/h") == 0) { // string compare i if argv[1] matches "/h" (==0)
            help(); // explicit call for help
        } else if ((argc == 3) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v1") == 0)) {           
            projectile_1D_v1();              
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v2") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user
            printf("Using interactive mode\n");
            printf("Please enter a height (ft):\n");
            scanf("%lf", &y_init);
            projectile_1D_v2( y_init ); // result table included in the function
        } else if ((argc == 5) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v2") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            y_init = atof( argv[ 4 ] );
            projectile_1D_v2( y_init ); // result table included in the function
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v3") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user
            printf("Using interactive mode\n");
            
            printf("Please enter a height (ft):\n");
            scanf("%lf", &y_init);
            
            projectile_1D_v3( y_init, &t_final, &y_final, &v_y_final ); // result table not included in the function            
            
            // result table
            printf("Name                    Value\n");
            printf("==================================\n");
            printf("y_init (ft):            %8.4f\n", y_init);
            printf("y_final (ft):           %8.4f\n", y_final);
            printf("t_init (s):             %8.4f\n", 0.0);
            printf("t_final (s):            %8.4f\n", t_final);
            printf("v_y_init (ft/s):        %8.4f\n", 0.0);
            printf("v_y_final (ft/s):       %8.4f\n", v_y_final);
            printf("==================================\n");
        } else if ((argc == 5) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v3") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            y_init = atof( argv[ 4 ] );
            
            projectile_1D_v3( y_init, &t_final, &y_final, &v_y_final );  // result table not included in the function
            
            // result table
            printf("Name                    Value\n");
            printf("==================================\n");
            printf("y_init (ft):            %8.4f\n", y_init);
            printf("y_final (ft):           %8.4f\n", y_final);
            printf("t_init (s):             %8.4f\n", 0.0);
            printf("t_final (s):            %8.4f\n", t_final);
            printf("v_y_init (ft/s):        %8.4f\n", 0.0);
            printf("v_y_final (ft/s):       %8.4f\n", v_y_final);
            printf("==================================\n");            
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v4") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n");
            
            printf("Please enter a height (ft):\n");
            scanf("%lf", &y_init);
            printf("Please enter the number of points:\n");
            scanf("%lf", &n);
            
            projectile_1D_v4( y_init, time, y_time, v_y_time, n );  // result table not included in the function
            
            // result table
            printf("Name                    Value\n");
            printf("==================================\n");
            printf("y_init (ft):            %8.4f\n", y_init);
            printf("y_final (ft):           %8.4f\n", y_time[n-1]);
            printf("t_init (s):             %8.4f\n", 0.0);
            printf("t_final (s):            %8.4f\n", time[n-1]);
            printf("v_y_init (ft/s):        %8.4f\n", 0.0);
            printf("v_y_final (ft/s):       %8.4f\n", v_y_time[n-1]);
            printf("==================================\n\n");
            printf("Index   Time (s)     y_time (ft)   v_y_time (ft/s)\n");
            printf("================================================\n");               
            for (index = 0; index < n; index++) {
                printf("%d      %8.4f       %8.4f       %8.4f\n", index, time[index], y_time[index], v_y_time[index]);            
            }
            printf("================================================\n");   
        } else if ((argc == 6) && (strcmpi(argv[ 1 ], "/1D") == 0) && (strcmpi(argv[ 2 ], "/v4") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            y_init = atof( argv[ 4 ] );
            n = atoi( argv[ 5 ] );
            
            projectile_1D_v4( y_init, time, y_time, v_y_time, n );  // result table not included in the function
            
            // result table
            printf("Name                    Value\n");
            printf("==================================\n");
            printf("y_init (ft):            %8.4f\n", y_init);
            printf("y_final (ft):           %8.4f\n", y_time[n-1]);
            printf("t_init (s):             %8.4f\n", 0.0);
            printf("t_final (s):            %8.4f\n", time[n-1]);
            printf("v_y_init (ft/s):        %8.4f\n", 0.0);
            printf("v_y_final (ft/s):       %8.4f\n", v_y_time[n-1]);
            printf("==================================\n\n");
            printf("Index   Time (s)     y_time (ft)   v_y_time (ft/s)\n");
            printf("================================================\n");               
            for (index = 0; index < n; index++) {
                printf("%d      %8.4f       %8.4f       %8.4f\n", index, time[index], y_time[index], v_y_time[index]);            
            }
            printf("================================================\n");   
        } else if ((argc == 3) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v1") == 0)) {           
            projectile_2D_v1();             
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v2") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n"); 
            
            printf("Please enter a velocity (ft/s):\n");
            scanf("%lf", &v_init);
            printf("Please enter an angle (radians):\n");
            scanf("%lf", &theta);            
            
            projectile_2D_v2( v_init, theta );  // result table included in the function  
            
        } else if ((argc == 6) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v2") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            v_init = atof( argv[ 4 ] );
            theta = atof( argv[ 5 ] );
            
            projectile_2D_v2( v_init, theta ); // result table included in the function   
            
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v3") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n"); 
            
            printf("Please enter a velocity (ft/s):\n");
            scanf("%lf", &v_init);
            printf("Please enter an angle (radians):\n");
            scanf("%lf", &theta);      
            
            projectile_2D_v3( v_init, theta, &t_final, &y_final, &v_y_final, &x_final, &v_x_final ); // result table not included in the function   
            
            // result table
            printf("Name                        Value\n");
            printf("==================================\n");
            printf("t_init (ft):             %8.4f\n", 0.0);
            printf("t_final (s):             %8.4f\n", t_final);
            printf("theta (radians):         %8.4f\n", theta);    
            printf("y_init (ft):             %8.4f\n", y_init);
            printf("y_final (ft):            %8.4f\n", y_final);
            printf("v_y_init (ft/s):         %8.4f\n", (v_init * sin(theta)) );
            printf("v_y_final (ft/s):        %8.4f\n", v_y_final);
            printf("x_init (ft):             %8.4f\n", 0.0);
            printf("x_final (ft):            %8.4f\n", x_final);
            printf("v_x_init (ft/s):         %8.4f\n", (v_init * cos(theta)));
            printf("v_x_final (ft/s):        %8.4f\n", v_x_final);
            printf("==================================\n");            
        } else if ((argc == 6) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v3") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            v_init = atof( argv[ 4 ] );
            theta = atof( argv[ 5 ] );
            
            projectile_2D_v3( v_init, theta, &t_final, &y_final, &v_y_final, &x_final, &v_x_final ); // result table not included in the function   
            
            // result table
            printf("Name                        Value\n");
            printf("==================================\n");
            printf("t_init (ft):             %8.4f\n", 0.0);
            printf("t_final (s):             %8.4f\n", t_final);
            printf("theta (radians):         %8.4f\n", theta);    
            printf("y_init (ft):             %8.4f\n", y_init);
            printf("y_final (ft):            %8.4f\n", y_final);
            printf("v_y_init (ft/s):         %8.4f\n", (v_init * sin(theta)) );
            printf("v_y_final (ft/s):        %8.4f\n", v_y_final);
            printf("x_init (ft):             %8.4f\n", 0.0);
            printf("x_final (ft):            %8.4f\n", x_final);
            printf("v_x_init (ft/s):         %8.4f\n", (v_init * cos(theta)));
            printf("v_x_final (ft/s):        %8.4f\n", v_x_final);
            printf("==================================\n");                  
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v4") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n");  
            
            printf("Please enter a velocity (ft/s):\n");
            scanf("%lf", &v_init);
            printf("Please enter an angle (radians):\n");
            scanf("%lf", &theta);  
            printf("Please enter the number of points:\n");            
            scanf("%lf", &n);
            
            projectile_2D_v4( v_init, theta, time, y_time, v_y_time, x_time, v_x_time, n ); // result table not included in the function
            
            // result table
            printf("Name                        Value\n");
            printf("========================================================================================\n");  
            printf("t_init (ft):             %8.4f\n", 0.0);
            printf("t_final (s):             %8.4f\n", time[n-1]);
            printf("theta (radians):         %8.4f\n", theta);    
            printf("y_init (ft):             %8.4f\n", y_init);
            printf("y_final (ft):            %8.4f\n", y_time[n-1]);
            printf("v_y_init (ft/s):         %8.4f\n", (v_init * sin(theta)) );
            printf("v_y_final (ft/s):        %8.4f\n", v_y_time[n-1]);
            printf("x_init (ft):             %8.4f\n", 0.0);
            printf("x_final (ft):            %8.4f\n", x_time[n-1]);
            printf("v_x_init (ft/s):         %8.4f\n", (v_init * cos(theta)));
            printf("v_x_final (ft/s):        %8.4f\n", v_x_time[n-1]);
            printf("========================================================================================\n\n");  
            printf("index      time (s)      x_time (ft)    v_x_time (ft/s)    y_time (ft)   v_y_time (ft/s)\n");
            printf("========================================================================================\n");  
            for (index = 0; index < n; index++) {
               printf("%d         %8.4f        %8.4f         %8.4f         %8.4f     %8.4f\n", 
                       index, time[index], x_time[index], v_x_time[index], y_time[index], v_y_time[index]);            
            }
            printf("========================================================================================\n");              
        } else if ((argc == 7) && (strcmpi(argv[ 1 ], "/2D") == 0) && (strcmpi(argv[ 2 ], "/v4") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            v_init = atof( argv[ 4 ] );
            theta = atof( argv[ 5 ] );
            n = atof( argv[ 6 ] );
            
            projectile_2D_v4( v_init, theta, time, y_time, v_y_time, x_time, v_x_time, n ); // result table not included in the function
            
            // result table
            printf("Name                        Value\n");
            printf("========================================================================================\n");       
            printf("t_init (ft):             %8.4f\n", 0.0);
            printf("t_final (s):             %8.4f\n", time[n-1]);
            printf("theta (radians):         %8.4f\n", theta);    
            printf("y_init (ft):             %8.4f\n", y_init);
            printf("y_final (ft):            %8.4f\n", y_time[n-1]);
            printf("v_y_init (ft/s):         %8.4f\n", (v_init * sin(theta)) );
            printf("v_y_final (ft/s):        %8.4f\n", v_y_time[n-1]);
            printf("x_init (ft):             %8.4f\n", 0.0);
            printf("x_final (ft):            %8.4f\n", x_time[n-1]);
            printf("v_x_init (ft/s):         %8.4f\n", (v_init * cos(theta)));
            printf("v_x_final (ft/s):        %8.4f\n", v_x_time[n-1]);
            printf("========================================================================================\n\n");       
            printf("index      time (s)      x_time (ft)    v_x_time (ft/s)    y_time (ft)   v_y_time (ft/s)\n");
            printf("========================================================================================\n");       
            for (index = 0; index < n; index++) {
               printf("%d         %8.4f        %8.4f         %8.4f         %8.4f     %8.4f\n", 
                       index, time[index], x_time[index], v_x_time[index], y_time[index], v_y_time[index]);            
            }
            printf("========================================================================================\n");     
        } else if ((argc == 3) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v1")) == 0) {            
            simple_supported_beam_v1(); // result table included in the function
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v2") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n");    
            
            printf("Enter in the load (lbs):\n");    
            scanf("%lf", &W);  
            printf("Enter in the length of the beam (ft):\n");
            scanf("%lf", &L);    
            printf("Enter in the location of the load (ft):\n");
            scanf("%lf", &a);    
            
            simple_supported_beam_v2( W, L, a ); // result table included in the function
            
        } else if ((argc == 7) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v2") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            W = atof( argv[ 4 ] );
            L = atof( argv[ 5 ] );
            a = atof( argv[ 6 ] );
            
            simple_supported_beam_v2(W, L, a); // result table included in the function
            
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v3") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n"); 
            
            printf("Enter in the load (lbs):\n");    
            scanf("%lf", &W);  
            printf("Enter in the length of the beam (ft):\n");
            scanf("%lf", &L);    
            printf("Enter in the location of the load (ft):\n");
            scanf("%lf", &a);    
            
            simple_supported_beam_v3( W, L, a, &R1, &R2 ); // result table not included in the function
            
            //result table
            printf("Name                    Value\n");
            printf("================================\n");
            printf("W (lbs):                %8.4f\n", W);
            printf("L (ft):                 %8.4f\n", L);
            printf("a (ft):                 %8.4f\n", a);
            printf("R1 (lbs):               %8.4f\n", R1);
            printf("R2 (lbs):               %8.4f\n", R2);
            printf("================================\n");            
        } else if ((argc == 7) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v3") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            W = atof( argv[ 4 ] );
            L = atof( argv[ 5 ] );
            a = atof( argv[ 6 ] );
            
            simple_supported_beam_v3( W, L, a, &R1, &R2 ); // result table not included in the function
            
            //result table
            printf("Name                    Value\n");
            printf("================================\n");
            printf("W (lbs):                %8.4f\n", W);
            printf("L (ft):                 %8.4f\n", L);
            printf("a (ft):                 %8.4f\n", a);
            printf("R1 (lbs):               %8.4f\n", R1);
            printf("R2 (lbs):               %8.4f\n", R2);
            printf("================================\n"); 
        } else if ((argc == 4) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v4") == 0) && (strcmpi(argv[ 3 ], "/i") == 0)) {  
            // capture y_init from user            
            printf("Using interactive mode\n"); 
            
            printf("Enter in the (distributed) load (lbs):\n");    
            scanf("%lf", &W);  
            printf("Enter in the length of the beam (ft):\n");
            scanf("%lf", &L);  
            printf("Enter in the number of points:\n");
            scanf("%d", &n);    
            
            a_delta = L / ( n - 1 );
            for (index = 0; index < n; index++) {
                W_array[index] = W;
                a_array[index] = index * a_delta;
            }
            
            simple_supported_beam_v4( W_array, L, a_array, R1_array, R2_array, n ); // result table not included in the function

            W = 0.0;
            R1 = 0.0;
            R2 = 0.0;    
            
            //result table
            printf("Index   W (lbs)         L (ft)         a (pts)      R1 (lbs)        R2 (lbs)\n");   
            printf("===========================================================================\n");
            for (index = 0; index < n; index++) {
                printf("%d      %8.4f       %8.4f       %8.4f       %8.4f       %8.4f\n", 
                        index, W_array[index], L, a_array[index], R1_array[index], R2_array[index]);
                W = W + W_array[index]; // sum all the loads
                R1 = R1 + R1_array[index]; // sum all the R1's
                R2 = R2 + R2_array[index]; // sum all the R2's        
            }
            printf("===========================================================================\n\n");
            printf("Name                    Value\n");
            printf("================================\n");
            printf("W (lbs):                %8.4f\n", W);
            printf("L (ft):                 %8.4f\n", L);
            // printf("a (ft):                 %8.4f\n", a);
            printf("R1 (lbs):               %8.4f\n", R1);
            printf("R2 (lbs):               %8.4f\n", R2);
            printf("================================\n");         
        } else if ((argc == 7) && (strcmpi(argv[ 1 ], "/beam") == 0) && (strcmpi(argv[ 2 ], "/v4") == 0) && (strcmpi(argv[ 3 ], "/p") == 0)) {  
            printf("Using parametric mode\n");
            
            W = atof( argv[ 4 ] );
            L = atof( argv[ 5 ] );
            n = atof( argv[ 6 ] );
            
            a_delta = L / ( n - 1 );
            for (index = 0; index < n; index++) {
                W_array[index] = W;
                a_array[index] = index * a_delta;
            }
            
            simple_supported_beam_v4( W_array, L, a_array, R1_array, R2_array, n ); // result table not included in the function

            W = 0.0;
            R1 = 0.0;
            R2 = 0.0;    
            
            //result table
            printf("Index   W (lbs)         L (ft)         a (pts)      R1 (lbs)        R2 (lbs)\n");   
            printf("===========================================================================\n");
            for (index = 0; index < n; index++) {
                printf("%d      %8.4f       %8.4f       %8.4f       %8.4f       %8.4f\n", 
                        index, W_array[index], L, a_array[index], R1_array[index], R2_array[index]);
                W = W + W_array[index]; // sum all the loads
                R1 = R1 + R1_array[index]; // sum all the R1's
                R2 = R2 + R2_array[index]; // sum all the R2's        
            }
            printf("===========================================================================\n\n");
            printf("Name                    Value\n");
            printf("================================\n");
            printf("W (lbs):                %8.4f\n", W);
            printf("L (ft):                 %8.4f\n", L);
            // printf("a (ft):                 %8.4f\n", a);
            printf("R1 (lbs):               %8.4f\n", R1);
            printf("R2 (lbs):               %8.4f\n", R2);
            printf("================================\n");            
        } else {
            help();
        }
    }    
    return (EXIT_SUCCESS);
}

/* 
 * Name: help(void)
 * Desc: Help menu
 * Args: void input
 */
int help(void){
    printf("==================================================\n");
    printf("File: Physics_Driver.c\n");
    printf("Copy: Copyright (c) 2021 Chattada Viriyaphap\n");
    printf("vERS: 1.0.0 11/22/2021 CV - Initial version\n");
    printf("Desc: Physics Calculator\n");
    printf("==================================================\n");
    printf("Usage:\n");
    printf("    /i - Interactive mode\n");
    printf("        /1D - One dimensional motion\n");
    printf("        /2D - Two dimensional motion\n");
    printf("        /beam - Beam problem\n");
    printf("    /p - Parametric mode\n");
    printf("        /1D - One dimensional motion\n");
    printf("        /2D - Two dimensional motion\n");
    printf("        /beam - Beam problem\n");
    printf("    /h - Help\n");
    printf("==================================================\n");
    printf("Examples:\n");
    printf("    Physics_Demo.exe /1D /v1\n");
    printf("    Physics_Demo.exe /1D /v2 /i\n");
    printf("    Physics_Demo.exe /1D /v2 /p 100.0\n");
    printf("    Physics_Demo.exe /1D /v3 /i\n");
    printf("    Physics_Demo.exe /1D /v3 /p 100.0\n");
    printf("    Physics_Demo.exe /1D /v4 /i\n");
    printf("    Physics_Demo.exe /1D /v4 /p 100.0 11\n");
    printf("    Physics_Demo.exe /2D /v1\n");
    printf("    Physics_Demo.exe /2D /v2 /i\n");
    printf("    Physics_Demo.exe /2D /v2 /p 100.0 0.785\n");
    printf("    Physics_Demo.exe /2D /v3 /i\n");
    printf("    Physics_Demo.exe /2D /v3 /p 100.0 0.785\n");
    printf("    Physics_Demo.exe /2D /v4 /i\n");
    printf("    Physics_Demo.exe /2D /v4 /p 100.0 0.785 11\n");
    printf("    Physics_Demo.exe /beam /v1\n");
    printf("    Physics_Demo.exe /beam /v2 /i\n");
    printf("    Physics_Demo.exe /beam /v2 /p 100.0 10.0 6.0\n");
    printf("    Physics_Demo.exe /beam /v3 /i\n");
    printf("    Physics_Demo.exe /beam /v3 /p 100.0 10.0 6.0\n");
    printf("    Physics_Demo.exe /beam /v4 /i\n");
    printf("    Physics_Demo.exe /beam /v4 /p 100.0 10.0 11\n");
}

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