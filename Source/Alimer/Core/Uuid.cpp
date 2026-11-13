// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.
// Implementation based on: https://github.com/mariusbancila/stduuid and nazara engine: https://github.com/NazaraEngine/NazaraEngine/blob/main/include/Nazara/Core/Uuid.hpp

#include "Alimer/Core/Uuid.h"
#include "Alimer/Core/String.h"

#if defined(_WIN32)
#include "Platform/Win32/WindowsPlatform.h"
#include <objbase.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CFUUID.h>
#else
#include <uuid/uuid.h>
#endif

namespace
{
    [[nodiscard]] constexpr inline uint8_t Hex2Char(const char ch) noexcept
    {
        if (ch >= static_cast<char>('0') && ch <= static_cast<char>('9'))
            return static_cast<uint8_t>(ch - static_cast<char>('0'));
        if (ch >= static_cast<char>('a') && ch <= static_cast<char>('f'))
            return static_cast<uint8_t>(10 + ch - static_cast<char>('a'));
        if (ch >= static_cast<char>('A') && ch <= static_cast<char>('F'))
            return static_cast<uint8_t>(10 + ch - static_cast<char>('A'));
        return 0;
    }
}

using namespace Alimer;

const Uuid Uuid::Empty;

Uuid Uuid::Generate()
{
    Array<uint8_t, 16> uuid;

#if defined(_WIN32)
    GUID id;
    ThrowIfFailed(CoCreateGuid(&id));

    for (uint32_t i = 0; i < 4; ++i)
        uuid[i] = static_cast<uint8_t>(id.Data1 >> ((3 - i) * 8 & 0xFF));

    for (uint32_t i = 0; i < 2; ++i)
        uuid[4 + i] = static_cast<uint8_t>(id.Data2 >> ((1 - i) * 8 & 0xFF));

    for (uint32_t i = 0; i < 2; ++i)
        uuid[6 + i] = static_cast<uint8_t>(id.Data3 >> ((1 - i) * 8 & 0xFF));

    for (uint32_t i = 0; i < 8; ++i)
        uuid[8 + i] = static_cast<uint8_t>(id.Data4[i]);
#elif defined(__APPLE__)
    auto guid = CFUUIDCreate(NULL);
    auto bytes = CFUUIDGetUUIDBytes(guid);
    CFRelease(guid);

    uuid[0] = bytes.byte0;
    uuid[1] = bytes.byte1;
    uuid[2] = bytes.byte2;
    uuid[3] = bytes.byte3;
    uuid[4] = bytes.byte4;
    uuid[5] = bytes.byte5;
    uuid[6] = bytes.byte6;
    uuid[7] = bytes.byte7;
    uuid[8] = bytes.byte8;
    uuid[9] = bytes.byte9;
    uuid[10] = bytes.byte10;
    uuid[11] = bytes.byte11;
    uuid[12] = bytes.byte12;
    uuid[13] = bytes.byte13;
    uuid[14] = bytes.byte14;
    uuid[15] = bytes.byte15;
#else
    uuid_t id;
    uuid_generate(id);

    std::copy(std::begin(id), std::end(id), uuid.begin());
#endif

    return uuid;
}

std::optional<Uuid> Uuid::FromString(std::string_view str)
{
    if (str.empty() || str.size() != 36)
        return {};
    bool firstDigit = true;
    size_t hasBraces = 0;
    size_t index = 0;

    std::array<uint8_t, 16> data{ { 0 } };

    if (str.front() == '{')
        hasBraces = 1;
    if (hasBraces && str.back() != '}')
        return {};

    for (size_t i = hasBraces; i < str.size() - hasBraces; ++i)
    {
        if (str[i] == '-') continue;

        if (index >= 16 || !IsHex(str[i]))
        {
            return {};
        }

        if (firstDigit)
        {
            data[index] = static_cast<uint8_t>(Hex2Char(str[i]) << 4);
            firstDigit = false;
        }
        else
        {
            data[index] = static_cast<uint8_t>(data[index] | Hex2Char(str[i]));
            index++;
            firstDigit = true;
        }
    }

    if (index < 16)
    {
        return {};
    }

    return Uuid{ data };
}

Array<char, 37> Uuid::ToStringArray() const
{
    std::array<char, 37> uuidStr; //< Including \0
    snprintf(uuidStr.data(), uuidStr.size(), "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        _data[0], _data[1], _data[2], _data[3], _data[4], _data[5], _data[6], _data[7],
        _data[8], _data[9], _data[10], _data[11], _data[12], _data[13], _data[14], _data[15]
    );

    return uuidStr;
}

String Uuid::ToString() const
{
    Array<char, 37> uuidStr = ToStringArray();

    return String(uuidStr.data(), uuidStr.size() - 1);
}
