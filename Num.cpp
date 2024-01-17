#include <iostream>
#include <array>
#include <stdexcept>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <cstdint>


const size_t array_size = 32;
typedef std::array<uint32_t, array_size> BigNum_t;

void converter_to_BigNumber(const std::string& hex_string, std::array<uint32_t, array_size>& Arr_BigNumber) {

    const size_t max_hex_length = 256;

    // Check if the length exceeds the maximum allowed length
    if (hex_string.size() > max_hex_length) {
        throw std::invalid_argument("Input hex string exceeds the maximum length of 256 characters");
    }

    std::fill(Arr_BigNumber.begin(), Arr_BigNumber.end(), 0);   // fill array with zeros 

    for (int i = 0; i < hex_string.size(); i++) {
        unsigned temp;
        int j = hex_string.size() - 1 - i;                     // забезпечується прохід строки з кінця 

        // Check if the character is a valid hex digit
        if (!((hex_string[j] >= '0' && hex_string[j] <= '9') || (hex_string[j] >= 'A' && hex_string[j] <= 'F'))) {
            std::cerr << "Error: Invalid hex digit in the input string." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if ((hex_string[j] >= '0') && (hex_string[j] <= '9')) {
            temp = hex_string[j] - '0';
        }
        else if ((hex_string[j] >= 'A') && (hex_string[j] <= 'F')) {
            temp = hex_string[j] - 'A' + 10;
        }

        Arr_BigNumber[i / 8] ^= temp << ((i % 8) * 4);

    }
}

size_t Get_BigIndex_BigNumber(const std::array<uint32_t, array_size>& Arr_BigNumber) {

    for (int i = array_size - 1; i > 0; --i) {
        if (Arr_BigNumber[i] != 0) {
            return i;
        }
    }
    return 0;
}

std::string convert_to_HexString(const std::array<uint32_t, array_size>& Arr_BigNumber) {
    std::ostringstream hexStringStream;

    size_t startIndex = Get_BigIndex_BigNumber(Arr_BigNumber);

    hexStringStream << std::hex << std::setw(8) << std::setfill('0') << Arr_BigNumber[startIndex];

    for (int i = startIndex - 1; i >= 0; --i) {
        hexStringStream << std::hex << std::setw(8) << std::setfill('0') << Arr_BigNumber[i];
    }

    return hexStringStream.str();
}

std::array<uint32_t, array_size> LongAdd(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B) {
    std::array<uint32_t, array_size> C;
    uint32_t carry = 0;

    for (size_t i = 0; i < array_size; ++i) {
        uint64_t temp = static_cast<uint64_t>(A[i]) + B[i] + carry;
        C[i] = static_cast<uint32_t>(temp & 0xFFFFFFFF);
        carry = static_cast<uint32_t>(temp >> 32);
    }

    return C;
}

int LongCmp(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B) {
    int i = array_size - 1;

    // Find the first non-equal word
    while (i >= 0 && A[i] == B[i]) {
        i--;
    }

    // If all words are equal, return 0
    if (i == -1) {
        return 0;
    }

    // Compare the non-equal word
    return (A[i] > B[i]) ? 1 : -1;
}

// Віднімання двох великих чисел 
std::array<uint32_t, array_size> LongSub(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B) {
    std::array<uint32_t, array_size>C;
    uint32_t borrow = 0;

    for (size_t i = 0; i < array_size; ++i) {
        int64_t temp = static_cast<int64_t>(A[i]) - B[i] - borrow;
        if (temp >= 0) {
            C[i] = static_cast<uint32_t>(temp);
            borrow = 0;
        }
        else {
            C[i] = static_cast<uint32_t>((1ULL << 32) + temp);
            borrow = 1;
        }
    }

    // Check if borrowing occurred
    if (borrow != 0) {
        throw std::runtime_error("Error: Subtraction result is negative.");
    }

    return C;
}
//  Множення великих 1024 бітових чисел 
std::array<uint32_t, array_size * 2> LongMul(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B) {
    std::array<uint32_t, array_size * 2> result;
    std::fill(result.begin(), result.end(), 0);

    for (int i = 0; i < array_size; ++i) {
        uint64_t carry = 0;
        for (int j = 0; j < array_size; ++j) {
            uint64_t temp = static_cast<uint64_t>(A[i]) * B[j] + result[i + j] + carry;
            result[i + j] = static_cast<uint32_t>(temp & 0xFFFFFFFF);
            carry = temp >> 32;
        }
        result[i + array_size] = carry;
    }

    return result;
}

// Додаткові функції для роботи з 2048бітовими числами 
size_t Get_BigIndex_Very_BigNumber(const std::array<uint32_t, array_size * 2>& Arr_BigNumber) {

    for (int i = array_size - 1; i > 0; --i) {
        if (Arr_BigNumber[i] != 0) {
            return i;
        }
    }
    return 0;
}

std::string convert_to_Big_HexString(const std::array<uint32_t, array_size * 2 >& Arr_BigNumber) {
    std::ostringstream hexStringStream;

    size_t startIndex = Get_BigIndex_Very_BigNumber(Arr_BigNumber);

    hexStringStream << std::hex << std::setw(8) << std::setfill('0') << Arr_BigNumber[startIndex];

    for (int i = startIndex - 1; i >= 0; --i) {
        hexStringStream << std::hex << std::setw(8) << std::setfill('0') << Arr_BigNumber[i];
    }

    return hexStringStream.str();
}

// Піднесення до квадрату великого числа 
std::array<uint32_t, array_size * 2> LongSquare(const std::array<uint32_t, array_size>& A) {
    return LongMul(A, A);
}

// Ділення двох великих чисел
size_t BitLength(BigNum_t n) {
    size_t res = 0;
    size_t i = array_size - 1;

    while (i != 0 && n[i] == 0)
    {
        i--;
    }

    uint32_t num = n[i];

    while (num != 0)
    {
        num >>= 1;
        res++;
    }

    res += i * 32;

    return res;

}

void longShiftBitsToHigh(BigNum_t& n, uint32_t l)
{
    const uint32_t mask = 1 << 31;

    for (uint32_t j = 0; j < l; j++)
    {
        for (size_t i = array_size; i > 0; i--)
        {
            uint32_t t;
            if (i == 1)
            {
                t = 0;
            }
            else
            {
                t = (n[i - 2] & mask) >> 31;
            }
            n[i - 1] = (n[i - 1] << 1) | t;
        }
    }
}

bool isZero(const std::array<uint32_t, array_size>& arr) {
    return std::all_of(arr.begin(), arr.end(), [](uint32_t i) { return i == 0; });
}

BigNum_t divide(const BigNum_t& A, const BigNum_t& B)
{
    if (isZero(B))
    {
        throw std::runtime_error("Помилка: Ділення на нуль.");
    }

    size_t k = BitLength(B);
    BigNum_t R(A);
    BigNum_t Q;
    std::fill(Q.begin(), Q.end(), 0);

    while (LongCmp(R, B) >= 0)
    {
        size_t t = BitLength(R);
        BigNum_t C(B);
        longShiftBitsToHigh(C, t - k);

        if (LongCmp(R, C) < 0)
        {
            t--;
            C = BigNum_t(B);
            longShiftBitsToHigh(C, t - k);
        }

        R = LongSub(R, C);
        BigNum_t h;
        std::fill(h.begin(), h.end(), 0);
        h[0] = 1;
        longShiftBitsToHigh(h, t - k);
        Q = LongAdd(Q, h);
    }

    return Q;
}

// Велике ділення (ділення 2048 бітових чисел)
bool Big_isZero(const std::array<uint32_t, array_size * 2>& arr) {
    return std::all_of(arr.begin(), arr.end(), [](uint32_t i) { return i == 0; });
}

size_t Big_BitLength(std::array<uint32_t, array_size * 2> n) {
    size_t res = 0;
    size_t i = (array_size * 2) - 1;

    while (i != 0 && n[i] == 0)
    {
        i--;
    }

    uint32_t num = n[i];

    while (num != 0)
    {
        num >>= 1;
        res++;
    }

    res += i * 32;

    return res;

}

void Big_longShiftBitsToHigh(std::array<uint32_t, array_size * 2>& n, uint32_t l)
{
    const uint32_t mask = 1 << 31;

    for (uint32_t j = 0; j < l; j++)
    {
        for (size_t i = (array_size * 2); i > 0; i--)
        {
            uint32_t t;
            if (i == 1)
            {
                t = 0;
            }
            else
            {
                t = (n[i - 2] & mask) >> 31;
            }
            n[i - 1] = (n[i - 1] << 1) | t;
        }
    }
}

std::array<uint32_t, array_size * 2> Big_LongAdd(const std::array<uint32_t, array_size * 2>& A, const std::array<uint32_t, array_size * 2>& B) {
    std::array<uint32_t, array_size * 2> C;
    uint32_t carry = 0;

    for (size_t i = 0; i < (array_size * 2); ++i) {
        uint64_t temp = static_cast<uint64_t>(A[i]) + B[i] + carry;
        C[i] = static_cast<uint32_t>(temp & 0xFFFFFFFF);
        carry = static_cast<uint32_t>(temp >> 32);
    }

    return C;
}

int Big_LongCmp(const std::array<uint32_t, array_size * 2>& A, const std::array<uint32_t, array_size * 2>& B) {
    int i = (array_size * 2) - 1;

    while (i >= 0 && A[i] == B[i]) {
        i--;
    }

    if (i == -1) {
        return 0;
    }

    return (A[i] > B[i]) ? 1 : -1;
}

std::array<uint32_t, array_size * 2> Big_LongSub(const std::array<uint32_t, array_size * 2>& A, const std::array<uint32_t, array_size * 2>& B) {
    std::array<uint32_t, array_size * 2>C;
    uint32_t borrow = 0;

    for (size_t i = 0; i < (array_size * 2); ++i) {
        int64_t temp = static_cast<int64_t>(A[i]) - B[i] - borrow;
        if (temp >= 0) {
            C[i] = static_cast<uint32_t>(temp);
            borrow = 0;
        }
        else {
            C[i] = static_cast<uint32_t>((1ULL << 32) + temp);
            borrow = 1;
        }
    }

    // Check if borrowing occurred
    if (borrow != 0) {
        throw std::runtime_error("Error: Subtraction result is negative.");
    }

    return C;
}


std::array<uint32_t, array_size * 2> Big_Divide(const std::array<uint32_t, array_size * 2>& A, const std::array<uint32_t, array_size * 2>& B)
{
    if (Big_isZero(B))
    {
        throw std::runtime_error("Помилка: Ділення на нуль.");
    }

    size_t k = Big_BitLength(B);
    std::array<uint32_t, array_size * 2> R = A;
    std::array<uint32_t, array_size * 2> Q;
    std::fill(Q.begin(), Q.end(), 0);

    while (Big_LongCmp(R, B) >= 0)
    {
        size_t t = Big_BitLength(R);
        std::array<uint32_t, array_size * 2> C = B;
        Big_longShiftBitsToHigh(C, t - k);

        if (Big_LongCmp(R, C) < 0)
        {
            t--;
            C = std::array<uint32_t, array_size * 2>(B);
            Big_longShiftBitsToHigh(C, t - k);
        }

        R = Big_LongSub(R, C);
        std::array<uint32_t, array_size * 2> h;
        std::fill(h.begin(), h.end(), 0);
        h[0] = 1;
        Big_longShiftBitsToHigh(h, t - k);
        Q = Big_LongAdd(Q, h);
    }

    return Q;
}

std::array<uint32_t, array_size * 2> Big_Modulus(const std::array<uint32_t, array_size * 2>& A, const std::array<uint32_t, array_size * 2>& B)
{
    if (Big_isZero(B))
    {
        throw std::runtime_error("Помилка: Ділення на нуль.");
    }

    size_t k = Big_BitLength(B);
    std::array<uint32_t, array_size * 2> R = A;
    std::array<uint32_t, array_size * 2> Q;
    std::fill(Q.begin(), Q.end(), 0);

    while (Big_LongCmp(R, B) >= 0)
    {
        size_t t = Big_BitLength(R);
        std::array<uint32_t, array_size * 2> C = B;
        Big_longShiftBitsToHigh(C, t - k);
        if (Big_LongCmp(R, C) < 0)
        {
            t--;
            C = std::array<uint32_t, array_size * 2>(B);
            Big_longShiftBitsToHigh(C, t - k);
        }

        R = Big_LongSub(R, C);
        std::array<uint32_t, array_size * 2> h;
        std::fill(h.begin(), h.end(), 0);
        h[0] = 1;
        Big_longShiftBitsToHigh(h, t - k);
        Q = Big_LongAdd(Q, h);

    }
    //  std::cout << "Все ок з великим модулем Big_Modulus " << std::endl ; 
    return R;
}


std::array<uint32_t, array_size * 2> convertTo64(const std::array<uint32_t, array_size>& input) {
    std::array<uint32_t, array_size * 2> result = { 0 };

    for (size_t i = 0; i < 32; ++i) {
        result[i] = input[i];
    }

    return result;
}

//Остача від ділення двох великих чисел (А mod B)
BigNum_t modulus(const BigNum_t& A, const BigNum_t& B)
{
    if (isZero(B))
    {
        throw std::runtime_error("Помилка: Ділення на нуль.");
    }

    size_t k = BitLength(B);
    BigNum_t R(A);
    BigNum_t Q;
    std::fill(Q.begin(), Q.end(), 0);

    while (LongCmp(R, B) >= 0)
    {
        size_t t = BitLength(R);
        BigNum_t C(B);
        longShiftBitsToHigh(C, t - k);

        if (LongCmp(R, C) < 0)
        {
            t--;
            C = BigNum_t(B);
            longShiftBitsToHigh(C, t - k);
        }

        R = LongSub(R, C);
        BigNum_t h;
        std::fill(h.begin(), h.end(), 0);
        h[0] = 1;
        longShiftBitsToHigh(h, t - k);
        Q = LongAdd(Q, h);
    }

    return R;
}

// Найбільший спільний дільник
BigNum_t GCD(BigNum_t A, BigNum_t B) {
    while (B != BigNum_t{ 0 }) {

        BigNum_t temp;
        std::fill(temp.begin(), temp.end(), 0);
        temp = B;
        B = modulus(A, B);
        A = temp;

    }
    return A;
}

// Найменше спільне кратне
std::array<uint32_t, array_size * 2>LCD(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B) {

    std::array<uint32_t, array_size * 2> result;
    std::fill(result.begin(), result.end(), 0);

    std::array<uint32_t, array_size * 2> mul;
    std::fill(mul.begin(), mul.end(), 0);
    mul = LongMul(A, B);

    std::array<uint32_t, array_size> gcd;
    std::fill(gcd.begin(), gcd.end(), 0);
    gcd = GCD(A, B);

    std::array<uint32_t, array_size * 2> gcd_1;
    std::fill(gcd_1.begin(), gcd_1.end(), 0);
    gcd_1 = convertTo64(gcd);

    result = Big_Divide(mul, gcd_1);

    return result;
}

// (A + B) mod N
std::array<uint32_t, array_size> Mod_Add(const std::array<uint32_t, array_size>& A, std::array<uint32_t, array_size>& B, std::array<uint32_t, array_size>& N) {
    std::array<uint32_t, array_size> add1;
    std::fill(add1.begin(), add1.end(), 0);
    add1 = modulus(A, N);

    std::array<uint32_t, array_size> add_2;
    std::fill(add_2.begin(), add_2.end(), 0);
    add_2 = modulus(B, N);

    std::array<uint32_t, array_size> add;
    std::fill(add.begin(), add.end(), 0);
    add = LongAdd(add1, add_2);

    std::array<uint32_t, array_size> add_mod;
    std::fill(add_mod.begin(), add_mod.end(), 0);
    add_mod = modulus(add, N);

    return add_mod;
}

// (A - B) mod N
std::array<uint32_t, array_size> Mod_Sub(const std::array<uint32_t, array_size>& A, std::array<uint32_t, array_size>& B, std::array<uint32_t, array_size>& N) {

    std::array<uint32_t, array_size> sub;
    std::fill(sub.begin(), sub.end(), 0);
    sub = LongSub(A, B);

    std::array<uint32_t, array_size> result;
    std::fill(result.begin(), result.end(), 0);
    result = modulus(sub, N);

    return result;
}

// (A * B) mod N     
std::array<uint32_t, array_size * 2> Mod_Mul(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B, const std::array<uint32_t, array_size>& N)
{
    std::array<uint32_t, array_size * 2> mul;
    std::fill(mul.begin(), mul.end(), 0);
    mul = LongMul(A, B);

    std::array<uint32_t, array_size * 2> mod;
    std::fill(mod.begin(), mod.end(), 0);
    mod = convertTo64(N);

    return Big_Modulus(mul, mod);
}

// (A ^ 2) mod N 
std::array<uint32_t, array_size * 2> Mod_Squere_Mul(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& N)
{
    std::array<uint32_t, array_size * 2> mul;
    std::fill(mul.begin(), mul.end(), 0);
    mul = LongMul(A, A);

    std::array<uint32_t, array_size * 2> mod;
    std::fill(mod.begin(), mod.end(), 0);
    mod = convertTo64(N);

    return Big_Modulus(mul, mod);
}

// (A ^ B) mod N

uint32_t Get_Bit(const std::array<uint32_t, array_size>& A, uint32_t bit_number) {

    if (bit_number >= array_size * 32) {
        // bit_number is out of range
        return 0;
    }

    uint32_t word_index = bit_number / 32;
    uint32_t bit_index = bit_number % 32;

    return (A[word_index] >> bit_index) & 1;
}

// Множення 64х коміркових чисел (результат обрублюємо , тобто в-дь не подвоюється к-ть комірок )
std::array<uint32_t, array_size * 2> Static_Big_LongMul(const std::array<uint32_t, array_size * 2>& A, const std::array<uint32_t, array_size * 2>& B) {
    std::array<uint32_t, array_size * 2> result;
    std::fill(result.begin(), result.end(), 0);

    for (int i = 0; i < (array_size * 2); ++i) {
        uint64_t carry = 0;
        for (int j = 0; j < (array_size * 2); ++j) {
            uint64_t temp = static_cast<uint64_t>(A[i]) * B[j] + result[i + j] + carry;
            result[i + j] = static_cast<uint32_t>(temp);
            carry = temp >> 32;
        }
        result[i + array_size] = carry;
    }

    //  std::cout << "Все ок з статичним великим множенням Static_Big_LongMul " << std::endl ;  
    return result;
}

std::array<uint32_t, array_size * 2> Mod_LongPow(const std::array<uint32_t, array_size>& A, const std::array<uint32_t, array_size>& B, const std::array<uint32_t, array_size>& N) {

    std::array<uint32_t, array_size * 2> C;
    std::fill(C.begin(), C.end(), 0);
    C[0] = 1;
    //  std::cout << "Початкове значення С :  " << convert_to_Big_HexString(C) << std::endl;

    std::array<uint32_t, array_size * 2> A_1;
    std::fill(A_1.begin(), A_1.end(), 0);
    A_1 = convertTo64(A);
    //   std::cout << "Початкове значення А_1 " << convert_to_Big_HexString(A_1) << std::endl;

    std::array<uint32_t, array_size * 2> mod;
    std::fill(mod.begin(), mod.end(), 0);
    mod = convertTo64(N);
    //  std::cout << "Початкове значення mod " << convert_to_Big_HexString(mod) << std::endl;

    std::array<uint32_t, array_size * 2> B1;
    std::fill(B1.begin(), B1.end(), 0);
    B1 = convertTo64(B);
    //  std::cout << "Почтакове значенння B1 " << convert_to_Big_HexString(B1) << std::endl;

    //  std::cout << "Все класно пройшло до початку циклу ! " << std::endl ; 

    int size = 0;
    size = Big_BitLength(B1);
    // std::cout << "Бітова довжина В1 " << size << std::endl;
    for (int i = size - 1; i >= 0; --i) {
        if (Get_Bit(B, i) == 1) {
            std::array<uint32_t, array_size * 2> temp = Static_Big_LongMul(C, A_1);
            //  std::cout << "значення temp  " << convert_to_Big_HexString(temp) << std::endl;
            C = Big_Modulus(temp, mod);
            //  std::cout << "Нове значення С " << convert_to_Big_HexString(C) << std::endl;
              //std::cout << "Все класно пройшло коли біт - 1 " << std::endl ; 
        }
        if (Get_Bit(B, i) == 0) {
            std::array<uint32_t, array_size * 2> temp = Static_Big_LongMul(C, C);
            // std::cout << "значення temp " << convert_to_Big_HexString(temp) << std::endl;
            C = Big_Modulus(temp, mod);
            //   std::cout << "Нове значення С " << convert_to_Big_HexString(C) << std::endl;
             //  std::cout << "Все класно пройшло коли біт це 0 " << std::endl ; 
        }
        // std::cout << "Юхууу вйшли вдасло з циклу " << std::endl ; 
        std::cout << " значення С " << convert_to_Big_HexString(C) << std::endl;

    }

    return C;
}


int main() {

    std::string hex_1;
    std::cout << "Enter a hexadecimal number: ";
    std::cin >> hex_1;

    std::string hex_2;
    std::cout << "Enter a hexadecimal number: ";
    std::cin >> hex_2;

    std::string hex_3;
    std::cout << "Enter a hexadecimal number: (mod) ";
    std::cin >> hex_3;

    BigNum_t Arr_1;
    converter_to_BigNumber(hex_1, Arr_1);
    /* try {
         converter_to_BigNumber(hex_1, Arr_1);
         std::cout << "Converted array:\n";
         for (const auto& num : Arr_1) {
         std::cout << num << ' ';

         }
         std::cout << std::endl;
     }
     catch (const std::exception& e) {
         std::cerr << "Error: " << e.what() << std::endl;
     }  */

    std::array<uint32_t, array_size> Arr_2;
    converter_to_BigNumber(hex_2, Arr_2);
    /*try {
        converter_to_BigNumber(hex_2, Arr_2);
        std::cout << "Converted array:\n";
        for (const auto& num : Arr_2) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } */

    std::array<uint32_t, array_size> Arr_3;
    converter_to_BigNumber(hex_3, Arr_3);
    /*try {
        converter_to_BigNumber(hex_3, Arr_3);
        std::cout << "Converted array:\n";
        for (const auto& num : Arr_3) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } */

    // Виклик функції LongAdd
    std::array<uint32_t, array_size> sum = LongAdd(Arr_1, Arr_2);

    std::string sum_string = convert_to_HexString(sum);
    std::cout << "Sum Hex String: " << sum_string << std::endl;

    // Порівняння двох великих чисел 
    int cmp = LongCmp(Arr_1, Arr_2);
    std::cout << "Comparison result: " << cmp << std::endl;

    // Віднімання двох великих чисел LongSub
    std::array<uint32_t, array_size> sub = LongSub(Arr_1, Arr_2);
    std::string sub_string = convert_to_HexString(sub);
    std::cout << "Sub Hex String: " << sub_string << std::endl;

    std::array<uint32_t, array_size * 2> product = LongMul(Arr_1, Arr_2);
    std::cout << "Product: " << convert_to_Big_HexString(product) << std::endl;

    // Піднесення до квадрату 
    std::array<uint32_t, array_size * 2> product_squere = LongSquare(Arr_1);
    std::cout << "Product Squere: " << convert_to_Big_HexString(product_squere) << std::endl;

    // Ділення двох великх чисел
    std::cout << "Division: " << " " << convert_to_HexString(divide(Arr_1, Arr_2)) << std::endl;

    // Остача ві ділення двох великх чисел А mod B 
    std::cout << "A mod B: " << " " << convert_to_HexString(modulus(Arr_1, Arr_2)) << std::endl;

    // Найбільший спільний дільник 
    std::cout << "GCD(A,B): " << " " << convert_to_HexString(GCD(Arr_1, Arr_2)) << std::endl;

    // Найменше спільне кратне 
    std::cout << "LCD(A,B): " << " " << convert_to_Big_HexString(LCD(Arr_1, Arr_2)) << std::endl;

    // (A + B ) mod N
    std::cout << "(A + B) mod N: " << " " << convert_to_HexString(Mod_Add(Arr_1, Arr_2, Arr_3)) << std::endl;

    // (A - B) mod N
    std::cout << "(A - B) mod N: " << " " << convert_to_HexString(Mod_Sub(Arr_1, Arr_2, Arr_3)) << std::endl;

    // (A * B) mod N
    std::cout << "(A * B) mod N: " << " " << convert_to_Big_HexString(Mod_Mul(Arr_1, Arr_2, Arr_3)) << std::endl;

    // (A ^ 2) mod N
    std::cout << "(A ^ 2) mod N: " << " " << convert_to_Big_HexString(Mod_Squere_Mul(Arr_1, Arr_3)) << std::endl;

    // (A ^ B) mod N   ?????????????????????????????????????
   // std::cout << "(A ^ B) mod N: " << " " << convert_to_Big_HexString(Mod_LongPow(Arr_1, Arr_2, Arr_3)) << std::endl;

    // Тестування програми : (перевірка тотожностей )

    std::cout << "Testing ! " << std::endl;

    // (a + b) *  c = c * (a + b) = a*c + b*c

    /*  std::array<uint32_t, array_size * 2> result_1;
    std::fill(result_1.begin(), result_1.end(), 0);

    std::array<uint32_t, array_size * 2>result_2;
    std::fill(result_2.begin(), result_2.end(), 0);

    std::array<uint32_t, array_size * 2> result_3;
    std::fill(result_3.begin(), result_3.end(), 0);

    // a + b
    std::array<uint32_t, array_size > t1;
    std::fill(t1.begin(), t1.end(), 0);

    // a*c
    std::array<uint32_t, array_size * 2> t2;
    std::fill(t2.begin(), t2.end(), 0);

    // b*c
    std::array<uint32_t, array_size * 2> t3;
    std::fill(t3.begin(), t3.end(), 0);

    t1 = LongAdd(Arr_1, Arr_2);
    t2 = LongMul(Arr_1, Arr_3);
    t3 = LongMul(Arr_2, Arr_3);

    result_1 = LongMul(t1,Arr_3);
    result_2 = LongMul(Arr_3,t1);
    result_3 = Big_LongAdd(t2, t3);

    std::cout << "(a + b) *  c : " << convert_to_Big_HexString(result_1) << std::endl;
    std::cout << "c * (a + b) : " << convert_to_Big_HexString(result_2) << std::endl;
    std::cout << "a*c + b*c : " << convert_to_Big_HexString(result_3) << std::endl;   */

    // n * a = a + a + ...+ a (нехай n = 100 , тобто у десятковій системі , 100 це 256 )

    /* std::array<uint32_t, array_size * 2 >result_1;
    std::fill(result_1.begin(), result_1.end(), 0);

    std::array<uint32_t, array_size >result_;
    std::fill(result_.begin(), result_.end(), 0);

    std::array<uint32_t, array_size * 2>result_2;
    std::fill(result_2.begin(), result_2.end(), 0);

    std::cout << "Enter 100 for testing " << std::endl;
    std::string hex ;
    std::cin >> hex ;
    std::array<uint32_t, array_size > n;
    converter_to_BigNumber(hex, n);

    int i = 256;
    result_1 = LongMul(n,Arr_1);
    while(i > 0){
        result_ = LongAdd(result_, Arr_1);
        i = i - 1 ;
    }
    result_2 = convertTo64(result_);

    std::cout << " n * a : " << convert_to_Big_HexString(result_1) << std::endl;
    std::cout << " a + .. + a : " << convert_to_Big_HexString(result_2) << std::endl; */

    // (a + b) *  c === c * (a + b) === a * c + b * c (mod n)

   /* std::string n;
    std::cout << "Enter mod for testing :  ";
    std::cin >> n;
    std::array<uint32_t, array_size > mod;
    converter_to_BigNumber(n, mod);
    std::array<uint32_t, array_size * 2> MOD;
    std::fill(MOD.begin(), MOD.end(), 0);
    MOD = convertTo64(mod);

    std::array<uint32_t, array_size * 2> result_1;
    std::fill(result_1.begin(), result_1.end(), 0);

    std::array<uint32_t, array_size * 2>result_2;
    std::fill(result_2.begin(), result_2.end(), 0);

    std::array<uint32_t, array_size * 2> result_3;
    std::fill(result_3.begin(), result_3.end(), 0);

    // a + b
    std::array<uint32_t, array_size > t1;
    std::fill(t1.begin(), t1.end(), 0);

    // a*c
    std::array<uint32_t, array_size * 2> t2;
    std::fill(t2.begin(), t2.end(), 0);

    // b*c
    std::array<uint32_t, array_size * 2> t3;
    std::fill(t3.begin(), t3.end(), 0);

    t1 = LongAdd(Arr_1, Arr_2);
    t2 = LongMul(Arr_1, Arr_3);
    t3 = LongMul(Arr_2, Arr_3);

    result_1 = LongMul(t1,Arr_3);
    result_2 = LongMul(Arr_3,t1);
    result_3 = Big_LongAdd(t2, t3);

    std::array<uint32_t, array_size * 2> result_m1;
    std::fill(result_m1.begin(), result_m1.end(), 0);

    std::array<uint32_t, array_size * 2>result_m2;
    std::fill(result_m2.begin(), result_m2.end(), 0);

    std::array<uint32_t, array_size * 2> result_m3;
    std::fill(result_m3.begin(), result_m3.end(), 0);

    result_m1 = Big_Modulus(result_1, MOD);
    result_m2 = Big_Modulus(result_2, MOD);
    result_m3 = Big_Modulus(result_3, MOD);

    std::cout << "((a + b) *  c ) mod n: " << convert_to_Big_HexString(result_m1) << std::endl;
    std::cout << "(c * (a + b)) mod n  : " << convert_to_Big_HexString(result_m2) << std::endl;
    std::cout << "(a*c + b*c) mod n  : " << convert_to_Big_HexString(result_m3) << std::endl;    */

    // n * a === a + .. + a (mod m )  (нехай n = 100, у десятковій системі числення це 256)  Arr_3 = m 

    /*std::array<uint32_t, array_size * 2 >result_1;
    std::fill(result_1.begin(), result_1.end(), 0);

    std::array<uint32_t, array_size >result_;
    std::fill(result_.begin(), result_.end(), 0);

    std::array<uint32_t, array_size * 2>result_2;
    std::fill(result_2.begin(), result_2.end(), 0);

    std::cout << "Enter 100 for testing " << std::endl;
    std::string hex ;
    std::cin >> hex ;
    std::array<uint32_t, array_size > n;
    converter_to_BigNumber(hex, n);

    std::array<uint32_t, array_size * 2 >mod;
    std::fill(mod.begin(), mod.end(), 0);
    mod = convertTo64(Arr_3);

    result_1 = LongMul(n,Arr_1);
    std::array<uint32_t, array_size * 2 >res_1;
    std::fill(res_1.begin(), res_1.end(), 0);
    res_1 = Big_Modulus(result_1, mod);

    int i  = 256;
    while(i > 0){
        result_ = LongAdd(result_, Arr_1);
        i = i - 1 ;
    }
    result_2 = convertTo64(result_);
    std::array<uint32_t, array_size * 2 >res_2;
    std::fill(res_2.begin(), res_2.end(), 0);
    res_2 = Big_Modulus(result_2, mod);

    std::cout << " (n * a ) mod m  : " << convert_to_Big_HexString(res_1) << std::endl;
    std::cout << " (a + .. + a) mod m  : " << convert_to_Big_HexString(res_2) << std::endl; */

}






