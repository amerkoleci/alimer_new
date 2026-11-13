// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.
// Code used from rbfx: https://github.com/rbfx/rbfx/blob/master/Source/Urho3D/Container/RefCounted.h

#pragma once

#include "Alimer/Core/Assert.h"
#include <cstddef>
#include <type_traits>
#include <utility>
#include <atomic>

namespace Alimer
{
    /// Reference count structure.
    struct ALIMER_API RefCount final
    {
    private:
        /// Constructor.
        RefCount() = default;

    public:
        /// Destruct.
        ~RefCount()
        {
            // Set reference counts below zero to fire asserts if this object is still accessed
            refs.store(-1, std::memory_order_relaxed);
            weakRefs.store(-1, std::memory_order_relaxed);
        }

        /// Allocate RefCount using its default allocator.
        static RefCount* Allocate();
        /// Free RefCount using its default allocator.
        static void Free(RefCount* instance);

        /// Add strong reference. Returns new reference count.
        /// If the object has been destroyed, behavior is undefined.
        int AddRefStrong()
        {
            const int oldValue = refs.fetch_add(1, std::memory_order_relaxed);
            ALIMER_ASSERT(oldValue >= 0);
            return oldValue + 1;
        }

        /// Release strong reference. Returns new reference count.
        /// If 0 is returned, the object must be destroyed immediately.
        /// If the object has been destroyed, behavior is undefined.
        int ReleaseRefStrong()
        {
            const int oldValue = refs.fetch_sub(1, std::memory_order_acq_rel);
            ALIMER_ASSERT(oldValue >= 1);
            return oldValue - 1;
        }

        /// Add weak reference. Returns new reference count.
        /// If the control block has been destroyed, behavior is undefined.
        int AddRefWeak()
        {
            const int oldValue = weakRefs.fetch_add(1, std::memory_order_relaxed);
            ALIMER_ASSERT(oldValue >= 0);
            return oldValue + 1;
        }

        /// Release weak reference. Returns new reference count.
        /// If 0 is returned, the control block must be destroyed immediately.
        int ReleaseRefWeak()
        {
            const int oldValue = weakRefs.fetch_sub(1, std::memory_order_acq_rel);
            ALIMER_ASSERT(oldValue >= 1);
            return oldValue - 1;
        }

        /// Try add strong reference. The object may be destroyed.
        /// Returns true if strong reference was added.
        bool TryAddRefStrong()
        {
            int count = refs.load(std::memory_order_acquire);
            while (count > 0)
            {
                if (refs.compare_exchange_weak(count, count + 1, std::memory_order_acquire, std::memory_order_relaxed))
                    return true;
            }

            return false;
        }

        /// Mark object as destroyed.
        void MarkObjectDestroyed() { refs.store(-1, std::memory_order_release); }

        /// Return strong reference count.
        int Refs() const { return refs.load(std::memory_order_relaxed); }

        /// Return weak reference count.
        int WeakRefs() const { return weakRefs.load(std::memory_order_relaxed); }

    private:
        /// Reference count. If below zero, the object has been destroyed.
        std::atomic_int32_t refs{ 0 };
        /// Weak reference count.
        std::atomic_int32_t weakRefs{ 0 };
    };

    /// Base class for intrusively reference-counted objects. These are noncopyable and non-assignable.
    class ALIMER_API RefCounted
    {
    public:
        /// Constructor. Allocate the reference count structure and set an initial self weak reference.
        RefCounted();
        /// Destructor. Mark as expired and also delete the reference count structure if no outside weak references exist.
        virtual ~RefCounted();

        RefCounted(const RefCounted&) = delete;
        RefCounted& operator=(const RefCounted&) = delete;
        RefCounted(RefCounted&&) = delete;
        RefCounted& operator=(RefCounted&&) = delete;

