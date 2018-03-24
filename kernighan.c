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

    gettimeofday(&tv1, NULL);
    for (s=0; s < SAMPLE_SIZE; s++) {
        for (; samples[s]; bits_set++) {
            samples[s] &= samples[s] - 1;
        }
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