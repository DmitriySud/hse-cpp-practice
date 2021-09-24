#pragma once

#include <vector>
#include <cstdint>
#include <functional>

class BigInteger {
public:
    BigInteger(std::int64_t);
    BigInteger();

    BigInteger(const BigInteger&);
    BigInteger(BigInteger&&);

    BigInteger& operator=(const BigInteger&);
    BigInteger& operator=(BigInteger&&);
    BigInteger& operator=(std::int64_t);

    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;

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

    static CellType SumTwoCells(CellType, CellType, CellType&);
    static CellType DifTwoCells(CellType, CellType, CellType&);
    static ContainerType ModuleSum(const ContainerType&, const ContainerType&);
    static ContainerType ModuleDif(const ContainerType&, const ContainerType&);
    template <typename Comparer>
    static bool ModuleCompare(const ContainerType&, const ContainerType&);

    static constexpr std::size_t kModule = 100;
};
