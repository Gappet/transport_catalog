#pragma once

#include <deque>
#include <string>
#include <tuple>

#include "geo.h"

struct Stop {
  std::string name_;
  transpot_guide::detail::Coordinates
      coordinates_;  /// достаточно относительные пространство имен использовать

  std::tuple<double, double> AsTuple() const;

  bool operator==(const Stop& other) const {
    return AsTuple() == other.AsTuple();
  }
};

struct PairStopHash {
  size_t operator()(std::pair<const Stop*, const Stop*> other) const {
    return hasher_(other.first) + 1000 * hasher_(other.second);
  }
  std::hash<const void*> hasher_;
};

struct Bus {
  std::string name_;
  std::deque<Stop*> route_stops_;
  int stops_on_route = 0;
  int unique_stops = 0;
  double lengh = 0;
  double curvature = 1;
  bool is_roundtrip = false;
  bool operator==(const Bus other) const { return this->name_ == other.name_; }
};
