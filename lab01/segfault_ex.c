#include <stdio.h>

int main() {
    int a[20];
    for (int i = 0; i<1000; i++) {  // In my computer, iterating 439 times (i<440)
                                    // does not cause segfault;
                                    // but iterating 440 times does.     
        printf("%d\n",i);

        // Result of printing out the addresses of the elements of the array:
        // It seems like in my computer I have the memory chunk allocated to the program
        // which lies between: 0x...a8920 and 0x...a8ffc
        // with 0x...a8fff being the first address causing a segfault

        a[i] = i;
        int *address = &a[i];
        printf("%p\n", address);
        // printf("%Lf", *(address+1));
    }
    
}