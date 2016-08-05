// https://github.com/philsquared/Catch/blob/master/docs/own-main.md
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include <cstdlib>

#include <google/protobuf/stubs/common.h>
#include <mapnik/datasource_cache.hpp>

void outOfMemHandler()
{
    std::cerr << "Unable to satisfy request for memory\n";

    std::abort();
}

int main (int argc, char* const argv[])
{
    std::set_new_handler(outOfMemHandler);

    try
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
    }
    catch (std::exception const& ex) {
        std::clog << ex.what() << "\n";
        return -1;
    }
    mapnik::datasource_cache::instance().register_datasources(MAPNIK_PLUGINDIR);
    int result = Catch::Session().run( argc, argv );
    if (!result) printf("\x1b[1;32m ✓ \x1b[0m\n");
    google::protobuf::ShutdownProtobufLibrary();
    return result;
}
