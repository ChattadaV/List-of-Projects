/* 
 * File: Fundamentals_Driver.c
 * Copy: Copyright (c) 2021 Chattada Viriyaphap
 * BlazerID: chattada
 * Vers: 1.0.0 11/23/2021
 * Desc: Driver for testing concepts 
 */ 

#include <stdio.h> // printf, scanf, definitions
#include <stdlib.h>
#include <string.h> // strcpy, strcmpi
#include <math.h> // math library
#include <time.h>

#define KMS_PER_MILE 1.609 // conversion constant
#define pi approximate_pi() // use function approximate_pi to define value for pi
#define n_max 11
#define a_y -32.2 // gravity acceleration

/* typedef declarations */
/* For typedefs_01 */
typedef struct {
    char orientation;
    int cylinders;
    char fuel[10];
} engine_type;
typedef struct {
    char make[10];
    char model[10];
    char color[10];
    int doors;
    int mileage;
    engine_type engine;
} auto_type;

/* For typedefs_02 */
typedef enum {
    unknown,
    filled_rectangle,
    hollow_rectangle,
    filled_circle,
    hollow_circle,
    filled_right_triangle          
} cross_section_enum;
typedef struct {
    cross_section_enum cross_section;
    char input_names[4][10];
    double input_values[4];
    double area;
    double perimeter;
    double x_bar;
    double y_bar;
    double I_x;
    double I_y;
} geometric_properties;

/* For typedefs_03 */
typedef enum {
    unknown_status,
    active,
    inactive
} status_enum;
typedef struct {    
    char user_id[10];
    char first_name[20];
    char last_name[20];
    status_enum status;
} user_properties;

/* For typedefs_04 */
typedef struct {
    int n;  
    double y_init; // initial height
    double v_y_init; // initial velocity
    double time[n_max];
    double y_time[n_max];
    double v_y_time[n_max];   
} projectile_1D_results;

// Function prototypes
int get_info(void); // print information about the user
int Figure_2_1(void); // converts from miles to kilometers
void arithmetic_operators(void);
void integer_division(void);
void integer_remainder(void);
void formatting_integers(void);
void formatting_doubles(void);
double approximate_pi(void);
void type_casts(void);
void interactive_01(void);
void parametric_01(const char first_name[], const char last_name[], const char course_department[], const int course_number);
void for_loops_01(void);
void for_loops_02(void);
int for_loops_03(int start, int increment, int end);
void while_loops_01(void);
void while_loops_02(void);
int rand_range(int min, int max );

void calculator_v1(void);
void calculator_v2(void);
double add_double(double first, double second);
double subtract_double(double first, double second);
double multiply_double(double first, double second);
double divide_double(double first, double second);
double help_calculator(void);

void typedefs_01(void);
void typedefs_02(void);
int update_struct( geometric_properties *example );
int display_struct( geometric_properties example );

void typedefs_03(int argc, char** argv);
int get_info_interactive( char user_id[], char first_name[], char last_name[], status_enum *status);
int get_user_properties_interactive( user_properties *example );
int get_user_properties_parametric( int argc, char** argv, user_properties *example );
int import_user_properties_file( FILE *input, user_properties *example ); // "get"
int export_user_properties_file( FILE *output, user_properties example ); // "set"
int display_user_properties( user_properties example ); 
void help_typedefs_03(int argc, char** argv);

/* For typedefs_04 */
int projectile_1D_v3(double y_init, double time[], double y_time[], double v_y_time[], int n);
int projectile_1D_v3_modified( projectile_1D_results *results );

void help(void);

// argc - count of the number of arguments
// argv - array of arguments from command line
// indexing is 0-based
/* 
 * Name: int main(int argc, char** argv)
 * Desc: Used to drive other functions for the purpose to demo features in
 * class.  Function calls are commented/uncommented depending on which features 
 * are to be tested.  
 */
int main (int argc, char** argv) {
    int index;
    char first_name[10]; // maxmimum 10-1 = 9 characters long
    char last_name[10];
    char course_department[3];
    int course_number;
    int start;
    int increment;
    int end;
    int results;
    
    printf("argc: %d\n", argc);
    for (index = 0; index < argc; index++) {
        printf("argv[ %d ] has the value of %s\n", index, argv[ index ]);
    }
    
    /* 
     * how to test what arguments are coming from the command line must have 
     * at least one argument --> name of the executable. Use an if statement to
     * determine which function to call. Use the function name as the command 
     * line argument (arbitrary).
     */
    if (argc < 2) {
        help(); // implicit call for help
    } else if ((strcmpi(argv[ 1 ], "/h") == 0) || (strcmpi(argv[1], "/?") == 0)) {
        help(); // explicit call for help
    } else if (strcmpi(argv[ 1 ], "/get_info") == 0) {
        get_info();
    } else if (strcmpi(argv[ 1 ], "/Figure_2_1") == 0) {
        Figure_2_1(); 
    } else if (strcmpi(argv[ 1 ], "/arithmetic_operators") == 0) {
        arithmetic_operators();
    } else if (strcmpi(argv[ 1 ], "/integer_division") == 0) {
        integer_division();
    } else if (strcmpi(argv[ 1 ], "/integer_remainder") == 0) {
        integer_remainder();
    } else if (strcmpi(argv[ 1 ], "/formatting_integers") == 0) { 
        formatting_integers();
    } else if (strcmpi(argv[ 1 ], "/formatting_doubles") == 0) {
        formatting_doubles();
    } else if (strcmpi(argv[ 1 ], "/type_casts") == 0) {
        type_casts();
    } else if (strcmpi(argv[ 1 ], "/interactive_01") == 0) {
        interactive_01();
    // parametric_01 with argc = 6 below comes before parametric_01 with argc = 2 because argc = 6 is more restrictive
    } else if ((strcmpi(argv[ 1 ], "/parametric_01") == 0) && (argc == 6)) {
        // strcpy --> string copy
        strcpy(first_name, argv[2]); // string copy argv[2] to variable first_name
        strcpy(last_name, argv[3]); // string copy argv[3] to last_name
        strcpy(course_department, argv[4]); // string copy argv[4] to course_department
        course_number = atoi(argv[5]); // atoi convert string to integer
        
        // call to output function
        parametric_01(first_name, last_name, course_department, course_number);
    } else if ((strcmpi(argv[ 1 ], "/parametric_01") == 0) && (argc == 2)) {
        // capture information from user
        printf("Please enter a first name:\n");
        scanf("%s", &first_name);
        
        printf("Please enter a last name:\n");
        scanf("%s", &last_name);
        
        printf("Please enter a course department:\n");
        scanf("%s", &course_department);
        
        printf("Please enter a course number:\n");
        scanf("%d", &course_number);
        
        // call to output function
        parametric_01(first_name, last_name, course_department, course_number);      
    } else if (strcmpi(argv[1], "/for_loops_01") == 0) {
        for_loops_01();
    } else if (strcmpi(argv[1], "/for_loops_02") == 0) {
        for_loops_02();
    } else if ((strcmpi(argv[1], "/for_loops_03") == 0) && (argc == 2)) { // 2 arguments in cmd (fundamentals_demo.exe, /for_loops_03)
        // assume the user wants to gather the information interactively
        printf("Please enter a starting integer:\n");
        scanf("%d", &start);
        
        printf("Please enter an increment integer:\n");
        scanf("%d", &increment);
        
        printf("Please enter an ending integer:\n");
        scanf("%d", &end);
        
        results = for_loops_03(start, increment, end);
        printf("Congratulations! Your loop ran %d times!\n", results);
    } else if (strcmpi(argv[1], "/while_loops_01") == 0) { 
        while_loops_01();
    } else if (strcmpi(argv[1], "/while_loops_02") == 0) { 
        while_loops_02(); // simple guessing game :)
    } else if (strcmpi(argv[1], "/calculator_v1") == 0) {
        calculator_v1();
    } else if (strcmpi(argv[1], "/calculator_v2") == 0) {
        calculator_v2();
    } else if (strcmpi(argv[1], "/typedefs_01") == 0) {
        typedefs_01();
    } else if (strcmpi(argv[1], "/typedefs_02") == 0) {
        typedefs_02();
    } else if (strcmpi(argv[1], "/typedefs_03") == 0) {
        typedefs_03(argc, argv);
    } else if (strcmpi(argv[1], "/typedefs_04") == 0) {
        // collect user information
        printf("Welcome to typedefs_04!\n");
        
        int n;
        double y_init;
        double time[n_max];
        double y_time[n_max];
        double v_y_time[n_max];
        projectile_1D_results sample_results; // declared but not initialized
        
        // n = n_max;
        n = 0;
        y_init = 0;
        
        // continue to prompt them for a valid number
        while (( n <= 0) || ( n > n_max )) {
            printf("Please enter the number of values (0 < n <= %d) you want to calculate.\n", n_max);
            scanf("%d", &n);
        }
        while (y_init <= 0.0) {
            printf("Please enter a positive height.\n");
            scanf("%lf", &y_init); // address of y_init
        }
        sample_results.n = n;
        sample_results.y_init = y_init;
        
        projectile_1D_v3(y_init, time, y_time, v_y_time, n);
        projectile_1D_v3_modified(&sample_results);
        
        // printing the tables
        printf("Name                                    Values\n");
        printf("==============================================\n");
        printf("y_init:                                 %.4f\n", y_init);
        printf("n:                                      %d\n", n);
        for (index = 0; index < n; index++) {
            printf("    time[ %d ]:                          %.4f\n", index, time[index]);
            printf("    y_time[ %d ]:                        %.4f\n", index, y_time[index]);
            printf("    v_y_time[ %d ]:                      %.4f\n", index, v_y_time[index]);
        }
        printf("==============================================\n\n");
        
        printf("Name                                    Values\n");       
        printf("==============================================\n");
        printf("y_init:                                 %.4f\n", sample_results.y_init);
        printf("n:                                      %d\n", sample_results.n);
        for (index = 0; index < n; index++) {
            printf("    time[ %d ]:                          %.4f\n", index, sample_results.time[index]);
            printf("    y_time[ %d ]:                        %.4f\n", index, sample_results.y_time[index]);
            printf("    v_y_time[ %d ]:                      %.4f\n", index, sample_results.v_y_time[index]);
        }        
        printf("==============================================\n\n");
    } else {
        help(); // implicit call for help  
    }    
    return(EXIT_SUCCESS);        
}

