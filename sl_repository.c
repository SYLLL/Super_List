
/* Suyi Liu
 * sliu92@jhu.edu
 * Project3
 * 600.120 Intermediate Programming
 * Constructs a superlist with each
 * element consisting of a unique key,
 * an associated data pointer, a pointer
 * to an integer representing repetitions,
 * and a next pointer as well as a down 
 * pointer.
 * I have 
 * Update, delete, get and print are
 * performed.
 */
#include <stdio.h>
#include <stdlib.h>
#include "sl_repository.h"
#include "rand100.h"

/*
 * Constructor of single element in superlist.
 * k represents unique key
 * v is a pointer pointing to associated data
 * r is a pointer pointing to how many times 
 * the element repeats in the superlist
 * next is a pointer pointing to its next
 * element
 * down is a pointer pointing to the itself
 * in the lower level
 */
typedef struct dummy_element {
    int k;
    int *v;
    int *r;
    struct dummy_element *next;
    struct dummy_element *down;
} element;

/*
 * The dummy head in each of the levels.
 */
static element *Dummy_head;

/*
 * The counter counting how much effort taken.
 * so far.
 */
static unsigned long int counter;

/*
 * The percentage of elements in the upper.
 * level divided by how many elements in the
 * lower level.
 */
static int prob;

/*
 * Number of unique elements in the list.
 */
static int size;

/*
 * Number of levels in the super list.
 */
static int arraysize;

/*
 * The max number of levels.
 */
static int maximun;

/*
 * Initialize somw integer variables.
 * ind is the index of array of pointers.
 * in, j are indices of array of ints.
 * k is a counter in print method.
 * result is what is going to be returned
 * in delete method.
 */
int ind, in, j, k, result;

/*
 * Initialize the super list.
 * @param p the probability.
 * set size, counter to 0
 * set arraysize to 1
 * malloc some memoty for Dummy_head
 */
void Repository_init( int p ) {

    prob = p;
    size = 0;
    counter = 0;
    arraysize = 1;
    maximun = 30;
    /*Make the initial Dummy_head.*/
    Dummy_head = malloc(sizeof(element));
    Dummy_head->next = NULL;
    Dummy_head->down = NULL;    
}

/*
 * Updates value into the super list.
 * @param key  the unique key
 * @param data the data associated
 * @return 1 if an element is inserted
 *         0 if an element already existed
 *        -1 is there occurred a problem
 */
int  Repository_update( int key, int data ) {
  
    element *temp;
    temp = Dummy_head;
    /*Initialize an array of pointers.*/
    element *arr[arraysize+1];
    ind = 1;
    /*Check if temp went really down, below the list.*/ 
    while (temp != NULL) { 
        /*Check if temp has reached the end of this level.*/
        while (temp->next != NULL) { 
            if (temp->next->k == key) {
                /*Change data stored in the address.*/
                *temp->next->v = data;
                return 0;
            } else if (temp->next->k > key) {
                /*Store the smaller element pointer.*/
                break;
            } else {
                temp = temp->next;
                counter++;
            }
        }
            /*Store the end element pointer.*/
        arr[ind] = temp;
        counter++;
        temp = temp->down;
        ind++;
        counter++;
    }    
    /*To make index equal to number of levels.*/
    ind--;  
    
    /*Inserting the element.*/
    do 
    {   
        element *telement;
        element *pelement;
        pelement = malloc(sizeof(element));
        if (pelement == NULL)
        {
            return -1;
        }
       
        pelement->k = key;
        
        /*Check if we have to build a new level.*/
        if(ind >= 1) {
            /*Insert the element after pointer.*/
            pelement->next = arr[ind]->next;
            counter++;
            arr[ind]->next = pelement;
            counter++;
            /*Check if the element is inserted to bottom.*/
            if (ind < arraysize ) {
                /*Make pointers pointing to existing memory.*/
                pelement->down = telement;
                counter++;
                /*Pointing to same data as lower level self.*/
                pelement->v = telement->v;
                /*Pointing to same repetition as lower self.*/
                pelement->r = telement->r;
                /*Repetition is increased by one.*/
                (*pelement->r)++;
            } else {
                /*Mallocing space for everything new.*/
                pelement->down = NULL;
                pelement->r = malloc(sizeof(int));
                /*Set up repetition to only once.*/
                if(pelement->r == NULL) return(-1);
                *pelement->r = 1;
                pelement->v = malloc(sizeof(int));
                if(pelement->v == NULL) return(-1);
                *pelement->v = data;
            }
            /*Extract earlier element in the array.*/
            ind--;
        } else {
            /*Construct a new level.*/
            element *nelement;
            /*Nelement is the new Dummyhead of new level.*/
            nelement = malloc(sizeof(element));
            if (nelement == NULL)
            {
                return -1;
            }
            nelement->next = pelement;
            counter++;
            pelement->down = telement;
            counter++;
            pelement->v = telement->v;
            pelement->next = NULL;
            nelement->down = Dummy_head;
            counter++;
            pelement->r = telement->r;
            /*Mark the global Dummy_head pointer to.*/
            /*the new dummy_head created.*/
            Dummy_head = nelement;
            counter++;
            (*pelement->r)++;
            arraysize++;
        }
        /*Mark the pelement to be temp element.*/
        /*For down pointings later on.*/
        telement = pelement;
        counter++;
    /*I use do while loop to make sure element is inserted.*/
    /*at least once.*/
    /*Also I eliminate the infinite inserting if probability*/
    /*equals 100*/
    } while (rand100() <= prob && arraysize < maximun );
    /*Increase the size of bottom level(num of unique records).*/
    size++;
    return 1;
    
}

