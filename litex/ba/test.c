#include "ba_core.h"
#include "ba_types.h"
#include "test_data.h"
#include <stdio.h>

void testmain() {

  // create and initialize test data
  BAData data = {0};
  initialize_ba_data(&data);

  // make a copy of initial state
  BAData initial_data = data;

  // run optimization
  printf("\nRunning bundle adjustment optimization...\n");
  OptimizationStats stats = optimize_ba(&data);

  // report results
  printf("\nOptimization Results:\n");
  printf("Initial average reprojection error: %.3f pixels\n",
         stats.initial_error);
  printf("Final average reprojection error: %.3f pixels\n", stats.final_error);
  printf("Number of iterations: %d\n", stats.iterations);
  printf("Error reduction: %.1f%%\n",
         100.0 * (stats.initial_error - stats.final_error) /
             stats.initial_error);
}
