#include "VariantMatrix.hpp"
#include "VariantMatrixViews.hpp"
#include "filtering.hpp"
#include "statecounts.hpp"
#include "hapcounts.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <gsl/gsl_matrix_char.h>

using namespace Sequence;
using namespace std;

void
gsl_test()
{
    vector<double> pos;
    vector<int8_t> data;
    size_t nsam = 50;
    size_t nsites = 500;
    for (size_t i = 0; i < nsam * nsites; ++i)
        {
            data.push_back(i); // yes, this will overflow a lot
        }
    for (size_t i = 0; i < nsites; ++i)
        pos.push_back(i);
    auto gview = gsl_matrix_char_view_array_with_tda(
        reinterpret_cast<char*>(data.data()), nsites, nsam, nsam);

    VariantMatrix m(data, pos);

    if (m.nsites != gview.matrix.size1 || m.nsam != gview.matrix.size2)
        {
            throw runtime_error("dimension mismatch");
        }
    for (size_t site = 0; site < m.nsites; ++site)
        {
            for (size_t hap = 0; hap < m.nsam; ++hap)
                {
                    auto from_gview = static_cast<int8_t>(
                        gsl_matrix_char_get(&gview.matrix, site, hap));
                    auto from_matrix = m.at(site, hap);
                    if (from_gview != from_matrix)
                        {
                            throw runtime_error("value mismatch");
                        }
                }
        }
}

void
single_site_test()
{
    vector<double> pos{ 1 };
    vector<int8_t> data{ 0, 1 };
    VariantMatrix m(move(data), move(pos));
    for (size_t site = 0; site < m.nsites; ++site)
        {
            for (size_t h = 0; h < m.nsam; ++h)
                {
                    auto g = m.at(site, h);
                }
        }
    cout << "pass single site test\n";
}

void
statecount_test(const VariantMatrix& m)
{
    auto c = process_variable_sites(m);
    for (auto& ci : c)
        {
            for (auto&& cii : ci.counts)
                {
                    cout << int(cii.first) << ' ' << cii.second << '\n';
                }
            cout << "----\n";
        }
}

void
hapcount_test()
{
    vector<int8_t> hap1( 10, 1 );
    vector<int8_t> hap2( 10, 0 );
    vector<int8_t> hap3( 10, 2 );

    vector<double> pos(10, 0.0); // dummy
    vector<int8_t> data;
    for (int i = 0; i < 10; ++i)
        {
            if (i % 2 == 0.)
                {
                    data.insert(data.end(), hap1.begin(), hap1.end());
                }
            else
                {
                    data.insert(data.end(), hap2.begin(), hap2.end());
                }
        }
    data.insert(data.end(), hap3.begin(), hap3.end());
    auto gview = gsl_matrix_char_view_array_with_tda(
        reinterpret_cast<char*>(data.data()),11,10,10); 
    auto transpose
        = gsl_matrix_char_alloc(gview.matrix.size2, gview.matrix.size1);
    auto check = gsl_matrix_char_transpose_memcpy(transpose, &gview.matrix);
    if (check)
        {
            throw runtime_error("transpose failed");
        }
    vector<int8_t> data2(transpose->data,
                         transpose->data
                             + transpose->size1 * transpose->size2);
    VariantMatrix m(data2, pos);
    auto hapcounts = haplotype_counts(m);
    for (auto& hi : hapcounts)
        {
            cout << hi.first << ' ' << hi.second << '\n';
        }
    cout << "done with hapcount test\n";
}

int
main(int argc, char** argv)
{
    single_site_test();
    gsl_test();
    hapcount_test();
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
    statecount_test(m);
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
    auto mcopy = m;
    auto nremoved = filter_sites(m, [](const RowView& row) {
        return count(row.begin(), row.end(), 1) < 2;
    });
    cout << nremoved << ' ' << m.nsites << ' ' << m.nsam << ' '
         << m.data.size() << '\n';
    for (size_t site = 0; site < m.nsites; ++site)
        {
            cout << m.positions[site] << '\n';
            for (size_t h = 0; h < m.nsam; ++h)
                {
                    cout << int(m.at(site, h)) << '\n';
                }
        }
    m = move(mcopy);
    cout << "Haplotype removal test:\n";
    nremoved = filter_haplotypes(m, [](const ColView& col) {
        return count(col.begin(), col.end(), 1) < 3;
    });
    cout << nremoved << ' ' << m.nsites << ' ' << m.nsam << ' '
         << m.data.size() << '\n';
    for (size_t i = 0; i < m.nsam; ++i)
        {
            for (size_t j = 0; j < m.nsites; ++j)
                {
                    cout << int(m.at(j, i));
                }
            cout << '\n';
        }
}
