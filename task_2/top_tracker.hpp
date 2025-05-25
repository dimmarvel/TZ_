#pragma once

#include <deque>
#include <string>
#include <vector>
#include <chrono>

struct PlayerAction
{
	std::string player_id;
	std::string action;
	std::chrono::steady_clock::time_point timestamp;

	PlayerAction(const std::string &player,
				 const std::string &act,
				 std::chrono::steady_clock::time_point ts = std::chrono::steady_clock::now())
		: player_id(player), action(act), timestamp(ts) {}
};

class TopTracker
{
public:
	using clock = std::chrono::steady_clock;

	TopTracker(std::size_t capacity,
			   std::chrono::milliseconds timeout)
		: capacity_(capacity), timeout_(timeout) {}

	void add_action(const PlayerAction &pa);
	void prune();
	std::vector<PlayerAction> get_actions() const;

	std::size_t capacity()  const noexcept { return capacity_; }
	std::chrono::milliseconds timeout() const noexcept { return timeout_; }

private:
	std::size_t                    capacity_;
	std::chrono::milliseconds      timeout_;
	std::deque<PlayerAction>       actions_;
};
