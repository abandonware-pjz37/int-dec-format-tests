int-dec-format-tests
====================

Comparison of decimal integers formatting algorithms

| mac                             | linux                          |
|---------------------------------|--------------------------------|
| [![Build Status][master]][repo] | [![Build Status][linux]][repo] |

[linux]: https://travis-ci.org/ruslo/int-dec-format-tests.png?branch=travis.linux
[master]: https://travis-ci.org/ruslo/int-dec-format-tests.png?branch=master
[repo]: https://travis-ci.org/ruslo/int-dec-format-tests

### Task
Convert vector of integer values to `char` buffer.

* [input parameters]
(https://github.com/ruslo/int-dec-format-tests/blob/d91111b02d64746b8665f08654bde5bf337c5c5d/source/main.cpp#L107)
* [run engine](https://github.com/ruslo/int-dec-format-tests/blob/master/source/Algos.hpp)
* [algorithms](https://github.com/ruslo/int-dec-format-tests/tree/master/source/algos)

### Algorithms
* [fmt::FormatInt](https://github.com/vitaut/format)
* [boost::spirit::karma](http://www.boost.org/doc/libs/1_55_0/libs/spirit/doc/html/spirit/karma.html)
* [alexandrescu](https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920)
* [tmpbuf](https://github.com/ruslo/int-dec-format-tests/blob/master/source/algos/tmpbuf.hpp)
(no digits counting, no reverse)
* [reverse](https://github.com/ruslo/int-dec-format-tests/blob/master/source/algos/reverse.hpp)
(no digits counting, reverse chars in-situ)
* [counting](https://github.com/ruslo/int-dec-format-tests/blob/master/source/algos/counting.hpp)
(division free [counting](https://github.com/ruslo/int-dec-format-tests/blob/master/source/CountDigits.hpp) algorithm)

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

### Arch 32-bit build
```bash
> cmake -H. -B_builds/make-release -DCMAKE_BUILD_TYPE=Release -DBUILD32=ON
> cmake --build _builds/make-release
> (cd _builds/make-release && ctest -VV)
```

### Options
* Number of iterations can be reduced using `-DFAST_TEST=ON` option
* If `-DLONG_TEST` option is `ON`, number of iterations increased and additional `8Mb` test added
* Set `Algo*::enabled` to `false` to exclude
[algorithm](https://github.com/ruslo/int-dec-format-tests/blob/master/source/Algos.hpp) from test

### Results
* https://github.com/ruslo/int-dec-format-tests/tree/master/results
