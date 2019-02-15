#pragma once
#ifndef HEADER_H
#define HEADER_H
#include <vector>
#include <set>
#include <map>

using namespace std;

typedef set <int> T_indexes;
typedef T_indexes T_stops;
typedef T_stops T_route;
typedef vector <T_route> T_routes;
typedef int T_cost;
typedef map <T_cost, T_stops> T_stops_of_cost;

struct T_friend
{
	int money_;
	int stopInd_;
	bool hasSeasonTicket_;
	T_stops_of_cost stopsOfCost_;
	T_friend()
		:
		money_{},
		stopInd_{},
		hasSeasonTicket_{}
	{}
	typedef vector <T_friend> T_friends;
	void fillStopsOfCost(int ticketPrice, T_routes const & routes);
	void fillStopsOfCostWithSeasonTicket(T_routes const & routes);
	void fillStopsOfCostWithMoney(int ticketPrice, T_routes const & routes);
	static bool stopsHasStopFromRout(T_stops const & stops, T_route const & route);
	bool successfullySetStopCost(int stopInd, int & stopCost) const;
	bool successfullySetStopTotalCost(int stopInd, T_friends const & friends, int & resStopTotalCost);
	void calcResult(int stopIndMin, int nStopIndMax, T_friends const & friends);
};
#endif