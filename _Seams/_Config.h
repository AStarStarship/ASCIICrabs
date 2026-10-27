// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef PACKAGE_CONFIGURATION
#define PACKAGE_CONFIGURATION
#include "../_ConfigHeader.h"

// Default to the fully sown release seam (runs the whole tree). Override
// with -DSEAM=<n> to debug a specific seam layer.
#ifndef SEAM
#define SEAM SEAM_N
#endif
/* Seams:
#define CRABS_CORE          1
#define CRABS_ITOS          2
#define CRABS_FTOS          3
#define CRABS_UNIPRINTER    4
#define CRABS_CLOCK         5
#define CRABS_ARRAY         6
#define CRABS_STACK         7
#define CRABS_STRING        8
#define CRABS_REGEX         9
#define CRABS_LOOM          10
#define CRABS_LIST          11
#define CRABS_BOOK          12
#define CRABS_MAP           13
#define CRABS_TABLE         14
#define CRABS_DIC           15
#define CRABS_OP            16
#define CRABS_ROOM          17
#define CRABS_MATHCORE      18
#define CRABS_RANDOM        19
#define CRABS_FILE          20
#define CRABS_PROMISE       21
#define CRABS_SCRIPT        22
#define CRABS_RELEASE       23
*/

#include "../_ConfigDefault.h"
#include "../_ConfigFooter.h"
#endif
