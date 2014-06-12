#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <algorithm>

enum class Direction
{
	Up,
	Down,
	Left,
	Right,
	None
};

std::vector<int> string_To_Puzzle(const std::string& s)
{
	std::vector<int> puzzle;

	for (char c : s) {
		if (isdigit(c)) {
			puzzle.push_back(c - '0');
		}
	}

	return puzzle;
}

Direction oposite(Direction d)
{
	return d == Direction::Right ? Direction::Left  :
		   d == Direction::Left  ? Direction::Right :
		   d == Direction::Up    ? Direction::Down  :
		   d == Direction::Down  ? Direction::Up    :
		                          Direction::None;
}

std::pair<std::vector<int>, std::vector<Direction>> move(const std::pair<std::vector<int>, std::vector<Direction>>& state,
	const Direction direction,
	const bool check_origin,
	const int blank_position)
{
	const Direction origin = state.second.size() ? state.second.front() : Direction::None;
	if (check_origin && origin != Direction::None) {
		if (direction == oposite(origin))
			return make_pair(std::vector<int>(), std::vector<Direction>());
	}

	int new_blank_position = -1;
	if (direction == Direction::Up)
		new_blank_position = blank_position - 3;
	else if (direction == Direction::Left)
		new_blank_position = blank_position % 3 == 0 ? -1 : blank_position - 1;
	else if (direction == Direction::Right)
		new_blank_position = blank_position % 3 == 2 ? -1 : blank_position + 1;
	else if (direction == Direction::Down)
		new_blank_position = blank_position + 3;

	if (new_blank_position >= 0 && new_blank_position < 9) {
		std::pair<std::vector<int>, std::vector<Direction>> new_state(state);
		std::swap(new_state.first[new_blank_position], new_state.first[blank_position]);
		new_state.second.push_back(direction);
		return new_state;
	}

	return make_pair(std::vector<int>(), std::vector<Direction>());
}

std::vector<std::pair<std::vector<int>, std::vector<Direction>>> generate_Next_States(const std::pair<std::vector<int>, std::vector<Direction>> & state)
{
	const int blank_position = std::find(state.first.cbegin(), state.first.cend(), 0) - state.first.cbegin();
	std::vector<std::pair<std::vector<int>, std::vector<Direction>>> next_states;

	for (auto direction : { Direction::Up, Direction::Left, Direction::Right, Direction::Down }) {
		const auto next_state = move(state, direction, true, blank_position);
		if (next_state.first.size())
			next_states.push_back(next_state);
	}

	return next_states;
}

std::string print_Direction(const Direction d)
{
	if (d == Direction::Up)
		return "UP";
	else if (d == Direction::Left)
		return "Left";
	else if (d == Direction::Right)
		return "RIGHT";
	else if (d == Direction::Down)
		return "DOWN";
	else if (d == Direction::None)
		return "INITIAL STATE";
}

void print_Puzzle(const std::pair<std::vector<int>, std::vector<Direction>>& p)
{
	int i = 0;

	if (p.second.size())
		std::cout << print_Direction(p.second.back()) << std::endl;
	else
		std::cout << "INITIAL POSITION" << std::endl;
	for (int n : p.first) {
		if (i % 3 == 0) {
			std::cout << std::endl;
		}

		std::cout << n << ' ';
		i++;
	}
	std::cout << '\n' << std::endl;
}

void print_Puzzle(const std::pair<std::vector<int>, std::vector<Direction>>& p, Direction d)
{
	int i = 0;

	std::cout << print_Direction(d) << std::endl;
	for (int n : p.first) {
		if (i % 3 == 0) {
			std::cout << std::endl;
		}

		std::cout << n << ' ';
		i++;
	}
	std::cout << '\n' << std::endl;
}

std::vector<std::pair<std::vector<int>, std::vector<Direction>>> next_Level(std::vector<std::pair<std::vector<int>, std::vector<Direction>>>& current)
{
	std::vector<std::pair<std::vector<int>, std::vector<Direction>>> next_level;

	for (std::pair<std::vector<int>, std::vector<Direction>>& i : current) {
		const auto next_states = generate_Next_States(i);
		next_level.insert(next_level.end(), next_states.begin(), next_states.end());
	}

	return next_level;
}

void print_Trace(const std::pair<std::vector<int>, std::vector<Direction>>& initial, const std::pair<std::vector<int>, std::vector<Direction>>& moves)
{
	print_Puzzle(initial);
	std::pair<std::vector<int>, std::vector<Direction>> current = initial;
	for (const Direction& d: moves.second) {
		const int blank_position = std::find(current.first.begin(), current.first.end(), 0) - current.first.begin();
		current = move(current, d, false, blank_position);
		print_Puzzle(current, d);
	}
}

void breadth_First_Search(const std::pair<std::vector<int>, std::vector<Direction>>& state,
	const std::vector<int>& f)
{
	for (std::vector<std::pair<std::vector<int>, std::vector<Direction>>> current_level = generate_Next_States(state);
		next_Level(current_level).size() > 0;
		current_level = next_Level(current_level))
	for (std::pair<std::vector<int>, std::vector<Direction>>& i : current_level) {
		if (i.first == f) {
			print_Trace(state, i);
			return;
		}
	}
}

int main()
{
	std::string input_initial_state, input_final_state;

	/*std::cout << "Enter 8-puzzle starting state by rows (0 for blank): ";
	std::getline(std::cin, input_initial_state);

	std::cout << "Enter ending state by rows (0 for blank): ";
	std::getline(std::cin, input_final_state);*/

	const std::vector<int> initial_puzzle = { 1, 2, 3, 0, 4, 5, 6, 7, 8 };
	const std::pair<std::vector<int>, std::vector<Direction>> initial_state = make_pair(initial_puzzle, std::vector<Direction>()); //string_To_Puzzle(input_initial_state);
	const std::vector<int> final_puzzle = { 1, 2, 3, 6, 4, 5, 0, 7, 8 };//{ 2, 3, 5, 1, 4, 0, 6, 7, 8 };//{ 1, 2, 5, 3, 7, 4, 6, 0, 8 }; //
	const std::pair<std::vector<int>, std::vector<Direction>> final_state = make_pair(final_puzzle, std::vector<Direction>()); //string_To_Puzzle(input_final_state);

	breadth_First_Search(initial_state, final_puzzle);

	char c;
	std::cin >> c;
	return 0;
}