        /// Increment reference count. Can also be called outside of a RefPtr for traditional reference counting. Returns new reference count value. Operation is atomic.
        int32_t AddRef();
        /// Decrement reference count and delete self if no more references. Can also be called outside of a RefPtr for traditional reference counting. Returns new reference count value. Operation is atomic.
        int32_t ReleaseRef();
        /// Return reference count.
        /// @property
        [[nodiscard]] int32_t Refs() const;
        /// Return weak reference count.
        /// @property
        [[nodiscard]] int32_t WeakRefs() const;

        /// Return pointer to the reference count structure.
        [[nodiscard]] RefCount* RefCountPtr() const { return _refCount; }

    private:
        /// Pointer to the reference count structure.
        RefCount* _refCount{};
    };

    namespace Detail
    {
        /// Returns true if the type is derived from RefCounted.
        template <class T>
        constexpr bool IsRefCountedType = std::is_base_of_v<RefCounted, T>;

        /// Base class for ref pointer.
        template <class InterfaceType, class RefCountedType, bool SameType = std::is_same_v<InterfaceType, RefCountedType>>
        class RefPtrBase;

        template <class InterfaceType>
        class RefPtrBase<InterfaceType, InterfaceType, true>
        {
        public:
            RefPtrBase() noexcept = default;

            explicit RefPtrBase(const RefPtrBase& rhs) noexcept
                : _refCounted(rhs._refCounted)
            {
                if (_refCounted != nullptr)
                    _refCounted->AddRef();
            }

            explicit RefPtrBase(RefPtrBase&& rhs) noexcept
                : _refCounted(rhs._refCounted)
            {
                rhs._refCounted = nullptr;
            }

            RefPtrBase(InterfaceType* ptr, const RefCounted* refCounted, bool addReference = true) noexcept
                : _refCounted(ptr)
            {
                ALIMER_ASSERT_MSG(!ptr || ptr == refCounted, "RefPtr<T, T> cannot be created with different interface and refcounted pointers");

                if (_refCounted != nullptr && addReference)
                    _refCounted->AddRef();
            }

            ~RefPtrBase()
            {
                if (_refCounted)
                    _refCounted->ReleaseRef();
            }

            void Swap(RefPtrBase& rhs) noexcept
            {
                std::swap(_refCounted, rhs._refCounted);
            }

            const InterfaceType* GetRefCounted() const noexcept { return _refCounted; }
            InterfaceType* GetMutableRefCounted() const noexcept { return _refCounted; }
            InterfaceType* GetPointer() const noexcept { return _refCounted; }

        private:
            InterfaceType* _refCounted{};
        };

        template <class InterfaceType, class RefCountedType>
        class RefPtrBase<InterfaceType, RefCountedType, false>
        {
        public:
            RefPtrBase() noexcept = default;

            explicit RefPtrBase(const RefPtrBase& rhs) noexcept
                : _refCounted(rhs._refCounted)
                , _ptr(rhs._ptr)
            {
                if (_refCounted != nullptr)
                    _refCounted->AddRef();
            }

            explicit RefPtrBase(RefPtrBase&& rhs) noexcept
                : _refCounted(rhs._refCounted)
                , _ptr(rhs._ptr)
            {
                rhs._refCounted = nullptr;
                rhs._ptr = nullptr;
            }

            RefPtrBase(InterfaceType* ptr, const RefCounted* refCounted, bool addReference = true) noexcept
                : _refCounted(ptr ? refCounted : nullptr)
                , _ptr(ptr)
            {
                if (_refCounted != nullptr && addReference)
                    _refCounted->AddRef();
            }

            ~RefPtrBase()
            {
                if (_refCounted)
                    _refCounted->ReleaseRef();
            }

            void Swap(RefPtrBase& rhs) noexcept
            {
                std::swap(_refCounted, rhs._refCounted);
                std::swap(_ptr, rhs._ptr);
            }

            const RefCountedType* GetRefCounted() const noexcept { return _refCounted; }
            RefCountedType* GetMutableRefCounted() const noexcept { return const_cast<RefCountedType*>(_refCounted); }
            InterfaceType* GetPointer() const noexcept { return _ptr; }

