#include "metrics.h"
#include "wav.h"

#include <bitset>
#include <cmath>


double EvaluateMSE(const std::vector<char> &lhs, const std::vector<char> &rhs) {
	char left_buf, right_buf;
	double sum = 0;
	for (size_t i = 0; i < lhs.size(); i++) {
		std::bitset<8> left, right;
		left_buf = lhs.at(i);
		right_buf = rhs.at(i);
		left = left_buf;
		right = right_buf;
		for (size_t j = 0; j < 8; j++) {
			sum += pow((left.test(j) - right.test(j)), 2);
		}
	}
	return sum / (lhs.size() * 8);
}

double EvaluateNMSE(const std::vector<char> &lhs, const std::vector<char> &rhs) {
	char left_buf, right_buf;
	double sum_numerator = 0;
	double sum_denominator = 0;
	for (size_t i = 0; i < lhs.size(); i++) {
		std::bitset<8> left, right;
		left_buf = lhs.at(i);
		right_buf = rhs.at(i);
		left = left_buf;
		right = right_buf;
		for (size_t j = 0; j < 8; j++) {
			sum_numerator += pow((left.test(j) - right.test(j)), 2);
			sum_denominator += pow(left.test(j), 2);
		}
	}
	return sum_numerator / sum_denominator;
}