#ifndef ORIENTATION_H
#define ORIENTATION_H

#ifdef __cplusplus
extern "C" {
#endif

#define REAL double
void exactinit();
REAL orient2d(REAL *pa, REAL *pb, REAL *pc);

#ifdef __cplusplus
}
#endif

#endif // ORIENTATION_H