        private:
            const RefCountedType* _refCounted{};
            InterfaceType* _ptr{};
        };

        /// Base class for weak pointer reference counting.
        class WeakPtrRefCountBase
        {
        public:
            /// Return the object's reference count, or 0 if null pointer or if object has expired.
            int Refs() const noexcept { return (_refCount && _refCount->Refs() >= 0) ? _refCount->Refs() : 0; }

            /// Return the object's weak reference count.
            int WeakRefs() const noexcept
            {
                if (!IsExpired())
                {
                    return _refCount->WeakRefs() - 1;
                }

                return _refCount ? _refCount->WeakRefs() : 0;
            }

            /// Return whether the object has expired. If null pointer, always return true.
            bool IsExpired() const noexcept { return _refCount ? _refCount->Refs() < 0 : true; }

            /// Return pointer to the RefCount structure.
            RefCount* RefCountPtr() const noexcept { return _refCount; }

        protected:
            WeakPtrRefCountBase() noexcept = default;

            explicit WeakPtrRefCountBase(const WeakPtrRefCountBase& rhs) noexcept
                : _refCount(rhs._refCount)
            {}

            explicit WeakPtrRefCountBase(WeakPtrRefCountBase&& rhs) noexcept
                : _refCount(rhs._refCount)
            {
                rhs._refCount = nullptr;
            }

            explicit WeakPtrRefCountBase(RefCount* refCount) noexcept
                : _refCount(refCount)
            {}

            void Swap(WeakPtrRefCountBase& rhs) noexcept
            {
                std::swap(_refCount, rhs._refCount);
            }

            void AddRef() noexcept
            {
                if (_refCount)
                    _refCount->AddRefWeak();
            }

            void ReleaseRef() noexcept
            {
                if (_refCount)
                {
                    if (_refCount->ReleaseRefWeak() == 0)
                        RefCount::Free(_refCount);
                }
            }

        private:
            RefCount* _refCount{};
        };

        /// Base class for weak pointer.
        template <class InterfaceType, class RefCountedType, bool SameType = std::is_same_v<InterfaceType, RefCountedType>>
        class WeakPtrBase;

        template <class InterfaceType>
        class WeakPtrBase<InterfaceType, InterfaceType, true> : public WeakPtrRefCountBase
        {
        public:
            WeakPtrBase() noexcept = default;

            explicit WeakPtrBase(const WeakPtrBase& rhs) noexcept
                : WeakPtrRefCountBase(rhs)
                , _refCounted(rhs._refCounted)
            {
                AddRef();
            }

            explicit WeakPtrBase(WeakPtrBase&& rhs) noexcept
                : WeakPtrRefCountBase(std::move(rhs))
                , _refCounted(rhs._refCounted)
            {
                rhs._refCounted = nullptr;
            }

            WeakPtrBase(RefCount* refCount, InterfaceType* ptr, const RefCounted* refCounted) noexcept
                : WeakPtrRefCountBase(refCount)
                , _refCounted(ptr)
            {
                ALIMER_ASSERT_MSG(!ptr || ptr == refCounted, "WeakPtr<T, T> cannot be created with different interface and refcounted pointers");

                AddRef();
            }

            ~WeakPtrBase() { ReleaseRef(); }

            void Swap(WeakPtrBase& rhs) noexcept
            {
                WeakPtrRefCountBase::Swap(rhs);
                std::swap(_refCounted, rhs._refCounted);
            }

            const InterfaceType* GetRefCounted() const noexcept { return _refCounted; }
            InterfaceType* GetMutableRefCounted() const noexcept { return _refCounted; }
            InterfaceType* GetPointer() const noexcept { return _refCounted; }

        private:
            InterfaceType* _refCounted{};
        };

