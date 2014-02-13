int-dec-format-tests
====================

Comparison of decimal integers formatting algorithms

### Task
Convert vector of integer values to `char` buffer.

* [input parameters]
(https://github.com/ruslo/int-dec-format-tests/blob/d91111b02d64746b8665f08654bde5bf337c5c5d/source/main.cpp#L107)
* [run engine](https://github.com/ruslo/int-dec-format-tests/blob/master/source/Algos.hpp)
* [algorithms](https://github.com/ruslo/int-dec-format-tests/tree/master/source/algos)

### Usage
* install boost
* verify correctness:
```bash
> cmake -H. -B_builds/make-debug -DCMAKE_BUILD_TYPE=Debug
> cmake --build _builds/make-debug
> (cd _builds/make-debug && ctest -VV)
```
* run tests:
```bash
> cmake -H. -B_builds/make-release -DCMAKE_BUILD_TYPE=Release
> cmake --build _builds/make-release
> (cd _builds/make-release && ctest -VV)
```

### Results
* https://github.com/ruslo/int-dec-format-tests/tree/master/results
