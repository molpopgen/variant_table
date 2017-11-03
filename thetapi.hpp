#ifndef SEQUENCE_SUMMSTATS_THETAPI_HPP__
#define SEQUENCE_SUMMSTATS_THETAPI_HPP__

#include "statecounts.hpp"

namespace Sequence
{
    double thetapi(const std::vector<StateCounts> & sc);
    double thetapi(const std::vector<StateCounts> & sc, int nthreads);
};

#endif
