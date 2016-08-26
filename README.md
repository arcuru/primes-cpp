# Primes-cpp
A compact primes library containing a highly optimized prime sieve and deterministic primality test.

This library exists mostly because I find the algorithm, and it's optimizations, interesting. There is a faster and more featured implementation that you can find at [kimwalisch/primesieve](https://github.com/kimwalisch/primesieve), but this small library has most of the benefits but with only a few hundred lines of code. This is a better starting point to understand why this can be fast.

# The Sieve of Eratosthenes
This library uses [the Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes) algorithm to find primes in a range. The basic idea is that, rather than using trial division to find primes, you start with a large array denoting every possible number as a prime number, and mark numbers as not prime by checking all multiples.

Check out wikipedia to see it graphically, it's easier to understand that way. Here is a full implementation in Cpp code to find all primes up to N.

```
std::vector<bool> is_prime(N, true);
is_prime[0] = is_prime[1] = false; // By definition
for (int i = 2; i*i < N; ++i) {
  if (is_prime[i]) {
    for (int j = i*i; j < N; j += i) {
      is_prime[j] = false;
    }
  }
}
```

After this is run, we can check if any number `x` less than `N` is prime by checking `is_prime[x]`.

# How it's fast
There are 4 different things that contribute to this implementation being very fast.

## Cache-awareness (segmented sieving) 
The largest speedup comes from handling the memory accesses in a sane manor.

To get an idea of what the memory access pattern looks like, consider trying to find all primes up to 10^9 (1,000,000). Since you need an array with space for every possible prime, this could use up to 1 GB of total space (10^9 bytes). Since the algorithm has to loop through the whole array for every prime less than the sqrt(10^9), that's going to take a long time.

If you break up the array into segments that fit into L1 cache, the memory access times drop considerably. By keeping a list of all the primes you need to sieve (every primes less than sqrt(N)) and sieving each of those on every segment, you can trade a few extra processor instructions (e.g. calculating which index you'll need to begin marking off at) for a far better memory access pattern.

## Multi-threading
This goes almost without saying. Once you have the algorithm set up to sieve in segments for cache awareness, modifying it to spread the segments among the available processor threads is relatively trivial.

This implementation will read the number of available threads in the system and scale to that automatically, or it will accept threads as an optional input.

## Wheel Factorization
This next bit starts to complicate the math a little bit.

You may have noticed that storing all possible places is rather inefficient, as the only even number is 2. So you could just store only places for all odd numbers and handle 2 as a special case.

More formally this technique is called [wheel factorization](https://en.wikipedia.org/wiki/Wheel_factorization).

What you may *not* have realized is that we can extend this further by factoring out all the multiples of 3. Because the least common multiple of 2 and 3 is 6, we know that only numbers that fit the form `X % 6 == 1 || X % 6 == 5` can be prime. For every other possible value the numbers are divisible by either 2 or 3.

```
if X % 6 == 0, then X is divisible by both 2 and 3
if X % 6 == 2, then X is divisible by 2
if X % 6 == 3, then X is divisible by 3
if X % 6 == 4, then X is divisible by 2
```

We can extend this to `2, 3, 5` wheel factorization, wherein every prime (other than 2, 3, or 5) modulo 30 (2 * 3 * 5) is equal to one of the following.

> 1 7 11 13 17 19 23 29

Obviously this can be extended further, pre-sieving out many primes in advance. However this implementation sticks with `2, 3, 5` wheel factorization for reasons I explain below.

Now we only need to store 8 possible primes per 30 values. If each place was being stored in a byte, we've reduced our memory requirements by almost 75%

## Bitpacking
Obviously, we can compact our memory even further by using a bitpacking data structure. In my example code above I used std::vector\<bool\>, which is bitpacked, but with our wheel factorization the math going in and out may get a little complicated.

It's a bit more efficient to avoid the overhead and roll our own.

Now the reason I stopped at `2, 3, 5` wheel factorization should become a bit more clear. There just happen to be *8* possible primes per 30 number segment, which packs nicely into a byte.

After doing both (wheel factorization and bitpacking) we've reduced the memory requirements to find all primes up to 10^9 from 1 GB down to ~33 MB.

# Building

Cmake is used for setting up the build system. On first run it will download a copy of [googletest](https://github.com/google/googletest) to use for unit testing.

```
> mkdir build
> cd build
> cmake ..
> make
```

This will build two binaries `unittest` and `benchmark`, that do exactly what their names imply. `benchmark` will take in a command line argument to test different size 'sieving', but is overall not very sophisticated.
