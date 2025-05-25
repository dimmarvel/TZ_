#include <cassert>
#include <thread>
#include <iostream>
#include "top_tracker.hpp"

using namespace std::chrono_literals;

void print_actions(const std::vector<PlayerAction>& acts)
{
	for(const auto& it : acts)
	{
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(it.timestamp.time_since_epoch()).count();
		std::cout << "[" << it.player_id << ": " << time << "] " << it.action << std::endl;
	}
}

int main()
{
	// Max 3 actions, alive = 200 мс
	TopTracker tr(3, 200ms);

	tr.add_action({"p1", "jump"});
	assert(tr.get_actions().size() == 1);

	tr.add_action({"p2", "ba_bax1"});
	tr.add_action({"p3", "ba_bax2"});
	tr.add_action({"p4", "ba_bax3"});
	
	print_actions(tr.get_actions());
	assert(tr.get_actions().size() == 3);
	assert(tr.get_actions().front().player_id == "p2");

	std::this_thread::sleep_for(250ms); // wait stale
	tr.prune();
	assert(tr.get_actions().empty());

	std::cout << "All tests passed :) \n";
	return 0;
}