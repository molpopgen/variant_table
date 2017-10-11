#include "VariantMatrix.hpp"
#include <algorithm>
#include <stdexcept>

using namespace Sequence;
using namespace std;

int
main(int argc, char** argv)
{
	// Construct two haplotypes
    vector<int8_t> hap1{ 0, 1, 1, 0 };
    vector<int8_t> hap2{ 1, 0, 1, 1 };

    vector<double> pos{ 0, 1, 2, 3 };
	// Convert the data into our new format
    vector<int8_t> data;
    for (size_t i = 0; i < hap1.size(); ++i)
        {
            data.push_back(hap1[i]);
            data.push_back(hap2[i]);
        }


	// This vector of vector thing is like how 
	// Sequence::PolyTable currently does things
    vector<vector<int8_t>> like_libseq{ move(hap1), move(hap2) };

	// Create our new type
    VariantMatrix m(move(data), move(pos));
	if (m.nsam != 2)
	{
		throw runtime_error("nsam is wrong");
	}
	if (m.nsites != 4)
	{
		throw runtime_error("number of variants is wrong");
	}
	if (m.positions.size() != 4)
	{
		throw runtime_error("length of positions vector is wrong");
	}
    for (size_t i = 0; i < m.nsam; ++i)
        {
            for (size_t j = 0; j < m.nsites; ++j)
                {
                    // The indexing is a potential gotcha:
                    if (m.get(j, i) != like_libseq[i][j])
                        {
                            throw std::runtime_error(
                                "Houston, we have a problem!");
                        }
                    if (m.at(j, i) != like_libseq[i][j])
                        {
                            throw std::runtime_error(
                                "Houston, we have a problem, part deux!");
                        }
                }
        }
}
