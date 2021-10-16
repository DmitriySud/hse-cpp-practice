#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <type_traits>

namespace {

static constexpr std::size_t CalcModule(std::size_t width) {
    return width == 1 ? 10 : 10 * CalcModule(width - 1);
}

}  // namespace

class BigInteger {
private:
    static constexpr std::size_t kWidth = 2;
    static constexpr std::size_t kModule = CalcModule(kWidth);

public:
    template <typename T, class = typename std::enable_if<std::is_integral_v<T>>::type>
    BigInteger(T integer) : BigInteger(static_cast<std::int64_t>(integer)) {
    }
    BigInteger(std::int64_t);
    BigInteger(std::uint64_t);
    BigInteger(const std::string&);
    BigInteger(std::string&&);
    BigInteger();

    BigInteger(const BigInteger&);
    BigInteger(BigInteger&&);

    BigInteger& operator=(const BigInteger&);
    BigInteger& operator=(BigInteger&&);
    BigInteger& operator=(std::int64_t);

    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;
    BigInteger operator-() const;
    BigInteger operator+() const;

    BigInteger operator*(const BigInteger&) const;
    BigInteger operator/(const BigInteger&) const;
    BigInteger operator%(const BigInteger&) const;

    bool operator<(const BigInteger&) const;
    bool operator>(const BigInteger&) const;

    bool operator<=(const BigInteger&) const;
    bool operator>=(const BigInteger&) const;

    bool operator==(const BigInteger&) const;

    std::string ToString() const;

private:
    using CellType = std::size_t;
    using ContainerType = std::vector<CellType>;

    using Less = std::less<CellType>;
    using More = std::greater<CellType>;
    using Eq = std::equal_to<CellType>;

    BigInteger(std::int8_t, ContainerType&&);

    std::int8_t sign_;
    std::vector<CellType> container_;

    BigInteger& FixSign();

    void ConstuctFromString(const std::string&);

    static CellType SumTwoCells(CellType, CellType, CellType&);
    static CellType DifTwoCells(CellType, CellType, CellType&);
    static ContainerType ModuleSum(const ContainerType&, const ContainerType&);
    static ContainerType ModuleDif(const ContainerType&, const ContainerType&);
    static ContainerType MultiplyOne(const ContainerType&, CellType, std::size_t);

    template <typename Comparer>
    static bool ModuleCompare(const ContainerType&, const ContainerType&);
};

std::ostream& operator<<(std::ostream&, const BigInteger&);
std::istream& operator>>(std::istream&, BigInteger&);
