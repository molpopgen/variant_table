#include "VariantMatrix.hpp"
#include "VariantMatrixViews.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

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

    // Iterate over row views:
    for (size_t site = 0; site < m.nsites; ++site)
        {
            auto x = get_RowView(m, site);
            if (x.size() != m.nsam)
                {
                    throw std::runtime_error("row size error");
                }
            for (auto i = x.begin(); i != x.end(); ++i)
                {
                    if (m.get(site, i - x.begin()) != *i)
                        {
                            throw std::runtime_error("row iteration error");
                        }
                }
            cout << "reverse iterate: ";
            for (auto i = x.rbegin(); i != x.rend(); ++i)
                {
                    cout << int(*i);
                }
            cout << '\n';
        }
    // Iterate over column views
    for (size_t hap = 0; hap < m.nsam; ++hap)
        {
            auto x = get_ColView(m, hap);
            auto b = x.cbegin();
			//*b=1;
            auto e = x.cend();
            for (; b < e; ++b)
                {
                    cout << int(*b);
                }
            cout << '\n';
            auto b2 = x.cbegin();
            auto e2 = x.cend() - 1;
            for (; e2 >= b2; --e2)
                {
                    cout << int(*e2);
                }
            cout << '\n';
            cout << "reverse iterate haplotype:\n";
            auto b3 = x.rbegin();
            auto e3 = x.rend();
            for (; b3 < e3; ++b3)
                {
					cout << int(*b3);
                }
			cout << '\n';
            for (size_t i = 0; i < x.size(); ++i)
                {
                    if (x[i] != like_libseq[hap][i])
                        {
                            throw std::runtime_error("haplotype error");
                        }
                }
        }
}
