#include "thetapi.hpp"
#include <numeric>
#include <functional>
#include <future>
#include <iostream>
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

    inline double
    site_homozygosity(const StateCounts& sc)
    {
        const double denom
            = static_cast<double>(sc.n) * static_cast<double>(sc.n - 1);
        double homozygosity = 0.0;
        for (const auto& state : sc.counts)
            {
                if (state.first >= 0)
                    {
                        homozygosity
                            += static_cast<double>(state.second)
                               * static_cast<double>(state.second - 1);
                    }
            }
        homozygosity /= denom;
        return homozygosity;
    }

    inline double
    site_heterozygosity_range(std::vector<StateCounts>::const_iterator beg,
                              std::vector<StateCounts>::const_iterator end)
    {
        return std::accumulate(beg, end, 0.0,
                               [](const double d, const StateCounts& c) {
                                   return d + (1.0 - site_homozygosity(c));
                               });
    }

    double
    thetapi(const std::vector<StateCounts>& sc, const int nthreads)
	//Likely a bad implementation, in that it'll be slower than
	//serial version for small numbers of sites.
    {
        int nthreads_ = nthreads;
        if (nthreads_ < 0
            || nthreads
                   > static_cast<int>(std::thread::hardware_concurrency()))
            nthreads_ = std::thread::hardware_concurrency();
        int block_size = sc.size() / nthreads_;
        std::vector<std::future<double>> futures;
        auto b = sc.begin();
        while (b < sc.end())
            {
                futures.emplace_back(
                    std::async(std::launch::async,
                               std::bind(site_heterozygosity_range, b,
                                         min(b + block_size, sc.end()))));
                b += block_size;
            }
        double ssh = 0.0;
        for (auto& f : futures)
            {
                ssh += f.get();
            }
        return ssh;
    }
}
