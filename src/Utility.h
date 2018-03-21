// This is the Utility.h File:

#ifndef __UTILITY__
#define __UTILITY__

#include <string>

// Specific Functions:
void _calcfreq(float* const, const char* const, const bool _print = false);
void _detecode(const float* const, std::string* const, const bool _print = false);
void _compress(const char* const);
void _decmprss(const char* const);
void _help();

// General Functions:
void _pause();

#endif