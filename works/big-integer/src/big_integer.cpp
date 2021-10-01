#include "big_integer.hpp"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cmath>

BigInteger::BigInteger() : sign_(1), container_(1) {
}

BigInteger::BigInteger(std::int64_t init_value)
    : BigInteger(static_cast<uint64_t>(std::abs(init_value))) {
    sign_ = init_value < 0 ? -1 : 1;
}

BigInteger::BigInteger(std::uint64_t init_value) : sign_(1), container_(1, init_value) {
    while (container_[container_.size() - 1] >= kModule) {
        container_.emplace_back(container_[container_.size() - 1] / kModule);
        container_[container_.size() - 2] %= kModule;
    }
}

BigInteger::BigInteger(const std::string& str) {
    ConstuctFromString(str);
}

BigInteger::BigInteger(std::string&& str) {
    ConstuctFromString(str);
}

void BigInteger::ConstuctFromString(const std::string& str) {
    container_.clear();
    sign_ = 1;

    std::size_t idx = str.size();
    while (idx - kWidth < idx) {
        container_.emplace_back(std::stoul(str.substr(idx - kWidth, kWidth)));
        idx -= kWidth;
    }

    std::string last = str.substr(str[0] == '-' ? 1 : 0, idx);
    if (!last.empty()) {
        container_.emplace_back(std::stoul(last));
    }

    if (!str.empty()) {
        sign_ = str[0] == '-' ? -1 : 1;
    }
}

BigInteger::BigInteger(const BigInteger& other) : sign_(other.sign_), container_(other.container_) {
}

BigInteger::BigInteger(BigInteger&& other)
    : sign_(other.sign_), container_(std::move(other.container_)) {
}

BigInteger::BigInteger(std::int8_t sign, ContainerType&& container)
    : sign_(sign), container_(std::move(container)) {
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    sign_ = other.sign_;
    container_ = other.container_;

    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& other) {
    sign_ = other.sign_;
    other.sign_ = 0;
    container_ = std::move(other.container_);

    return *this;
}

BigInteger& BigInteger::operator=(std::int64_t other) {
    sign_ = other < 0 ? -1 : 1;
    container_.assign(1, other * sign_);
    while (container_[container_.size() - 1] >= kModule) {
        container_.emplace_back(container_[container_.size() - 1] / kModule);
        container_[container_.size() - 2] %= kModule;
    }

    return *this;
}

BigInteger::CellType BigInteger::SumTwoCells(CellType lhs, CellType rhs, CellType& carry) {
    CellType result = (lhs + rhs) % kModule;
    carry += result < std::max(lhs, rhs);
    return result;
}

BigInteger::CellType BigInteger::DifTwoCells(CellType lhs, CellType rhs, CellType& carry) {
    carry += lhs < rhs;
    return lhs >= rhs ? lhs - rhs : kModule - (rhs - lhs);
}

BigInteger::ContainerType BigInteger::ModuleSum(const ContainerType& lhs,
                                                const ContainerType& rhs) {
    ContainerType result;
    CellType carry = 0;
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (lhs_iter != lhs.end() || rhs_iter != rhs.end() || carry != 0) {
        CellType new_carry{0};
        CellType lhs_cell = lhs_iter != lhs.end() ? *lhs_iter++ : 0;
        CellType rhs_cell = rhs_iter != rhs.end() ? *rhs_iter++ : 0;

        CellType res_cell = SumTwoCells(lhs_cell, rhs_cell, new_carry);
        res_cell = SumTwoCells(res_cell, carry, new_carry);

        result.emplace_back(res_cell);
        carry = new_carry;
    }

    return result;
}

template <typename Comparer>
bool BigInteger::ModuleCompare(const ContainerType& lhs, const ContainerType& rhs) {
    Comparer comp;

    if (lhs.size() != rhs.size()) {
        return comp(lhs.size(), rhs.size());
    }

    auto lhs_iter = lhs.rbegin();
    auto rhs_iter = rhs.rbegin();

    while (lhs_iter != lhs.rend() && *lhs_iter == *rhs_iter) {
        lhs_iter++;
        rhs_iter++;
    }

    return lhs_iter == lhs.rend() ? comp(1, 1) : comp(*lhs_iter, *rhs_iter);
}

