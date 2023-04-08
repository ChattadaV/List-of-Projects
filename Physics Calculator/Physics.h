/* 
 * File:   Physics.h
 * Author: chattada
 *
 * Created on December 10, 2021, 11:14 PM
 */

#define a_y -32.2

void projectile_1D_v1(void);
int projectile_1D_v2(double y_init);
int projectile_1D_v3(double y_init, double *t_final, double *y_final, double *v_y_final);
int projectile_1D_v4(double y_init, double time[], double y_time[], double v_y_time[], int n);

void projectile_2D_v1(void);
int projectile_2D_v2(double v_init, double theta);
int projectile_2D_v3(double v_init, double theta, double *t_final, double *y_final, double *v_y_final, double *x_final, double *v_x_final);
int projectile_2D_v4(double v_init, double theta, double time[], double y_time[], double v_y_time[], double x_time[], double v_x_time[], int n );

void simple_supported_beam_v1(void);
int simple_supported_beam_v2(double W, double L, double a);
int simple_supported_beam_v3(double W, double L, double a, double *R1, double *R2);
int simple_supported_beam_v4(double W[], double L, double a[], double R1[], double R2[], int n);

