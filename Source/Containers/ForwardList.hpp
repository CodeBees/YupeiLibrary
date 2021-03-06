#pragma once

#include "..\MemoryResource\MemoryResource.hpp"
#include "..\HelperMacros.hpp"
#include "..\ConstructDestruct.hpp"
#include "..\TypeTraits.hpp"
#include "..\Iterator.hpp"
#include <initializer_list>
#include <algorithm>
#include <memory>

namespace Yupei
{
    template<typename>
    class forward_list;

    namespace Internal
    {
        template<typename>
        struct ForwardNode;

        template<typename>
        struct ForwardBeginNode;

        template<typename T>
        struct ForwardNodeTraits
        {
            using Node = ForwardNode<T>;
            using BeginNode = ForwardBeginNode<T>;
            using IterPointer = BeginNode*;
            using ConstIterPointer = const BeginNode*;
            using NodePointer = Node*;
            using BeginPointer = BeginNode*;
           
            static inline IterPointer AsIter(ForwardBeginNode<T>* p) noexcept
            {
                return p;
            }

            static inline IterPointer AsIter(ForwardNode<T>* p) noexcept
            {
                return static_cast<IterPointer>(p);
            }

            static inline BeginPointer AsBegin(ForwardBeginNode<T>* p) noexcept
            {
                return p;
            }

            static inline BeginPointer AsBegin(ForwardNode<T>* p) noexcept
            {
                return static_cast<BeginPointer>(p);
            }

            static inline NodePointer AsNode(ForwardBeginNode<T>* p) noexcept
            {
                return reinterpret_cast<NodePointer>(p);
            }

            static inline NodePointer AsNode(ForwardNode<T>* p) noexcept
            {
                return p;
            }
        };

        template<typename T>
        struct ForwardBeginNode
        {
            using Pointer = ForwardNode<T>*;
            using BeginPointer = typename ForwardNodeTraits<T>::BeginPointer;

            Pointer Next_ = {};

            BeginPointer AsBegin() const noexcept
            {
                return ForwardNodeTraits<T>::AsBegin(Next_);
            }
        };

        template<typename T>
        struct ForwardNode : ForwardBeginNode<T>
        {
            T Value_;
        };

        template<typename T>
        class ForwardListIterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using reference = value_type&;
            using pointer = value_type*;
            using difference_type = std::ptrdiff_t;

        private:
            using Traits = ForwardNodeTraits<T>;
            using IterPointer = typename Traits::IterPointer;
            using NodePointer = typename Traits::NodePointer;
            using BeginPointer = typename Traits::BeginPointer;
            using MyType = ForwardListIterator<T>;

            IterPointer ptr_;

            explicit ForwardListIterator(ForwardBeginNode<T>* p) noexcept
                :ptr_{Traits::AsIter(p)}
            {}

            explicit ForwardListIterator(ForwardNode<T>* p) noexcept
                :ptr_{Traits::AsIter(p)}
            {}

            constexpr explicit ForwardListIterator(std::nullptr_t) noexcept
                :ptr_{nullptr}
            {}

            NodePointer AsNode() const noexcept
            {
                return Traits::AsNode(ptr_);
            }

            BeginPointer AsBegin() const noexcept
            {
                return Traits::AsBegin(ptr_);
            }

        public:
            constexpr ForwardListIterator() noexcept
                : ForwardListIterator{nullptr}
            {}

            reference operator *() const noexcept
            {
                return AsNode()->Value_;
            }

            pointer operator ->() const noexcept
            {
                return std::addressof(AsNode()->Value_);
            }

            MyType& operator ++() noexcept
            {
                ptr_ = Traits::AsIter(ptr_->Next_);
                return *this;
            }

            MyType operator ++(int) noexcept
            {
                auto tmp(*this);
                ++*this;
                return tmp;
            }

            friend bool operator ==(const MyType& lhs, const MyType& rhs) noexcept
            {
                return lhs.ptr_ == rhs.ptr_;
            }

            friend bool operator !=(const MyType& lhs, const MyType& rhs) noexcept
            {
                return lhs.ptr_ != rhs.ptr_;
            }

            template<typename>
            friend class Yupei::forward_list;

            template<typename>
            friend class ForwardListConstIterator;
        };

        template<typename T>
        class ForwardListConstIterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using reference = const value_type&;
            using pointer = const value_type*;
            using difference_type = std::ptrdiff_t;

        private:
            using Traits = ForwardNodeTraits<T>;
            using Node = typename Traits::Node;
            using BeginNode = typename Traits::BeginNode;
            using IterPointer = typename Traits::IterPointer;
            using NodePointer = typename Traits::NodePointer;
            using BeginPointer = typename Traits::BeginPointer;
            using MyType = ForwardListConstIterator<T>;