/* 
 * Name: void help(void)
 * Desc: prints out help
 * Args: no input/output
 */
void help(void) {
    printf("Usage:\n");
    printf("    fundamentals_demo.exe /h                         ... calls help\n");
    printf("    fundamentals_demo.exe /get_info                  ... calls get_info\n");
    printf("    fundamentals_demo.exe /Figure_2_1                ... calls Figure_2_1\n");
    printf("    fundamentals_demo.exe /arithmetic_operators      ... calls arithmetic_operators\n");
    printf("    fundamentals_demo.exe /integer_division          ... calls integer_division\n");
    printf("    fundamentals_demo.exe /integer_remainder         ... calls integer_remainder\n");
    printf("    fundamentals_demo.exe /formatting_integers       ... calls formatting_integers\n");
    printf("    fundamentals_demo.exe /formatting_doubles        ... calls formatting_doubles\n");
    printf("    fundamentals_demo.exe /type_casts                ... calls type_casts\n");
    printf("    fundamentals_demo.exe /interactive_01            ... calls interactive_01\n");
    printf("    fundamentals_demo.exe /parametric_01             ... calls parametric_01\n");
    printf("    fundamentals_demo.exe /parametric_01 Chattada Viriyaphap EE233  ... calls parametric_01 using cmd line args\n");
    printf("    fundamentals_demo.exe /for_loops_01              ... calls for_loops_01\n");
    printf("    fundamentals_demo.exe /for_loops_02              ... calls for_loops_02\n");   
    printf("    fundamentals_demo.exe /for_loops_03              ... calls for_loops_03\n");   
    printf("    fundamentals_demo.exe /for_loops_03 0 1 9        ... calls for_loops_03\n"); // count from 0 up to 9   
    printf("    fundamentals_demo.exe /while_loops_01            ... calls while_loops_01\n");
    printf("    fundamentals_demo.exe /while_loops_02            ... calls while_loops_02\n");
    printf("    fundamentals_demo.exe /calculator_v1             ... calls calculator_v1\n");
    printf("    fundamentals_demo.exe /calculator_v2             ... calls calculator_v2\n");
    printf("    fundamentals_demo.exe /typedefs_01               ... calls typedefs_01\n");
    printf("    fundamentals_demo.exe /typedefs_02               ... calls typedefs_02\n");
    printf("    fundamentals_demo.exe /typedefs_03               ... calls typedefs_03\n");
    printf("    fundamentals_demo.exe /typedefs_04               ... calls typedefs_04\n");
}


int get_info(void) {
    printf("Firstname: Chattada\n");
    printf("Lastname: Viriyaphap\n");
    printf("BlazerID: chattada\n");
    printf("Initials: CV\n");
    printf("Languages: MatLab, C/C++, Python\n");
    printf("Goals (Class): To be proficient in C language (and get an A!)\n");
    printf("Goals (Professional): Become an airline pilot\n");
    printf("Graduation: Spring 2024\n");
    printf("Computer Model: Dell Latitude 5510\n");
    printf("Computer CPU: i5-10210U @1.60 Ghz\n");
    printf("Computer Memory: 8 Gb (7.64 Gb Usable)\n");
    
    return (EXIT_SUCCESS);
}

/* 
 * Name: Figure_2_1 (from Figure_1_13)
 * Desc: Convert from miles to kilometers
 * Args: 
 *      miles - input
 *      kilometers - output
 */
int Figure_2_1(void) {
    double miles, kms; // input - miles, output - kilometers
    
    // get the distance in miles
    printf("Enter the distance in miles:\n");
    scanf("%lf", &miles);
    
    // convert the distance into kilometers
    kms = KMS_PER_MILE * miles;
    
    // display the distance in kilometers
    printf("That equals %.4f kilometers.\n", kms);
    
    return(EXIT_SUCCESS);
}

/* 
 * Name: arithmetic_operators(void)
 * Desc: Demonstration of arithmetic operators
 */
