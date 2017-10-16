#include "hapcounts.hpp"
#include <algorithm>

using namespace std;

namespace Sequence
{
    bool
    haplotypes_differ(const ConstColView& col_i, const ConstColView& col_j)
    {
        auto diffs = mismatch(col_i.begin(), col_i.end(), col_j.begin(),
                              [](const int8_t a, const int8_t b) {
                                  if (a < 0 || b < 0)
                                      return true;
                                  return a == b;
                              });
        if (diffs.first < col_i.end())
            {
                return true;
            }
        return false;
    }

    vector<pair<size_t, uint32_t>>
    haplotype_counts(const VariantMatrix& m)
    {
        // Step 1: flag each row view as unique or not.
        // We do NOT allow missing data to cause a difference.
        vector<int8_t> is_unique(m.nsam, 1);
        vector<ConstColView> views; // Copying views is cheap
        for (size_t i = 0; i < m.nsam - 1; ++i)
            {
                auto col_i = get_ConstColView(m, i);
                views.push_back(col_i);
                for (size_t j = i + 1; j < m.nsam && is_unique[i]; ++j)
                    {
                        auto col_j = get_ConstColView(m, j);
                        if (!haplotypes_differ(col_i, col_j))
                            {
                                is_unique[j] = 0;
                            }
                    }
            }
        views.push_back(get_ConstColView(m, m.nsam - 1));
        vector<pair<size_t, uint32_t>> rv;
        for (size_t i = 0; i < views.size(); ++i)
            {
                if (is_unique[i])
                    {
                        uint32_t hcount = count_if(
                            views.begin() + i, views.end(),
                            [&views, i](const ConstColView& a) {
                                return !haplotypes_differ(a, views[i]);
                            });
                        rv.push_back(make_pair(i, hcount));
                    }
            }
        return rv;
    }
}
