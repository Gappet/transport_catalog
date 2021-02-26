#include "map_renderer.h"

#include <iostream>
#include <sstream>

using namespace std::literals;

bool IsZero(double value) { return std::abs(value) < EPSILON; }

SphereProjector CreatorSphereProjector(std::deque<Stop>& stops,
                                       RenderSettings& settings) {
  std::vector<transpot_guide::detail::Coordinates> points;
  for (auto& stop : stops) {
    points.push_back(stop.coordinates_);
  }
  return SphereProjector(points.begin(), points.end(), settings.width,
                         settings.height, settings.padding);
}

std::vector<svg::Polyline> DrawLineofRoad(const std::deque<Bus>& buses,
                                          RenderSettings& settings,
                                          SphereProjector& projector) {
  std::vector<svg::Polyline> lines;
  size_t cnt_color_palette = 0;
  for (const auto& bus : buses) {
    svg::Polyline line;
    if (!bus.route_stops_.empty()) {
      for (auto& stop : bus.route_stops_) {
        line.AddPoint(projector(stop->coordinates_));
        line.SetStrokeColor(settings.color_palette[cnt_color_palette]);
        line.SetStrokeWidth(settings.line_width);
        line.SetFillColor(svg::NoneColor);
        line.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        line.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
      }

      if (!bus.is_roundtrip) {
        for (auto itr = bus.route_stops_.rbegin() + 1;
             itr < bus.route_stops_.rend(); ++itr) {
          line.AddPoint(projector((*itr)->coordinates_));
        }
      }

      if (cnt_color_palette < settings.color_palette.size()) {
        ++cnt_color_palette;
      } else {
        cnt_color_palette = 0;
      }
      lines.push_back(line);
    }
  }
  return lines;
}

