#include "request_handler.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"
#include "map_renderer.h"

using namespace std::string_literals;

namespace transpot_guide {
namespace output {
json::Node GetInfoRoute(::transpot_guide::TransportCatalogue& transport_catalog,
                        const std::string_view bus, int id) {
  json::Dict out;
  out.insert({"request_id", json::Node(id)});
  if (!transport_catalog.IsBus(bus)) {
    out.insert({"error_message"s, json::Node("not found"s)});
  } else {
    Bus* route = transport_catalog.FindRoute(bus);
    out.insert({"stop_count"s, json::Node(route->stops_on_route)});
    out.insert({"unique_stop_count"s, json::Node(route->unique_stops)});
    out.insert({"curvature"s, json::Node(route->curvature)});
    out.insert({"route_length", json::Node(route->lengh)});
  }
  return json::Node(out);
}

json::Node GetInfoStop(::transpot_guide::TransportCatalogue& transport_catalog,
                       const std::string_view stop, int id) {
  json::Dict out;
  out.insert({"request_id", json::Node(id)});
  if (!transport_catalog.IsStop(stop)) {
    out.insert({"error_message"s, json::Node("not found"s)});
  } else {
    auto buses = transport_catalog.GetBusofStop(stop);
    if (!buses.empty()) {
      json::Array buses_out;
      for (auto& bus : buses) {
        buses_out.push_back(json::Node(std::string(bus)));
      }
      out.insert({"buses"s, json::Node(buses_out)});
    } else {
      out.insert({"buses"s, json::Node(json::Array())});
    }
  }

  return json::Node(out);
}



json::Node OutputData(TransportCatalogue& transport_catalog, json::Array query,
                      RenderSettings& setting) {
  json::Array out;
  for (auto& i : query) {
    if (i.AsMap()["type"s] == "Bus"s) {
      out.push_back(GetInfoRoute(transport_catalog,
                                 i.AsMap()["name"].AsString(),
                                 i.AsMap()["id"].AsInt()));
    }
    if (i.AsMap()["type"s] == "Map"s) {
      out.push_back(GetMapOfRoad(transport_catalog, setting, i.AsMap()["id"].AsInt()));
    }

    if (i.AsMap()["type"s] == "Stop"s) {
      out.push_back(GetInfoStop(transport_catalog, i.AsMap()["name"].AsString(),
                                i.AsMap()["id"].AsInt()));
    }
  }

  return json::Node(out);
}
}  // namespace output
}  // namespace transpot_guide