        template <class InterfaceType, class RefCountedType>
        class WeakPtrBase<InterfaceType, RefCountedType, false>
            : public WeakPtrRefCountBase
        {
        public:
            WeakPtrBase() noexcept = default;

            explicit WeakPtrBase(const WeakPtrBase& rhs) noexcept
                : WeakPtrRefCountBase(rhs)
                , _refCounted(rhs._refCounted)
                , _ptr(rhs._ptr)
            {
                AddRef();
            }

            explicit WeakPtrBase(WeakPtrBase&& rhs) noexcept
                : WeakPtrRefCountBase(std::move(rhs))
                , _refCounted(rhs._refCounted)
                , _ptr(rhs._ptr)
            {
                rhs._refCounted = nullptr;
                rhs._ptr = nullptr;
            }

            WeakPtrBase(RefCount* refCount, InterfaceType* ptr, const RefCountedType* refCounted) noexcept
                : WeakPtrRefCountBase(ptr ? refCount : nullptr)
                , _refCounted(ptr ? refCounted : nullptr)
                , _ptr(ptr)
            {
                AddRef();
            }

            ~WeakPtrBase() { ReleaseRef(); }

            void Swap(WeakPtrBase& rhs) noexcept
            {
                WeakPtrRefCountBase::Swap(rhs);
                std::swap(_refCounted, rhs._refCounted);
                std::swap(_ptr, rhs._ptr);
            }

            const RefCountedType* GetRefCounted() const noexcept { return _refCounted; }
            RefCountedType* GetMutableRefCounted() const noexcept { return const_cast<RefCountedType*>(_refCounted); }
            InterfaceType* GetPointer() const noexcept { return _ptr; }

        private:
            const RefCountedType* _refCounted{};
            InterfaceType* _ptr{};
        };
    }

    /// Ref pointer template class with intrusive reference counting.
    template <class InterfaceType, class RefCountedType = InterfaceType>
    class RefPtr : public Detail::RefPtrBase<InterfaceType, RefCountedType>
    {
    public:
        using ThisType = RefPtr<InterfaceType, RefCountedType>;
        using BaseType = Detail::RefPtrBase<InterfaceType, RefCountedType>;

        RefPtr() noexcept = default;
        RefPtr(std::nullptr_t) noexcept {}   // NOLINT(google-explicit-constructor)
        RefPtr(const ThisType& rhs) noexcept = default;
        RefPtr(ThisType&& rhs) noexcept = default;

        /// Construct with explicit reference counter.
        RefPtr(InterfaceType* ptr, const RefCounted* refCounted, bool addReference = true) noexcept
            : BaseType(ptr, refCounted, addReference)
        {}

        /// Construct from another ref pointer.
        template <class U1, class U2>
        RefPtr(const RefPtr<U1, U2>& rhs) noexcept // NOLINT(google-explicit-constructor)
            : BaseType(rhs.GetPointer(), rhs.GetRefCounted())
        {}

        /// Construct from a raw pointer.
        template <class U>
        explicit RefPtr(U* ptr) noexcept
            : BaseType(ptr, ptr)
        {}

        /// Assign from another ref pointer.
        ThisType& operator=(const ThisType& rhs) noexcept
        {
            if (this->GetPointer() != rhs.GetPointer())
            {
                ThisType temp{ rhs };
                this->Swap(temp);
            }
            return *this;
        }

        /// Assign from another ref pointer.
        template <class U1, class U2> ThisType& operator=(const RefPtr<U1, U2>& rhs) noexcept
        {
            if (this->GetPointer() != rhs.GetPointer())
            {
                ThisType temp{ rhs };
                this->Swap(temp);
            }
            return *this;
        }

        /// Move-assign from another ref pointer.
        ThisType& operator=(ThisType&& rhs) noexcept
        {
            ThisType temp{ std::move(rhs) };
            this->Swap(temp);

            return *this;
        }

        /// Move-assign from another ref pointer.
        template <class U1, class U2> ThisType& operator=(RefPtr<U1, U2>&& rhs) noexcept
        {
            ThisType temp{ std::move(rhs) };
            this->Swap(temp);

            return *this;
        }

