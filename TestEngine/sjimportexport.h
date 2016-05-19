#pragma once

#ifdef SYNA_SJ_EXPORTS
#define SYNA_SJ_API __declspec(dllexport)
#else
#define SYNA_SJ_API __declspec(dllimport)
#endif