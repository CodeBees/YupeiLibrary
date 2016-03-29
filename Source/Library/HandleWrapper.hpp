#pragma once

#include "Extensions.hpp"
#include <utility>

namespace Yupei
{
    template<typename Closer>
    class HandleWrapper
    {
    public:
        using HandleType = typename Closer::HandleType;

        const HandleType InvalidHandle = Closer::InvalidHandle;

        HandleWrapper() noexcept
            :handle_{InvalidHandle}
        {}

        explicit HandleWrapper(HandleType handle) noexcept
            :handle_{handle}
        {}

        DISABLECOPY(HandleWrapper)

        HandleWrapper(HandleWrapper&& other) noexcept
            :handle_ {other.handle_}
        {
            other.handle_ = InvalidHandle;
        }

        HandleType& operator=(HandleWrapper&& other) noexcept
        {
            HandleWrapper {std::move(i)}.swap(*this);
            return *this;
        }

        void swap(HandleWrapper& other) noexcept
        {
            std::swap(handle_, other.handle_);
        }

        explicit operator bool() const noexcept
        {
            return handle_ != InvalidHandle;
        }

        HandleType Get() const noexcept
        {
            return handle_;
        }

    private:
        HandleType handle_;
    };

    template<typename T>
    void swap(HandleWrapper<T>& lhs, HandleWrapper<T>& rhs)
    {
        lhs.swap(rhs);
    }
}