        /// Assign from a raw pointer.
        template <class U> ThisType& operator=(U* ptr) noexcept
        {
            if (this->GetPointer() != ptr)
            {
                ThisType temp{ ptr };
                this->Swap(temp);
            }
            return *this;
        }

        /// Point to the object.
        InterfaceType* operator->() const noexcept
        {
            ALIMER_ASSERT(this->GetPointer());
            return this->GetPointer();
        }

        /// Dereference the object.
        InterfaceType& operator*() const noexcept
        {
            ALIMER_ASSERT(this->GetPointer());
            return *this->GetPointer();
        }

        /// Test for less than with another ref pointer.
        template <class U1, class U2> bool operator <(const RefPtr<U1, U2>& rhs) const noexcept { return this->GetRefCounted() < rhs.GetRefCounted(); }

        /// Test for equality with another ref pointer.
        template <class U1, class U2> bool operator ==(const RefPtr<U1, U2>& rhs) const noexcept { return this->GetRefCounted() == rhs.GetRefCounted(); }

        /// Test for inequality with another ref pointer.
        template <class U1, class U2> bool operator !=(const RefPtr<U1, U2>& rhs) const noexcept { return this->GetRefCounted() != rhs.GetRefCounted(); }

        /// Convert to a raw pointer.
        operator InterfaceType* () const noexcept { return this->GetPointer(); }    // NOLINT(google-explicit-constructor)

        /// Reset.
        void Reset() noexcept
        {
            ThisType temp;
            this->Swap(temp);
        }

        /// Reset with another pointer.
        template <class U>
        void Reset(U* ptr) noexcept
        {
            ThisType temp(ptr);
            this->Swap(temp);
        }

        /// Reset with another pointers.
        void Reset(InterfaceType* ptr, const RefCounted* refCounted) noexcept
        {
            ThisType temp(ptr, refCounted);
            this->Swap(temp);
        }

        /// Detach without destroying the object even if the refcount goes zero. To be used for scripting language interoperation.
        InterfaceType* Detach() noexcept
        {
            InterfaceType* ptr = this->GetPointer();
            if (ptr)
            {
                RefCount* refCount = RefCountPtr();
                refCount->AddRefStrong(); // 2 refs
                Reset(); // 1 ref
                refCount->ReleaseRefStrong(); // 0 refs
            }
            return ptr;
        }

        /// Perform a static cast from a ref pointer of another type.
        template <class U1, class U2> void StaticCast(const RefPtr<U1, U2>& rhs) noexcept
        {
            ThisType temp(static_cast<InterfaceType*>(rhs.GetPointer()), rhs.GetRefCounted());
            this->Swap(temp);
        }

        /// Perform a dynamic cast from a ref pointer of another type.
        template <class U1, class U2> void DynamicCast(const RefPtr<U1, U2>& rhs) noexcept
        {
            ThisType temp(dynamic_cast<InterfaceType*>(rhs.GetPointer()), rhs.GetRefCounted());
            this->Swap(temp);
        }

        /// Return the raw pointer.
        InterfaceType* Get() const noexcept { return this->GetPointer(); }

        /// Return the object's reference count, or 0 if the pointer is null.
        int Refs() const noexcept { return this->GetRefCounted() ? this->GetRefCounted()->Refs() : 0; }

        /// Return the object's weak reference count, or 0 if the pointer is null.
        int WeakRefs() const noexcept { return this->GetRefCounted() ? this->GetRefCounted()->WeakRefs() : 0; }

        /// Return pointer to the RefCount structure.
        RefCount* RefCountPtr() const noexcept { return this->GetRefCounted() ? this->GetRefCounted()->RefCountPtr() : nullptr; }

        /// Return hash value for HashSet & HashMap. Use the same hash function as for raw pointers!
        size_t ToHash() const noexcept { return size_t(uintptr_t(this->GetRefCounted())); }
    };

