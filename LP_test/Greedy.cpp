//this file must be included in Build (currently excluded)
#include<iostream> 
#include<set>
#include<vector>

int main()
{
	std::set<int> X{ 1,2,3,4,5,6,7,8,9,10,11,12 };
	std::set<int> S1 = { 1,2,3,4,5,6 };
	std::set<int> S2 = { 5,6,8,9 };
	std::set<int> S3 = { 1,4,7,10 };
	std::set<int> S4 = { 2,5,7,8,11 };
	std::set<int> S5 = { 3, 6, 9, 12 };
	std::set<int> S6 = { 10,11 };
	std::vector<std::set<int>> F{ S1,S2,S3,S4,S5,S6 };

	// let U = X
	std::set<int> U = X;
	// let C = {}
	std::vector<int> C;
	// While U is not empty
	while (!U.empty())
	{
		// Select S from F which maximizes |S ^ U|
		int Smax = 0;
		int currMax = 0;
		//for every S in F
		for (int index = 0; index < F.size(); index++)
		{
			auto& S = F[index];

			int count = 0;
			// for every element e in S
			for (int e : S)
			{
				// check if it is in U
				if (U.find(e) != U.end())
					count++;
			}
			if (count > currMax)
			{
				currMax = count;
				Smax = index;
			}
		}
		auto& S = F[Smax];
		// U = U - S
		for (auto& e : S)
		{
			U.erase(e);
		}

		// C = C U S
		C.push_back(Smax + 1);
	}

	for (int e : C)
	{
		std::cout << e << " ";
	}


	return 0;
}