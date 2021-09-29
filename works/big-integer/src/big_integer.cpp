#include "big_integer.hpp"
#include <sstream>
#include <iostream>

BigInteger::BigInteger() : sign_(1), container_(1) {
}

BigInteger::BigInteger(std::int64_t init_value)
    : sign_(init_value < 0 ? -1 : 1), container_(1, init_value * sign_) {
    while (container_[container_.size() - 1] >= kModule) {
        container_.emplace_back(container_[container_.size() - 1] / kModule);
        container_[container_.size() - 2] %= kModule;
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

    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (lhs_iter != lhs.end() && *lhs_iter == *rhs_iter) {
        lhs_iter++;
        rhs_iter++;
    }

    return lhs_iter == lhs.end() ? comp(1, 1) : comp(*lhs_iter, *rhs_iter);
}

BigInteger::ContainerType BigInteger::ModuleDif(const ContainerType& lhs,
                                                const ContainerType& rhs) {
    ContainerType result;
    CellType carry = 0;
    auto lhs_iter = lhs.begin();
    auto rhs_iter = rhs.begin();

    while (rhs_iter != rhs.end() || carry != 0) {
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

BigInteger BigInteger::operator+(const BigInteger& other) const {
    if (sign_ == other.sign_) {
        return BigInteger(sign_, ModuleSum(container_, other.container_));
    } else if (ModuleCompare<Less>(container_, other.container_)) {
        return BigInteger(other.sign_, ModuleDif(other.container_, container_));
    } else {
        auto res = BigInteger(sign_, ModuleDif(container_, other.container_));
        if (res.container_.size() == 1 && res.container_[0] == 0) {
            res.sign_ = 1;
        }
        return res;
    }
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    BigInteger copy(other);
    copy.sign_ *= -1;

    return this->operator+(copy);
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
        // std::cout << *item << std::endl;
        ostream << *item;
    }

    return ostream.str();
}
