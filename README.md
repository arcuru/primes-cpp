# Primes-cpp
A compact primes library containing a highly optimized prime sieve and deterministic primality test.

# Building

Cmake is used for setting up the build system. On first run it will download a copy of [googletest](https://github.com/google/googletest) to use for unit testing.

```
> mkdir build
> cd build
> cmake ..
> make
```

This will build two binaries `unittest` and `benchmark`.
