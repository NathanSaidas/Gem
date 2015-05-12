#ifndef GEM_API_H
#define GEM_API_H

#ifdef GEM_FRAMEWORK_EXPORT
#define GEM_API _declspec(dllexport)
#elif GEM_FRAMEWORK_IMPORT
#define GEM_API _declspec(dllimport)
#else
#define GEM_API
#endif

#include <string>
#include <vector>

//std::string export
template class GEM_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
//std::vector<string> export
template class GEM_API std::vector<std::string>;

#endif