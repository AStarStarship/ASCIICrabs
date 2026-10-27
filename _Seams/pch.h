// Copyright AStarship <https://astarship.net>.
// Precompiled Header File
#pragma once
//
//// Standard C++ headers
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <random>
//#include <iostream>
//
//// Project configuration and common headers
//#include <_Config.h>
//
// The root ASCIICrabs _Config.h chain needs the seam table (_Seams.h) to
// define SEAM via _ConfigFooter.h's default. pch.h is the first header of
// every core TU (_Seams/_Main.hxx), so it pulls in the seam table here.
// _Seams/_Config.h then sees SEAM_N already defined and keeps its default
// SEAM; per-test TUs set SEAM via -DSEAM=<n>.
#include "_Seams.h"