/*
 * Deletes element in the list.
 * If a line does not have an element 
 * after deletion, free this line.
 * @param key  the unique key
 * @return 1 if the element is found
 * and deleted
 *         0 if the element was not there
 */
int  Repository_delete( int key ) {
    element *temp;
    element *temph;
    element *pelement;
    temp = Dummy_head;
    result = 0;
    /*Check if temp goes really down, below the list.*/
    while (temp != NULL) {
        /*Check if temp has reached end of this level.*/
        while (temp->next != NULL){
            if (temp->next->k == key) {
                pelement = temp->next;
                counter++;
                temp->next = pelement->next;
                counter++;
                free(pelement);
                pelement = NULL;
                result = 1;
                /*Check if the entire level should be freed.*/
                /*Since there is only a dummy head in the level.*/
                if(temp == Dummy_head && temp->next == NULL && arraysize > 1 ) {
                    /*Pin the dummy head ready to be deleted.*/
                    temph = temp;
                    counter++;
                    /*Change the dummy head to lower dummy head.*/
                    Dummy_head = Dummy_head->down;
                    counter++;
                    arraysize--;
                }
                /*Temp continues to go down.*/
                break;
            } else if (temp->next->k > key) {
                /*Temp continues to go down.*/
                break;
            } else {
                /*Temp keeps going to the right.*/
                temp = temp->next;
                counter++;
            }
        }
        /*Temp also goes down when it is end of list.*/
        temp = temp->down;
        counter++;
        /*Free temph if it exists.*/
        if (temph != NULL)
        {
            free(temph);
            temph = NULL;
        }

    }
    
    /*Deduce the number of unique records by 1 if one is deleted.*/
    if (result == 1) {
        size--;
    }
   
     return result;
}

/*
 * Gets the key.
 * stores the data in the parameter
 * @param key the unique key
 * @param data the pointer that is going
 * to store the data got
 * @return 1 if the element is found
 *         0 if no such element exists
 */
int  Repository_get ( int key, int *data) {
 
    element *temp;
    temp = Dummy_head;
    /*Check of temp goes very down, outside list.*/
    while (temp != NULL) {
        while (temp->next != NULL) {
            if (temp->next->k == key) {
                /*Store value into the data parameter memory.*/
                *data = *temp->next->v;
                return 1;
            } else if (temp->next->k > key) {
                /*Goes down.*/
                break;
            } else {
                /*Keeps going to next.*/
                temp = temp->next;
                counter++;
            } 
        }
        /*Also goes down when hitting end of the list.*/
        temp = temp->down;
        counter++;
    }
    /*If nothing has been returned, means nothing found.*/
    return 0;
}

/*
 * Prints the number of unique records inthe list.
 * Prints number of records in each level.
 * Prints how many levels are there.
 * Prints each element.
 * Prints how many steps have performed so far.
 * @param print_elements types of instructions 
 * referring to how to display the list.
 * if parameter is 1, print unique records in the
 * list
 * if parameter is 2, print how the super list
 * looks like vertically.
 *
 */
void Repository_print ( int print_elements ) {
    /*Initialize an array used to store how many elements.*/
    /*In each level, index refers two level numbers.*/
    /*1 represents top level, index goes up.*/
    int levelsize[arraysize+1];
    /*Setting default values in array to 0 initially.*/
    for (j = 0; j <= arraysize; j++ ) {
        levelsize[j] = 0;
    }
  
    /*If list is empty, print these.*/
    printf("Number of unique records in the list: %d\n", size);
    in = 1;
    element *temp;
    element *temp1;
    temp = Dummy_head;
    /*Count and store number of elements in each level.*/         
    do
    {   
        /*Temp1 follows temp to be dummy head of each level.*/
        temp1 = temp;
        while (temp1->next != NULL) {
            levelsize[in]++;
            temp1 = temp1->next;
        }           
        in++; 
        /*Prevent temp from going beneath the list.*/
        /*Because I have to use dummy head of bottom level.*/
        /*Later on.*/          
        if(temp->down != NULL) 
        {
            temp = temp->down;
        }
    } while (in<arraysize+1);
        
    /*Printing out number of elements in each level from top to bottom.*/
    for (in = 1; in < arraysize+1; in++) {
        printf("Number of elements in level %d is : %d", in, levelsize[in]);
        printf("\n");
    } 
        
    /*Makes temp to be the first real element in the bottom level.*/    
    temp = temp->next;

    while (temp!= NULL) {
        /*Just print the elements once.*/
        if (print_elements == 1) {
            printf("(%3d,%3d)\n", temp->k , *temp->v);
        }
        /*Print the elements according to their recurrences.*/
        if (print_elements == 2) {
            /*Prints bottom level which don't have down elements.*/
            printf("(%3d,%3d)", temp->k, *temp->v);
            /*Peints upper levels which has arrows pointing down.*/
            for (k = 0; k < (*temp->r-1); k++) {
                printf("<--(%3d,%3d)", temp->k, *temp->v);
            }
            printf("\n");
        }
        temp = temp->next;
    }
        
    /*Print how many levels are there.*/   
    printf("Current number of levels: %d\n", arraysize);
    /*Print how much effort taken so far.*/
    printf("Steps of effort so far: %u\n", counter);
    printf("\n");    
}


