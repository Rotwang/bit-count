#include "sample-data.h"
#include <sys/resource.h>
#include <stdio.h>
#include <sys/time.h>


int main(void) {
    unsigned short samples[] = RANDOM_SAMPLE;
    unsigned int bits_set = 0;
    struct timeval  tv1, tv2;
    struct rusage usage;
    unsigned int s = 0;
    unsigned char * p;

    static const unsigned char BitsSetTable256[256] = {
#       define B2(n) n,     n+1,     n+1,     n+2
#       define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#       define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
        B6(0), B6(1), B6(1), B6(2)
    };


    gettimeofday(&tv1, NULL);
    for (s=0; s < SAMPLE_SIZE; s++) {
        p = (unsigned char *) &samples[s];
        bits_set += BitsSetTable256[p[0]] + BitsSetTable256[p[1]];
    }
    gettimeofday(&tv2, NULL);

    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        perror("Couldn't get system usage");
    }

    printf("Bits set: %u\n", bits_set);
    printf("Total time = %ld microseconds\n", (tv2.tv_usec - tv1.tv_usec));
    printf("User CPU time = %ld seconds %ld microseconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System CPU time = %ld seconds %ld microseconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    printf("Voluntary/Involuntary ctx = %lu/%lu\n", usage.ru_nvcsw, usage.ru_nivcsw);

    return 0;
}