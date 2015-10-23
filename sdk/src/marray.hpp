/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef MARRAY_HPP
#define MARRAY_HPP

#define ARRAY_BOUNDS_CHECK  0 
#define FORTRAN_ORDER  0

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <new>


namespace quotek {

  namespace data {

    using namespace std;

    template <class array_element_type>
    class array1d{

    private:

      int size1;
      array_element_type * internal_array;

    public:

    /** Returns size of first dimension. */
    inline  int length1(void){
          return size1;
      }

    /** Returns a reference to array[x1]. This is the main function we use to store and retrieve data.
        @param x1 index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1)
    */ 

    inline  array_element_type & at(int x1){
    #if ARRAY_BOUNDS_CHECK == 1 

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);
           raise(SIGSEGV); 
       }
    #endif

        return  internal_array[x1];

      }


      /** Array constructor.
       *  @param dim1 size of first dimension of array. 
       */

      array1d(int  dim1){

          if( dim1 <= 0 ){
              printf("dim1 is less than or equal to 0\n");
              printf("dim1=%d \n", dim1);
              printf ("file %s, line %d.\n", __FILE__, __LINE__);  
              raise(SIGSEGV);           
          }
          else{
              size1=dim1;
              internal_array = new array_element_type[size1];
          }

      }

      /** Array destructor */
      ~array1d(){

        delete []  internal_array;

      }

    //note that even though array1d is a template, inside defintion of array1d
    //array1d means same as array1d<array_element_type>
    private:
    //prohibit copy constructor
      array1d( array1d &);

