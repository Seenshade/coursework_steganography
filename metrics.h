#pragma once

#include <fstream>
#include <vector>

struct Metrics{
	double mse;
	double nmse;
};

double EvaluateMSE(const std::vector<char> &lhs, const std::vector<char> &rhs);

double EvaluateNMSE(const std::vector<char> &lhs, const std::vector<char> &rhs);