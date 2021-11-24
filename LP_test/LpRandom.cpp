#include <map>
#include <unordered_set>
#include <set>
#include <random>
#include <numeric>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "src/optimization.h"


using namespace alglib;


std::vector<int> roundRandom(const std::vector<double>& p, int reps)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> boolMat(p.size(), 0);
    for (int i = 0; i < p.size(); i++)
    {
        std::discrete_distribution<> d({ 1 - p[i],p[i] });
        for (int j = 0; j < reps; j++)
        {
            boolMat[i] = boolMat[i] || (int)d(gen);
        }
    }
    return boolMat;
}


bool isValidCover(const std::vector<int>& X, const std::vector<std::set<int>>& F, const std::vector<int>& ans)
{
    for (int i = 0; i < X.size(); i++)
    {
        bool isInSomeF = false;
        for (int j = 0; j < F.size(); j++)
        {
            if (ans[j] == 1)
            {
                if (F[j].find(X[i]) != F[j].end())
                {
                    isInSomeF = true;
                }
            }
        }
        if (!isInSomeF)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv)
{
    //inputs
    std::vector<int> X{ 1,2,3,4,5,6,7,8,9,10,11,12 };
    std::set<int> S1 = { 1,2,3,4,5,6 };
    std::set<int> S2 = { 5,6,8,9 };
    std::set<int> S3 = { 1,4,7,10 };
    std::set<int> S4 = { 2,5,7,8,11 };
    std::set<int> S5 = { 3, 6, 9, 12 };
    std::set<int> S6 = { 10,11 };
    std::vector<std::set<int>> F{ S1,S2,S3,S4,S5,S6 };
    //inputs
    /*std::vector<int> X{ 1,2,3};
    std::set<int> S1 = { 1,2};
    std::set<int> S2 = { 2,3 };
    std::set<int> S3 = { 1,3 };
    std::vector<std::set<int>> F{ S1,S2,S3};*/



    std::string aStr = "[";
    for (int iElement = 0; iElement < X.size(); iElement++)
    {
        aStr += "[";
        for (int iF = 0; iF < F.size(); iF++)
        {
            if (F[iF].find(X[iElement]) != F[iF].end())
            {
                aStr += "1";
            }
            else
            {
                aStr += "0";
            }
            if (iF != F.size() - 1)
                aStr += ",";
        }
        aStr += "] ";
        if (iElement != X.size() - 1)
            aStr += ", ";

    }
    aStr += "]";


    std::string alStr = "[ ";
    std::string auStr = "[ ";
    std::string bndlStr = "[ ";
    std::string bnduStr = "[ ";
    std::string cStr = "[ ";
    std::string sStr = "[ ";
    for (int i = 0; i < X.size(); i++)
    {
        alStr += "1";
        auStr += "+inf";
        bndlStr += "0";
        bnduStr += "+1";
        cStr += "1";
        sStr += "1";

        if (i != X.size() - 1)
        {
            alStr += ", ";
            auStr += ", ";
            bndlStr += ", ";
            bnduStr += ", ";
            cStr += ", ";
            sStr += ", ";
        }
    }
    alStr += " ]";
    auStr += " ]";
    bndlStr += " ]";
    bnduStr += " ]";
    cStr += " ]";
    sStr += " ]";


    //constraints
    real_2d_array a = aStr.c_str();
    real_1d_array al = alStr.c_str();
    real_1d_array au = auStr.c_str();

    real_1d_array bndl = bndlStr.c_str();
    real_1d_array bndu = bnduStr.c_str();

    //objective func
    real_1d_array c = cStr.c_str();
    real_1d_array s = sStr.c_str();


    real_1d_array x;
    minlpstate state;
    minlpreport rep;

    int nVars = F.size();
    int nElements = X.size();

    minlpcreate(nVars, state);

    minlpsetcost(state, c);
    minlpsetbc(state, bndl, bndu);
    minlpsetlc2dense(state, a, al, au, nElements);

    minlpsetscale(state, s);

    // Solve
    minlpoptimize(state);
    minlpresults(state, x, rep);

    printf("LP solution : %s\n", x.tostring(3).c_str()); // EXPECTED: [0.5,0.5,0.5]

    std::vector<double> p;
    for (int i = 0; i < nVars; i++)
    {
        p.push_back(x.getcontent()[i]);
    }

    double OPTf = std::accumulate(begin(p), end(p), 0.0);
    std::cout << "Cost: " << OPTf<<"\n\n\n\n";


    int reps = std::ceil(std::log(4 * nElements));
    double CostUpperBound = 4 * std::log(4 * nElements) * OPTf;
    double cost = std::numeric_limits<double>().max();
    std::vector<int> ans;
    int i = 0;
    do
    {
        i++;
        ans = roundRandom(p, reps);
        cost = 0;
        std::cout << "after Rounding iteration-"<<i<<"\nSolution : ";
        for (auto e : ans)
        {
            cost += e;
            std::cout << e << " ";
        }
        std::cout <<"\nCost : " << cost;
    } while (!isValidCover(X, F, ans) and cost >= CostUpperBound);
    

    return 0;
}