std::vector<svg::Text> DrawNameOfRoad(const std::deque<Bus>& buses,
                                      RenderSettings& settings,
                                      SphereProjector& projector) {
  std::vector<svg::Text> NameOfRoad;
  size_t cnt_color_palette = 0;
  for (const auto& bus : buses) {
    if (!bus.route_stops_.empty()) {
      if (bus.is_roundtrip && bus.route_stops_.front()->name_ == bus.route_stops_.back()->name_) {
        svg::Text text_first;
        svg::Text text_second;
        text_first.SetPosition(projector(bus.route_stops_[0]->coordinates_))
            .SetOffset(settings.bus_label_offset)
            .SetFontSize(settings.bus_label_front_size)
            .SetFontFamily("Verdana"s)
            .SetFontWeight("bold")
            .SetData(bus.name_)
            .SetFillColor(settings.color_palette[cnt_color_palette]);

        text_second.SetPosition(projector(bus.route_stops_[0]->coordinates_))
            .SetOffset(settings.bus_label_offset)
            .SetFontSize(settings.bus_label_front_size)
            .SetFontFamily("Verdana"s)
            .SetFontWeight("bold")
            .SetData(bus.name_)
            .SetFillColor(settings.underlayer_color)
            .SetStrokeColor(settings.underlayer_color)
            .SetStrokeWidth(settings.underlayer_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

        NameOfRoad.push_back(text_second);
        NameOfRoad.push_back(text_first);

      } else {
        svg::Text text_first_first_stop;
        svg::Text text_second_first_stop;
        svg::Text text_first_secon_stop;
        svg::Text text_second_second_stop;

        text_first_first_stop
            .SetPosition(projector(bus.route_stops_[0]->coordinates_))
            .SetOffset(settings.bus_label_offset)
            .SetFontSize(settings.bus_label_front_size)
            .SetFontFamily("Verdana"s)
            .SetFontWeight("bold")
            .SetData(bus.name_)
            .SetFillColor(settings.color_palette[cnt_color_palette]);

        text_second_first_stop
            .SetPosition(projector(bus.route_stops_[0]->coordinates_))
            .SetOffset(settings.bus_label_offset)
            .SetFontSize(settings.bus_label_front_size)
            .SetFontFamily("Verdana"s)
            .SetFontWeight("bold")
            .SetData(bus.name_)
            .SetFillColor(settings.underlayer_color)
            .SetStrokeColor(settings.underlayer_color)
            .SetStrokeWidth(settings.underlayer_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

        text_first_secon_stop
            .SetPosition(projector(
                bus.route_stops_[(bus.route_stops_.size() - 1)]->coordinates_))
            .SetOffset(settings.bus_label_offset)
            .SetFontSize(settings.bus_label_front_size)
            .SetFontFamily("Verdana"s)
            .SetFontWeight("bold")
            .SetData(bus.name_)
            .SetFillColor(settings.color_palette[cnt_color_palette]);

        text_second_second_stop
            .SetPosition(projector(
                bus.route_stops_[(bus.route_stops_.size() - 1)]->coordinates_))
            .SetOffset(settings.bus_label_offset)
            .SetFontSize(settings.bus_label_front_size)
            .SetFontFamily("Verdana"s)
            .SetFontWeight("bold")
            .SetData(bus.name_)
            .SetFillColor(settings.underlayer_color)
            .SetStrokeColor(settings.underlayer_color)
            .SetStrokeWidth(settings.underlayer_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

        NameOfRoad.push_back(text_second_first_stop);
        NameOfRoad.push_back(text_first_first_stop);

        NameOfRoad.push_back(text_second_second_stop);
        NameOfRoad.push_back(text_first_secon_stop);
      }

      if (cnt_color_palette < settings.color_palette.size()) {
        ++cnt_color_palette;
      } else {
        cnt_color_palette = 0;
      }
    }
  }
  return NameOfRoad;
}

std::vector<svg::Circle> DrawStop(const std::deque<Stop>& stops,
                                  RenderSettings& settings,
                                  SphereProjector& projector) {
  std::vector<svg::Circle> stops_point;
    for (const auto& stop : stops) {
      svg::Circle stop_point;
      stop_point.SetCenter(projector(stop.coordinates_))
          .SetRadius(settings.stop_radius)
          .SetFillColor("white"s);
      stops_point.push_back(stop_point);
    }

  return stops_point;
}

std::vector<svg::Text> DrawStopName(const std::deque<Stop>& stops,
                                    RenderSettings& settings,
                                    SphereProjector& projector) {
  std::vector<svg::Text> stops_name;
    for (const auto& stop : stops) {
      svg::Text first_text;
      svg::Text second_text;
      first_text.SetPosition(projector(stop.coordinates_))
          .SetOffset(settings.stop_label_offset)
          .SetFontSize(settings.stop_label_font_size)
          .SetFontFamily("Verdana"s)
          .SetData(stop.name_)
          .SetFillColor("black"s);
      second_text.SetPosition(projector(stop.coordinates_))
          .SetOffset(settings.stop_label_offset)
          .SetFontSize(settings.stop_label_font_size)
          .SetFontFamily("Verdana"s)
          .SetData(stop.name_)
          .SetFillColor(settings.underlayer_color)
          .SetStrokeColor(settings.underlayer_color)
          .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
          .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
          .SetStrokeWidth(settings.underlayer_width);
      stops_name.push_back(second_text);
      stops_name.push_back(first_text);
    }
  return stops_name;
}

json::Node GetMapOfRoad(::transpot_guide::TransportCatalogue& transport_catalog,
                        RenderSettings& settings, int id) {
  auto raw_stops = transport_catalog.GetStops();
  std::deque<Stop> stops;
  for (auto& stop : raw_stops) {
    if (!transport_catalog.GetBusofStop(stop.name_).empty()) {
      stops.push_back(stop);
    }
  }

  SphereProjector projector = CreatorSphereProjector(stops, settings);
  std::sort(stops.begin(), stops.end(),
            [](auto& left, auto& right) { return left.name_ < right.name_; });

  auto buses = transport_catalog.GetBus();
  std::sort(buses.begin(), buses.end(),
            [](auto& left, auto& right) { return left.name_ < right.name_; });

  std::vector<svg::Polyline> lines = DrawLineofRoad(buses, settings, projector);
  std::vector<svg::Text> NamesOfRoad =
      DrawNameOfRoad(buses, settings, projector);

  std::vector<svg::Circle> stop_points = DrawStop(stops, settings, projector);

  std::vector<svg::Text> stop_names = DrawStopName(stops, settings, projector);

  svg::Document doc;

  for (auto& line : lines) {
    doc.Add(line);
  }

  for (auto& name : NamesOfRoad) {
    doc.Add(name);
  }

  for (auto& point : stop_points) {
    doc.Add(point);
  }

  for (auto& name : stop_names) {
    doc.Add(name);
  }

//  doc.Render(std::cout);
  //
  //
  //  return json::Node();

  std::stringstream svg_data;
  doc.Render(svg_data);
  json::Dict out;
  out.insert({"request_id", json::Node(id)});
  out.insert({"map", json::Node(svg_data.str())});
  return json::Node(out);
}

