#include "API.hh"
#include "api_export.hh"

namespace holovibes::api
{

// Implement functions for the "record" submodule
static uint transform_get_batch_size() { return API.transform.get_batch_size(); }

static ApiCode transform_set_batch_size(uint value) { return API.transform.set_batch_size(value); }

// Define the dispatch table
static const HolovibesApi api = {
    .transform = {.get_batch_size = transform_get_batch_size, .set_batch_size = transform_set_batch_size}};

} // namespace holovibes::api

// Provide access to the dispatch table
const HolovibesApi* create_holovibes_api() { return &holovibes::api::api; }