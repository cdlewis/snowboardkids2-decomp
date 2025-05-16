#include "common.h"

typedef struct {
    s16 m[9];
} Mat3x3;

void createYRotationMatrix(Mat3x3* matrix, s16 angle);
void createZRotationMatrix(s16 matrix[3][3], s16 angle);
