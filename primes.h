#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <thread>

const uint32_t L1D_CACHE_SIZE = 32768;

class threaded_bitpack;

class Primes
{
    public:
        // Member functions
        Primes();
        ~Primes();

        // Test 1 number for primality
        bool isPrime(uint64_t n) const;

        // Creates an internal data structure with all primes up to limit
        // Speeds up isPrime() and is used internally for getList()
        void sieve(uint64_t limit, std::size_t threads = std::thread::hardware_concurrency());

        // Calculate pi(x), number of primes below x
        // Returns the exact number if available, otherwise
        // it returns an upper bound
        uint64_t pi(uint64_t x) const;

        // Get a vector full of primes up to limit
        const std::vector<uint64_t>& getList(uint64_t limit = 0);

    private:
        std::unique_ptr<threaded_bitpack> pSieve;
        std::vector<uint64_t> pList;
};

