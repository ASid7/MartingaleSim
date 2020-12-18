/*
Simplest Martingale simulator
https://en.wikipedia.org/wiki/Martingale_(betting_system)

This is one function implamentation ;)

*/

#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <numeric>


int main(int args, char* argv[])
try
{

	double initial_sum = 1024.0;
	double initial_bet_size = 1.0;
	double initial_bet_coef = 2.0;
	double max_bet_number = 100.0;
	double bet_determination_number = 48.0;
	double bet_coef_changer = 1.0;
	std::size_t max_repeats = 1000;

	try {
		if (args > 1)initial_sum = std::stod(argv[1]);
		if (args > 2)initial_bet_size = std::stod(argv[2]);
		if (args > 3)initial_bet_coef = std::stod(argv[3]);
		if (args > 4)bet_coef_changer = std::stod(argv[4]);
		if (args > 5)max_bet_number = std::stoi(argv[5]);
		if (args > 6)bet_determination_number = std::stoi(argv[6]);
		if (args > 7)max_repeats = std::stoi(argv[7]);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Use programm with optional parameters by number:\n";
		std::cout << "1: " << "\tinitial amount, default 1024.0\n";
		std::cout << "2: " << "\tinitial and minimal bet_size, default 1.0\n";
		std::cout << "3: " << "\tinitial bet size multiplier, default 2.0\n";
		std::cout << "4: " << "\tmultiplier for bet size multiplier, default 1.0\n";
		std::cout << "5: " << "\tmaximum random number, default 100.0, min random number always 0.0\n";
		std::cout << "6: " << "\tbet result number, win if strictly less, default 48.0\n";
		std::cout << "7: " << "\tmaximum number of losses to reset strategy, default 1000" << std::endl;
		return EXIT_FAILURE;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, max_bet_number);

	double bet_size = initial_bet_size;
	double sum = initial_sum;
	double max_sum = 0.0;
	double bet_inc_coef = initial_bet_coef;

	std::size_t bet_counter = 0;
	std::size_t win_counter = 0;
	std::size_t cur_wins_number = 0;
	std::size_t cur_loss_number = 0;

	std::vector<std::size_t> wins_series;
	auto SaveWinsSeries = [&wins_series](std::size_t series_lenght)
	{
		if (series_lenght > 0)
		{
			if (wins_series.size() <= series_lenght) wins_series.resize(series_lenght + 1);
			++wins_series.at(series_lenght);
		}
	};

	std::vector<std::size_t> losses_series;
	auto SaveLossSeries = [&losses_series](std::size_t series_lenght)
	{
		if (series_lenght > 0)
		{
			if (losses_series.size() < series_lenght) losses_series.resize(series_lenght);
			++losses_series.at(series_lenght - 1);
		}
	};

	while (sum > bet_size)
	{

		auto bet = dis(gen);
		++bet_counter;
		std::cout << bet_counter << '\t' << sum << '\t' << bet;

		if (bet < bet_determination_number)
		{
			SaveLossSeries(cur_loss_number);
			cur_loss_number = 0;

			++win_counter;
			++cur_wins_number;
			sum += bet_size;
			if (sum > max_sum) max_sum = sum;
			std::cout << '\t' << "win!" << '\t' << bet_size << '\t' << sum;

			// reset bet size on Win
			bet_size = initial_bet_size;
			bet_inc_coef = initial_bet_coef;

			std::cout << '\t' << bet_size;
		}
		else
		{
			SaveWinsSeries(cur_wins_number);
			cur_wins_number = 0;

			++cur_loss_number;
			sum -= bet_size;
			std::cout << '\t' << "loss" << '\t' << bet_size << '\t' << sum;

			// change bet size on loss
			bet_size *= bet_inc_coef;
			if (bet_size < initial_bet_size) bet_size = initial_bet_size;
			bet_inc_coef *= bet_coef_changer;

			if (cur_loss_number == max_repeats)
			{
				std::cout << '\t' << "max losses detected:" << cur_loss_number << ". Reset bet!";
				if (losses_series.size() <= cur_loss_number) losses_series.resize(cur_loss_number + 1);
				++losses_series.at(cur_loss_number);
				bet_inc_coef = initial_bet_coef;
				bet_size = initial_bet_size;
			}
			std::cout << '\t' << bet_size;

		}
		std::cout << '\n';
	} //while

	SaveWinsSeries(cur_wins_number);
	SaveLossSeries(cur_loss_number);

	auto PrintStatistics = [](const std::vector<std::size_t>& bet_numbers)
	{
		auto total_bet_secuences = std::accumulate(std::cbegin(bet_numbers), std::cend(bet_numbers), 0);
		int num = 1;
		for (auto& number : bet_numbers)
		{
			std::cout << num << "\t\t" << number << "\t~" << 100 * number / total_bet_secuences << "%\n";
			++num;
		}
	};

	std::cout << "Wins counter\tAmount\n";
	PrintStatistics(wins_series);
	std::cout << "Loss counter\tAmount\n";
	PrintStatistics(losses_series);

	std::cout << "start with " << initial_sum << " end with " << sum << " loss " << (initial_sum - sum) << '\n';
	std::cout << "max sum:" << max_sum << " bet size:" << initial_bet_size << "  bet coef:" << initial_bet_coef
		<< "mul for bet coef:" << bet_coef_changer << '\n';
	std::cout << "wins:" << win_counter << '(' << (static_cast<double>(win_counter) / bet_counter) * 100 << "%)\n";
	std::cout << "loss:" << (bet_counter - win_counter) << '('
		<< (static_cast<double>(bet_counter - win_counter) / bet_counter) * 100 << "%)\n";

} // int main(...) try
catch (const std::exception& e)
{
	std::cout << e.what() << std::endl;
	return EXIT_FAILURE;
}
