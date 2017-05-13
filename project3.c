
/* 
 * Suyi liu
 * sliu92@jhu.edu
 * 600.120 intermediate programming
 * Project 3 driver class
 * incorporated command line arguments
 * If no input, program still runs.
 * set default value for max range as 100
 * default value for num op as 1000000
 * default value for seed as 1
 * default value for probability as 50 percent
 * also printing is modified.
 * if max range is smaller than 100,
 * print whole list if number of operation
 * is less than 1000000
 * if larger than 1000000
 * only print the list of unique records.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "sl_repository.h"

unsigned long int next =1;

/*Static global integer variable of largest range of key value.*/
static int Max_range;

/*Static global integer variable of number of operations.*/
static int num_op;

/*Static global integer variable of seed.*/
static unsigned int s;

/*Static global integer variable of probability percentage.*/
static int probability;

/*Method for command line arguments.*/
void Usage(int argc, char *argv[]); 

void Get_next_op( char *random_op, int *random_int, int range );
int  rand( void ); 
void srand( unsigned int seed );

/*
 * Main program.
 * Randomly generates operations, approximately half getting
 * half deleting, a quarter deleting, a quarter updating
 * @param argc argument counter
 * @param *argv[] argument stings pointers
 * Count how many operations of each operation so far
 * and print out list according to num op and range
 *
 */
main( int argc, char *argv[] )
{    
     /*Call usage method which stores input into string memories.*/
     Usage(argc, argv);

     printf("\nGenerating %d operations with range %d and random seed %d \n", num_op, Max_range, s);
 
    int    rand_key; 
    char   rand_op;
    int    ret; 
    int           ret_data;
    int    i;

    unsigned int sum_data;
    int num_get, num_delete, num_update;
    
    /*Main print instructions are commented out.*/
    printf("-------------------------------------------------------------------------\n");
   
    srand( s );

    sum_data   = 0;

    num_get    = 0;
    num_delete = 0;
    num_update = 0;
   
    /*Initialize the list according to probability value.*/
    Repository_init( probability );

    for( i = 1; i <= num_op; i++ )
    {
        Get_next_op( &rand_op, &rand_key, Max_range );


        switch( rand_op )
        {
            case 'G':
                num_get++;
                ret = Repository_get( rand_key, &ret_data );
                if( ret < 0 ) {
                    printf("\nError in Get with return value %d \n", ret );
                }
                if( ret == 1 ) sum_data = sum_data + (ret_data%100);
                break;

            case 'U':
                num_update++;
                ret = Repository_update( rand_key, i );
                if( ret < 0 ) {
                    printf("\nError in Update with return value %d \n", ret );
                }
                break;

            case 'D':
                num_delete++;
                ret = Repository_delete( rand_key );
                if( ret < 0 ) {
                    printf("\nError in Delete with return value %d \n", ret );
                }
                break;

            default:
                printf("\nError!!!, Illegal operation %c\n", rand_op );
                return 0;
        }

        if( (i % (num_op/10)) == 0 )
        {
            printf("Printing after %d operations\n", i );
            if( Max_range > 100 ) 
            {
                Repository_print( 0 );
            } else {
                if (num_op < 1000000) {
                    Repository_print( 2 );
                } else {
                    Repository_print( 1 );
                }
            }
        }
    }
    printf("Operation summary: get %d, delete %d, update %d\n", num_get, num_delete, num_update );
    printf("Sum of all Repository_get data modulo 100 is %d\n", sum_data );
}

/*
 * The method reads user input.
 * @param argc argument counter
 * @argv[] pointer to argument strings
 * It reads user input and sets
 * max range, seed, probability and
 * operations to certain values
 *
 */
void Usage(int argc, char *argv[])
{
    /* Setting defaults */
    Max_range = 100;
    num_op  = 1000000;
    s = 1;
    probability = 50;

    /* first argument is the name of the executable */
    while( --argc > 0 )
    {   
        /*Get rid of first string.*/
        argv++;

        if( !strncmp( *argv, "-r", 2 ) )
        {   /*Store value after -v into max range.*/
            sscanf(argv[1], "%d", &Max_range );
            argc--; argv++;
        }else if( !strncmp( *argv, "-o", 2 ) ){
            /*Store value after -o into num op.*/
            sscanf(argv[1], "%d", &num_op );
            argc--; argv++;
        }else if( !strncmp( *argv, "-s", 2 ) ){
            /*Store value after -s into seed.*/
            sscanf(argv[1], "%u", &s );
            argc--; argv++;
        }else if( !strncmp( *argv, "-p", 2 ) ){
            /*Store value after -p into probability.*/
            sscanf(argv[1], "%d", &probability );
            argc--; argv++;
        }else{
            /*Print out menu if input is other thing.*/
            printf( "Usage: \n%s\n%s\n%s\n%s\n%s\n%s\n",
                "\t[-r <max range>]   : max range, default is 100",
                "\t[-o <operation>]   : number of operations, default is 1000000",
                "\t[-s <seed>]        : seed for random generator, default 1",
                "\t[-p <probability>] : probability percentage, default is 50");            exit( -1 );
        }
    }
}

void Get_next_op( char *random_op, int *random_int, int range )
{
    int  random_number;
    int  limit;

    random_number = rand() % 4;

    switch( random_number )
    {
        case 0:
        case 1:
                *random_op = 'G';
                break;
        case 2:
                *random_op = 'D';
                break;
        case 3:
                *random_op = 'U';
    }

    limit = (32768 / range ) * range;
    while( ( random_number = rand() ) >= limit ) ;

    *random_int = random_number % range + 1;
}

/* Pseudo random generator on 0..32767 */
int  rand(void)
{
    next = next *1103515245 + 12345;
    return (unsigned int)(next/65536)%32768;
}

void srand( unsigned int seed )
{
    next = seed;
}

