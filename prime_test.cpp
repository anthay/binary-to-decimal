/*  prime_test.cpp - test prime[0-4].cpp

    By Anthony C. Hay, 2013. See http://howtowriteaprogram.blogspot.com/

    This is free and unencumbered public domain software; see http://unlicense.org/
    I believe this code to be correct, but I may be wrong; use at your own risk.

    Compiled on OS X with g++ 4.2
        g++ -O3 -DTEST0 prime_test.cpp -o prime0_test
        g++ -O3 -DTEST1 prime_test.cpp -o prime1_test
        g++ -O3 -DTEST2 prime_test.cpp -o prime2_test
        g++ -O3 -DTEST3 prime_test.cpp -l mpir -o prime3_test
        g++ -O3 -DTEST4 prime_test.cpp -l mpir -o prime4_test
 
    Compiled on Windows with VC++ 2010
        cl /nologo /EHs /O2 /DTEST0 prime_test.cpp /Feprime0_test.exe
        cl /nologo /EHs /O2 /DTEST1 prime_test.cpp /Feprime1_test.exe
        cl /nologo /EHs /O2 /DTEST2 prime_test.cpp /Feprime2_test.exe

        cl /nologo /EHs /O2 /DTEST3
            /I C:\mpir-2.6.0\lib\Win32\Release
            prime_test.cpp /Feprime3_test.exe
            /link C:\mpir-2.6.0\lib\Win32\Release\mpir.lib

        cl /nologo /EHs /O2 /DTEST4
            /I C:\mpir-2.6.0\lib\Win32\Release
            prime_test.cpp /Feprime4_test.exe
            /link C:\mpir-2.6.0\lib\Win32\Release\mpir.lib
 */

#define PRIME_UNDER_TEST

#if defined(TEST0)
#include "prime0.cpp"
#elif defined(TEST1)
#include "prime1.cpp"
#elif defined(TEST2)
#include "prime2.cpp"
#elif defined(TEST3)
#include "prime3.cpp"
#elif defined(TEST4)
#include "prime4.cpp"
#else
"to test prime0.cpp define TEST0; to test prime1.cpp define TEST1; ..."
#endif

#include <sstream>


int g_failure_count; // running count of unit test failures found



// check count_leading_zeros() function works as expected
void test_count_leading_zeros()
{
#if defined(TEST2) || defined(TEST3)
    for (int i = 0; i <= num_frag_t_size; ++i) {
        num_frag_t x = ~static_cast<num_frag_t>(0);
        if (i == num_frag_t_size)
            x = 0;
        else
            x >>= i;
        const int count = count_leading_zeros(x);
        if (count != i) {
            ++g_failure_count;
            std::cout
                << "test failed: count_leading_zeros(" << x
                << ") got value " << count
                << " expected " << i << '\n';
        }
    }
#endif
}


#if defined(TEST2) || defined(TEST3)
void test_num_bits(const num_vec_t & num, int expected)
{
    const int actual = num_bits(num);
    if (actual != expected) {
        ++g_failure_count;
        std::cout
            << "test_num_bits failed:"
            << " got value " << actual
            << " expected " << expected << '\n';
    }
}
#endif

void test_num_bits()
{
#if defined(TEST2) || defined(TEST3)
    num_vec_t p;
    test_num_bits(p, 0);

    p.push_back(0);
    test_num_bits(p, 0);

    p[0] = 1;
    test_num_bits(p, 1);

    p[0] = 3;
    test_num_bits(p, 2);

    p[0] = 4;
    test_num_bits(p, 3);

    p[0] = static_cast<num_frag_t>(1) << (num_frag_t_size - 1);
    test_num_bits(p, num_frag_t_size);

    p.push_back(0);
    test_num_bits(p, num_frag_t_size);

    p[1] = 1;
    test_num_bits(p, num_frag_t_size + 1);

    p[1] = 7;
    test_num_bits(p, num_frag_t_size + 3);

    p[1] = p[0];
    test_num_bits(p, num_frag_t_size * 2);
#endif
}


// check decimal representation of (2^n)-1, for given 'n', matches given 'expected'
void prime_test(int n, const char * expected)
{
    num_vec_t p;
    make_prime(n, p);
    const std::string s(to_string(p));

    if (s != expected) {
        ++g_failure_count;
        std::cout
            << "test failed: for n " << n
            << " got value " << s
            << " expected " << expected << '\n';
    }
}


