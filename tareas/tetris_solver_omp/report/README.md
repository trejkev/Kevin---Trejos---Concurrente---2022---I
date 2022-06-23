# Results report

Here will be listed all the results analysis from the comparison of the different results obtained from serial, concurrent with pthreads, and concurrent with OpenMP solutions.

## Errors disclaimer

When the programming from pthreads approach was adapted to OpenMP usage, some issues were found, these will be listed and analyzed below, by the hand of Valgrind tsan sanitizer complaining logs.

### OpenMP dificulties

Private data usage for OpenMP is different to the approach taken with pthreads approach, everything that is passed to parallel pragmas as private must be reassigned, and this is because of the internal directives of OpenMP. So, to avoid dealing with this issue, private data inserted to parallel pragmas was avoided, instead private data records were created, used, and deleted into the parallel pragma, and the values from these records needed out of this pragma were passed through a shared variable thread safe (each thread has its own memory portion, so that all of them can write the variable at a time without any problem).

### Sanitizer results

All the four sanitizers were run:

1. asan: Memory address bugs, like memory leaks and use after free.
2. msan: Uninitialized memory usage.
3. tsan: Thread sanitizer for data race detection.
4. ubsan: Undefined behavior sanitizer to check conditions that may lead to undefined behaviors.
5. memcheck: General memory error detector. Like accessing memory out of bounds, use after free, and so on.

Inbetween all of these sanitizers, only tsan complained about the code, reason why only this sanitizer will be analized as part of the sanitizer results.

#### Thread Sanitizer (tsan) errors

To limit the amount of warnings shown by the sanitizer only 2 threads were run.

The first error is shown in the figure 1.

<p align="center">
<img width="800" src="https://user-images.githubusercontent.com/18760154/175184493-6fcbe1b0-0b0b-4d9e-957a-e2a644e8edfb.png">
</p>
<p align="center">Figure 1. First error from tsan, race condition.</p>




