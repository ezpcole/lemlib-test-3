#pragma once
#include "lemlib/api.hpp"
#include "path.h"
#include <vector>

void skills(lemlib::Chassis &chassis, std::vector<point> waypoints,
            intake_states &intake_state, pros::adi::Pneumatics &tongue);