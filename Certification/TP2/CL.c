//#include <stdio.h>
//#include <stdlib.h>

/*@
 ensures \result == ((a < b) ? a : b);
@*/
int min(int a, int b) {
    int result = b;
    if (a <= b) {
        result = a;
    }

    return result;
}


/*@
  requires \valid(array_src);
  requires \valid(array_dest);
  requires n >= 0;

  ensures \forall integer i; 0 <= i < n ==> array_dest[i] == array_src[i];
@*/
void copy_array(int* array_src, int* array_dest, int n) {
    /*@
       loop invariant \forall integer j; 0 <= j < i ==> array_dest[j] == array_src[j];
       loop variant n - i;
     @*/
    for (int i = 0; i < n; i++) {
        array_dest[i] = array_src[i];
    }
}

/*@
  requires \valid(array_1+(0..n-1));
  requires \valid(array_2+(0..n-1));
  requires n >= 0;

  ensures (\forall integer i; 0 <= i < n ==> array_1[i] == array_2[i]) ==> \result == 1;
  ensures (\exists integer i; 0 <= i < n && array_1[i] != array_2[i]) ==> \result == 0;
@*/
int compare_array(int* array_1, int* array_2, int n) {
    /*@
       loop invariant (\forall integer j; 0 <= j < i ==> array_1[j] == array_2[j]) && 0 <= i < n;
       loop variant n - i;
     @*/
    for (int i = 0; i < n; i++) {
        if(array_1[i] != array_2[i]) {
            return 0;
        }
    }

    return 1;
}
/*
void fill_array(int* array, int n, int value)
{
    for (int i = 0; i < n; i) {
        array[i] = value;
    }
}




int null_array(int* array, int n) {
    int result = 1;

    for (int i = 0; i < n; i++) {
        if(array[i] != 0) {
            result = 0;
        }
    }

    return result;
}

int palindrome_array(int* array, int n) {
    int result = 1;

    for (int i = 0; i < n; i++) {
        if(array[i] != array[n - i]) {
            result = 0;
        }
    }

    return result;
}
*/
/*@
  requires \valid(array+(0..n-1));
  requires n >= 0;

  ensures \forall integer i; 0 <= i < n ==> array[i] >= array[\result];
  ensures 0 <= \result < n;
@*/

int min_array(int* array, int n) {
    int min = array[0];
    int index_min = 0;

    /*@
       loop invariant \forall integer j; 0 <= j < i ==> array[i] >= array[index_min];
       loop variant n - i;
     @*/
    for (int i = 0; i < n; i++) {
        if (array[i] <= min) {
            min = array[i];
            index_min = i;
        }
    }

    return index_min;
}

/*
int search_array(int* array, int n, int value) {
    int index_last = 0;

    for (int i = 0; i < n; i++) {
        if (array[i] == value) {
            index_last = i;
        }
        return index_last;
    }
}
void swap (int* p, int* q) {
       p = q + p;
       q = q - p;
       p = q - p;
}
*/

int main(int argc, const char *argv[])
{
    return 0;
}
