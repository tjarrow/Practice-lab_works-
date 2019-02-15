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

ok(friends[0].stopsOfCost_.size() == 5, "Number of stops for 1st friend equals 5");
ok(friends[0].stopsOfCost_.size() == 1 &&
   friends[1].stopsOfCost_.size() == 1 &&
   friends[2].stopsOfCost_.size() == 1, "Number of stops for all friends equals 1"); //
//тесты на граничные значения
ok(friends[0].stopsOfCost_.size() == 3, "Number of stops for 1st friend equals 3");
// тесты на неверные значения
ok(friends[0].stopsOfCost_.size() == 5, "Number of stops for 1st friend equals 5");
ok(routes[0].size() < 1, "Route size must be >= 1 - error code = -1" );
return exit_status();
}
#else

#endif
