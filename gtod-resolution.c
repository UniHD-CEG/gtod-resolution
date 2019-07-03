#include <sys/time.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

double kahanSum(int64_t *nums, int count) {
    double sum = 0.0f;
    double c = 0.0f;
    double t, y;
    int i;
    for (i = 0; i < count; ++i) {
        y = (double)nums[i] - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

int main(int argc, char** argv) {
  int n = 10000000;
  if (argc > 1) {
    n = atoi(argv[1]);
  }
  printf("N = %d\n", n);
  int64_t *diffs = malloc(sizeof(int64_t) * n);
  assert(diffs != NULL);

  struct timeval last;
  gettimeofday(&last, NULL);
  struct timeval start = last;
  for (int i = 0; i < n; ++i) {
    struct timeval now;
    gettimeofday(&now, NULL);
    diffs[i] = (int64_t)(now.tv_usec - last.tv_usec) +
        (int64_t)(now.tv_sec - last.tv_sec) * 1000000;
    last = now;
  }
  struct timeval stop = last;

  double total = (double)(stop.tv_usec - start.tv_usec) +
      (double)(stop.tv_sec - start.tv_sec) * 1000000;
  double sum = kahanSum(diffs, n);
  double mean = sum / n;
  double max = 0;
  for (int i = 0; i < n; ++i) if (diffs[i] > max) max = diffs[i];
  printf("error: %lfus\n", total - sum);
  printf("mean:  %lfus\n", mean);
  printf("max:   %lfus\n", max);
  free(diffs);
}