BigInteger::ContainerType BigInteger::ModuleDif(const ContainerType& lhs,
                                                const ContainerType& rhs) {
    ContainerType result;
    CellType carry = 0;
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (lhs_iter != lhs.end() || carry != 0) {
        CellType new_carry{0};
        CellType lhs_cell = lhs_iter != lhs.end() ? *lhs_iter++ : 0;
        CellType rhs_cell = rhs_iter != rhs.end() ? *rhs_iter++ : 0;

        CellType res_cell = DifTwoCells(lhs_cell, rhs_cell, new_carry);
        res_cell = DifTwoCells(res_cell, carry, new_carry);

        result.emplace_back(res_cell);
        carry = new_carry;
    }

    while (result.size() > 1 && result[result.size() - 1] == 0) {
        result.pop_back();
    }

    return result;
}

BigInteger& BigInteger::FixSign() {
    if (this->container_.size() == 1 && this->container_[0] == 0) {
        this->sign_ = 1;
    }
    return *this;
}

BigInteger BigInteger::operator+(const BigInteger& other) const {
    if (sign_ == other.sign_) {
        return BigInteger(sign_, ModuleSum(container_, other.container_));
    } else if (ModuleCompare<Less>(container_, other.container_)) {
        return BigInteger(other.sign_, ModuleDif(other.container_, container_));
    } else {
        return BigInteger(sign_, ModuleDif(container_, other.container_)).FixSign();
    }
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    return this->operator+(-other);
}

BigInteger BigInteger::operator-() const {
    BigInteger copy(*this);
    copy.sign_ *= -1;

    return copy.FixSign();
}

BigInteger BigInteger::operator+() const {
    return BigInteger(*this);
}

BigInteger::ContainerType BigInteger::MultiplyOne(const ContainerType& other, CellType mult,
                                                  std::size_t shift) {
    ContainerType res(shift, 0);

    CellType carry = 0;
    for (auto& item : other) {
        CellType cur = item * mult + carry;
        res.emplace_back(cur % kModule);
        carry = cur / kModule;
    }

    while (carry != 0) {
        res.emplace_back(carry % kModule);
        carry /= kModule;
    }

    return res;
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger res;
    res.sign_ = this->sign_ * other.sign_;
    std::size_t shift = 0;
    for (auto& item : this->container_) {
        res = res + BigInteger(1, MultiplyOne(other.container_, item, shift++));
    }

    return res;
}

BigInteger BigInteger::operator/(const BigInteger& other) const {
    BigInteger res;
    BigInteger cur = *this;
    if (other == BigInteger(0)) {
        throw std::logic_error("div by zero");
    }

    while (cur >= other) {
        BigInteger mult = 1;
        while (other * mult * kModule < cur) {
            mult = mult * kModule;
        }

        cur = cur - other * mult;
        res = res + mult;
    }

    res.sign_ = this->sign_ * other.sign_;

    return res;
}

BigInteger BigInteger::operator%(const BigInteger& other) const {
    if (other == BigInteger(0)) {
        throw std::logic_error("div by zero");
    }
    BigInteger result = *this - (*this / other) * other;
    if (result < 0) {
        result = result + other;
    }

    return result;
}

bool BigInteger::operator<(const BigInteger& other) const {
    bool sign_compare = sign_ == other.sign_;
    return !sign_compare && sign_ < other.sign_ ||
           sign_compare && ModuleCompare<Less>(container_, other.container_) == sign_ > 0;
}

bool BigInteger::operator>(const BigInteger& other) const {
    bool sign_compare = sign_ == other.sign_;
    return !sign_compare && sign_ > other.sign_ ||
           sign_compare && ModuleCompare<More>(container_, other.container_) == sign_ > 0;
}

bool BigInteger::operator==(const BigInteger& other) const {
    bool sign_compare = sign_ == other.sign_;
    return sign_compare && ModuleCompare<Eq>(container_, other.container_);
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return this->operator<(other) || this->operator==(other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return this->operator>(other) || this->operator==(other);
}

std::string BigInteger::ToString() const {
    std::ostringstream ostream;

    if (sign_ == -1) {
        ostream << "-";
    }
    for (auto item = container_.rbegin(); item != container_.rend(); ++item) {
        if (item != container_.rbegin()) {
            ostream << std::setw(kWidth) << std::setfill('0');
        }
        ostream << *item;
    }

    return ostream.str();
}

std::ostream& operator<<(std::ostream& stream, const BigInteger& num) {
    return stream << num.ToString();
}

std::istream& operator>>(std::istream& stream, BigInteger& num) {
    std::string tem;
    stream >> tem;
    num = BigInteger(tem);
    return stream;
}
