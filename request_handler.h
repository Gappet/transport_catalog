#pragma once
#include <iostream>
#include <string_view>

#include "json.h"
#include "transport_catalogue.h"
#include "map_renderer.h"

namespace transpot_guide {
namespace output {

json::Node GetInfoRoute(::transpot_guide::TransportCatalogue& transport_catalog,
                        const std::string_view bus, int id);

json::Node GetInfoStop(::transpot_guide::TransportCatalogue& transport_catalog,
                       const std::string_view stop, int id);


json::Node OutputData(::transpot_guide::TransportCatalogue& transport_catalog,
                      json::Array data, RenderSettings& setting);

}  // namespace output

}  // namespace transpot_guide
