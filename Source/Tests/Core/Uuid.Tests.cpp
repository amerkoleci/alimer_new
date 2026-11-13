// Copyright (c) Amer Koleci and Contributors.
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include "doctest.h"
#include "Alimer/Core/Uuid.h"
using namespace Alimer;

TEST_SUITE("core::Uuid")
{
    TEST_CASE("Uuid: default initialization behaves correctly")
    {
        Uuid nullUuid;
        CHECK(nullUuid.IsEmpty());
        CHECK(nullUuid.GetData() == Array<uint8_t, 16>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        CHECK(nullUuid.ToString() == "00000000-0000-0000-0000-000000000000");
        CHECK(nullUuid.ToStringArray() == Array<char, 37>{"00000000-0000-0000-0000-000000000000"});
        CHECK(nullUuid == Uuid{});
        CHECK(nullUuid >= Uuid{});
        CHECK(nullUuid <= Uuid{});
        CHECK_FALSE(nullUuid > Uuid{});
        CHECK_FALSE(nullUuid < Uuid{});
        CHECK(nullUuid != Uuid::Generate());
        CHECK(Uuid::FromString(nullUuid.ToString()) == nullUuid);
    }

    TEST_CASE("Uuid: parse")
    {
        CHECK(Uuid::FromString("00000000-0000-0000-0000-000000000000") == Uuid());
        CHECK(Uuid::FromString("1b0e29af-fd4a-43e0-ba4c-e9334183b1f1") == Uuid({ 0x1B, 0x0E, 0x29, 0xAF, 0xFD, 0x4A, 0x43, 0xE0, 0xBA, 0x4C, 0xE9, 0x33, 0x41, 0x83, 0xB1, 0xF1 }));

        // Testing some invalid cases
        CHECK(Uuid::FromString("Alimer Engine").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af_fd4a_43e0_ba4c_e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a_43e0_ba4c_e93341-3b1f1").has_value() == false);
        CHECK(Uuid::FromString("Zb0e29af-fd4a-43e0-ba4c-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29a\t-fd4a-43e0-ba4c-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4\v-43e0-ba4c-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a-\r3e0-ba4c-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a-43e\n-ba4c-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a-43e0-\0a4c-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a-43e0-ba4\n-e9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a-43e0-ba4c-g9334183b1f1").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-fd4a-43e0-ba4c-e9334183b1fG").has_value() == false);
        CHECK(Uuid::FromString("1b0e29af-HELL-OWOR-LDDD-e9334183b1f1").has_value() == false);
    }
}
