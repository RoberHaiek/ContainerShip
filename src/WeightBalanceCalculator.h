/*
 * WeightBalanceCalculator.h
 *
 *  Created on: Apr 18, 2020
 *      Author: n_b_a
 */

#ifndef WEIGHTBALANCECALCULATOR_H_
#define WEIGHTBALANCECALCULATOR_H_
#include<iostream>
using namespace std;

enum BalanceStatus  {APPROVED , X_IMBALANCED , Y_IMBALANCED , X_Y_IMBALANCED }BalanceStatus ;


enum BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);


#endif /* WEIGHTBALANCECALCULATOR_H_ */
