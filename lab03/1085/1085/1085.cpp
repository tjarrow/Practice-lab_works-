#include "stdafx.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm> 

using namespace std;

typedef set < int > T_indexes;
typedef T_indexes T_stops;
typedef T_stops T_route;
typedef vector < T_route > T_routes;
typedef int T_cost;
typedef map < T_cost, T_stops > T_stops_of_cost;

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

	void fillStopsOfCost(int ticketPrice, T_routes const & routes)
	{
		if (hasSeasonTicket_)
		{
			fillStopsOfCostWithSeasonTicket(routes);
		}
		else
		{
			fillStopsOfCostWithMoney(ticketPrice,routes);
		}
	}

	void  fillStopsOfCostWithSeasonTicket(T_routes const & routes)
	{
		stopsOfCost_[0].insert(stopInd_);

		T_indexes processedRoutesIndexes;
		bool amended{};

		do
		{
			amended = false;

			for (size_t i{}; i < routes.size(); ++i)
			{
				auto const & route_i = routes[i];

				if (processedRoutesIndexes.count(i))
				{
					continue;
				}

				if (stopsHasStopFromRout(stopsOfCost_[0],route_i))
				{
					amended = true;
					stopsOfCost_[0].insert(route_i.begin(),route_i.end());
					processedRoutesIndexes.insert(i);
				}
				
			}
		} while (amended);
	}

	static bool stopsHasStopFromRout(T_stops const & stops, T_route const & route)
	{
		for (auto stop :stops)
		{
			if (route.count(stop))
			{
				return true;
			}
		}
		return false;
	}

	void fillStopsOfCostWithMoney(int ticketPrice, T_routes const & routes)
	{
		stopsOfCost_[0].insert(stopInd_);
		T_indexes   processedRoutesIndexes;

		for (int costCur{ ticketPrice }; costCur <= money_; costCur += ticketPrice)
		{
			bool amended{};
			auto & stopsOfcostCur = stopsOfCost_[costCur];
			auto & stopsOfCostPrev = stopsOfCost_[costCur - ticketPrice];

			for (size_t i{}; i < routes.size(); ++i)
			{
				auto const & route_i = routes[i];

				if (processedRoutesIndexes.count(i))
				{
					continue;
				}

				if (stopsHasStopFromRout(stopsOfCostPrev, route_i))
				{
					amended = true;
					stopsOfcostCur.insert(route_i.begin(), route_i.end());
					processedRoutesIndexes.insert(i);
				}
			}

			if (!amended)
			{
				break;
			}
		}
	}

	bool successfullySetStopCost(int stopInd, int & stopCost) const
	{
		for (auto const & costAndStops : stopsOfCost_)
		{
			auto costCur = costAndStops.first;
			auto const & stops_cur = costAndStops.second;
			if (stops_cur.count(stopInd))
			{
				stopCost = costCur;
				return  true;
			}
		}

		return  false;
	}
};

typedef vector <T_friend > T_friends;

bool successfullySetStopTotalCost(int stopInd,T_friends const & friends,int & resStopTotalCost)
{
	for (auto const & friendCur :friends)
	{
		int stopCurCost{};
		if (!friendCur.successfullySetStopCost(stopInd,stopCurCost))
		{
			return  false;
		}
		resStopTotalCost += stopCurCost;
	}
	return  true;
}
void calcResult(int stopIndMin,int nStopIndMax, T_friends const & friends)
{
	int stopIndRes{};
	const int STOP_TOTAL_COST_MIN_START{ -1 };
	int stopTotalCostMin{ STOP_TOTAL_COST_MIN_START };

	for (int stopInd{ stopIndMin }; stopInd <= nStopIndMax; ++stopInd)
	{
		int stopTotalCost{};

		if (successfullySetStopTotalCost(stopInd, friends, stopTotalCost) && (stopTotalCostMin == STOP_TOTAL_COST_MIN_START || stopTotalCost < stopTotalCostMin))
		{
			stopIndRes = stopInd;
			stopTotalCostMin = stopTotalCost;
		}
	}
	cout << stopIndRes;

	if (stopIndRes)
	{
		cout << ' ' << stopTotalCostMin;
	}
	cout << endl;
}

int main()
{
	const int ticketPrice = 4;
	const int stopIndMin = 1;
	int nStopIndMax{}; 
	int mRoutesTotal{};
	int kFriendTotal{};

	cin >> nStopIndMax; 
	cin >> mRoutesTotal; 

	T_routes routes(mRoutesTotal);
	
	for (auto & route : routes)
	{
		int routeSize{};
		cin >> routeSize; 

		for (int i{}; i < routeSize; ++i) 
		{
			int stopInd{};
			cin >> stopInd;
			route.insert(stopInd);
		}
	}

	cin >> kFriendTotal; 
	T_friends friends(kFriendTotal);

	for (auto & friendCur : friends)
	{
		cin >> friendCur.money_; 
		cin >> friendCur.stopInd_; 
		cin >> friendCur.hasSeasonTicket_; 
	}

	for (auto & friendCur : friends)
	{
		friendCur.fillStopsOfCost(ticketPrice,routes);
	}

	calcResult(stopIndMin,nStopIndMax,friends);
	
}