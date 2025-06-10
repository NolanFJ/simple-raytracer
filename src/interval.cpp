#include "interval.h"
#include "math.h"

// Define static members
const Interval Interval::empty{ Interval(infinity, -infinity) };
const Interval Interval::universe{ Interval(-infinity, infinity) };