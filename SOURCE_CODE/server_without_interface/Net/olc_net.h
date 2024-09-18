#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_client.h"
#include "net_server.h"
#include "net_connection.h"
#include "net_transmit.h"
#include "net_image.h"
#include "net_events.h"

#include <conio.h>
#include <bitset>
#include <deque>

const unsigned short MINMOUSE = 0x01;
const unsigned short MAXMOUSE = 0x08;
const unsigned short MINKEY = 0x08;
const unsigned short MAXKEY = 0xFF;
bool CanSend[MAXKEY] = {0}, hold[MAXKEY] = {0};