// pointer_lab.cpp
#include <cstdio>   // for printf

int main() {
    int x = 42;
    int* p = &x;

    printf("=== Single variable and pointer ===\n");
    printf("x value      = %d\n", x);
    printf("&x (address) = %p\n", (void*)&x);
    printf("p  (address) = %p\n", (void*)p);
    printf("*p (value)   = %d\n\n", *p);

    // Modify via pointer
    *p = 100;
    printf("After *p = 100:\n");
    printf("x  = %d\n", x);
    printf("*p = %d\n\n", *p);

    printf("=== Array and pointer arithmetic ===\n");
    int arr[5] = {10, 20, 30, 40, 50};
    int* pa = arr;  // same as &arr[0]

    for (int i = 0; i < 5; ++i) {
        printf("Index %d: arr[%d] = %d, address = %p\n",
               i, i, arr[i], (void*)&arr[i]);
    }
    printf("\n");

    for (int i = 0; i < 5; ++i) {
        printf("Pointer step %d: *(pa + %d) = %d, address = %p\n",
               i, i, *(pa + i), (void*)(pa + i));
    }

    return 0;
}