            IterPointer ptr_;

            explicit ForwardListConstIterator(const ForwardBeginNode<T>* p) noexcept
                :ptr_{Traits::AsIter(const_cast<BeginPointer>(p))}
            {}

            explicit ForwardListConstIterator(const ForwardNode<T>* p) noexcept
                : ptr_{Traits::AsIter(const_cast<NodePointer>(p))}
            {}

            constexpr explicit ForwardListConstIterator(std::nullptr_t) noexcept
                : ptr_{nullptr}
            {}

            Node* AsNode() const noexcept
            {
                return Traits::AsNode(ptr_);
            }

            BeginNode* AsBegin() const noexcept
            {
                return Traits::AsNode(ptr_);
            }

        public:
            constexpr ForwardListConstIterator() noexcept
                : ForwardListConstIterator{nullptr}
            {}

            ForwardListConstIterator(const ForwardListIterator<T>& other)
                :ptr_{other.ptr_}
            {}

            reference operator *() const noexcept
            {
                return AsNode()->Value_;
            }

            pointer operator ->() const noexcept
            {
                return std::addressof(AsNode()->Value_);
            }

            MyType& operator ++() noexcept
            {
                ptr_ = Traits::AsIter(ptr_->Next_);
                return *this;
            }

            MyType operator ++(int) noexcept
            {
                auto tmp(*this);
                ++*this;
                return tmp;
            }

            friend bool operator ==(const MyType& lhs, const MyType& rhs) noexcept
            {
                return lhs.ptr_ == rhs.ptr_;
            }

            friend bool operator !=(const MyType& lhs, const MyType& rhs) noexcept
            {
                return lhs.ptr_ != rhs.ptr_;
            }

