[[![C Actions](https://github.com/SpiNNakerManchester/spinn_common/actions/workflows/c_actions.yml/badge.svg)](https://github.com/SpiNNakerManchester/spinn_common/actions/workflows/c_actions.yml)
[![Documentation Status](https://readthedocs.org/projects/spinn-common/badge/?version=latest)](https://spinn-common.readthedocs.io/en/latest)

SpiNN Common
============

This is a library of support code for programming in C on the SpiNNaker platform. It is mostly comprised of a high-performance implementation of standard fixed-point arithmetic operations, but it also includes implementations of bit-field handling, a circular buffer support library, and a random number generator (with support for various distributions).

Building
--------

Make sure you have the `SPINN_DIRS` environment variable set to the root of an installation of `spinnaker_tools`, that you have the relevant ARM GCC cross-compilation toolchain installed, and then do `make` followed by `make install`.

Documentation
=============
[SpiNN Common C documentation](http://spinn-common.readthedocs.io/en/latest)
