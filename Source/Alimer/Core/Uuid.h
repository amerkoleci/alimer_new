// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Containers.h"
#include "Alimer/Core/Hash.h"
#include <optional>
#include <algorithm>

namespace Alimer
{
    /// 32-bit hash value for a string.
    class ALIMER_API Uuid final
    {
    public:
        constexpr Uuid()
            :_data{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
        {
        }

        constexpr Uuid(const Array<uint8_t, 16>& value)
            : _data(value)
        {
        }

        Uuid(const Uuid&) = default;
        Uuid& operator=(const Uuid&) = default;
        Uuid(Uuid&&) = default;
        Uuid& operator=(Uuid&&) = default;

        constexpr bool operator==(const Uuid& rhs) const
        {
            return std::equal(
                std::begin(_data), std::end(_data),
                std::begin(rhs._data), std::end(rhs._data)
            );
        }

        constexpr bool operator!=(const Uuid& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool operator<(const Uuid& rhs) const
        {
            return std::lexicographical_compare(
                std::begin(_data), std::end(_data),
                std::begin(rhs._data), std::end(rhs._data)
            );
        }

        constexpr bool operator>(const Uuid& rhs) const
        {
            return rhs.operator<(*this);
        }

        constexpr bool operator<=(const Uuid& rhs) const
        {
            return !rhs.operator<(*this);
        }

        constexpr bool operator>=(const Uuid& rhs) const
        {
            return !operator<(rhs);
        }

        [[nodiscard]] constexpr bool IsEmpty() const
        {
            constexpr uint8_t zeroBytes[16]{};
            return std::equal(
                std::begin(_data), std::end(_data),
                std::begin(zeroBytes), std::end(zeroBytes)
            );
        }

        [[nodiscard]] static Uuid Generate();

        /// Return data as array.
        [[nodiscard]] constexpr const Array<uint8_t, 16>& GetData() const { return _data; }

        [[nodiscard]] static std::optional<Uuid> FromString(std::string_view str);

        [[nodiscard]] Array<char, 37> ToStringArray() const;
        [[nodiscard]] String ToString() const;

        /// Empty UUID.
        static const Uuid Empty;

    private:
        Array<uint8_t, 16> _data;
    };
}

template<> struct std::hash<Alimer::Uuid>
{
    std::size_t operator()(const Alimer::Uuid& value) const noexcept
    {
        size_t result = 0;

        const auto& data = value.GetData();
        for (size_t i = 0; i < data.size(); ++i)
        {
            Alimer::HashCombine(result, data[i]);
        }

        return result;
    }
};