    /// Ref pointer alias with automatic type deduction. Can be used only for complete types!
    template <class T>
    using RefPtrT = std::conditional_t<Detail::IsRefCountedType<T>, RefPtr<T, T>, RefPtr<T, RefCounted>>;

    /// Perform a static cast from one ref pointer type to another.
    template <class T, class U1, class U2> RefPtrT<T> StaticCast(const RefPtr<U1, U2>& ptr) noexcept
    {
        RefPtrT<T> ret;
        ret.StaticCast(ptr);
        return ret;
    }

    /// Perform a dynamic cast from one ref pointer type to another.
    template <class T, class U1, class U2> RefPtrT<T> DynamicCast(const RefPtr<U1, U2>& ptr) noexcept
    {
        RefPtrT<T> ret;
        ret.DynamicCast(ptr);
        return ret;
    }

    /// Weak pointer template class with intrusive reference counting. Does not keep the object pointed to alive.
    template <class InterfaceType, class RefCountedType = InterfaceType>
    class WeakPtr : public Detail::WeakPtrBase<InterfaceType, RefCountedType>
    {
    public:
        using ThisType = WeakPtr<InterfaceType, RefCountedType>;
        using BaseType = Detail::WeakPtrBase<InterfaceType, RefCountedType>;
        using RefPtrType = RefPtr<InterfaceType, RefCountedType>;

        WeakPtr() noexcept = default;
        WeakPtr(std::nullptr_t) noexcept {} // NOLINT(google-explicit-constructor)
        WeakPtr(const ThisType& rhs) noexcept = default;
        WeakPtr(ThisType&& rhs) noexcept = default;

        /// Construct with explicit reference counter.
        WeakPtr(RefCount* refCount, InterfaceType* ptr, const RefCounted* refCounted) noexcept
            : BaseType(refCount, ptr, refCounted)
        {}

        /// Copy-construct from another weak pointer allowing implicit upcasting.
        template <class U1, class U2>
        WeakPtr(const WeakPtr<U1, U2>& rhs) noexcept // NOLINT(google-explicit-constructor)
            : BaseType(rhs.RefCountPtr(), rhs.GetPointer(), rhs.GetRefCounted())
        {}

        /// Construct from a shared pointer.
        template <class U1, class U2>
        WeakPtr(const RefPtr<U1, U2>& rhs) noexcept // NOLINT(google-explicit-constructor)
            : BaseType(rhs.RefCountPtr(), rhs.GetPointer(), rhs.GetRefCounted())
        {}

        /// Construct from a raw pointer.
        template <class U, std::enable_if_t<std::is_base_of_v<InterfaceType, U>, int> = 0>
        explicit WeakPtr(U* ptr) noexcept
            : BaseType(ptr ? ptr->RefCountPtr() : nullptr, ptr, ptr)
        {}

        /// Assign from a ref pointer.
        template <class U1, class U2> ThisType& operator=(const RefPtr<U1, U2>& rhs) noexcept
        {
            ThisType temp{ rhs };
            this->Swap(temp);

            return *this;
        }

        /// Assign from another weak pointer.
        ThisType& operator=(const ThisType& rhs) noexcept
        {
            ThisType temp{ rhs };
            this->Swap(temp);

            return *this;
        }

        /// Assign from another weak pointer.
        template <class U1, class U2> ThisType& operator=(const WeakPtr<U1, U2>& rhs) noexcept
        {
            ThisType temp{ rhs };
            this->Swap(temp);

            return *this;
        }

        /// Move-assign from another weak pointer.
        ThisType& operator=(ThisType&& rhs) noexcept
        {
            ThisType temp(std::move(rhs));
            this->Swap(temp);

            return *this;
        }

        /// Assign from a raw pointer.
        template <class U> ThisType& operator=(U* ptr) noexcept
        {
            ThisType temp{ ptr };
            this->Swap(temp);

            return *this;
        }