void arithmetic_operators(void) {
    int a_int;
    int b_int;
    double a_double;
    double b_double;
    
    a_int = 10;
    b_int = 3;
    a_double = 10.0;
    b_double = 3.0;
    
    printf("Fundamentals: Demonstration of Arithmetic Operators\n");
    printf("========================================\n");
    printf("%d + %d = %d\n", a_int, b_int, (a_int + b_int));    
    printf("%f + %f = %f\n", a_double, b_double, (a_double + b_double));
    printf("%d + %f = %f\n", a_int, b_double, (a_int + b_double));
    printf("%f + %d = %f\n", a_double, b_int, (a_double + b_int));
    
    printf("%d - %d = %d\n", a_int, b_int, (a_int - b_int));    
    printf("%f - %f = %f\n", a_double, b_double, (a_double - b_double));
    printf("%d - %f = %f\n", a_int, b_double, (a_int - b_double));
    printf("%f - %d = %f\n", a_double, b_int, (a_double - b_int));
    
    printf("%d * %d = %d\n", a_int, b_int, (a_int * b_int));    
    printf("%f * %f = %f\n", a_double, b_double, (a_double * b_double));
    printf("%d * %f = %f\n", a_int, b_double, (a_int * b_double));
    printf("%f * %d = %f\n", a_double, b_int, (a_double * b_int));
    
    printf("%d / %d = %d\n", a_int, b_int, (a_int / b_int));    
    printf("%f / %f = %f\n", a_double, b_double, (a_double / b_double));
    printf("%d / %f = %f\n", a_int, b_double, (a_int / b_double));
    printf("%f / %d = %f\n", a_double, b_int, (a_double / b_int));
    
    printf("%d %% %d = %d\n", a_int, b_int, (a_int % b_int)); // remainder of 10/3         
    
    printf("========================================\n");
}

/*
 * Name: void integer_division(void)
 * Desc: Demonstration of Results of Integer Division 
 */
void integer_division(void) {
    printf("Fundamentals: Results of Integer Division\n");
    printf("========================================\n");        
    
    // "normal" examples
    printf("%d / %d = %d\n", 3, 15, (3/15) );
    printf("%d / %d = %d\n", 4, 15, (4/15) );
    printf("%d / %d = %d\n", 5, 15, (5/15) );
    printf("%d / %d = %d\n", 6, 15, (6/15) );
    printf("%d / %d = %d\n", 7, 15, (7/15) );
    printf("%d / %d = %d\n", 8, 15, (8/15) );
    printf("%d / %d = %d\n", 14, 15, (14/15) );
    printf("%d / %d = %d\n", 15, 15, (15/15) );
    printf("%d / %d = %d\n", 15, 3, (15/3) );
    printf("%d / %d = %d\n", 16, 3, (16/3) );
    printf("%d / %d = %d\n", 17, 3, (17/3) );
    printf("%d / %d = %d\n", 18, 3, (18/3) );
    
    // negative examples
    printf("%d / %d = %d\n", 16, -3, (16/-3) );
    printf("%d / %d = %d\n", -16, 3, (-16/3) );
    printf("%d / %d = %d\n", -16, -3, (-16/-3) );
    
    printf("%d / %d = %d\n", 0, 4, (0/4) );
    
    // divide by zero // will crash the executable
    // printf("%d / %d = %d\n", 4, 0, (4/0) ); 
    
    printf("========================================\n");
}

/*
 * Name: void integer_remainder(void)
 * Desc: Demonstration of Results of % Operation 
 */
void integer_remainder(void) {
    printf("Fundamentals: Results of Integer Remainder\n");
    printf("========================================\n");    
    
    // "normal" examples
    printf("%d %% %d = %d\n", 3, 15, (3%15));
    printf("%d %% %d = %d\n", 4, 5, (4%5));
    printf("%d %% %d = %d\n", 5, 5, (5%5));
    printf("%d %% %d = %d\n", 6, 5, (6%5));
    printf("%d %% %d = %d\n", 7, 5, (7%5));
    printf("%d %% %d = %d\n", 8, 5, (8%5));
    printf("%d %% %d = %d\n", 5, 3, (5%3));
    printf("%d %% %d = %d\n", 5, 4, (5%4));
    printf("%d %% %d = %d\n", 15, 5, (15%5));
    printf("%d %% %d = %d\n", 15, 6, (15%6));
    
    // negative examples // will crash the executable
    printf("%d %% %d = %d\n", 15, -7, (15%-7));
    printf("%d %% %d = %d\n", -15, 7, (-15%7));
    printf("%d %% %d = %d\n", -15, -7, (-15%-7));
    
    // remainder by 0
    // printf("%d %% %d = %d\n", 15, 0, (15%0));
    
    printf("========================================\n");
}

/*
 * Name: void formatting_integers(void)
 * Desc: Demonstration of Displaying 234 and –234 Using Different Formatting
 */
void formatting_integers(void) {
    printf("Fundamentals: Examples of Formatting Type Integer Values\n");
    printf("========================================\n");

    printf("%4d\n", 234);
    printf("%5d\n", 234);
    printf("%6d\n", 234);
    printf("%1d\n", 234);
    
    printf("%4d\n", -234);
    printf("%5d\n", -234);
    printf("%6d\n", -234);
    printf("%2d\n", -234); 
    
    printf("========================================\n");    
}

/*
 * Name: void formatting_doubles(void)
 * Desc: Demonstration of Formatting Type double Values 
 */
void formatting_doubles(void) {     
    printf("Fundamentals: Examples of Formatting Type Double Values\n");
    printf("========================================\n");    
    printf("%f\n", pi);
    printf("%16f\n", pi);
    printf("%16.8f\n", pi);
    printf("%16.12f\n", pi);
    printf("%5.2f\n", pi);
    printf("%3.2f\n", pi);
    printf("%5.3f\n", pi);
    printf("%4.2f\n", .1234);        
    printf("========================================\n");    
}

/*
 * Name: double approximate_pi(void)
 * Desc: Calculate pi using the Gregory–Leibniz series
 */
double approximate_pi(void) {
    int count;
    int sign;
    double sum;
        
    sign = 1;
    sum = 0.0;
    
    for (count = 1; count <= 19999; count += 2) {
        sum = sum +  sign * (1 / ((double) count)); 
        /* Display the iteration number, the addendum and the running total.
         * printf("%d %f %f\n", count, (sign * (1 / ((double) count))), (4*sum) );
         */
        // 
        sign = -sign;        
    }
    sum = 4 * sum;
    return(sum);
}

/*
 * Name: void type_casts(void)
 * Desc: Demonstration of Examples of the Use of Type Casts 
 */
void type_casts(void) {
    int int_a;
    int int_b;
    double double_a;
    double double_b;
    char char_a;
    
    int_a = 5;
    int_b = 6;
    double_a = 5.0;
    double_b = 6.0;    
    char_a = 'G'; // single quote --> char
    
    printf("Fundamentals: Examples of Use of Type Casts\n");
    printf("========================================\n");    
    
    // changing the datatype of the value (not the variable)
    // int --> double
    printf("%d --> %f\n", 5, ((double) 5) );
    printf("%d --> %f\n", int_a, ((double) int_a) );
    printf("%d + %d --> %f\n", int_a, int_b, ((double) (int_a + int_b)) );
    
    // double --> int
    printf("%f --> %d\n", double_a, ((int) double_a) );
    printf("%f + %f --> %d\n", double_a, double_b, ((int) (double_a + double_b)) );
    
    int_a = (int) (double_a + double_b);
    printf("int_a: %d\n", int_a);
    int_b = (int) (double_b / double_a);
    printf("int_b: %d\n", int_b);
    
    int_b = (int) (5.0 / 3.0); // truncation
    printf("int_b: %d\n", int_b);
    
    printf("1.9999 --> %d\n", (int) 1.9999); // truncation
    
    // char --> int
    printf("%c --> %d\n", char_a, ((int) char_a)); // ASCII code for each character (decimation value for upper case G)
    
    // int --> char
    printf("%d --> %c\n", 97, ((char) 97)); // ASCII code for each character (decimation value for lower case a)
    char_a = (char) 97; // lowercase a
    
    printf("========================================\n");
}

