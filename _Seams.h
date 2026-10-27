// Copyright AStarship <https://astarship.net>.
// Root ASCIICrabs seam table. _ConfigHeader.h does "#include <_Seams.h>"
// (root, angle-bracket), so the CRABS_* seam numbers must be defined HERE.
// _Seams/_Seams.h carries the same table for the seam-test config's relative
// include; keep the two in sync. This file was found empty on this host,
// which made the root config chain non-buildable (CRABS_RELEASE undefined,
// SEAM never set) — restored to the canonical table.
#pragma once
#ifndef _SEAMS_H
#define _SEAMS_H
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
#define SEAM_N CRABS_RELEASE
#endif  // _SEAMS_H