// check decimal representation of (2^n)-1, for given 'n', has given number
// of digits and starts and ends with given digit sequences
void prime_test(int n, int num_digits,
    const char * expected_start, const char * expected_end)
{
    if (n > 1000000) {
        // each of the bigger primes may take a few minutes to calculate
        std::cout << "Calculating 2^n-1 for n=" << n << "...\n";
    }

    num_vec_t p;
    make_prime(n, p);
    const std::string s(to_string(p));

    const size_t start_len = strlen(expected_start);
    const std::string start(start_len < s.size() ? s.substr(0, start_len) : s);
    const size_t end_len = strlen(expected_end);
    const std::string end(end_len < s.size() ? s.substr(s.size() - end_len) : s);

    if (s.size() != num_digits) {
        ++g_failure_count;
        std::cout
            << "test failed: for n " << n
            << " got " << s.size()
            << " digits, expected " << num_digits
            << '\n';
    }
    if (start != expected_start) {
        ++g_failure_count;
        std::cout
            << "test failed: for n " << n
            << " got " << start
            << " start, expected " << expected_start
            << '\n';
    }
    if (end != expected_end) {
        ++g_failure_count;
        std::cout
            << "test failed: for n " << n
            << " got " << end
            << " end, expected " << expected_end
            << '\n';
    }
}


// calculate and check all Mersenne primes up to (2^86,243)-1 (larger primes
// take too long to convert to decimal for this version of to_string())
void test_prime_calculation()
{
    // known Mersenne primes from http://en.wikipedia.org/wiki/Mersenne_prime

    prime_test(       2, "3");
    prime_test(       3, "7");
    prime_test(       5, "31");
    prime_test(       7, "127");
    prime_test(      13, "8191");
    prime_test(      17, "131071");
    prime_test(      19, "524287");
    prime_test(      31, "2147483647");
    prime_test(      61, "2305843009213693951");
    prime_test(      89, "618970019642690137449562111");
    prime_test(     107, "162259276829213363391578010288127");
    prime_test(     127, "170141183460469231731687303715884105727");

    //                  number of     first 9       last 9
    //                n    digits      digits       digits
    prime_test(     521,      157, "686479766", "115057151");
    prime_test(     607,      183, "531137992", "031728127");
    prime_test(    1279,      386, "104079321", "168729087");
    prime_test(    2203,      664, "147597991", "697771007");
    prime_test(    2281,      687, "446087557", "132836351");
    prime_test(    3217,      969, "259117086", "909315071");
    prime_test(    4253,     1281, "190797007", "350484991");
    prime_test(    4423,     1332, "285542542", "608580607");
    prime_test(    9689,     2917, "478220278", "225754111");
    prime_test(    9941,     2993, "346088282", "789463551");
    prime_test(   11213,     3376, "281411201", "696392191");
    prime_test(   19937,     6002, "431542479", "968041471");
    prime_test(   21701,     6533, "448679166", "511882751");
    prime_test(   23209,     6987, "402874115", "779264511");
    prime_test(   44497,    13395, "854509824", "011228671");
    prime_test(   86243,    25962, "536927995", "433438207");

#if defined(TEST2) || defined(TEST3) || defined(TEST4)
    // these take too long for prime0.cpp and prime1.cpp
    prime_test(  110503,    33265, "521928313", "465515007");
    prime_test(  132049,    39751, "512740276", "730061311");
    prime_test(  216091,    65050, "746093103", "815528447");
    prime_test(  756839,   227832, "174135906", "544677887");
    prime_test(  859433,   258716, "129498125", "500142591");
#endif

#if defined(TEST3) || defined(TEST4)
    // these take too long for prime0.cpp, prime1.cpp and prime2.cpp
    // (prime3.cpp and prime4.cpp can do them in 20 minutes or so)
    prime_test( 1257787,   378632, "412245773", "089366527");
    prime_test( 1398269,   420921, "814717564", "451315711");
    prime_test( 2976221,   895932, "623340076", "729201151");
    prime_test( 3021377,   909526, "127411683", "024694271");
    prime_test( 6972593,  2098960, "437075744", "924193791");
    prime_test(13466917,  4053946, "924947738", "256259071");
    prime_test(20996011,  6320430, "125976895", "855682047");
    prime_test(24036583,  7235733, "299410429", "733969407");
    prime_test(25964951,  7816230, "122164630", "577077247");
    prime_test(30402457,  9152052, "315416475", "652943871");
    prime_test(32582657,  9808358, "124575026", "053967871");
    prime_test(37156667, 11185272, "202254406", "308220927");
    prime_test(42643801, 12837064, "169873516", "562314751");
    prime_test(43112609, 12978189, "316470269", "697152511");
    prime_test(57885161, 17425170, "581887266", "724285951");
#endif
}


