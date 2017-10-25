#include "thetapi.hpp"

namespace Sequence
{
    double
    thetapi(const std::vector<StateCounts>& sc)
    {
        double ssh = 0.0;
        for (const auto& sci : sc)
            {
                const double denom = static_cast<double>(sci.n)
                                     * static_cast<double>(sci.n - 1);
                double homozygosity = 0.0;
                for (const auto& state : sci.counts)
                    {
                        if (state.first >= 0)
                            {
                                homozygosity
                                    += static_cast<double>(state.second)
                                       * static_cast<double>(state.second - 1);
                            }
                    }
                homozygosity /= denom;
                ssh += 1.0 - homozygosity;
            }
        return ssh;
    }
}