/*
 * Name: void interactive_01(void)
 * Desc: Demonstration of scanf and printf interactively using strings
 */
void interactive_01(void) {
    // When allocating the size of a string, ALWAYS allocate one size LARGER than the largest piece
    char first_name[10]; // expecting length of first name string to be maximum 9 characters long
    char last_name[10]; // expecting length of last name string to be maximum 9 characters long
    char course_department[3]; // expecting length of course department string to be maximum 2 characters long
    int course_number;    

    // collect this information from the user
    printf("Please enter your first name (no more than 9 characters):\n");
    scanf("%s", &first_name);

    printf("Please enter your last name (no more than 9 characters):\n");
    scanf("%s", &last_name);

    printf("Please enter a course department (no more than 2 characters):\n");
    scanf("%s", &course_department);

    printf("Please enter a course number:\n");
    scanf("%d", &course_number);
        
    // print the output
    printf("Hello %s %s! Welcome to %s%d!\n", first_name, last_name, course_department, course_number);
}

/*
 * Name: void parametric_01(const char first_name[], const char last_name[], const char course_department[], const int course_number )
 * Desc: Demonstration of scanf and printf parametrically using strings
 */
void parametric_01(const char first_name[], const char last_name[], const char course_department[], const int course_number ) {
    // do some error checking
    printf("Hello %s %s! Welcome to %s%d!\n", first_name, last_name, course_department, course_number);  
}

/* 
 * Name: void for_loops_01(void)
 * Desc: Demonstration of different ways to use for loops
 */
void for_loops_01(void) {
    int index;
    double current;
    
    /* Syntax:
     *      initialization expression - where the loop starts
     *      loop repetition condition - must evaluate to true for the loop to continue
     *      update expression - how the loop increments/decrements

     *  for ( initialization expression; loop repetition condition; update expression)
     *  {   
     *      block of code/statements
     *      statement;
     *  }
     */ 
    
    // ascending values at increment of 1
    printf("index      value\n");
    printf("======================================\n");
    for (index=0; index<=9; index++) {
        printf("%d          %d\n", index, 2*index);
    }
    printf("======================================\n\n");
    
    // descending values at increment of -1
    printf("index      value\n");
    printf("======================================\n");
    for (index = 9; index >= 0; index--) {
        printf("%d          %d\n", index, 2*index);
    }
    printf("======================================\n\n");
    
    // ascending values at increment of to the power of 2
    printf("current      value\n");
    printf("======================================\n");
    for (current = 0.0; current <= 2.0; current = current + 0.1) {
        printf("%.2f          %.2f\n", current, pow(current, 2.0)); // 2nd value is current squared
    }
    printf("======================================\n\n");
    
    // example of empty for_loop
    printf("index      value\n");
    printf("======================================\n");
    for (index = 0; index < 0; index = index + 1) {
        printf("%d          %d\n", index, 2*index);        
    }
    printf("======================================\n");
}

/* 
 * Name: void for_loops_02(void)
 * Desc: Demonstration of different ways to use for loops
 */
void for_loops_02(void) {
    int index_outer;
    int index_inner;
    double current_outer;
    double current_inner;
    
    // find number of outer and inner loops then multiply it
    printf("index          value\n");
    printf("======================================\n");
    for (index_outer = 1; index_outer <= 12; index_outer++) {
        for (index_inner = 1; index_inner <= 10; index_inner++) {
            // use the indeces to fill out the multiplication table
            // index_inner * index_outer --> total number of times this code will run
            printf("%d, %d          %d\n", index_outer, index_inner, (index_outer * index_inner));
        }
    }
    printf("======================================\n\n");        
    
    // find number of outer and inner loops according to value of pi then multiply it
    printf("current          value\n");
    printf("======================================\n");
    for (current_outer = 0.0; current_outer < 10.0; current_outer++) {
        for (current_inner = 0.0; current_inner < (2 * M_PI); current_inner = current_inner + (M_PI / 4.0)) {
            printf("%.4f, %.4f          %.4f\n", current_outer, current_inner, (current_outer * current_inner));
        }
    }
    printf("======================================\n\n");    
    
    // making table of values of for-loop
    for (index_outer = 0; index_outer < 12; index_outer++) { // stops at 11
        for (index_inner = 0; index_inner < 9; index_inner++) { // stops at 8
            printf("%d,", ((index_outer + 1) * (index_inner + 1)));
        }
        printf("%d\n", ((index_outer + 1) * (index_inner + 1)));
    }
}

/* 
 * Name: int for_loops_03(int start, int increment, int end)
 * Desc: Demonstration of different ways to use for loops
 * Args:
 *  start - where the loop starts
 *  increment - how much the value changes each loop
 *  end - ending value (inclusive)
 *  returns number of iterations
 */
int for_loops_03(int start, int increment, int end) {
    int count;
    int index;
    
    count = 0; // to keep track of how many times the for-loop has run
    
    if (increment < 0) {
        if (end > start) {
            printf("Warning: Increment is negative but the end is greater than the start!\n");
        } else {
            // end <= start
            printf("index      value\n");
            printf("======================================\n");
            for(index = start; index >= end; index = index + increment){ // increment is negative
                printf("%d          %d\n", index, (2 * index));
                count++; // count = count + 1
            }
            printf("======================================\n");    
        }
    } else if (increment > 0) {
        if (end < start) {
            printf("Warning: Increment is positive but the end is less than the start!\n");
        } else {
            // end >= start
            printf("index      value\n");
            printf("======================================\n");
            for(index = start; index <= end; index = index + increment){ // increment is positive
                printf("%d          %d\n", index, (2 * index));
                count++; // count = count + 1
            }
            printf("======================================\n"); 
        }
    } else {
        // increment = 0 --> infinite loop because index never changes
        printf("Warning: Increment must be non-zero (or you will get an infinite loop)!\n");
    }
    return(count); // return how many times for-loop has run
}

/* 
 * Name: void while_loops_01(void)
 * Desc: Demonstration of different ways to use while loops
 */
