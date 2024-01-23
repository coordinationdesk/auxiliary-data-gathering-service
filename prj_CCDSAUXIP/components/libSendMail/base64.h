
#ifndef _BASE64_H_
#define _BASE64_H_
#include <string>

std::string base64_encode(unsigned char const* , unsigned int in_len);
std::string base64_decode(std::string const&);

#endif

