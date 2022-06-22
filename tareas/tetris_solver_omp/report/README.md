# Results report

Here will be listed all the results analysis from the comparison of the different results obtained from serial, concurrent with pthreads, and concurrent with OpenMP solutions.

## Errors disclaimer

When the programming from pthreads approach was adapted to OpenMP usage, some issues were found, these will be listed and analyzed below, by the hand of Valgrind tsan sanitizer complaining logs.

### OpenMP dificulties

1. Private data usage for OpenMP is different to the approach taken with pthreads approach, everything that is passed to parallel pragmas as private must be reassigned, and this is because of the internal directives of OpenMP. So, to avoid dealing with this issue, private data inserted to parallel pragmas was avoided, instead private data records were created, used, and deleted into the parallel pragma, and the values from these records needed out of this pragma were passed through a shared variable thread safe (each thread has its own memory portion, so that all of them can write the variable at a time without any problem).