void while_loops_01(void) {
    int index;
    double balance; // check card balance
    double purchase; // attempt to buy something
    int broke; // flag that represents a condition
    int count;
    
    /* Syntax:
     *      loop repetition condition - while this is true, the loop continue
     * - make sure to initialize the condition outside the while loop
     * - make sure the condition changes at some point in the while loop (or the loop will be infinite)
     * 
     *  while (loop repetition condition)
     * {    
     *      block of code/statements
     *      statement;
     * }      
     */
    
    // https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Standard_C_Library/Functions/srand    
    // https://en.wikibooks.org/wiki/C_Programming/time.h
    srand( time(NULL) );    
    
    // basic while loop
    index = 0; // start value
    printf("index      value\n");
    printf("======================================\n");
    while (index <= 9) {
        printf("%d          %d\n", index, (2 * index));
        index++; // index = index + 1
    }
    printf("======================================\n\n");
    
    // basic while loop with random number generator
    index = 0; // initialize value again
    printf("index      value\n");
    printf("======================================\n");
    while (index <= 9) {
        printf("%d          %d\n", index, rand());
        index++; // index = index + 1
    }
    printf("======================================\n\n");
    
    // basic while loop to track money spending
    balance = 1000;
    count = 0;
    printf("count      value            balance\n");
    printf("======================================\n");
    while (balance > 0.0) {
        // can purchase things
        purchase = rand() / 100.0; // convert an integer value to dollars/cents
        balance = balance - purchase;
        count++;
        printf("%d          %.2f            %.2f\n", count, purchase, balance);
    }
    printf("======================================\n\n");
    
    /*
     * while loop to track money spending using a flag that represents 
     * broke (stop buying) or not broke (keep buying) so we don't end up with 
     * negative balance at the end
    */
    balance = 1000;
    count = 0;
    broke = 0; // not broke
    printf("count      value            balance\n");
    printf("======================================\n");
    while (broke == 0) {
        purchase = rand() / 100.0;
        if (purchase < balance) {
            // can buy
            balance = balance - purchase;
            count++;
            printf("%d          %.2f            %.2f\n", count, purchase, balance);
        } else {
            // cannot buy because not enough money
            broke = 1;
        }
    }
    printf("======================================\n");
}

/* 
 * Name: void while_loops_02(void)
 * Desc: Simple game of guessing
 *      Demonstration an interactive way to use while loops
 *      
 */
void while_loops_02(void){
    int valid;
    int first;
    int second;
    int chosen; // chosen by computer
    int guess;
    int count_current;
    int count_max;
    
    valid = 0; // valid is false
    count_current = 0;
    count_max = 3; // consider asking user how many times they'd like to guess
    
    // srand(time(0));
    srand(time(NULL)); // Creates the seed for the random number generator.
    
    printf("Let's play a guessing game!  Pick two integers.\n");
    printf("I'll pick one in between them.\n");
    printf("I'll give you %d guesses to figure out my integer!\n", count_max);
    printf("Let's go!!!\n\n");
    
    printf("Please enter the minimum integer value\n");
    scanf("%d", &first);
    // make sure minimum and maximum value are different
    while (valid == 0) {
        printf("Please enter the maximum integer value\n");
        scanf("%d", &second);
        // make sure minimum and maximum value are different
        if (second > first) {
            // continue
            valid = 1;
        } else {
            valid = 0;
        }
    }
    // assume the user wants to continue
    chosen = rand_range(first, second);
    valid = 0; // the user has not yet picked the correct value
    while ((valid == 0) && (count_current < count_max)) {
        printf("Please guess the value that I have picked!\n");
        count_current++;
        printf("Try #%d of %d\n", count_current, count_max);
        scanf("%d", &guess);
        if (guess == chosen) { // guessed correctly, won the game
            printf("Congratulations! You guessed the correct number!\n");
            valid = 1;
            // loop terminates
        } else if (guess < chosen) { // guessed a wrong number (too low)
            printf("Nope! Number is too low!\n");
            // loop continues
        } else if (guess > chosen) { // guessed a wrong number (too high)
            printf("Nope! Number is too high!\n");
            // loop continues
        }
    }
    if (valid == 0) { // lost the game
        printf("You did not guess correctly within the number of tries :( the correct number is %d ... Wanna play again?\n", chosen);
    } else { // won the game
        printf("Wanna play again?\n");
    }
}

/* 
 * Name: int rand_range(int min, int max )
 * Desc: Demonstration of a random integer generator
 */
int rand_range(int min, int max ) {    
    return (min + rand() / (RAND_MAX / (max - min + 1) + 1)); // RAND_MAX --> find max value across all random integers in the list
}

/* 
 * Name: void calculator_v1(void)
 * Desc: Demonstration of top down design, control of flow and data validation
 */
void calculator_v1(void) {
    double first;
    double second;
    double results;
    int option;
    int valid; // use this as a flag to determine whether to continue

    valid = 0; // false 
    
    printf("Welcome to calculator_v1!\n");
    printf("===================================\n");
    printf("Allows the user to enter two values\n");
    printf("and an operation!\n");
    printf("===================================\n");
    printf("Options:\n");
    printf(" 1 - addition\n");
    printf(" 2 - subtraction\n");
    printf(" 3 - multiplication\n");
    printf(" 4 - division\n");
    printf("===================================\n");
    
    while (valid == 0) {
        printf("Please enter an option (1-4)\n");
        scanf("%d", &option);
        
        if ((option >= 1) && (option <= 4)) {
            valid = 1;
        } else {
            // prompt the user again --> run the loop again
            printf("Warning: Invalid option!\n");
        }
        
        // assume the user wants to continue
        valid = 0; // reset the flag
        
        printf("Please enter the first value:\n");
        scanf("%lf", &first); // percent L F
                
        while (valid == 0) {
            printf("Please enter the second value:\n");
            scanf("%lf", &second); // percent L F
            
            if ((option == 4) && (second == 0.0)) {
                printf("Please enter a non-zero value!\n");
            } else {
                // nothing to prompt the user
                valid = 1; // escape out of the while loop
            }
        }
       
        switch (option) {
            case 1:
                // addition
                results = add_double(first, second);
                printf("%.4f + %.4f = %.4f\n", first, second, results);
                break;
            case 2:
                // subtraction
                results = subtract_double(first, second);
                printf("%.4f - %.4f = %.4f\n", first, second, results);                
                break;
            case 3:
                // multiplication
                 results = multiply_double(first, second);
                printf("%.4f * %.4f = %.4f\n", first, second, results);               
                break;
            case 4:
                // division - assume a non-zero second value
                results = divide_double(first, second);
                printf("%.4f / %.4f = %.4f\n", first, second, results);                
                break;
            default:
                printf("Error: Unhandled option!\n");                
        }
    }
}

/* 
 * Name: void calculator_v2(void)
 * Desc: Demonstration of top down design, control of flow and data validation
 */
