// qacpp_type_traits.h
//

#ifndef INCLUDED_QACPP_TYPE_TRAITS
#define INCLUDED_QACPP_TYPE_TRAITS

namespace __gnu_cxx
{
    template<typename _Tp, typename _Up>
    struct __promote_2;

    template<>    struct __promote_2 <char, char>
    {
      typedef int __type;
    };

    struct __promote_2 <char, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <char, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <char, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <char, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <char, short>
    {
      typedef int __type;
    };

    struct __promote_2 <char, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <char, int>
    {
      typedef int __type;
    };

    struct __promote_2 <char, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <char, long>
    {
      typedef int __type;
    };

    struct __promote_2 <char, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <char, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <char, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <char, float>
    {
      typedef int __type;
    };

    struct __promote_2 <char, double>
    {
      typedef int __type;
    };

    struct __promote_2 <char, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, char>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, short>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, int>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, long>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, float>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, double>
    {
      typedef int __type;
    };

    struct __promote_2 <wchar_t, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, char>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, short>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, int>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, long>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, float>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, double>
    {
      typedef int __type;
    };

    struct __promote_2 <bool, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, char>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, short>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, int>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, long>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, float>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, double>
    {
      typedef int __type;
    };

    struct __promote_2 <signed char, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, float>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned char, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <short, char>
    {
      typedef int __type;
    };

    struct __promote_2 <short, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <short, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <short, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <short, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <short, short>
    {
      typedef int __type;
    };

    struct __promote_2 <short, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <short, int>
    {
      typedef int __type;
    };

    struct __promote_2 <short, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <short, long>
    {
      typedef int __type;
    };

    struct __promote_2 <short, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <short, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <short, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <short, float>
    {
      typedef int __type;
    };

    struct __promote_2 <short, double>
    {
      typedef int __type;
    };

    struct __promote_2 <short, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, float>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned short, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <int, char>
    {
      typedef int __type;
    };

    struct __promote_2 <int, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <int, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <int, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <int, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <int, short>
    {
      typedef int __type;
    };

    struct __promote_2 <int, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <int, int>
    {
      typedef int __type;
    };

    struct __promote_2 <int, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <int, long>
    {
      typedef int __type;
    };

    struct __promote_2 <int, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <int, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <int, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <int, float>
    {
      typedef int __type;
    };

    struct __promote_2 <int, double>
    {
      typedef int __type;
    };

    struct __promote_2 <int, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, float>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned int, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <long, char>
    {
      typedef int __type;
    };

    struct __promote_2 <long, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <long, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <long, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <long, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <long, short>
    {
      typedef int __type;
    };

    struct __promote_2 <long, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <long, int>
    {
      typedef int __type;
    };

    struct __promote_2 <long, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <long, long>
    {
      typedef int __type;
    };

    struct __promote_2 <long, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <long, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <long, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <long, float>
    {
      typedef int __type;
    };

    struct __promote_2 <long, double>
    {
      typedef int __type;
    };

    struct __promote_2 <long, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, float>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, char>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, short>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, int>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, long>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, float>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, double>
    {
      typedef int __type;
    };

    struct __promote_2 <long long, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, float>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, double>
    {
      typedef int __type;
    };

    struct __promote_2 <unsigned long long, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <float, char>
    {
      typedef int __type;
    };

    struct __promote_2 <float, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <float, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <float, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <float, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <float, short>
    {
      typedef int __type;
    };

    struct __promote_2 <float, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <float, int>
    {
      typedef int __type;
    };

    struct __promote_2 <float, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <float, long>
    {
      typedef int __type;
    };

    struct __promote_2 <float, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <float, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <float, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <float, float>
    {
      typedef int __type;
    };

    struct __promote_2 <float, double>
    {
      typedef int __type;
    };

    struct __promote_2 <float, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <double, char>
    {
      typedef int __type;
    };

    struct __promote_2 <double, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <double, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <double, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <double, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <double, short>
    {
      typedef int __type;
    };

    struct __promote_2 <double, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <double, int>
    {
      typedef int __type;
    };

    struct __promote_2 <double, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <double, long>
    {
      typedef int __type;
    };

    struct __promote_2 <double, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <double, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <double, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <double, float>
    {
      typedef int __type;
    };

    struct __promote_2 <double, double>
    {
      typedef int __type;
    };

    struct __promote_2 <double, long double>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, char>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, wchar_t>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, bool>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, signed char>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, unsigned char>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, short>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, unsigned short>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, int>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, unsigned int>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, long>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, unsigned long>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, long long>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, unsigned long long>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, float>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, double>
    {
      typedef int __type;
    };

    struct __promote_2 <long double, long double>
    {
      typedef int __type;
    };

}
#endif
