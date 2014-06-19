#include "dstats.h"

View views[] = {
    //{(uint[]){ID_SERVER}, 1},
    //{(uint[]){ID_SERVER, ID_SECTION}, 2},
    //{(uint[]){ID_SERVER, ID_SECTION, ID_PLACEMENT}, 3},
    {(uint[]){ID_SERVER, ID_SECTION, ID_PLACEMENT, BANNER_TYPE}, 4},
    //{(uint[]){ID_CAMPAIGN, ID_PLAN_CAMPAIGN, ID_BANNER, ID_CHANNEL}, 4},
    //{(uint[]){ID_SERVER, ID_SECTION, ID_PLACEMENT, BANNER_TYPE, ID_CAMPAIGN, ID_PLAN_CAMPAIGN, ID_BANNER, ID_CHANNEL}, 8}
};

void parse_line(char **fields, Dstats *dstats) {
    dstats->id_server = fields[ID_SERVER];
    dstats->id_section = fields[ID_SECTION];
    dstats->id_placement = fields[ID_PLACEMENT];
    dstats->banner_type = fields[BANNER_TYPE];
    dstats->uuid = fields[UUID_INDEX];
}