void calculator_v2(void) {       
    // https://en.wikipedia.org/wiki/Reverse_Polish_notation
    double first;
    double second;
    double results;
    int option;
    int valid; // flag
    int first_run; // the results from subsequent runs become the first value
    int done; // flag // allows the user to exit the application
    
    printf("Welcome to calculator_v2!\n");
    printf("===================================\n");
    printf("Allows the user to enter two values\n");
    printf("and an operation!\n");  
    
    valid = 0; // false
    first_run = 1; // true
    done = 0; // false // the user is not done until they specifically select exit
    
    help_calculator();
    
    while (done == 0) {
        if (first_run == 1) {
            // capture two values
            printf("Please enter the first value:\n");
            scanf("%lf", &first); // percent L F            
            printf("Please enter the second value:\n");
            scanf("%lf", &second); // percent L F
            first_run = 0; // after the first run, set this flag to false
        } else {
            // capture only the second value and resue the result from the previous run
            printf("Please enter the second value:\n");
            scanf("%lf", &second); // percent L F
        }

        valid = 0; // reinforce flag to false so while-loop below will run at least once
        while (valid == 0)  {
            printf("Please enter an option (1-4) or 9 - help or 0 - exit\n");
            scanf("%d", &option);    

            if (option == 1) {
                // addition
                results = add_double(first, second);
                printf("%.4f + %.4f = %.4f\n", first, second, results);
                first = results; // save it to variable first for next operation (option 2)         
                valid = 1;
/*
                printf("%.4f + %.4f = %.4f\n", first, second, add_doubles (first, second));
                // if no output is needed
                first = add_double(first, second);
                printf("%lf\n", first); // act as a result 
*/
            } else if (option == 2) {
                // subtraction
                results = subtract_double(first, second);
                printf("%.4f - %.4f = %.4f\n", first, second, results);
                first = results; // save it to variable first for next operation (option 3)
                valid = 1;
            } else if (option == 3) {
                // multiplication
                results = multiply_double(first, second);
                printf("%.4f * %.4f = %.4f\n", first, second, results);
                first = results; // save it to variable first for next operation (option 4)
                valid = 1;
            } else if (option == 4) {
                // division 
                while (second == 0.0) {
                    printf("Warning: Please enter a non-zero second value!\n");
                    scanf("%lf", &second);
                }
                results = divide_double(first, second);
                printf("%.4f / %.4f = %.4f\n", first, second, results);
                first = results;
                valid = 1;
            } else if (option == 9) {
                // call help
                help_calculator();
                // valid = 0; // reinforce the flag
            } else if (option == 0) {
                // allow the user to exit
                done = 1; // set flag to true // the user is done
                valid = 1;
            } else {
                // call help
                help_calculator();               
            }
        }
    }
}

/* 
 * Name: double help_calculator(void)
 * Desc: help menu for users
 * Args: no input or output args
 */
double help_calculator(void) {
    printf("===================================\n");
    printf("Options:\n");
    printf(" 1 - addition\n");
    printf(" 2 - subtraction\n");
    printf(" 3 - multiplication\n");
    printf(" 4 - division\n");
    printf(" 9 - help\n");
    printf(" 0 - exit\n");
    printf("===================================\n"); 
}

/* 
 * Name: double add_double(double first, double second)
 * Desc: add two doubles together
 * Args:
 *      input - first, second
 *      output - results
 */
double add_double(double first, double second) {
    double results;
    
    results = first + second;    
    return(results);
}

/* 
 * Name: double subtract_double(double first, double second)
 * Desc: subtract two doubles
 * Args:
 *      input - first, second
 *      output - results
 */
double subtract_double(double first, double second) {
    double results;
    
    results = first - second;
    return(results);    
}

/* 
 * Name: double multiply_double(double first, double second)
 * Desc: multiply two doubles
 * Args:
 *      input - first, second
 *      output - results
 */
double multiply_double(double first, double second) {
    double results;
    
    results = first * second;
    return(results);
}

/* 
 * Name: double divide_double(double first, double second)
 * Desc: divide two doubles
 * Args:
 *      input - first, second
 *      output - results
 */
double divide_double(double first, double second) {
    double results;
    
    if (second != 0.0) { // if second value is NOT equal to zero
        // proceed with calculation
        results = first / second;
    } else {
        // assign a value to the results variable - discussion
        results = 0.0; // not mathematically correct, but at least it's executable
        // results = (double) NULL;        
    }
    return(results);
}

/* 
 * Name: void typedefs_01(void)
 * Desc: driver for the creation, updating and displaying of a struct
 */
void typedefs_01(void) {
    auto_type current_auto;
    auto_type *current_auto_pointer;
    
    char make[10];
    char model[10];
    char color[10];
    int doors;
    int mileage;
    int cylinders;
    char orientation;
    char fuel[10];
    char extra;
    
    printf("Please enter a make:\n");
    scanf("%s", &make);
    
    printf("Please enter a model:\n");
    scanf("%s", &model);
    
    printf("Please enter a color:\n");
    scanf("%s", &color);
    
    printf("Please enter the number of doors (zero or more):\n");
    scanf("%d", &doors);
    
    printf("Please enter the mileage (zero or more):\n");
    scanf("%d", &mileage);
    
    printf("Please enter the number of cylinders (zero or more):\n");
    scanf("%d", &cylinders);
    scanf("%c", &extra ); // captures the "enter" that was provided after the integer // for when pressing enter of above scanf. That way scanf below (orientation) is not saved as enter key
    
    printf("Please enter the orientation (I, B, V, R, O)\n");
    scanf("%c", &orientation );
    
    printf("Please enter the fuel:\n");
    scanf("%s", &fuel);
    
    strcpy(current_auto.make, make); // string copy
    strcpy(current_auto.model, model);
    strcpy(current_auto.color, color);
    current_auto.doors = doors;
    current_auto.mileage = mileage;
    
    current_auto.engine.cylinders = cylinders;
    current_auto.engine.orientation = orientation; // note: no strcpy needed.
    strcpy(current_auto.engine.fuel, fuel);    
    
    current_auto_pointer = &current_auto;
    
    printf("Description                                 Value\n");
    printf("===============================================================\n");
    printf("current_auto.make                           %s\n", current_auto.make);
    printf("current_auto.model                          %s\n", current_auto.model);
    printf("current_auto.color                          %s\n", current_auto.color);
    printf("current_auto.doors                          %d\n", current_auto.doors);
    printf("current_auto.mileage                        %d\n", current_auto.mileage);
    printf("current_auto.engine.cylinders               %d\n", current_auto.engine.cylinders);
    printf("current_auto.engine.orientation             %c\n", current_auto.engine.orientation);
    printf("current_auto.engine.fuel                    %s\n", current_auto.engine.fuel);
    printf("\n");
        
    printf("address of current_auto                     %p\n", &current_auto);
    printf("address of current_auto.make                %p\n", &current_auto.make);
    printf("address of current_auto.model               %p\n", &current_auto.model);
    printf("address of current_auto.color               %p\n", &current_auto.color);
    printf("address of current_auto.doors               %p\n", &current_auto.doors);
    printf("address of current_auto.mileage             %p\n", &current_auto.mileage);
    printf("address of current_auto.engine              %p\n", &current_auto.engine);
    printf("address of current_auto.engine.orientation  %p\n", &current_auto.engine.orientation); 
    printf("address of current_auto.engine.cylinders    %p\n", &current_auto.engine.cylinders); 
    printf("address of current_auto.engine.fuel         %p\n", &current_auto.engine.fuel);    
    printf("\n");
    
    printf("value of current_auto_pointer               %p\n", current_auto_pointer);
    printf("address of current_auto_pointer             %p\n", &current_auto_pointer);
    printf("\n");
    
    printf("size of current_auto                        %d\n", sizeof(current_auto));
    printf("size of current_auto engine                 %d\n", sizeof(current_auto.engine));    
    printf("===============================================================\n");           
}

/* 
 * Name: void typedefs_02(void)
 * Desc: driver for the creation, updating and displaying of a struct
 */
