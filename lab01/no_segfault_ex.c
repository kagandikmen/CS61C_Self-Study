#include <stdio.h>
int main() {
    int a[5] = {1, 2, 3, 4, 5};
    unsigned total = 0;
    for (int j = 0; j < sizeof(a)/sizeof(int); j++) {   // fixing the scope problem using sizeof again
                                                        // the array always has sizeof(a)/sizeof(int)
                                                        // number of elements, even if the size of
                                                        // the array changes
            // printf("%d\n", j);   FOR DEBUGGING
        // a is 5 * 4 = 20 bytes long, so
        // the for loop is gone into 20 times (before the fix)
        // though the array has only 5 elements

        total += a[j];
        // a[j] = total + j;
    }
    printf("sum of array is %d\n", total);

}