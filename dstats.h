//
//  dstats.h
//  cardinality
//
//  Created by Lukáš Havrlant on 10.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_dstats_h
#define cardinality_dstats_h

#define ID_SERVER 0
#define ID_SECTION 1
#define ID_PLACEMENT 2
#define BANNER_TYPE 3
#define ID_CHANNEL 4
#define ID_CAMPAIGN 5
#define ID_PLAN_CAMPAIGN 6
#define MYSQL_DATE 7
#define MYSQL_TIME 8
#define ID_BANNER 9
#define STATUS 10
#define UniqueUserIDpartIP 11
#define BROWSER_ID 12
#define OS_ID 13
#define IP_COUNTRY 14
#define IP_REGION 15
#define SITE_CATEGORY 16
#define BANNER_VERSION 17
#define REMOTE_HOST 18
#define SCREENWIDTH 19
#define SCREENHEIGHT 20
#define COOKIE_ENABLED 21
#define UniqueUserIDpartUU 22
#define IP 23
#define SITE_URL 24
#define PAGE_KEYWORDS 25
#define ACTION_TEXT 26
#define UUID_INDEX 27
#define MOBILE_DEVICE_ID 28

#include "declarations.h"
#include <stdlib.h>

typedef struct {
    uint *fields_indices;
    uint fields_count;
} View;

typedef struct {
    View *views;
    uint length;
} ViewFilter;

#endif
