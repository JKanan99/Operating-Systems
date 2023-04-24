#include <stdio.h>

// 0000 0000 0000 0000 0000 1111 1111 1111 - offset 

// 0000 0000 0011 1111 1111 0000 0000 0000 - page 2

// 1111 1111 1100 0000 0000 0000 0000 0000 - page 1


void binary(int n)
{
    printf("%d is ", n); 
    for (int i = 31; i >= 0; i--) {
        int k = n >> i;
        if (k & 1) 
            printf("1");
        else printf("0");
        if(i % 4 == 0){
            printf(" ");
        }
    }
    printf("in binary\n\n");
}
  
unsigned offset(unsigned x, unsigned bits){
    unsigned mask = ~(~0 << bits);  
    unsigned result = x & mask; 

    return result;
}

unsigned page(unsigned x, unsigned bits){
    unsigned mask = ~(~0 << 16) >> 22; 
    unsigned result = (x & mask) >> 22; //should i switch to 4

    return result;
}
unsigned pagetwo(unsigned x, unsigned bits){
    unsigned mask = ~(~0 << 16) >> 12; 
    unsigned result = (x & mask) >> 12; //should i switch to 4

    return result;
}

int main(int argc, const char* argv[]){
    unsigned x = 4097;
    unsigned pg = page(x, 10); 
    unsigned pg2 = page(x,10); 
    unsigned off = offset(x, 12);

    binary(x);

    printf("For %u, the page is: %u and %u, and the offset is : %u\n\n", x, pg, pg2, off);

    return 0;
}