void typedefs_02(void) {
    geometric_properties example_01;
    geometric_properties example_02;
    
    update_struct(&example_01);
    update_struct(&example_02);
    
    // handle a circular cross section with radius 1.0
    example_01.cross_section = filled_circle;
    strcpy(example_01.input_names[0], "radius");
    example_01.input_values[0] = 1.0;
    
    // handle a rectangular cross-section with a length of 1.0 and a width of 2.0
    example_02.cross_section = filled_rectangle;
    strcpy(example_02.input_names[0], "length");
    strcpy(example_02.input_names[1], "width");
    example_02.input_values[0] = 1.0;
    example_02.input_values[1] = 2.0;    
    
    display_struct(example_01);
    display_struct(example_02);
    
    
    
}

/* 
 * Name: int update struct (geometric_properties *example)
 * Desc: update a struct by ref
 */
int update_struct( geometric_properties *example ) {
    // initialize all values
    example->cross_section = unknown;
    example->perimeter = 0.0;
    example->area = 0.0;
    example->I_x = 0.0;
    example->I_y = 0.0;
    example->x_bar = 0.0;
    example->y_bar = 0.0;
    
    // initialize the parallel arrays that correspond to the name/value (ex. radius = 1.0)
    strcpy(example->input_names[0], "");
    strcpy(example->input_names[1], "");
    strcpy(example->input_names[2], "");
    strcpy(example->input_names[3], "");
    
    example->input_values[0] = 0.0;
    example->input_values[1] = 0.0;
    example->input_values[2] = 0.0;
    example->input_values[3] = 0.0;
    
    return( EXIT_SUCCESS );
}


/* 
 * Name: int display struct (geometric_properties example)
 * Desc: display the fields consistently
 */
int display_struct( geometric_properties example ) { 
    int index;
    
    printf("Description                                     Value\n");
    printf("===============================================================\n"); 
    
    if (example.cross_section == filled_circle) {
        // only the radius (one value)
        printf("Cross_Section_Name                            filled_circle\n");
        printf("%s                                          %.4f\n", example.input_names[0], example.input_values[0]);        
    } else if (example.cross_section == filled_rectangle) {
        // more than one value
        printf("Cross_Section_Name                            filled_rectangle\n");        
        printf("%s                                          %.4f\n", example.input_names[0], example.input_values[0]);        
        printf("%s                                           %.4f\n", example.input_names[1], example.input_values[1]);
    } else if (example.cross_section == hollow_rectangle) {
        // four values
        printf("Cross_Section_Name                            hollow_rectangle\n");
        for (index = 0; index <= 3; index++) {
            printf("%s                                     %.4f\n", example.input_names[index], example.input_values[index]);            
        }
    } else if (example.cross_section == hollow_circle) {
        printf("Cross_Section_Name                            hollow_circle\n");
        for (index = 0; index <= 1; index++) {
            printf("%s                                     %.4f\n", example.input_names[index], example.input_values[index]);            
        }
    } else if (example.cross_section == filled_right_triangle) {
        printf("Cross_Section_Name                            filled_right_triangle\n");
        for (index = 0; index <= 1; index++) {
            printf("%s                                    %.4f\n", example.input_names[index], example.input_values[index]);            
        }                
    } else {
        // unknown cross section. Decide whether to print out values
        printf("Cross_Section_Name                       unknown\n");        
    }
    
    printf("Cross_Section Enum                              %d\n", example.cross_section);
    printf("Perimeter                                       %.4f\n", example.perimeter);
    printf("Area                                            %.4f\n", example.area);
    printf("I_x                                             %.4f\n", example.I_x);
    printf("I_y                                             %.4f\n", example.I_y);
    printf("x_bar                                           %.4f\n", example.x_bar);
    printf("y_bar                                           %.4f\n", example.y_bar);   
    printf("===============================================================\n\n");    
    
    return( EXIT_SUCCESS );
}

/* 
 * Name: void typedefs_03(int argc, char** argv)
 * Desc: driver for the creation, updating and displaying of a struct
 * Args: int argc - commandline argument count
 *      char** argv - commandline arguments
 */
void typedefs_03(int argc, char** argv) {
    int index;
    char user_id[10];
    char first_name[20];
    char last_name[20];
    status_enum status;
    user_properties example_01;
    user_properties example_02;
    FILE *output; // output file pointer
    FILE *input;
        
    printf("argc: %d\n", argc);
    for (index = 0; index < argc; index++ ) {
        printf("argv[ %d ] has the value %s.\n", index, argv[ index ]);
    }    
    
    printf("unknown:  %d\n", unknown); // matches to 0
    printf("active:   %d\n", active); // matches to 1
    printf("inactive: %d\n", inactive); // matches to 2
    
    if (argc < 3) {
        help_typedefs_03( argc, argv ); // implicit
    } else if (strcmpi( argv[ 2 ], "/h") == 0) {
        help_typedefs_03( argc, argv ); // explicit
    } else if ((argc == 3) && (strcmpi( argv[ 2 ], "/get_info_interactive") == 0)) {
        get_info_interactive(user_id, first_name, last_name, &status); // don't need & for the first 3 variables because their values will be erased, but value for status needs to be passed on
        
        printf("===============================================================\n");
        printf("user_id:                                           %s\n", user_id);
        printf("first_name:                                        %s\n", first_name);
        printf("last_name:                                         %s\n", last_name);
        printf("status:                                            %d\n", status);
        printf("===============================================================\n");
    } else if ((argc == 3) && (strcmpi( argv[ 2 ], "/get_user_properties_interactive") == 0)) {
        get_user_properties_interactive(&example_01);
        display_user_properties(example_01);
    } else if ((argc == 7) && (strcmpi( argv[ 2 ], "/get_user_properties_parametric") == 0)) {
        get_user_properties_parametric(argc, argv, &example_01);
        display_user_properties(example_01);
        
        output = fopen("example_01.csv", "w"); // overwrite the file
        export_user_properties_file(output, example_01);
        fclose(output);
    } else if ((argc == 4) && (strcmpi( argv[ 2 ], "/import_user_properties_file") == 0)) {
        input = fopen("example_01.csv", "r"); // read the file
        import_user_properties_file(input, &example_01);        
        fclose(input);
        
        display_user_properties(example_01);
        example_01.status = inactive;
        
        output = fopen("example_01_modified.csv", "w"); // write the file
        export_user_properties_file(output, example_01);
        fclose(output);      
        
        strcpy(example_02.user_id, example_01.user_id);
        strcpy(example_02.first_name, example_01.first_name);
        strcpy(example_02.last_name, example_01.last_name);
        example_02.status = example_01.status;
        
        output = fopen("log.csv", "a"); // append file
        export_user_properties_file(output, example_02);
        fclose(output);
    } else {
        help_typedefs_03( argc, argv ); // implicit
    }
}

/*
 * Name: void help_typedefs_03(void)
 * Desc: Specific help for typedefs_03.
 */
void help_typedefs_03(int argc, char** argv) {
    printf("Usage:\n");
    printf("    %s %s /h                                                       ... calls help\n", argv[ 0 ], argv[ 1 ]);    
    printf("    %s %s /get_info_interactive                                    ... uses individual variables\n", argv[ 0 ], argv[ 1 ]);    
    printf("    %s %s /get_user_properties_interactive                         ... uses a struct\n", argv[ 0 ], argv[ 1 ]);    
    printf("    %s %s /get_user_properties_parametric chattada Chattada Viriyaphap 1   ... uses a struct\n", argv[ 0 ], argv[ 1 ]);
    printf("    %s %s /import_user_properties_file example_01.csv              ... uses a struct\n", argv[ 0 ], argv[ 1 ]);            
}

