// PRQA S 1050 EOF
/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2008 Ren� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

#include <base64.h>
#include <iostream>

namespace { // unnamed
    const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

    inline bool is_base64(unsigned char c)  // PRQA S 2134
    {
      return ((isalnum(c) != 0 ) || (c == '+') || (c == '/'));
    }

} // unnamed

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 
{
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3] ; // PRQA S 4403 2
  unsigned char char_array_4[4] ;

  while (in_len-- > 0) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) { // PRQA S 4400
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; // PRQA S 3000, 3007, 3010, 4400 4
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++) { // PRQA S 4230, 4400
        ret += base64_chars[char_array_4[i]];
      }
      i = 0;
    }
  }

  if (i != 0)
  {
    for(j = i; j < 3; j++) { // PRQA S 4230, 4400
      char_array_3[j] = '\0';
    }

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; // PRQA S 3000, 3007, 3010, 4400 4
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++) { // PRQA S 4230
      ret += base64_chars[char_array_4[j]];
    }

    while((i++ < 3)) { // PRQA S 4400
      ret += '=';
    }

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) 
{
  int in_len = encoded_string.size(); // PRQA S 3000, 3010
  int i = 0; 
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4] ; // PRQA S 4403 2 
  unsigned char  char_array_3[3];
  std::string ret;

  while ((in_len-- > 0) && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) { // PRQA S 3000
    char_array_4[i++] = encoded_string[in_]; in_++; // PRQA S 3000
    if (i ==4) { // PRQA S 4400
      for (i = 0; i <4; i++) { // PRQA S 4230, 4400
        char_array_4[i] = base64_chars.find(char_array_4[i]);   // PRQA S 3000, 3010
      }

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4); // PRQA S 3000, 3007, 3010, 4400 3
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++) { // PRQA S 4230, 4400
        ret += char_array_3[i];
      }
      i = 0;
    }
  }

  if (i != 0) {
    for (j = i; j <4; j++) {// PRQA S 4230, 4400
      char_array_4[j] = 0;
    }

    for (j = 0; j <4; j++) {// PRQA S 4230, 4400
      char_array_4[j] = base64_chars.find(char_array_4[j]); // PRQA S 3010
    }

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4); // PRQA S 3000, 3007, 3010, 4400 3
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) { // PRQA S 4230
        ret += char_array_3[j];
    }
  }

  return ret;
}
