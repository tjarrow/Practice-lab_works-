#include "stdafx.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef set < int > T_indexes;
typedef T_indexes T_stops;
typedef T_stops T_route;
typedef vector < T_route > T_routes;
typedef int T_cost;
typedef map < T_cost, T_stops > T_stops_of_cost;

struct  T_friend
{
	int money_;
	int stop_ind_;
	bool has_season_ticket_;
	T_stops_of_cost stops_of_cost_;
	T_friend()
		:
		money_{},
		stop_ind_{},
		has_season_ticket_{}
	{}

	void fill_stops_of_cost(int ticket_price, T_routes const & routes)
	{
		if (has_season_ticket_)
		{
			fill_stops_of_cost_with_season_ticket(routes);
		}
		else
		{
			fill_stops_of_cost_with_money(ticket_price,routes);
		}
	}

	void  fill_stops_of_cost_with_season_ticket(T_routes const & routes)
	{
		stops_of_cost_[0].insert(stop_ind_);

		T_indexes processed_routes_indexes;
		bool amended{};

		do
		{
			amended = false;

			for (size_t i{}; i < routes.size(); ++i)
			{
				auto const & route_i = routes[i];

				if (processed_routes_indexes.count(i))
				{
					continue;
				}

				if (stops_has_stop_from_rout(stops_of_cost_[0],route_i))
				{
					amended = true;

					stops_of_cost_[0].insert(route_i.begin(),route_i.end());
					processed_routes_indexes.insert(i);
				}
			}
		} while (amended);
	}

	static bool stops_has_stop_from_rout(T_stops const & stops, T_route const & route)
	{
		for (auto stop :stops)
		{
			if (route.count(stop))
			{
				return  true;
			}
		}
		return  false;
	}

	void fill_stops_of_cost_with_money(int ticket_price, T_routes const & routes)
	{
		stops_of_cost_[0].insert(stop_ind_);
		T_indexes   processed_routes_indexes;

		for (int cost_cur{ ticket_price }; cost_cur <= money_; cost_cur += ticket_price)
		{
			bool amended{};
			auto & stops_of_cost_cur = stops_of_cost_[cost_cur];
			auto & stops_of_cost_prev = stops_of_cost_[cost_cur - ticket_price];

			for (size_t i{}; i < routes.size(); ++i)
			{
				auto const & route_i = routes[i];

				if (processed_routes_indexes.count(i))
				{
					continue;
				}

				if (stops_has_stop_from_rout(stops_of_cost_prev, route_i))
				{
					amended = true;
					stops_of_cost_cur.insert(route_i.begin(), route_i.end());
					processed_routes_indexes.insert(i);
				}
			}

			if (!amended)
			{
				break;
			}
		}
	}

	bool successfully_set_stop_cost(int stop_ind, int & stop_cost) const
	{
		for (auto const & cost_and_stops : stops_of_cost_)
		{
			auto cost_cur = cost_and_stops.first;
			auto const & stops_cur = cost_and_stops.second;
			if (stops_cur.count(stop_ind))
			{
				stop_cost = cost_cur;
				return  true;
			}
		}

		return  false;
	}
};

typedef vector < T_friend  > T_friends;

bool successfully_set_stop_total_cost(int stop_ind,T_friends const & friends,int & res_stop_total_cost)
{
	for (auto const & friend_cur :friends)
	{
		int stop_cur_cost{};
		if (!friend_cur.successfully_set_stop_cost(stop_ind,stop_cur_cost))
		{
			return  false;
		}
		res_stop_total_cost += stop_cur_cost;
	}
	return  true;
}
void calc_result(int stop_ind_min,int n_stop_ind_max, T_friends const & friends)
{
	int stop_ind_res{};
	const int STOP_TOTAL_COST_MIN_START{ -1 };
	int stop_total_cost_min{ STOP_TOTAL_COST_MIN_START };

	for (int stop_ind{ stop_ind_min }; stop_ind <= n_stop_ind_max; ++stop_ind)
	{
		int stop_total_cost{};

		if (successfully_set_stop_total_cost(stop_ind, friends, stop_total_cost) && (stop_total_cost_min == STOP_TOTAL_COST_MIN_START || stop_total_cost < stop_total_cost_min))
		{
			stop_ind_res = stop_ind;
			stop_total_cost_min = stop_total_cost;
		}
	}
	cout << stop_ind_res;

	if (stop_ind_res)
	{
		cout << ' ' << stop_total_cost_min;
	}
	cout << endl;
}

int main()
{
	const int TICKET_PRICE = 4;
	const int STOP_IND_MIN = 1;
	int n_stop_ind_max{};
	int m_routes_total{};
	int k_friends_total{};

	cin >> n_stop_ind_max;
	cin >> m_routes_total;

	T_routes routes(m_routes_total);
	
	for (auto & route : routes)
	{
		int route_size{};
		cin >> route_size;

		for (int i{}; i < route_size; ++i)
		{
			int stop_ind{};
			cin >> stop_ind;
			route.insert(stop_ind);
		}
	}

	cin >> k_friends_total;
	T_friends friends(k_friends_total);

	for (auto & friend_cur : friends)
	{
		cin >> friend_cur.money_;
		cin >> friend_cur.stop_ind_;
		cin >> friend_cur.has_season_ticket_;
	}

	for (auto & friend_cur : friends)
	{
		friend_cur.fill_stops_of_cost(TICKET_PRICE,routes);
	}

	calc_result(STOP_IND_MIN,n_stop_ind_max,friends);
}