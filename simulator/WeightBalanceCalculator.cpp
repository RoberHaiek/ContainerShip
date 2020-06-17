#include "../Interfaces/WeightBalanceCalculator.h"
#include <string>
using namespace std;
	//char loadUnload, int kg, int X, int Y
WeightBalanceCalculator::BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int x, int y){
	if(kg < 0 || x < 0 || y<0 || loadUnload=='A')
		return WeightBalanceCalculator::BalanceStatus::APPROVED;

	return WeightBalanceCalculator::BalanceStatus::APPROVED;
}
int WeightBalanceCalculator::readShipPlan(const std::string& full_path_and_file_name){
	if(full_path_and_file_name=="")
		return 0;
	return 0;
	}