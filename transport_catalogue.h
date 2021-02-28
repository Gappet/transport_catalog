#pragma once

#include <deque>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "domain.h"
#include "geo.h"

namespace transpot_guide {

class TransportCatalogue {
 public:
  void AddStop(std::string stop_name, double latitude, double longitude);

  void AddRoute(std::string bus, std::vector<std::string> stops, bool is_roundtrip);

  void AddDistance(std::string stop_from, std::string stop_to, int dist);

  Bus* FindRoute(std::string_view state);

  Stop* FindStop(std::string_view state);

  bool IsBus(std::string_view bus) const;

  bool IsStop(std::string_view stop) const;

  std::set<std::string_view> GetBusofStop(std::string_view stop);

  std::deque<Stop*> GetStops();

  std::deque<Bus*> GetBus();

 private:
  std::deque<Stop> stops_;
  std::deque<Bus> buses_;
  std::unordered_map<std::string_view, Bus*> routes_;
  std::unordered_map<std::string_view, Stop*> stop_coordinate_;
  std::unordered_map<std::string_view, std::set<std::string_view>>
      buses_of_stop_;
  std::unordered_map<std::pair<Stop*, Stop*>, int, PairStopHash>
      lengh_btw_stop_;
};
}  // namespace transpot_guide
