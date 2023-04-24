#include <stdio.h>

// this will be used for the next project 

//0000 0000 0000 0000 1101 1111 1011 0111
// &
// 0000 0000 0000 0000 0000 0000 0000 1111 //  offset mask

// 0000 0000 0000 0000 1111 1111 1111 0000 // page mask 

// ~0 == 1111 1111 1111 1111 1111 1111 1111
// ~0 << 4 = 1111 1111 1111 1111 1111 1111 0000
//~(~0 << 4) = 0000 0000 0000 0000 0000 0000 0000 1111

//~0 == 1111 1111 1111 1111 1111 1111 1111 1111
//~0 << 16 == 1111 1111 1111 1111 0000 0000 0000 1111
//~(~0 << 16) = 0000 0000 0000 0000 1111 1111 1111 1111
// ~(~0 << 16) >> 4 = 0000 0000 0000 0000 1111 1111 1111 0000

// 1 & 0 = 0
// 1 & 1 = 1

// << means multiply by 2 , bit shift left 
// 0001 << 1 == 0010
// 0010 << 1 == 0100
// 0100 << 1 == 1000
// 1000 << 1 == 0000

// ~1 == 0
// ~0 == 1 // invert the number 
// ~0000 == 1111 
// ~0 == 1111 (on a 4 bit machine)

//next time figure out how to do the outer page. we did 10 and 6, they did 10, 10, and 12.
//function to print binary represntation of a number, with spaces between each 4 bits
// void bin(unsigned n)
// {
//     unsigned i;
//     for (i = 1 << 31; i > 0; i = i / 2)
//         (n & i) ? printf("1") : printf("0");
//     if(i % 4 == 0) printf(" ");
// }

void decToBinary(int n)
{
    printf("%d is ", n); 
    for (int i = 31; i >= 0; i--) {
        int k = n >> i; // 
        if (k & 1) 
              printf("1");
        else printf("0");
        if( i % 4 ==0){
            printf(" ");
        }
    }
    printf("in binary\n");
}
  
unsigned offset(unsigned x, unsigned bits){
    unsigned mask = ~(~0 << bits);  
    unsigned result = x & mask; 

    return result;
}

unsigned pageone(unsigned x, unsigned bits){
    unsigned mask = ~(~0 << 16) >> 4; 
    unsigned result = (x & mask) >> 4; 

    return result;
}


int main(int argc, const char* argv[]){
    unsigned x = 4097;
    unsigned pg = page(x, 10); 
    unsigned off = offset(x, 6);

    decToBinary(x);

    printf("for %u, the page is: %u and the offset is : %u\n", x, pg, off);

    return 0;
}