// test make_prime() gives expected value for (2^n)-1 for all values
// of n between 1 and 64, and a few others
void test_basic_make_prime_calculation()
{
    for (int n = 1; n < 64; ++n) {
        std::ostringstream oss;
        oss << (((uint64_t)1 << n) - 1);
        prime_test(n, oss.str().c_str());
    }
    prime_test( 64, "18446744073709551615");
    prime_test( 96, "79228162514264337593543950335");
    prime_test(128, "340282366920938463463374607431768211455");
}


// construct a num_vec_t from given fragments and check we get the
// expected result when we convert it to decimal
void test_to_string(num_frag_t d, num_frag_t c, num_frag_t b, num_frag_t a,
    const char * expected)
{
    num_vec_t n;
    n.push_back(a);
    if (b || c || d)
        n.push_back(b);
    if (c || d)
        n.push_back(c);
    if (d)
        n.push_back(d);

    const std::string result(to_string(n));
    if (result != expected) {
        ++g_failure_count;
        std::cout
            << "test failed: expected '" << expected
            << "'; got '" << result
            << "'\n";
    }
}


// test binary->decimal conversions for some arbitrary values between 0 and (2^128)-1
void test_basic_binary_to_decimal_conversion()
{
#if defined(NUM_FRAG_T_SIZE) && (NUM_FRAG_T_SIZE == 64)
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, "0");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000001ULL, "1");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x00000000499602D2ULL, "1234567890");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x000000007FFFFFFFULL, "2147483647");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000080000000ULL, "2147483648");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x00000000FFFFFFFFULL, "4294967295");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000100000000ULL, "4294967296");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000100000001ULL, "4294967297");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x00000001FFFFFFFFULL, "8589934591");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000200000000ULL, "8589934592");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000FFFFFFFFFULL, "68719476735");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x000000FFFFFFFFFFULL, "1099511627775");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0DE0B6B3A763FFFFULL, "999999999999999999");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0DE0B6B3A7640000ULL, "1000000000000000000");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0DE0B6B3A7640001ULL, "1000000000000000001");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x1234567812345678ULL, "1311768465173141112");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, "18446744073709551615");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000001ULL, 0x0000000000000000ULL, "18446744073709551616");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x00000000FFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "79228162514264337593543950335");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000100000000ULL, 0x0000000000000000ULL, "79228162514264337593543950336");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000100000000ULL, 0x0000000000000001ULL, "79228162514264337593543950337");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0x3B9AC9FFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "79228162514264337593543950335999999999");
    test_to_string(0x0000000000000000ULL, 0x0000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, "340282366920938463463374607431768211455");
    test_to_string(0x0000000000000000ULL, 0x0000000000000001ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, "340282366920938463463374607431768211456");
#else
    test_to_string(0x00000000, 0x00000000, 0x00000000, 0x00000000, "0");
    test_to_string(0x00000000, 0x00000000, 0x00000000, 0x00000001, "1");
    test_to_string(0x00000000, 0x00000000, 0x00000000, 0x499602D2, "1234567890");
    test_to_string(0x00000000, 0x00000000, 0x00000000, 0x7FFFFFFF, "2147483647");
    test_to_string(0x00000000, 0x00000000, 0x00000000, 0x80000000, "2147483648");
    test_to_string(0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, "4294967295");
    test_to_string(0x00000000, 0x00000000, 0x00000001, 0x00000000, "4294967296");
    test_to_string(0x00000000, 0x00000000, 0x00000001, 0x00000001, "4294967297");
    test_to_string(0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF, "8589934591");
    test_to_string(0x00000000, 0x00000000, 0x00000002, 0x00000000, "8589934592");
    test_to_string(0x00000000, 0x00000000, 0x0000000F, 0xFFFFFFFF, "68719476735");
    test_to_string(0x00000000, 0x00000000, 0x000000FF, 0xFFFFFFFF, "1099511627775");
    test_to_string(0x00000000, 0x00000000, 0x0DE0B6B3, 0xA763FFFF, "999999999999999999");
    test_to_string(0x00000000, 0x00000000, 0x0DE0B6B3, 0xA7640000, "1000000000000000000");
    test_to_string(0x00000000, 0x00000000, 0x0DE0B6B3, 0xA7640001, "1000000000000000001");
    test_to_string(0x00000000, 0x00000000, 0x12345678, 0x12345678, "1311768465173141112");
    test_to_string(0x00000000, 0x00000000, 0x2B992DDF, 0xA23249D6, "3141592653589793238");
    test_to_string(0x00000000, 0x00000000, 0x3B9AC9FF, 0xFFFFFFFF, "4294967295999999999");
    test_to_string(0x00000000, 0x00000000, 0x7FFFFFFF, 0xFFFFFFFF, "9223372036854775807");
    test_to_string(0x00000000, 0x00000000, 0x80000000, 0x00000000, "9223372036854775808");
    test_to_string(0x00000000, 0x00000000, 0x80000000, 0x00000001, "9223372036854775809");
    test_to_string(0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, "18446744073709551615");
    test_to_string(0x00000000, 0x00000001, 0x00000000, 0x00000000, "18446744073709551616");
    test_to_string(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "79228162514264337593543950335");
    test_to_string(0x00000001, 0x00000000, 0x00000000, 0x00000000, "79228162514264337593543950336");
    test_to_string(0x00000001, 0x00000000, 0x00000000, 0x00000001, "79228162514264337593543950337");
    test_to_string(0x3B9AC9FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "79228162514264337593543950335999999999");
    test_to_string(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, "340282366920938463463374607431768211455");
