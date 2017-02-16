## Calculate Shannon's entropy

* C++ application, could accept a file or distribution with provided parameters, and calculate its entropy according to Shannon's defenition
* We take a probability to meet any byte in the file, multiply to its logarithm and accumulate for all possible bytes (yeah, 0x0 to 0xFF)
* See more detailed explanation in Wiki
* In case of a file we can do estimation about format
* We support uniform and normal distribution. In case of normal distribution we could assign mean and standard deviation
* Application made with a research purpose

## Explanation

* https://en.wiktionary.org/wiki/Shannon_entropy
* https://en.wikipedia.org/wiki/Normal_distribution
* https://en.wikipedia.org/wiki/Discrete_uniform_distribution
* Entropy value calculated without a leading coefficient, it means it could be a float value in range (0, 8), where 0.0 is an absolute order, 8.0 is an absolute chaos.

## Building and some specific

* Application is CMake-based and could be compiled on any platform that have CMake 3.0+ installed
* Just create build directory in the project catalog `mkdir build`, enter `cd build` it and execute `cmake ..`
* Boost required for compilation, we use console-based progress-bar to make entropy calculation look pretty
* I had to provide `std::codecvt<std::uint8_t>` specialization for binary file streams. MS compiler provide one, but GCC does not (and doesn't have to as it's not a C++ Standard requirement)
* Specific installation does not required, application is portable


## Applications

* Could be used to detect whether file was encrypted or archived (the value should be close to 8.0)
* Could be used for building correlation between data format and its entropy (dataset should be big enough)
* Could be used for estimation between distribution properties and entropy
* Application is intentionaly as simple as possible
