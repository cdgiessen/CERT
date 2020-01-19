# ConstExpr RayTracer (CERT)

Behold, the worlds fastest (runtime) raytracer!

Through the power of `constexpr`, we can trace rays in a place that was never meant to ray trace, the compiler.

Currently in a proof of concept state, more features are to be added in the future.

Example image output. 120 by 120 pixels at 25 samples per pixel.

![](https://github.com/cdgiessen/CERT/blob/master/data/120x120at25ssp.jpg)

## Build Requirements
Requires a C++20 ready compiler to build. Tested with clang-10.

Requires a recent version of cmake. Tested with cmake 3.13.4

## Building
Make sure you are running a C++20 capable compiler. Easiest way is to check by running `gcc -std=c++2a` or `clang -std=c++2a`. If the `std=c++2a` is unrecognized, a newer C++ compiler is needed.

To clone and build
```
git clone https://github.com/cdgiessen/CERT.git
mkdir build
cd build
cmake ../CERT
```

## Running
Once build, run the 'cert' executable located in the build directory.
The `output.ppm` should of been created.

## Credits
Based of the venerable [RayTracingInOneWeekend](http://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf)