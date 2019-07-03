# GTOD-resolution

The `gettimeofday` call is often critized for its lack of guarantees about
resolution and monotony, but often used due to its ubiquitous availability.
While monotony is tricky to enforce (NTP disabled, no manual time updates),
resolution can simply be measured for the system you are using to measure time.
This simple tool measures the average and worst case resolution (including
overhead of the call itself) of the `gettimeofday` call.
It uses the approach from [Finney 2001](https://link.springer.com/article/10.3758/BF03195362).

It executes a series of consecutive timing calls, calculates the time deltas
between them, and reports the average and maximum deltas, as well as a
potential integration error.
The time deltas are summed into a double value using the kahan summation
algorithm and then compared to an additional time measurement that surrounds
the inner timing loop.
The difference between the sum and the measured time indicates problematic
control flow overhead or rounding errors during the summation.

# Usage

Usage is simple:

```
$ ./gtod-resolution [iterations (10.000.000)]
```

An example run from one of our systems looks like this:

```
$ ./gtod-resolution
N = 10000000
error: 0.000000us
mean:  0.044175us
max:   666.000000us
```

This result is interpreted as follows:

- `mean:  0.04us`: The average resolution of the `gettimeofday` call is 0.04
  microseconds.
- `max:   666us`: The worst case resolution of the `gettimeofday` call is 666
  microseconds. A major factor for this variable is the overall system load.
- `N = 10000000`: 10M timing measurements have been computed.
- `error: 0.00us`: The difference between the sum of all time deltas and the
  measured total time is negligable.

# Meta

Authors: Alexander Matz @ 2019
License: MIT
