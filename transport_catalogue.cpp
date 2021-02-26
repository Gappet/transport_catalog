#include "transport_catalogue.h"

#include <algorithm>
#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>

using namespace std::string_literals;

namespace transpot_guide {
void TransportCatalogue::AddStop(std::string stop_name, double latitude,
                                 double longitude) {
  stops_.push_back({std::move(stop_name),
                    {std::move(latitude),
                     std::move(longitude)}});  /// для double излишне делать
                                               /// std::move, но можете оставить
  stop_coordinate_[stops_.back().name_] = &stops_.back();
}

void TransportCatalogue::AddDistance(std::string stop_from, std::string stop_to,
                                     int dist) {
  Stop* from = FindStop(stop_from);
  Stop* to = FindStop(stop_to);
  lengh_btw_stop_[std::make_pair(from, to)] = dist;
}

void TransportCatalogue::AddRoute(std::string bus,
                                  std::vector<std::string> stops,
                                  bool is_roundtrip) {
  std::deque<Stop*> stops_ref;
  for (auto& stop : stops) {
    stops_ref.push_back(FindStop(stop));
  }

  int stops_on_route;
  if (is_roundtrip) {
      stops_on_route = stops.size();
  } else {
      stops_on_route = 2 * stops.size() - 1;
  }
  int unique_stops =
      std::unordered_set<std::string>{stops.begin(), stops.end()}.size();

  double direct_lengh = 0;

  for (auto itr = stops_ref.begin(); itr < stops_ref.end() - 1; ++itr) {
    Stop* from = *itr;
    Stop* to = *(itr + 1);
    direct_lengh += ComputeDistance(from->coordinates_, to->coordinates_);
  }

  if (!is_roundtrip) {
    for (auto itr = stops_ref.rbegin(); itr < stops_ref.rend() - 1; ++itr) {
      Stop* from = *itr;
      Stop* to = *(itr + 1);
      direct_lengh += ComputeDistance(from->coordinates_, to->coordinates_);
    }
  }

  double real_lengh = 0;
  for (auto itr = stops_ref.begin(); itr < stops_ref.end() - 1; ++itr) {
    Stop* from = *itr;
    Stop* to = *(itr + 1);
    if (lengh_btw_stop_.count(std::make_pair(from, to))) {
      real_lengh += lengh_btw_stop_[std::make_pair(from, to)];
    } else if (lengh_btw_stop_.count(std::make_pair(to, from))) {
      real_lengh += lengh_btw_stop_[std::make_pair(to, from)];
    } else {
      real_lengh += ComputeDistance(from->coordinates_, to->coordinates_);
    }
  }

  if (!is_roundtrip) {
    for (auto itr = stops_ref.rbegin(); itr < stops_ref.rend() - 1; ++itr) {
      Stop* from = *itr;
      Stop* to = *(itr + 1);
      if (lengh_btw_stop_.count(std::make_pair(from, to))) {
        real_lengh += lengh_btw_stop_[std::make_pair(from, to)];
      } else if (lengh_btw_stop_.count(std::make_pair(to, from))) {
        real_lengh += lengh_btw_stop_[std::make_pair(to, from)];
      } else {
        real_lengh += ComputeDistance(from->coordinates_, to->coordinates_);
      }
    }
  }

  buses_.push_back({std::move(bus), std::move(stops_ref),
                    std::move(stops_on_route), std::move(unique_stops),
                    std::move(real_lengh), real_lengh / direct_lengh,
                    is_roundtrip});
  routes_[buses_.back().name_] = &buses_.back();
  for (Stop* stop : buses_.back().route_stops_) {
    buses_of_stop_[stop->name_].insert(buses_.back().name_);
  }
}

Bus* TransportCatalogue::FindRoute(std::string_view bus) {
  return routes_[bus];
}

Stop* TransportCatalogue::FindStop(std::string_view stop) {
  return stop_coordinate_[stop];
}

std::set<std::string_view> TransportCatalogue::GetBusofStop(
    std::string_view stop) {
  return buses_of_stop_[stop];
}

bool TransportCatalogue::IsBus(std::string_view bus) const {
  return routes_.count(bus);
}

bool TransportCatalogue::IsStop(std::string_view stop) const {
  return stop_coordinate_.count(stop);
}

const std::deque<Stop>& TransportCatalogue::GetStops() { return stops_; }

const std::deque<Bus>& TransportCatalogue::GetBus() { return buses_; }

}  // namespace transpot_guide
