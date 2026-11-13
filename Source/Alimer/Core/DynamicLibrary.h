// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Platform.h"

namespace Alimer
{

    class ALIMER_API DynamicLibrary final
    {
    public:
        DynamicLibrary() noexcept = default;
        explicit DynamicLibrary(StringView path) noexcept
        {
            _handle = OpenLibrary(path.data());
        }

        DynamicLibrary(DynamicLibrary&& other) noexcept
            : _handle(other._handle)
        {
            other._handle = nullptr;
        }

        DynamicLibrary& operator=(DynamicLibrary&& other) noexcept
        {
            if (this != &other)
            {
                Unload();
                _handle = other._handle;
                other._handle = nullptr;
            }
            return *this;
        }

        DynamicLibrary(const DynamicLibrary&) = delete;
        DynamicLibrary& operator=(const DynamicLibrary&) = delete;

        ~DynamicLibrary();

        bool Load(StringView path) noexcept;
        void Unload() noexcept;

        [[nodiscard]] bool IsLoaded() const noexcept { return _handle != nullptr; }

        // Resolves a symbol as the requested pointer type (typically a function
        // // pointer). Returns nullptr if absent or not loaded.
        template <typename T>
        [[nodiscard]] T GetSymbol(StringView name) const noexcept
        {
            static_assert(sizeof(T) == sizeof(void*), "GetSymbol<T> expects a pointer type.");
            void* symbol = (_handle != nullptr) ? GetLibrarySymbol(_handle, name) : nullptr;

            T result{};
            std::memcpy(&result, &symbol, sizeof(result));
            return result;
        }

    private:
        LibraryHandle _handle = nullptr;
    };
}