    //prohibit assignment operator
      array1d & operator=(array1d &);


    } ;

    template <class array_element_type>
    class array2d{

    private:
    //number of rows
      int size1;
    //number of columns
      int size2;

      array_element_type *  internal_array;

    public:

      /** Returns size of first dimension. */
      inline int length1(void){
          return size1;
      }

      /** Returns size of second dimension. */
      inline int length2(void){
          return size2;
      }

    /** Returns a reference to array[x1][x2]. This is the main function we use to store and retrieve data.
        @param x1 First dimension index of the element we want to retrieve or set in the array.
        @param x2 Second dimension index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1,x2)
    */ 

    inline  array_element_type & at(int x1, int x2){

    #if ARRAY_BOUNDS_CHECK == 1

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x2<0) || (x2>= size2) ){

           printf("index x2 is less than 0 or  equal to size2 or greater than size2\n");
           printf("x2=%d \n", x2);
           printf("size2=%d \n", size2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

    #endif

    //x1 is row number and x2 is column number

    #if FORTRAN_ORDER == 1
    //fortran convention
    //first index changes fastest
       return  internal_array[x2*size1 + x1];
    #else
    //C convention
    //last index changes fastest 
        return internal_array[x1*size2 + x2];
    #endif

      }


      /** Array constructor.
       *  @param dim1 size of the first dimension of array.
       *  @param dim2 size of the second dimension of array 
       */
      array2d(int  dim1, int dim2){

       if( dim1 <= 0 ){
           printf("dim1 is less than or  equal to 0\n");
           printf("dim1=%d \n", dim1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                 
       }
       else if( dim2 <= 0 ){
           printf("dim2 is less than or equal to 0\n");
           printf("dim2=%d \n", dim2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                  
       }
       else{
           size1=dim1;
           size2=dim2;
           internal_array = new array_element_type[size1*size2];
       }

      }

      /** Array destructor. */
      ~array2d(){

        delete [] internal_array;

      }

    //note that even though array2d is a template, inside defintion of array2d
    //array2d means same as array2d<array_element_type>
    private:
    //prohibit copy constructor
      array2d( array2d &);

    //prohibit assignment operator
      array2d & operator=(array2d &);


    } ;

    ////////////// end class array2d /////////////////////


    //////////////// start class array3d /////////////////////
    template <class array_element_type>
    class array3d{

    private:

    //x-axis
      int size1;
    //y-axis
      int size2;
    //z-axis (think of like floor in a building)
      int size3;

      array_element_type *  internal_array;

    //factors for Fortran order
      int F1, F2, F3;

    //factors for C order
      int C1, C2, C3;

    public:

      /** Returns size of first dimension. */
      inline int length1(void){
          return size1;
      }

      /** Returns size of second dimension. */
      inline int length2(void){
          return size2;
      }

      /** Returns size of third dimension. */
      inline int length3(void){
          return size3;
      }

    /** Returns a reference to array[x1][x2][x3]. This is the main function we use to store and retrieve data.
        @param x1 First dimension index of the element we want to retrieve or set in the array.
        @param x2 Second dimension index of the element we want to retrieve or set in the array.
        @param x3 Third dimension index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1, x2, x3)

    */
    inline  array_element_type & at(int x1, int x2, int x3){

    #if ARRAY_BOUNDS_CHECK == 1

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x2<0) || (x2>= size2) ){

           printf("index x2 is less than 0 or  equal to size2 or greater than size2\n");
           printf("x2=%d \n", x2);
           printf("size2=%d \n", size2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x3<0) || (x3>= size3) ){

           printf("index x3 is less than 0 or  equal to size3 or greater than size3\n");
           printf("x3=%d \n", x3);
           printf("size3=%d \n", size3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

    #endif

    #if FORTRAN_ORDER == 1
    //fortran convention
    //first index changes fastest
    //   return  internal_array[x3*size2*size1 + x2*size1 + x1];
       return  internal_array[x3*F3 + x2*F2 + x1*F1];
    #else
    //C convention
    //last index changes fastest 
    //  return  internal_array[x1*size2*size3 + x2*size3 + x3];
       return internal_array[x1*C1 + x2*C2 + x3*C3];

    #endif

      }


      /** Array constructor.
       *  @param dim1 size of the first dimension of array.
       *  @param dim2 size of the second dimension of array.
       *  @param dim3 size of the third dimension of array.
       */
      array3d(int  dim1, int dim2, int dim3){

       if( dim1 <= 0 ){
           printf("dim1 is less than or  equal to 0\n");
           printf("dim1=%d \n", dim1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim2 <= 0 ){
           printf("dim2 is less than or equal to 0\n");
           printf("dim2=%d \n", dim2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim3 <= 0 ){
           printf("dim3 is less than or equal to 0\n");
           printf("dim3=%d \n", dim3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else{ 
           size1=dim1;
           size2=dim2;
           size3=dim3;

           F3=size2*size1;
           F2=size1;
           F1=1;

           C1=size2*size3;
           C2=size3;
           C3=1;

           internal_array = new array_element_type[size1*size2*size3];
       }

      }

      /** Array destructor. */
      ~array3d(){

        delete []  internal_array;

      }

    //note that even though array3d is a template, inside defintion of array3d
    //array3d means same as array3d<array_element_type>
    private:
    //prohibit copy constructor
      array3d( array3d &);

    //prohibit assignment operator
      array3d & operator=(array3d &);


    } ;

    //////////////// start class array4d /////////////////////
    template <class array_element_type>
    class array4d{

    private:

    //x-axis
      int size1;
    //y-axis
      int size2;
    //z-axis (think of like floor in a building)
      int size3;
    //t-axis
      int size4;

      array_element_type *  internal_array;

    //factors for Fortran order
      int F1, F2, F3, F4;

    //factors for C order
      int C1, C2, C3, C4;

    public:

      /** Returns size of first dimension. */
      inline int length1(void){
          return size1;
      }

      /** Returns size of second dimension. */
      inline int length2(void){
          return size2;
      }

      /** Returns size of third dimension. */
      inline int length3(void){
          return size3;
      }

      /** Returns size of fourth dimension. */
      inline int length4(void){
          return size4;
      }

       
    /** Returns a referrence to array[x1][x2][x3][x4]. This is the main function we use to store and retrieve data.
        @param x1 First dimension index of the element we want to retrieve or set in the array.
        @param x2 Second dimension index of the element we want to retrieve or set in the array.
        @param x3 Third dimension index of the element we want to retrieve or set in the array.
        @param x4 Fourth dimension index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1, x2, x3,x4)
    */
    inline  array_element_type & at(int x1, int x2, int x3, int x4){

    #if ARRAY_BOUNDS_CHECK == 1

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x2<0) || (x2>= size2) ){

           printf("index x2 is less than 0 or  equal to size2 or greater than size2\n");
           printf("x2=%d \n", x2);
           printf("size2=%d \n", size2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x3<0) || (x3>= size3) ){

           printf("index x3 is less than 0 or  equal to size3 or greater than size3\n");
           printf("x3=%d \n", x3);
           printf("size3=%d \n", size3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x4<0) || (x4>= size4) ){

           printf("index x4 is less than 0 or  equal to size4 or greater than size4\n");
           printf("x4=%d \n", x4);
           printf("size4=%d \n", size4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);        exit(1);
       }

    #endif

    #if FORTRAN_ORDER == 1
    //fortran convention
    //first index changes fastest
    //   return  internal_array[x4*size3*size2*size1 + x3*size2*size1 + x2*size1 + x1];

        return  internal_array[ x4*F4 + x3*F3 + x2*F2 +x1*F1 ];
    #else
    //C convention
    //last index changes fastest 
    //  return  internal_array[x1*size2*size3*size4 + x2*size3*size4 +x3*size4 + x4];

        return  internal_array[ x1*C1 + x2*C2 + x3*C3 + x4*C4 ];
    #endif

      }


      /** Array constructor.
       *  @param dim1 size of the first dimension of array.
       *  @param dim2 size of the second dimension of array.
       *  @param dim3 size of the third dimension of array.
       *  @param dim4 size of the fourth dimension of array.
       */
      array4d(int  dim1, int dim2, int dim3, int dim4){

       if( dim1 <= 0 ){
           printf("dim1 is less than or  equal to 0\n");
           printf("dim1=%d \n", dim1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim2 <= 0 ){
           printf("dim2 is less than or equal to 0\n");
           printf("dim2=%d \n", dim2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);           
       }
       else if( dim3 <= 0 ){
           printf("dim3 is less than or equal to 0\n");
           printf("dim3=%d \n", dim3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);           
       }
       else if( dim4 <= 0 ){
           printf("dim4 is less than or equal to 0\n");
           printf("dim4=%d \n", dim4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else{ 
         size1=dim1;
         size2=dim2;
         size3=dim3;
         size4=dim4;

         internal_array = new array_element_type[size1*size2*size3*size4];

         F4=size3*size2*size1 ; 
         F3=size2*size1 ; 
         F2=size1 ; 
         F1=1;


         C1=size2*size3*size4 ;
         C2=size3*size4 ; 
         C3=size4 ;
         C4=1;

       }

      }

      /** Array Destructor. */
      ~array4d(){

        delete []  internal_array;

      }

    //note that even though array4d is a template, inside defintion of array4d
    //array4d means same as array4d<array_element_type>
    private:
    //prohibit copy constructor
      array4d( array4d &);

    //prohibit assignment operator
      array4d & operator=(array4d &);


    } ;

    ////////////// end class array4d /////////////////////

    //////////////// start class array5d /////////////////////
    template <class array_element_type>
    class array5d{

    private:

    //x-axis
      int size1;
    //y-axis
      int size2;
    //z-axis (think of like floor in a building)
      int size3;
    //t-axis
      int size4;
    //
      int size5;

      array_element_type *  internal_array;

    //factors for Fortran order
      int F1, F2, F3, F4, F5;

    //factors for C order
      int C1, C2, C3, C4, C5;

    public:

      /** Returns size of first dimension. */
      inline int length1(void){
          return size1;
      }

      /** Returns size of second dimension. */
      inline int length2(void){
          return size2;
      }
      
      /** Returns size of third dimension. */
      inline int length3(void){
          return size3;
      }

      /** Returns size of fourth dimension. */
      inline int length4(void){
          return size4;
      }

      /** Returns size of fifth dimension. */
      inline int length5(void){
          return size5;
      }

    /** Returns a referrence to array[x1][x2][x3][x4][x5]. This is the main function we use to store and retrieve data.
        @param x1 First dimension index of the element we want to retrieve or set in the array.
        @param x2 Second dimension index of the element we want to retrieve or set in the array.
        @param x3 Third dimension index of the element we want to retrieve or set in the array.
        @param x4 Fourth dimension index of the element we want to retrieve or set in the array.
        @param x5 Fifth dimension index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1, x2, x3,x4,x5)
    */
    inline  array_element_type & at(int x1, int x2, int x3, int x4, int x5){

    #if ARRAY_BOUNDS_CHECK == 1

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x2<0) || (x2>= size2) ){

           printf("index x2 is less than 0 or  equal to size2 or greater than size2\n");
           printf("x2=%d \n", x2);
           printf("size2=%d \n", size2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x3<0) || (x3>= size3) ){

           printf("index x3 is less than 0 or  equal to size3 or greater than size3\n");
           printf("x3=%d \n", x3);
           printf("size3=%d \n", size3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x4<0) || (x4>= size4) ){

           printf("index x4 is less than 0 or  equal to size4 or greater than size4\n");
           printf("x4=%d \n", x4);
           printf("size4=%d \n", size4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x5<0) || (x5>= size5) ){

           printf("index x5 is less than 0 or  equal to size5 or greater than size5\n");
           printf("x5=%d \n", x5);
           printf("size5=%d \n", size5);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }
    #endif

    #if FORTRAN_ORDER == 1
    //fortran convention
    //first index changes fastest
    //   return  internal_array[x5*size4*size3*size2*size1 + x4*size3*size2*size1 + x3*size2*size1 + x2*size1 + x1];

         return  internal_array[x5*F5 + x4*F4 + x3*F3 + x2*F2 + x1*F1];

    #else
    //C convention
    //last index changes fastest 
    //  return  internal_array[x1*size2*size3*size4*size5 + 
    //                               x2*size3*size4*size5 +
    //                                     x3*size4*size5 + 
    //                                           x4*size5 + x5];

         return  internal_array[x1*C1 + x2*C2 + x3*C3 + x4*C4 + x5*C5];
        
    #endif

      }


      /** Array constructor.
       *  @param dim1 size of the first dimension of array.
       *  @param dim2 size of the second dimension of array.
       *  @param dim3 size of the third dimension of array.
       *  @param dim4 size of the fourth dimension of array.
       *  @param dim5 size of the fifth dimension of array.
       */
      array5d(int  dim1, int dim2, int dim3, int dim4, int dim5){

       if( dim1 <= 0 ){
           printf("dim1 is less than or  equal to 0\n");
           printf("dim1=%d \n", dim1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim2 <= 0 ){
           printf("dim2 is less than or equal to 0\n");
           printf("dim2=%d \n", dim2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim3 <= 0 ){
           printf("dim3 is less than or equal to 0\n");
           printf("dim3=%d \n", dim3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim4 <= 0 ){
           printf("dim4 is less than or equal to 0\n");
           printf("dim4=%d \n", dim4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim5 <= 0 ){
           printf("dim5 is less than or equal to 0\n");
           printf("dim5=%d \n", dim5);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else{ 
         size1=dim1;
         size2=dim2;
         size3=dim3;
         size4=dim4;
         size5=dim5;

         internal_array = new array_element_type[size1*size2*size3*size4*size5];

         F5=size4*size3*size2*size1 ; 
         F4=size3*size2*size1 ; 
         F3=size2*size1 ; 
         F2=size1 ; 
         F1=1;

     
         C1=size2*size3*size4*size5 ; 
         C2=size3*size4*size5 ;
         C3=size4*size5 ; 
         C4=size5 ; 
         C5=1;

       }

     }
      /** Array destructor. */
      ~array5d(){

        delete []  internal_array;

      }

    //note that even though array5d is a template, inside defintion of array5d
    //array5d means same as array5d<array_element_type>
    private:
    //prohibit copy constructor
      array5d( array5d &);

    //prohibit assignment operator
      array5d & operator=(array5d &);


    } ;

    ////////////// end class array5d /////////////////////


    //////////////// start class array6d /////////////////////
    template <class array_element_type>
    class array6d{

    private:

    //x-axis
      int size1;
    //y-axis
      int size2;
    //z-axis (think of like floor in a building)
      int size3;
    //t-axis
      int size4;
    //
      int size5;

      int size6;

      array_element_type *  internal_array;

    //factors for Fortran order
      int F1, F2, F3, F4, F5, F6;

    //factors for C order
      int C1, C2, C3, C4, C5, C6;

    public:
      /** Returns size of first dimension. */
      inline int length1(void){
          return size1;
      }
      /** Returns size of second dimension. */
      inline int length2(void){
          return size2;
      }
      
      /** Returns size of third dimension. */
      inline int length3(void){
          return size3;
      }

      /** Returns size of fourth dimension. */
      inline int length4(void){
          return size4;
      }

      /** Returns size of fifth dimension. */
      inline int length5(void){
          return size5;
      }

      /** Returns size of sixth dimension. */
      inline int length6(void){
          return size6;
      }


    /** Returns a referrence to array[x1][x2][x3][x4][x5][x6]. This is the main function we use to store and retrieve data.
        @param x1 First dimension index of the element we want to retrieve or set in the array.
        @param x2 Second dimension index of the element we want to retrieve or set in the array.
        @param x3 Third dimension index of the element we want to retrieve or set in the array.
        @param x4 Fourth dimension index of the element we want to retrieve or set in the array.
        @param x5 Fifth dimension index of the element we want to retrieve or set in the array.
        @param x6 Sixth dimension index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1, x2, x3,x4,x5,x6)
    */
    inline  array_element_type & at(int x1, int x2, int x3, int x4, int x5, int x6){

    #if ARRAY_BOUNDS_CHECK == 1

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x2<0) || (x2>= size2) ){

           printf("index x2 is less than 0 or  equal to size2 or greater than size2\n");
           printf("x2=%d \n", x2);
           printf("size2=%d \n", size2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x3<0) || (x3>= size3) ){

           printf("index x3 is less than 0 or  equal to size3 or greater than size3\n");
           printf("x3=%d \n", x3);
           printf("size3=%d \n", size3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x4<0) || (x4>= size4) ){

           printf("index x4 is less than 0 or  equal to size4 or greater than size4\n");
           printf("x4=%d \n", x4);
           printf("size4=%d \n", size4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x5<0) || (x5>= size5) ){

           printf("index x5 is less than 0 or  equal to size5 or greater than size5\n");
           printf("x5=%d \n", x5);
           printf("size5=%d \n", size5);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x6<0) || (x6>= size6) ){

           printf("index x6 is less than 0 or  equal to size6 or greater than size6\n");
           printf("x6=%d \n", x6);
           printf("size6=%d \n", size6);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

    #endif

    #if FORTRAN_ORDER == 1
  
        return  internal_array[x6*F6 + x5*F5 + x4*F4 + x3*F3 + x2*F2 + x1*F1 ];

    #else
    

        return  internal_array[ x1*C1 + x2*C2 + x3*C3 + x4*C4 + x5*C5 + x6*C6 ];

    #endif

      }


      /** Array constructor.
       *  @param dim1 size of the first dimension of array.
       *  @param dim2 size of the second dimension of array.
       *  @param dim3 size of the third dimension of array.
       *  @param dim4 size of the fourth dimension of array.
       *  @param dim5 size of the fifth dimension of array.
       *  @param dim6 size of the sixth dimension of array.
       */
      array6d(int  dim1, int dim2, int dim3, int dim4, int dim5, int dim6){

       if( dim1 <= 0 ){
           printf("dim1 is less than or  equal to 0\n");
           printf("dim1=%d \n", dim1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim2 <= 0 ){
           printf("dim2 is less than or equal to 0\n");
           printf("dim2=%d \n", dim2);
            printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim3 <= 0 ){
           printf("dim3 is less than or equal to 0\n");
           printf("dim3=%d \n", dim3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim4 <= 0 ){
           printf("dim4 is less than or equal to 0\n");
           printf("dim4=%d \n", dim4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim5 <= 0 ){
           printf("dim5 is less than or equal to 0\n");
           printf("dim5=%d \n", dim5);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim6 <= 0 ){
           printf("dim6 is less than or equal to 0\n");
           printf("dim6=%d \n", dim6);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else{ 
         size1=dim1;
         size2=dim2;
         size3=dim3;
         size4=dim4;
         size5=dim5;
         size6=dim6;

         internal_array = new array_element_type[size1*size2*size3*size4*size5*size6];

         F6=size5*size4*size3*size2*size1 ; 
         F5=size4*size3*size2*size1 ; 
         F4=size3*size2*size1 ; 
         F3=size2*size1 ;
         F2=size1 ; 
         F1=1;

         C1=size2*size3*size4*size5*size6 ; 
         C2=size3*size4*size5*size6 ;
         C3=size4*size5*size6 ; 
         C4=size5*size6 ; 
         C5=size6 ; 
         C6=1 ;

      }

      }

      /** Array destructor. */
      ~array6d(){

        delete []  internal_array;

      }

    //note that even though array6d is a template, inside defintion of array6d
    //array6d means same as array6d<array_element_type>
    private:
    //prohibit copy constructor
      array6d( array6d &);

    //prohibit assignment operator
      array6d & operator=(array6d &);


    } ;

    ////////////// end class array6d /////////////////////

    //////////////// start class array7d /////////////////////
    template <class array_element_type>
    class array7d{

    private:

    //x-axis
      int size1;
    //y-axis
      int size2;
    //z-axis (think of like floor in a building)
      int size3;
    //t-axis
      int size4;
    //
      int size5;

      int size6;

      int size7;

      array_element_type *  internal_array;

    //factors for Fortran order
      int F1, F2, F3, F4, F5, F6, F7;

    //factors for C order
      int C1, C2, C3, C4, C5, C6, C7;

    public:

      /** Returns size of first dimension. */
      inline int length1(void){
          return size1;
      }

      /** Returns size of second dimension. */
      inline int length2(void){
          return size2;
      }

      /** Returns size of third dimension. */
      inline int length3(void){
          return size3;
      }

      /** Returns size of fourth dimension. */
      inline int length4(void){
          return size4;
      }

      /** Returns size of fifth dimension. */
      inline int length5(void){
          return size5;
      }

      /** Returns size of sixth dimension. */
      inline int length6(void){
          return size6;
      }

      /** Returns size of seventh dimension. */
      inline int length7(void){
          return size7;
      }


    /** Returns a referrence to array[x1][x2][x3][x4][x5][x6][x7]. 
        This is the main function we use to store and retrieve data.
        @param x1 First dimension index of the element we want to retrieve or set in the array.
        @param x2 Second dimension index of the element we want to retrieve or set in the array.
        @param x3 Third dimension index of the element we want to retrieve or set in the array.
        @param x4 Fourth dimension index of the element we want to retrieve or set in the array.
        @param x5 Fifth dimension index of the element we want to retrieve or set in the array.
        @param x6 Sixth dimension index of the element we want to retrieve or set in the array.
        @param x7 Seventh dimension index of the element we want to retrieve or set in the array.
        @return element of array pointed by (x1, x2, x3,x4,x5,x6,x7)
    */

    inline  array_element_type & at(int x1, int x2, int x3, int x4, int x5, int x6, int x7){

    #if ARRAY_BOUNDS_CHECK == 1

       if( (x1<0) || (x1>= size1) ){

           printf("index x1 is less than 0 or  equal to size1 or greater than size1\n");
           printf("x1=%d \n", x1);
           printf("size1=%d \n", size1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x2<0) || (x2>= size2) ){

           printf("index x2 is less than 0 or  equal to size2 or greater than size2\n");
           printf("x2=%d \n", x2);
           printf("size2=%d \n", size2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x3<0) || (x3>= size3) ){

           printf("index x3 is less than 0 or  equal to size3 or greater than size3\n");
           printf("x3=%d \n", x3);
           printf("size3=%d \n", size3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x4<0) || (x4>= size4) ){

           printf("index x4 is less than 0 or  equal to size4 or greater than size4\n");
           printf("x4=%d \n", x4);
           printf("size4=%d \n", size4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x5<0) || (x5>= size5) ){

           printf("index x5 is less than 0 or  equal to size5 or greater than size5\n");
           printf("x5=%d \n", x5);
           printf("size5=%d \n", size5);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x6<0) || (x6>= size6) ){

           printf("index x6 is less than 0 or  equal to size6 or greater than size6\n");
           printf("x6=%d \n", x6);
           printf("size6=%d \n", size6);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

       if( (x7<0) || (x7>= size7) ){

           printf("index x7 is less than 0 or  equal to size7 or greater than size7\n");
           printf("x7=%d \n", x7);
           printf("size7=%d \n", size7);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV); 
       }

    #endif

    #if FORTRAN_ORDER == 1
    //fortran convention
    //first index changes fastest
    //   return  internal_array[x7*size6*size5*size4*size3*size2*size1 + x6*size5*size4*size3*size2*size1 + 
    //                          x5*size4*size3*size2*size1 + x4*size3*size2*size1 + x3*size2*size1 + x2*size1 + x1];

         return internal_array[x7*F7 + x6*F6 + x5*F5 + x4*F4 + x3*F3 + x2*F2 +x1*F1];
    #else
    //C convention
    //last index changes fastest 
    //  return  internal_array[x1*size2*size3*size4*size5*size6*size7 + 
    //                               x2*size3*size4*size5*size6*size7 +
    //                                     x3*size4*size5*size6*size7 + 
    //                                           x4*size5*size6*size7 + 
    //                                                 x5*size6*size7 +
    //                                                       x6*size7 + x7];

        return internal_array[x1*C1 + x2*C2 + x3*C3 + x4*C4 + x5*C5 + x6*C6 + x7*C7];  
    #endif

      }


      /** Array constructor.
       *  @param dim1 size of the first dimension of array.
       *  @param dim2 size of the second dimension of array.
       *  @param dim3 size of the third dimension of array.
       *  @param dim4 size of the fourth dimension of array.
       *  @param dim5 size of the fifth dimension of array.
       *  @param dim6 size of the sixth dimension of array.
       *  @param dim7 size of the sixth dimension of array.
       */
      array7d(int  dim1, int dim2, int dim3, int dim4, int dim5, int dim6, int dim7){

       if( dim1 <= 0 ){
           printf("dim1 is less than or  equal to 0\n");
           printf("dim1=%d \n", dim1);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                    
       }
       else if( dim2 <= 0 ){
           printf("dim2 is less than or equal to 0\n");
           printf("dim2=%d \n", dim2);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim3 <= 0 ){
           printf("dim3 is less than or equal to 0\n");
           printf("dim3=%d \n", dim3);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim4 <= 0 ){
           printf("dim4 is less than or equal to 0\n");
           printf("dim4=%d \n", dim4);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                    
       }
       else if( dim5 <= 0 ){
           printf("dim5 is less than or equal to 0\n");
           printf("dim5=%d \n", dim5);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim6 <= 0 ){
           printf("dim6 is less than or equal to 0\n");
           printf("dim6=%d \n", dim6);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);                     
       }
       else if( dim7 <= 0 ){
           printf("dim7 is less than or equal to 0\n");
           printf("dim7=%d \n", dim7);
           printf ("file %s, line %d.\n", __FILE__, __LINE__);  
           raise(SIGSEGV);           
       }
       else{ 
         size1=dim1;
         size2=dim2;
         size3=dim3;
         size4=dim4;
         size5=dim5;
         size6=dim6;
         size7=dim7;

    //Fortran convention
         F7=size6*size5*size4*size3*size2*size1 ;
         F6=size5*size4*size3*size2*size1 ;
         F5=size4*size3*size2*size1 ;  
         F4=size3*size2*size1 ; 
         F3=size2*size1 ; 
         F2=size1 ;
         F1=1 ;

    //C convention
    //last index changes fastest 
         C1=size2*size3*size4*size5*size6*size7 ; 
         C2=size3*size4*size5*size6*size7 ;
         C3=size4*size5*size6*size7 ; 
         C4=size5*size6*size7 ; 
         C5=size6*size7 ;
         C6=size7 ; 
         C7=1 ;



         internal_array = new array_element_type[size1*size2*size3*size4*size5*size6*size7];
      }

     }

    //destructor
      ~array7d(){

        delete []  internal_array;

      }

    //note that even though array7d is a template, inside defintion of array7d
    //array7d means same as array7d<array_element_type>
    private:
    //prohibit copy constructor
      array7d( array7d &);

    //prohibit assignment operator
      array7d & operator=(array7d &);


    } ;

    ////////////// end class array7d /////////////////////
  
  
  }
}

#endif
