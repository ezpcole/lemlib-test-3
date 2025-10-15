#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_
#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "api.h"          // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/apix.h"    // IWYU pragma: keep

#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
void ready_blocks();
typedef struct optical_data {
  double hue;
  double saturation;
  double brightness;
  int32_t proximity;
} optical_data_t;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#endif

#endif // _PROS_MAIN_H_