            template<typename>
            friend class Yupei::forward_list;
        };       
    }

    template<typename T>
    class forward_list
    {
    private:
        using Traits = Internal::ForwardNodeTraits<T>;
        using Node = typename Traits::Node;
        using BeginNode = typename Traits::BeginNode;
        using NodePointer = typename Traits::NodePointer;

        template<typename... ParamsT>
        auto MakeNode(ParamsT&&... params)
        {
            const auto deleteFn = [this](NodePointer p) {allocator_.deallocate(p, 1);};
            std::unique_ptr<Node, decltype(deleteFn)> holder{allocator_.allocate(1), deleteFn};            
            Yupei::construct(std::addressof(holder->Value_), std::forward<ParamsT>(params)...);
            return holder;
        }

        void DeallocateNode(NodePointer p) noexcept
        {
            Yupei::destroy(p);
            allocator_.deallocate(p, 1);
        }

    public:
        using allocator_type = polymorphic_allocator<T>;
        CONTAINER_DEFINE(T)
        using iterator = Internal::ForwardListIterator<T>;
        using const_iterator = Internal::ForwardListConstIterator<T>;

        forward_list() = default;

        explicit forward_list(memory_resource_ptr pmr)
            :allocator_{pmr}
        {}

        forward_list(size_type count, const value_type& value, memory_resource_ptr pmr = {})
            :forward_list{pmr}
        {
            auto start = before_begin();
            for (size_type i{}; i < count; ++i)
                start = insert_after(start, value);
        }

        template<typename InputItT, typename = std::enable_if_t<is_input_iterator<InputItT>::value>>
        forward_list(InputItT first, InputItT last, memory_resource_ptr pmr = {})
            :forward_list{pmr}
        {
            insert_after(cbefore_begin(), first, last);
        }

        forward_list(std::initializer_list<value_type> l, memory_resource_ptr pmr = {})
            :forward_list{l.begin(), l.end(), pmr}
        {}

        forward_list(const forward_list& other)
        {
            insert_after(cbefore_begin(), other.begin(), other.end());
        }    

        forward_list& operator =(const forward_list& other)
        {
            assgin(other.begin(), other.end());
            return *this;
        }

        forward_list(forward_list&& other) noexcept
        {
            beginNode_.Next_ = other.beginNode_.Next_;
            other.beginNode_.Next_ = {};
        }

        forward_list& operator =(forward_list&& other) noexcept
        {
            forward_list(std::move(other)).swap(*this);
            return *this;
        }

        ~forward_list()
        {
            clear();
        }

        void clear() noexcept
        {
            iterator tmp;
            for (auto cur = begin(); cur != end(); cur = tmp)
            {
                tmp = std::next(cur);
                DeallocateNode(cur.AsNode());
            }
            beginNode_.Next_ = {};
        }

        template<typename InputItT, typename = std::enable_if_t<is_input_iterator<InputItT>{}>>
        void assign(InputItT first, InputItT last)
        {
            auto before = before_begin();
            auto cur = std::next(before);
            auto e = end();
            for (; cur != e && first != last;++before, (void)++first)
                *cur = *first;
            if (j == e)
                insert_after(before, first, last);
            else
                erase_after(before, e);
        }

        void assgin(size_type n, const value_type& v)
        {
            auto before = before_begin();
            auto cur = std::next(before);
            auto e = end();
            for (; cur != e && n != 0;++before, --n)
                *cur = v;
            if (j == e)
                insert_after(before, n, v);
            else
                erase_after(before, e);
        }
        
        allocator_type get_allocator() const noexcept
        {
            return allocator_type{allocator_};
        }

        iterator begin() noexcept
        {
            return iterator{beginNode_.Next_};
        }

        iterator before_begin() noexcept
        {
            return iterator{std::addressof(beginNode_)};
        }

        const_iterator before_begin() const noexcept
        {
            return const_iterator{std::addressof(beginNode_)};
        }

        const_iterator cbefore_begin() const noexcept
        {
            return before_begin();
        }

        const_iterator begin() const noexcept
        {
            return const_iterator{beginNode_.Next_};
        }

        const_iterator cbegin() const noexcept
        {
            return begin();
        }

        iterator end() noexcept
        {
            return iterator{nullptr};
        }

        const_iterator end() const noexcept
        {
            return const_iterator{nullptr};
        }

        const_iterator cend() const noexcept
        {
            return end();
        }

        bool empty() const noexcept
        {
            return beginNode_.Next_ == nullptr;
        }

        reference front()
        {
            return *begin();
        }

        const_reference front() const
        {
            return *cbegin();
        }

        iterator insert_after(const_iterator pos, const value_type& value)
        {
            return InsertAfter(pos, 1, value);
        }

        template<typename... Args>
        void emplace_front(Args&&... args)
        {
            InsertAfter(cbefore_begin(), 1, std::forward<Args>(args)...);
        }

        void push_front(const value_type& value)
        {
            InsertAfter(cbefore_begin(), 1, value);
        }

        void push_front(value_type&& value)
        {
            InsertAfter(cbefore_begin(), 1, std::move(value));
        }

        iterator insert_after(const_iterator pos, value_type&& value)
        {
            return InsertAfter(pos, 1, std::move(value));
        }

        iterator insert_after(const_iterator pos, size_type count, const value_type& value)
        {
            return InsertAfter(pos, count, value);
        }

        iterator insert_after(const_iterator pos, std::initializer_list<T> ilist)
        {
            return insert_after(pos, ilist.begin(), ilist.end());
        }

        template<typename InputItT, typename = std::enable_if_t<is_input_iterator<InputItT>::value>>
        iterator insert_after(const_iterator pos, InputItT first, InputItT last)
        {
            auto cur = pos.AsNode();
            NodePointer node = cur;
            const auto insertLast = cur->Next_;
            for (; first != last; ++first, cur = node)
            {
                node = MakeNode(*first).release();
                cur->Next_ = node;
            }
            cur->Next_ = insertLast;
            return iterator{node};
        }

        void resize(size_type count)
        {
            Resize(count);
        }

        void resize(size_type count, const value_type& value)
        {
            Resize(count, value);
        }      
        
        template<typename... ParamsT>
        iterator emplace_after(const_iterator pos, ParamsT&&... params)
        {
            return InsertAfter(pos, 1, std::forward<ParamsT>(params)...);
        }

        iterator erase_after(const_iterator pos)
        {
            const auto cur = pos.AsBegin();
            const auto toErase = cur->Next_;
            const auto last = toErase->Next_;
            cur->Next_ = last;
            DeallocateNode(toErase);
            return iterator{last};
        }

        iterator erase_after(const_iterator first, const_iterator last)
        {
            if (first != last)
            {
                const auto eraseStart = first.AsBegin();
                const auto eraseLast = last.AsNode();
                NodePointer node;
                for (auto cur = eraseStart->Next_; cur != eraseLast; cur = node)
                {
                    node = cur->Next_;
                    DeallocateNode(cur);
                }
                eraseStart->Next_ = eraseLast;
            }           
            return iterator{last.ptr_};
        }

        void pop_front()
        {
            erase_after(before_begin());
        }

        void splice_after(const_iterator pos, forward_list& other)
        {
            if (!other.empty())
            {
                iterator cur;
                for (cur = other.before_begin();++cur != other.end();)
                    ;
                const auto prevNext = pos.AsBegin()->Next_;
                cur.AsBegin()->Next_ = prevNext;
                pos.AsBegin()->Next_ = other.begin().AsNode();
                other.beginNode_.Next_ = {};
            }
        }

        void splice_after(const_iterator pos, forward_list& other, const_iterator it)
        {
            const auto tmp = it++;
            const auto cur = tmp.AsBegin();
            const auto toErase = it.AsBegin();
            cur ->Next_ = toErase ->Next_;
            toErase->Next_ = pos.AsBegin()->Next_;
            pos.AsBegin()->Next_ = toErase;            
        }

        void splice_after(const_iterator pos, forward_list& other,
            const_iterator first, const_iterator last)
        {
            if (first != last)
            {
                const auto spliceStart = first.AsBegin();
                const auto spliceLast = last.AsNode();
                auto toInsert = spliceStart->Next_;
                spliceStart->Next_ = spliceLast;
                const auto insertPos = pos.AsBegin();
                const auto prevAfterPos = insertPos->Next_;
                insertPos->Next_ = toInsert;
                for (;toInsert->Next_ != spliceLast;toInsert = toInsert->Next_)
                    ;
                toInsert->Next_ = prevAfterPos;
            }           
        }

        template <typename CompareT>
        void merge(forward_list& other, CompareT comp)
        {
            auto cur1 = cbefore_begin();
            auto cur2 = other.cbefore_begin();
            const_iterator realItem1; 
            const_iterator realItem2; 
            for (; realItem1 != cend() && realItem2 != cend();)
            {               
                realItem1 = std::next(cur1);
                realItem2 = std::next(cur2);
                if (comp(*realItem2, *realItem1))
                {    
                    auto tmp = std::next(cur2);
                    splice_after(cur1, other, cur2);                   
                    cur2 = tmp;
                }
                else ++cur1;
            }
        }

        template <typename CompareT>
        void merge(forward_list& other)
        {
            return merge(other, less<>());
        }
        
        void reverse() noexcept
        {
            auto start = begin().AsNode();
            if (start != nullptr)
            {
                auto p = start->Next_;
                start->Next_ = nullptr;
                while (p != nullptr)
                {
                    auto tmp = p->Next_;
                    p->Next_ = start;
                    start = p;
                    p = tmp;
                }
                beginNode_.Next_ = p;
            }
        }

        void swap(forward_list& other) noexcept
        {
            std::swap(beginNode_, other.beginNode_);
        }

    private:
        polymorphic_allocator<Node> allocator_;
        BeginNode beginNode_;

        template<typename... ParamsT>
        iterator InsertAfter(const_iterator pos, size_type count, ParamsT&&... params)
        {
            auto cur = pos.AsNode();
            const auto insertionLast = cur->Next_;
            for (size_type i{}; i < count; ++i)
            {
                auto node = MakeNode(std::forward<ParamsT>(params)...).release();               
                cur->Next_ = node;
                cur = node;
            }
            cur->Next_ = insertionLast;
            return iterator{cur};
        }

        template<typename... Args>
        void Resize(size_type count, Args&&... args)
        {
            auto prev = cbefore_begin();
            auto cur = cbegin();
            size_type now{};
            for (; now != count && cur != cend(); prev = cur, ++cur, ++now)
                ;
            if (cur == cend())
                InsertAfter(prev, count - now, std::forward<Args>(args)...);
            else if (now == count)
                erase_after(prev, cend());
        }
    };

    template<typename T>
    decltype(auto) begin(forward_list<T>& l) noexcept
    {
        return l.begin();
    }

    template<typename T>
    decltype(auto) begin(const forward_list<T>& l) noexcept
    {
        return l.begin();
    }

    template<typename T>
    decltype(auto) end(forward_list<T>& l) noexcept
    {
        return l.end();
    }

    template<typename T>
    decltype(auto) end(const forward_list<T>& l) noexcept
    {
        return l.end();
    }

    template<typename T>
    decltype(auto) cbegin(const forward_list<T>& l) noexcept
    {
        return begin(l);
    }

    template<typename T>
    decltype(auto) cend(const forward_list<T>& l) noexcept
    {
        return end(l);
    }

    template<typename T>
    bool operator == (const forward_list<T>& lhs, const forward_list<T>& rhs)
    {
        return std::mismatch(begin(lhs), end(lhs), begin(rhs), end(rhs)) == std::make_pair(end(lhs), end(rhs));
    }

    template<typename T>
    bool operator != (const forward_list<T>& lhs, const forward_list<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
