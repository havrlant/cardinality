#include "dstats.h"

void parse_line(char **fields, Dstats *dstats) {
    dstats->id_server = atoi(fields[ID_SERVER]);
    dstats->id_section = atoi(fields[ID_SECTION]);
    dstats->id_placement = atoi(fields[ID_PLACEMENT]);
    dstats->banner_type = atoi(fields[BANNER_TYPE]);
    dstats->uuid = fields[UUID_INDEX];
}