#endif
}


#if defined(TEST4)

// "normalise" given 'num'; if num is normalised the following will be true
//   1. num.size() > 0
//   2. if num is zero then num.size() == 1 && num[0] == 0
//   3. if num is non-zero then num.back() != 0
void normalise(num_vec_t & num)
{
    num_vec_t::size_type i = num.size();
    if (i == 0) {
        // assume an empty vector represents a zero value
        num.push_back(0);
    }
    else {
        // discard all 0-value number fragments from most significant end
        --i;
        while (i && num[i] == 0)
            --i;
        num.resize(i + 1);
    }
}


// product <- u * v; u and v must be normalised
static void mul(num_vec_t & product, const num_vec_t & u, const num_vec_t & v)
{
    // this code assumes mp_limb_t == num_frag_t
    const mp_size_t m = u.size();
    const mp_size_t n = v.size();
    std::vector<mp_limb_t> p(m + n, 0);

    if (m >= n)
        mpn_mul(&p[0], &u[0], m, &v[0], n);
    else
        mpn_mul(&p[0], &v[0], n, &u[0], m);

    normalise(p);
    product.swap(p);
}

inline num_vec_t & operator*=(num_vec_t & lhs, const num_vec_t & rhs)
{
    mul(lhs, rhs, lhs);
    return lhs;
}

#endif


// test 10^z conversion is correct
void test_zeros_binary_to_decimal_conversion(int z)
{
    num_vec_t ten, n;
    ten.push_back(10);
    n.push_back(1);
    for (int i = 0; i < z; ++i)
        n *= ten;

    // we're expecting s to be '1' followed by z '0's
    const std::string s(to_string(n));

    if (s.size() != z + 1) {
        ++g_failure_count;
        std::cout
            << "test failed: (z=" << z << ") wrong number of digits, expected " << z + 1
            << " got " << s.size() << "\n";
    }
    else if (s[0] != '1') {
        ++g_failure_count;
        std::cout
            << "test failed: (z=" << z << ") wrong first digit, expected '1' got '" << s[0] << "'\n";
    }
    else {
        for (int i = 1; i <= z; ++i) {
            if (s[i] != '0') {
                ++g_failure_count;
                std::cout
                    << "test failed: (z=" << z << ") wrong digit, expected '0' got '" << s[i] << "'\n";
                break;
            }
        }
    }
}


void test_zeros_binary_to_decimal_conversion()
{
    test_zeros_binary_to_decimal_conversion(0);
    test_zeros_binary_to_decimal_conversion(1);
    test_zeros_binary_to_decimal_conversion(2);
    test_zeros_binary_to_decimal_conversion(3);
    test_zeros_binary_to_decimal_conversion(55);
    test_zeros_binary_to_decimal_conversion(100);
    test_zeros_binary_to_decimal_conversion(456);
    test_zeros_binary_to_decimal_conversion(3210);
    test_zeros_binary_to_decimal_conversion(100000);
}


int main()
{
    test_count_leading_zeros();
    test_num_bits();
    test_basic_binary_to_decimal_conversion();
    test_basic_make_prime_calculation();
    test_zeros_binary_to_decimal_conversion();
    test_prime_calculation();

    std::cout << "total failures " << g_failure_count << '\n';
    return g_failure_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
