#include "top_tracker.hpp"

void TopTracker::add_action(const PlayerAction &pa)
{
	prune();
	if (actions_.size() == capacity_)
		actions_.pop_front();
	actions_.push_back(pa);
}

void TopTracker::prune()
{
	const auto now = clock::now();
	while (!actions_.empty() && (now - actions_.front().timestamp) > timeout_)
		actions_.pop_front();
}

std::vector<PlayerAction> TopTracker::get_actions() const
{
	return { actions_.begin(), actions_.end() };
}
