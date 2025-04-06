#ifndef BA_CORE_H
#define BA_CORE_H

#include "ba_types.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Simple pin-hole camera model parameters
#define FOCAL_LENGTH 500.0
#define PRINCIPAL_POINT_X 320.0
#define PRINCIPAL_POINT_Y 240.0

#define MAX_ITERATIONS 100
#define INITIAL_LEARNING_RATE 0.01
#define MIN_LEARNING_RATE 0.0001
#define MOMENTUM 0.9
#define LEARNING_RATE 0.01
#define ERROR_THRESHOLD 0.5 // pixel error threshold for convergence

typedef struct {
  double landmark_update[3];
  double pose_update[3];
} ParameterUpdate;

// Rotation matrix structure for convenience
typedef struct {
  double data[3][3];
} RotationMatrix;

typedef struct {
  double initial_error;
  double final_error;
  int iterations;
  int num_measurements;
} OptimizationStats;

// Creates rotation matrix from rx, ry, rz Euler angles
void create_rotation_matrix(const double *angles, RotationMatrix *R) {
  double rx = angles[0], ry = angles[1], rz = angles[2];

  // Pre-calculate common terms
  double cx = cos(rx), sx = sin(rx);
  double cy = cos(ry), sy = sin(ry);
  double cz = cos(rz), sz = sin(rz);

  // Calculate rotation matrix elements
  R->data[0][0] = cy * cz;
  R->data[0][1] = -cy * sz;
  R->data[0][2] = sy;

  R->data[1][0] = cz * sx * sy + cx * sz;
  R->data[1][1] = cx * cz - sx * sy * sz;
  R->data[1][2] = -cy * sx;

  R->data[2][0] = -cx * cz * sy + sx * sz;
  R->data[2][1] = cz * sx + cx * sy * sz;
  R->data[2][2] = cx * cy;
}

void project_point_detailed(const Pose *pose, const Landmark *landmark) {
  printf("\nStep-by-Step Projection Process:\n");
  printf("1. Initial 3D Point: (%.2f, %.2f, %.2f)\n", landmark->point[0],
         landmark->point[1], landmark->point[2]);

  // Step 1: Transform point from world to camera coordinates
  RotationMatrix R;
  create_rotation_matrix(pose->params + 3, &R);

  printf("\n2. Rotation Matrix:\n");
  for (int i = 0; i < 3; i++) {
    printf("[%.2f, %.2f, %.2f]\n", R.data[i][0], R.data[i][1], R.data[i][2]);
  }

  // Apply transformation
  double transformed[3] = {0, 0, 0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      transformed[i] += R.data[i][j] * landmark->point[j];
    }
    transformed[i] += pose->params[i]; // Add translation
  }

  printf("\n3. Point in Camera Coordinates: (%.2f, %.2f, %.2f)\n",
         transformed[0], transformed[1], transformed[2]);

  // Step 2: Perspective projection
  double projected[2];
  projected[0] =
      (FOCAL_LENGTH * transformed[0] / transformed[2]) + PRINCIPAL_POINT_X;
  projected[1] =
      (FOCAL_LENGTH * transformed[1] / transformed[2]) + PRINCIPAL_POINT_Y;

  printf("\n4. Perspective Projection:\n");
  printf("   X: %.2f = %.1f * %.2f / %.2f + %.1f\n", projected[0], FOCAL_LENGTH,
         transformed[0], transformed[2], PRINCIPAL_POINT_X);
  printf("   Y: %.2f = %.1f * %.2f / %.2f + %.1f\n", projected[1], FOCAL_LENGTH,
         transformed[1], transformed[2], PRINCIPAL_POINT_Y);

  printf("\n5. Final Image Coordinates: (%.2f, %.2f)\n", projected[0],
         projected[1]);
}

// Project a 3D point into 2D image coordinates
void project_point(const Pose *pose, const Landmark *landmark,
                   double projected[2]) {
  // 1. Create rotation matrix from pose angles
  RotationMatrix R;
  create_rotation_matrix(pose->params + 3, &R); // +3 to skip translation

  // 2. Apply rotation and translation (R|t)
  double transformed[3];
  for (int i = 0; i < 3; i++) {
    transformed[i] = 0;
    // Rotate
    for (int j = 0; j < 3; j++) {
      transformed[i] += R.data[i][j] * landmark->point[j];
    }
    // Translate
    transformed[i] += pose->params[i];
  }

  // 3. Perspective projection
  if (fabs(transformed[2]) < 1e-10) { // Check division by zero
    projected[0] = INFINITY;
    projected[1] = INFINITY;
    return;
  }

  // Apply pinhole camera model
  projected[0] =
      (FOCAL_LENGTH * transformed[0] / transformed[2]) + PRINCIPAL_POINT_X;
  projected[1] =
      (FOCAL_LENGTH * transformed[1] / transformed[2]) + PRINCIPAL_POINT_Y;
}

// Calculate reprojection error between projected and observed point
double calculate_error(const double projected[2], const double observed[2]) {
  double dx = projected[0] - observed[0];
  double dy = projected[1] - observed[1];
  return sqrt(dx * dx + dy * dy);
}

