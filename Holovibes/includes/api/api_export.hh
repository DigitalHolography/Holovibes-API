#include "aliases.hh"
#include "enum_api_code.hh"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    // Function pointers for the "record" submodule
    typedef struct
    {
        uint (*get_batch_size)(void);
        holovibes::ApiCode (*set_batch_size)(uint value);
    } ApiTransformModule;

    // Dispatch table for modules
    typedef struct
    {
        ApiTransformModule transform;
    } HolovibesApi;

    // Functions to get the singleton instance and API
    EXPORT const HolovibesApi* create_holovibes_api();

#ifdef __cplusplus
}
#endif