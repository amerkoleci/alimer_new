// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/RefCounted.h"
#if defined(ALIMER_CSHARP_BINDINGS)
#include "Alimer/CSharp/Bindings.h"
#endif

using namespace Alimer;

RefCount* RefCount::Allocate()
{
    return new RefCount();
}

void RefCount::Free(RefCount* instance)
{
    delete instance;
}

RefCounted::RefCounted()
    : _refCount(RefCount::Allocate())
{
    // Hold a weak ref to self to avoid possible double delete of the refcount
    _refCount->AddRefWeak();
}

RefCounted::~RefCounted()
{
    ALIMER_ASSERT(_refCount != nullptr);
    ALIMER_ASSERT(_refCount->Refs() == 0);
    ALIMER_ASSERT(_refCount->WeakRefs() > 0);

#if defined(ALIMER_CSHARP_BINDINGS)
    // TODO: Understand rbfx SetScriptObject
    Scripting::Callback(RefCounted_Delete, this);
#endif

    // Mark object as expired, release the self weak ref and delete the refcount if no other weak refs exist
    _refCount->MarkObjectDestroyed();
    if (_refCount->ReleaseRefWeak() == 0)
    {
        RefCount::Free(_refCount);
    }

    _refCount = nullptr;
}

int32_t RefCounted::AddRef()
{
    const int refs = _refCount->AddRefStrong();
    ALIMER_ASSERT(refs > 0);
#if defined(ALIMER_CSHARP_BINDINGS)
    Scripting::Callback(RefCounted_AddRef, this);
#endif
    return refs;
}

int32_t RefCounted::ReleaseRef()
{
    const int refs = _refCount->ReleaseRefStrong();
    if (refs == 0)
    {
        delete this;
        return 0;
    }

    return refs;
}

int32_t RefCounted::Refs() const
{
    return _refCount->Refs();
}

int32_t RefCounted::WeakRefs() const
{
    // Subtract one to not return the internally held reference
    return _refCount->WeakRefs() - 1;
}