double calculate_total_error(const BAData *data) {
  double total_error = 0.0;
  int num_valid = 0;

  if (!data) {
    printf("Error: data is NULL\n");
  }
  // printf("NUM_PROJECTIONS = %d\n", NUM_PROJECTIONS);
  // printf("data->projections = %p\n", (void *)data->projections);
  // printf("data->poses = %p\n", (void *)data->poses);
  // printf("data->landmarks = %p\n", (void *)data->landmarks);
  for (int i = 0; i < NUM_PROJECTIONS; i++) {
    const Projection *proj = &data->projections[i];
    const Pose *pose = &data->poses[proj->pose_idx];
    const Landmark *landmark = &data->landmarks[proj->landmark_idx];

    // project point
    double predicted[2];
    project_point(pose, landmark, predicted);

    // skip if projection is invalid (point behind camera)
    if (isinf(predicted[0]) || isinf(predicted[1])) {
      continue;
    }

    // add to total error
    total_error += calculate_error(predicted, proj->point);
    // printf("predicted: (%.2f, %.2f)\n", predicted[0], predicted[1]);
    // printf("observed: (%.2f, %.2f)\n", proj->point[0], proj->point[1]);
    // printf("error: %.2f\n", total_error);
    num_valid++;
  }

  return num_valid > 0 ? total_error / num_valid : INFINITY;
}

// Add debug information to optimize_ba function:
OptimizationStats optimize_ba(BAData *data) {
  OptimizationStats stats = {0};
  stats.initial_error = calculate_total_error(data);
  stats.num_measurements = NUM_PROJECTIONS;
  printf("\nInitialized BA Optimization\n");
  printf("Number of projections: %d\n", NUM_PROJECTIONS);
  printf("Number of poses: %d\n", NUM_POSES);
  printf("Number of landmarks: %d\n", NUM_LANDMARKS);
  printf("Initial average reprojection error: %.3f pixels\n",
         stats.initial_error);

  printf("\nStarting optimization with error: %.3f\n", stats.initial_error);

  // initialize parameter updates (for momentum)
  ParameterUpdate previous_updates[NUM_PROJECTIONS] = {0};
  double learning_rate = INITIAL_LEARNING_RATE;
  double previous_error = stats.initial_error;
  int consecutive_bad_updates = 0;

  // Main optimization loop
  for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
    double current_error = calculate_total_error(data);
    double total_adjustment = 0.0; // Track how much we're adjusting parameters

    if (current_error > previous_error) {
      consecutive_bad_updates++;
      learning_rate *= 0.5;
      if (learning_rate < MIN_LEARNING_RATE) {
        printf("learning rate too small, stoping at iteration %d\n", iter);
        break;
      }
    } else {
      consecutive_bad_updates = 0;
      if (current_error < ERROR_THRESHOLD) {
        printf("Converged at iteration %d with error %.3f\n", iter,
               current_error);
        break;
      }
    }

    // Stop if error isn't improving
    if (consecutive_bad_updates >= 3) {
      printf("Error stopped improving at iteration %d\n", iter);
      break;
    }

    // // Check for convergence
    // if(current_error < ERROR_THRESHOLD) {
    //     printf("Converged at iteration %d with error %.3f\n", iter,
    //     current_error); break;
    // }

    // For each projection, adjust pose and landmark to reduce error
    for (int i = 0; i < NUM_PROJECTIONS; i++) {
      Projection *proj = &data->projections[i];
      Pose *pose = &data->poses[proj->pose_idx];
      Landmark *landmark = &data->landmarks[proj->landmark_idx];

      // Get current projection
      double predicted[2];
      project_point(pose, landmark, predicted);

      // Skip if projection is invalid
      if (isinf(predicted[0]) || isinf(predicted[1])) {
        continue;
      }

      // Calculate error vector
      double error_x = proj->point[0] - predicted[0];
      double error_y = proj->point[1] - predicted[1];

      // Calculate new updates with momentum
      ParameterUpdate current_update = {0};

      // Landmark updates
      current_update.landmark_update[0] =
          learning_rate * error_x +
          MOMENTUM * previous_updates[i].landmark_update[0];
      current_update.landmark_update[1] =
          learning_rate * error_y +
          MOMENTUM * previous_updates[i].landmark_update[1];
      current_update.landmark_update[2] =
          learning_rate * (error_x + error_y) * 0.5 +
          MOMENTUM * previous_updates[i].landmark_update[2];

      // Pose updates
      current_update.pose_update[0] =
          learning_rate * error_x * 0.5 +
          MOMENTUM * previous_updates[i].pose_update[0];
      current_update.pose_update[1] =
          learning_rate * error_y * 0.5 +
          MOMENTUM * previous_updates[i].pose_update[1];

      // Apply updates
      landmark->point[0] += current_update.landmark_update[0];
      landmark->point[1] += current_update.landmark_update[1];
      landmark->point[2] += current_update.landmark_update[2];

      pose->params[0] += current_update.pose_update[0];
      pose->params[1] += current_update.pose_update[1];

      // Calculate total adjustment
      total_adjustment += fabs(current_update.landmark_update[0]) +
                          fabs(current_update.landmark_update[1]) +
                          fabs(current_update.landmark_update[2]);

      // Store updates for next iteration
      previous_updates[i] = current_update;
    }

    if (iter % 5 == 0) {
      printf(
          "Iteration %d, Error: %.3f, Learning Rate: %.6f, Adjustment: %.6f\n",
          iter, current_error, learning_rate, total_adjustment);
    }

    previous_error = current_error;
    stats.iterations = iter + 1;

    // Stop if adjustments become too small
    if (total_adjustment < 1e-6) {
      printf("Converged due to minimal adjustments at iteration %d\n", iter);
      break;
    }
  }

  stats.final_error = calculate_total_error(data);
  return stats;
}

#endif // BA_CORE_H
