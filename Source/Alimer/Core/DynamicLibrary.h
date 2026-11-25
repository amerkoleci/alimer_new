// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Base.h"
#include <bit>

namespace Alimer
{
    /* Dynamic Library */
    using LibraryHandle = void*; // HMODULE on Win32

    class ALIMER_API DynamicLibrary final
    {
    public:
        DynamicLibrary() = default;
        ~DynamicLibrary();

        DynamicLibrary(const DynamicLibrary&) = delete;
        DynamicLibrary& operator=(const DynamicLibrary&) = delete;

        DynamicLibrary(DynamicLibrary&& other) noexcept;
        DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

        bool Open(StringView path, String* error = nullptr) noexcept;
        void Close() noexcept;
        void* GetExport(StringView name, String* error = nullptr) const noexcept;

        bool IsValid() const noexcept { return _handle != nullptr; }

        // Resolves a symbol as the requested pointer type (typically a function
        // // pointer). Returns nullptr if absent or not loaded.
        template <typename T>
        [[nodiscard]] T GetExport(StringView name, String* error = nullptr) const noexcept
        {
            static_assert(sizeof(T) == sizeof(void*), "GetSymbol<T> expects a pointer type.");
            void* symbol = GetExport(name, error);

            return std::bit_cast<T>(symbol);
        }

    private:
        LibraryHandle _handle{ nullptr };
        bool _needsClose{ false };
    };
}
