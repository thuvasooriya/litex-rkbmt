#ifndef BA_TYPES_H
#define BA_TYPES_H

// Basic data structures for bundle adjustment
typedef struct {
  double params[6]; // [tx, ty, tz, rx, ry, rz]
} Pose;

typedef struct {
  double point[3]; // [x, y, z]
} Landmark;

typedef struct {
  double point[2];  // [u, v]
  int pose_idx;     // which pose observed this
  int landmark_idx; // which landmark was observed
} Projection;

// Problem size configuration
#define NUM_POSES 3
#define NUM_LANDMARKS 10
#define NUM_PROJECTIONS 20

// Container for all our data
typedef struct {
  Pose poses[NUM_POSES];
  Landmark landmarks[NUM_LANDMARKS];
  Projection projections[NUM_PROJECTIONS];
} BAData;

#endif // BA_TYPES_H