        /// Convert to a ref pointer. If expired, return a null ref pointer.
        RefPtrType Lock() const noexcept
        {
            if (this->RefCountPtr() && this->RefCountPtr()->TryAddRefStrong())
                return RefPtrType(this->GetPointer(), this->GetRefCounted(), false);
            else
                return RefPtrType();
        }

        /// Return raw pointer. If expired, return null.
        InterfaceType* Get() const noexcept { return this->IsExpired() ? nullptr : this->GetPointer(); }

        /// Point to the object.
        InterfaceType* operator->() const noexcept
        {
            ALIMER_ASSERT(!this->IsExpired());
            return this->GetPointer();
        }

        /// Dereference the object.
        InterfaceType& operator*() const noexcept
        {
            ALIMER_ASSERT(!this->IsExpired());
            return *this->GetPointer();
        }

        /// Test for equality with another weak pointer.
        template <class U1, class U2> bool operator ==(const WeakPtr<U1, U2>& rhs) const noexcept { return this->GetRefCounted() == rhs.GetRefCounted() && this->RefCountPtr() == rhs.RefCountPtr(); }

        /// Test for inequality with another weak pointer.
        template <class U1, class U2> bool operator !=(const WeakPtr<U1, U2>& rhs) const noexcept { return this->GetRefCounted() != rhs.GetRefCounted() || this->RefCountPtr() != rhs.RefCountPtr(); }

        /// Test for less than with another weak pointer.
        /// Weak pointers sharing the same control block are equivalent.
        template <class U1, class U2> bool operator <(const WeakPtr<U1, U2>& rhs) const noexcept { return this->RefCountPtr() < rhs.RefCountPtr(); }

        /// Convert to a raw pointer, null if the object is expired.
        operator InterfaceType* () const noexcept { return Get(); }   // NOLINT(google-explicit-constructor)

        /// Reset.
        void Reset() noexcept
        {
            ThisType temp;
            this->Swap(temp);
        }

        /// Reset with another pointer.
        template <class U>
        void Reset(U* ptr) noexcept
        {
            ThisType temp(ptr);
            this->Swap(temp);
        }

        /// Reset with another pointers.
        void Reset(RefCount* refCount, InterfaceType* ptr, const RefCounted* refCounted) noexcept
        {
            ThisType temp(refCount, ptr, refCounted);
            this->Swap(temp);
        }

        /// Perform a static cast from a weak pointer of another type.
        template <class U1, class U2>
        void StaticCast(const WeakPtr<U1, U2>& rhs) noexcept
        {
            ThisType temp(rhs.RefCountedPtr(), static_cast<InterfaceType*>(rhs.GetPointer()), rhs.GetRefCounted());
            this->Swap(temp);
        }

        /// Perform a dynamic cast from a weak pointer of another type.
        template <class U1, class U2>
        void DynamicCast(const WeakPtr<U1, U2>& rhs) noexcept
        {
            ThisType temp(rhs.RefCountedPtr(), dynamic_cast<InterfaceType*>(rhs.GetPointer()), rhs.GetRefCounted());
            this->Swap(temp);
        }

        /// Return hash value for HashSet & HashMap. Use the same hash function as for raw pointers!
        size_t ToHash() const noexcept { return size_t(uintptr_t(this->RefCountPtr())); }
    };

    /// Weak pointer alias with automatic type deduction. Can be used only for complete types!
    template <class T>
    using WeakPtrT = std::conditional_t<Detail::IsRefCountedType<T>, WeakPtr<T, T>, WeakPtr<T, RefCounted>>;

    /// Construct RefPtr.
    template <class T, class ... Args> RefPtrT<T> MakeRef(Args && ... args)
    {
        return RefPtrT<T>(new T(std::forward<Args>(args)...));
    }

    template <class T> RefPtr(T* ptr) -> RefPtr<T, std::conditional_t<Detail::IsRefCountedType<T>, T, RefCounted>>;
    template <class T> WeakPtr(T* ptr) -> WeakPtr<T, std::conditional_t<Detail::IsRefCountedType<T>, T, RefCounted>>;
}