/* 
 * Name: int get_info( char user_id[], char first_name[], char last_name[], status_enum status)
 * Desc: get the info using printf/scanf
 * Args: char user_id[], char first_name[], char last_name[] - all to be modified
 *      status_enum *status - to be modified
 *      int - success/failure
 */
int get_info_interactive( char user_id[], char first_name[], char last_name[], status_enum *status) {
    printf("Please enter a user_id:\n");
    scanf("%s", user_id); // no & needed for user_id of scanf because it's a passed by reference
    
    printf("Please enter a first_name:\n");
    scanf("%s", first_name);
    
    printf("Please enter a last_name:\n");
    scanf("%s", last_name);
    
    printf("Please enter the status (0 = unknown, 1 = active, 2 = inactive)\n");
    scanf("%d", status);
    
    return( EXIT_SUCCESS );
}


/* 
 * Name: int get_user_properties_interactive( user_properties *example )
 * Desc: get the user properties using printf/scanf
 * Args: user_properties *example - to be modified
 *      int - success/failure
 */
int get_user_properties_interactive( user_properties *example ) { 
    printf("Please enter a user_id:\n");
    scanf("%s", &example->user_id); // passed by address so & is needed for scanf
    
    printf("Please enter a first_name:\n");
    scanf("%s", &example->first_name);
    
    printf("Please enter a last_name:\n");
    scanf("%s", &example->last_name);    
    
    printf("Please enter the status (0 = unknown, 1 = active, 2 = inactive)\n");
    scanf("%d", &example->status);        
    
    return( EXIT_SUCCESS );
}

/* 
 * Name: int get_user_properties_parametric( int argc, char** argv, user_properties *example )
 * Desc: get the user properties using commandline arguments
 * Args: int argc - commandline argument count
 *      char** argv - commandline arguments
 *      user_properties *example - to be modified
 *      int - success/failure
 */
int get_user_properties_parametric( int argc, char** argv, user_properties *example ) {
    strcpy(example->user_id, argv[3]);
    strcpy(example->first_name, argv[4]);
    strcpy(example->last_name, argv[5]);
    example->status = atoi(argv[6]); // atoi convert string to integer
    
    return( EXIT_SUCCESS );
}

/* 
 * Name: int import_user_properties_file( FILE *input, user_properties *example )
 * Desc: allows us to "get" the user properties from a file.
 * Args: FILE *input - input file handle
 *      user_properties *example - to be modified
 *      int - success/failure
 */
int import_user_properties_file( FILE *input, user_properties *example ) {
    int results;
    
    results = EXIT_SUCCESS;
    
    if (input != NULL) {
        fscanf(input, "%[^,], %[^,], %[^,], %d", // %[^,] to select any characters except the comma (comma is the separator) // "%s, %s, %s, %d" // fscanf to make a call to scanf
                &example->user_id, 
                &example->first_name, 
                &example->last_name, 
                &example->status);
    } else {
        printf("Error: Failed to access file!\n");
        results = EXIT_FAILURE;
    }    
    return( EXIT_SUCCESS );
}

/* 
 * Name: int export_user_properties_file( FILE *output, user_properties example )
 * Desc: allows us to "set" the user properties in a file.
 * Args: FILE *output - output file handle
 *      user_properties example - byval struct for "reading" only 
 *      int - success/failure
 */
int export_user_properties_file( FILE *output, user_properties example ) {
    int results;
    
    results = EXIT_SUCCESS;
    if (output != NULL) {
/*
        fprintf(output, "user_id:       %s\n", example.user_id); 
        fprintf(output, "first_name:    %s\n", example.first_name);
        fprintf(output, "last_name:     %s\n", example.last_name);
        fprintf(output, "status:        %d\n", example.status);
*/
        // fprintf to print string to a specific file. printf to print string to screen
        fprintf(output, "%s, %s, %s, %d\n",
                example.user_id, 
                example.first_name, 
                example.last_name, 
                example.status);        
    } else {
        printf("Error: Unable to access file!\n");
        results = EXIT_FAILURE;
    }    
    return( EXIT_SUCCESS );
}

/* 
 * Name: int display_user_properties( user_properties example )
 * Desc: allows us to display to the console the properties of the user
 * Args: user_properties example - byval struct for "reading" only 
 *      int - success/failure
 */
int display_user_properties( user_properties example ) {
    printf("=============================================================\n");
    printf("user_id:                                %s\n", example.user_id);
    printf("first_name:                             %s\n", example.first_name);
    printf("last_name:                              %s\n", example.last_name);
    printf("status:                                 %d\n", example.status);
    printf("=============================================================\n");
    
    return( EXIT_SUCCESS );
}

/*
 * Name: int projectile_1D_v3(double y_init, double time[], double y_time[], double v_y_time[], int n)
 * Desc: Provided a y_init (byval), calculate the time, y_time and v_y_time vectors (byref) of length n (byval).
 * Args: y_init - initial height
 *      double time - time vector
 *      double y_time - position/height vector
 *      double v_y_time - velocity vector
 *      n - number of elements used in the parallel arrays
 *      int - success/failure      
 */
int projectile_1D_v3(double y_init, double time[], double y_time[], double v_y_time[], int n) {
    int index;
    double t_final;
    double t_delta; // time interval
    int results;
    
    results = EXIT_SUCCESS;
    
    if ((n <= n_max) && (y_init > 0.0)) {
        t_final = sqrt( ( -2 * y_init) / a_y );
        t_delta = ( t_final - 0.0) / ( n - 1 );
        for (index = 0; index < n; index++) {
            time[index] = t_delta * index;
            y_time[index] = y_init + ( 0.5 * a_y * pow(time[index], 2.0)); // v_y_init = 0.0;
            v_y_time[index] = a_y * time[index]; // v_y_init = 0.0;            
        }
    } else {
        results = EXIT_FAILURE;
        printf("Error: Initial conditions incorrect!\n");
    }
    return(results);    
}

/*
 * Name: int projectile_1D_v3_modified( projectile_1D_results *results )
 * Desc: accomplishes the same as above, yet uses a structure passed byref/address
 * Args: projectile_1D_results *results - structure passed byref (see above) with n_max elements in parallel arrays
 *      int - success/failure
 */
int projectile_1D_v3_modified( projectile_1D_results *results ) {
    int index;
    int return_value;
    double t_final;
    double t_delta;
    
    return_value = EXIT_SUCCESS; // zero
    
    if (( results->n <= n_max ) && ( results->y_init > 0.0 )) {
        t_final = sqrt(( -2 * results->y_init) / a_y );
        t_delta = ( t_final - 0.0 ) / ( results-> n -1 );
        for (index = 0; index < ( results->n ); index++) {
            results->time[index] = t_delta * index;
            results->y_time[index] = results->y_init + ( 0.5 * a_y * pow( results->time[index], 2.0 )); // v_y_init = 0.0;
            results->v_y_time[index] = a_y * results->time[index]; // v_y_init = 0.0;
        }
    }    
    return(return_value);
}
