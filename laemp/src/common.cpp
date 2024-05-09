#include "common.h"

double map(double value, double from_start, double from_end, double to_start, double to_end) {
    double from_range = from_end - from_start;
    double to_range = to_end - to_start;

    return (value - from_start) / from_range * to_range + to_start;
}