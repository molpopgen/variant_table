#include "filtering.hpp"
#include <algorithm>
#include <limits>
#include <cmath>

namespace Sequence
{
    std::int32_t
    filter_sites(VariantMatrix &m,
                 const std::function<bool(const RowView &)> &f)
    {
        std::int32_t rv = 0;
        auto mask = std::numeric_limits<std::int8_t>::min();
        for (std::size_t i = 0; i < m.nsites; ++i)
            {
                auto row = get_RowView(m, i);
                if (f(row))
                    {
                        ++rv;
                        std::transform(row.begin(), row.end(), row.begin(),
                                       [mask](double) { return mask; });
                        m.positions[i]
                            = std::numeric_limits<double>::quiet_NaN();
                    }
            }
        if (rv)
            {
                m.positions.erase(
                    std::remove_if(m.positions.begin(), m.positions.end(),
                                   [](double d) { return std::isnan(d); }),
                    m.positions.end());
                m.data.erase(std::remove(m.data.begin(), m.data.end(), mask),
                             m.data.end());
                m.nsites -= rv;
            }
        return rv;
    }
}
