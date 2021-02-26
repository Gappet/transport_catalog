#include "json_reader.h"

#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>

#include "geo.h"
#include "json.h"
#include "map_renderer.h"

namespace transpot_guide {
namespace input {
using namespace std::string_literals;

void InputData(TransportCatalogue& transport_catalog, json::Array data) {
  std::vector<json::Node> buses;
  std::vector<json::Node> stops;

  for (auto& stop_or_bus : data) {
    if (stop_or_bus.AsMap()["type"s] == "Bus"s) {
      buses.push_back(std::move(stop_or_bus.AsMap()));
    } else {
      stops.push_back(std::move(stop_or_bus.AsMap()));
    }
  }

  for (const auto& stop : stops) {
    json::Dict data_of_stop = stop.AsMap();
    transport_catalog.AddStop(data_of_stop["name"s].AsString(),
                              data_of_stop["latitude"s].AsDouble(),
                              data_of_stop["longitude"s].AsDouble());
  }

  for (auto& stop : stops) {
    json::Dict data_of_stop = stop.AsMap();
    if (data_of_stop.count("road_distances"s)) {
      for (auto& i : data_of_stop["road_distances"s].AsMap()) {
        transport_catalog.AddDistance(data_of_stop["name"s].AsString(), i.first,
                                      i.second.AsInt());
      }
    }
  }

  for (auto& bus : buses) {
    std::vector<std::string> stops_;
    for (json::Node& stop : bus.AsMap()["stops"s].AsArray()) {
      stops_.push_back(std::move(stop.AsString()));
    }
//    if (!bus.AsMap()["is_roundtrip"s].AsBool()) {
//      std::vector<std::string> rev_stop;
//      for (auto itr = stops_.rbegin() + 1; itr < stops_.rend(); ++itr) {
//        rev_stop.push_back(*itr);
//      }
//      stops_.insert(stops_.end(), std::make_move_iterator(rev_stop.begin()),
//                    std::make_move_iterator(rev_stop.end()));
//    }

    transport_catalog.AddRoute(bus.AsMap()["name"s].AsString(), stops_, bus.AsMap()["is_roundtrip"s].AsBool());
  }
}

svg::Color ParsingColor(json::Node& color) {
  svg::Color out;
  if (color.IsString()) {
    return color.AsString();
  }

  if (color.AsArray().size() == 3) {
    return svg::Rgb(color.AsArray()[0].AsInt(), color.AsArray()[1].AsInt(),
                    color.AsArray()[2].AsInt());
  }

  return svg::Rgba(color.AsArray()[0].AsInt(), color.AsArray()[1].AsInt(),
                   color.AsArray()[2].AsInt(), color.AsArray()[3].AsDouble());
}

RenderSettings ReadRenderSettings(json::Dict data) {
  RenderSettings settings;
  settings.width = data["width"s].AsDouble();
  settings.height = data["height"].AsDouble();
  settings.padding = data["padding"].AsDouble();
  settings.line_width = data["line_width"].AsDouble();
  settings.stop_radius = data["stop_radius"].AsDouble();
  settings.bus_label_front_size = data["bus_label_font_size"].AsDouble();
  settings.bus_label_offset =
      svg::Point(data["bus_label_offset"].AsArray()[0].AsDouble(),
                     data["bus_label_offset"].AsArray()[1].AsDouble());
  settings.stop_label_font_size = data["stop_label_font_size"].AsInt();
  settings.stop_label_offset =
      svg::Point(data["stop_label_offset"].AsArray()[0].AsDouble(),
                     data["stop_label_offset"].AsArray()[1].AsDouble());
  settings.underlayer_color = ParsingColor(data["underlayer_color"]);
  settings.underlayer_width = data["underlayer_width"].AsDouble();
  for (auto& color : data["color_palette"].AsArray()) {
    settings.color_palette.push_back(ParsingColor(color));
  }
  return settings;
}

}  // namespace input
}  // namespace transpot_guide
