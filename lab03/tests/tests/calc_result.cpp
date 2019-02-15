#include "stdafx.h"
#define MY_DEF_USE_LIBTAP
#ifdef MY_DEF_USE_LIBTAP
#define TAP_COMPILE
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include "libtap/cpp_tap.h"
#include "../../main/main/Header.h"
int main(int, char *[])
{
  plan_tests(5);
  plan_tests(5);
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
		friend_cur.fill_stops_of_cost(TICKET_PRICE, routes);
	}
  calc_result(STOP_IND_MIN, n_stop_ind_max, friends);

  ok(stop_ind_res == 1 && stop_total_cost_min == 4, "Stop number and minimum total cost are correct ");
  ok(stop_ind_res == 2 && stop_total_cost_min == 4, "Stop number and minimum total cost are correct ");
  ok(stop_ind_res == 1 && stop_total_cost_min == 8, "Stop number and minimum total cost are correct ");
  ok(stop_ind_res == 1 && stop_total_cost_min == 0, "Stop number and minimum total cost are correct ");
  ok(stop_ind_res == 0 , "Amount of money for each friend must be positive
  - error code == -2 ");
  return exit_status();
}
#else

#endif
