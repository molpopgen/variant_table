#include "VariantMatrix.hpp"
#include "VariantMatrixViews.hpp"
#include "filtering.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

using namespace Sequence;
using namespace std;

void single_site_test()
{
	vector<double> pos{1};
	vector<int8_t> data{0,1};
	VariantMatrix m(move(data),move(pos));
	for(size_t site = 0 ; site < m.nsites ; ++site)
	{
		for(size_t h=0;h<m.nsam;++h)
		{
			auto g = m.at(site,h);
		}
	}
	cout << "pass single site test\n";
}

int
main(int argc, char** argv)
{
	single_site_test();

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
    auto c0 = get_ColView(m, 0), c1 = get_ColView(m, 1);
    cout << "Swap test:\n";
    for (auto& i : c0)
        cout << int(i);
    cout << '\n';
    for (auto& i : c1)
        cout << int(i);
    cout << '\n';
    swap(c0, c1);
    for (auto& i : c0)
        cout << int(i);
    cout << '\n';
    for (auto& i : c1)
        cout << int(i);
    cout << '\n';

    cout << "Site removal test:\n";
    auto nremoved = filter_sites(m, [](const RowView& row) {
        return count(row.begin(), row.end(), 1) < 2;
    });
	cout << nremoved << ' ' << m.nsites << ' ' << m.nsam << ' ' << m.data.size() << '\n';
	for(size_t site=0;site < m.nsites ; ++site)
	{
		cout << m.positions[site] << '\n';
		for(size_t h = 0 ; h < m.nsam ; ++h)
		{
			std::cout << "h = " << h << '\n';
			cout << int(m.at(site,h)) << '\n';
		}
	}
}
