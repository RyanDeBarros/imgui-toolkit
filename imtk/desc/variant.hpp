#pragma once

#include "imtk/datapath.hpp"
#include "imtk/pkey.hpp"

#include <variant>

namespace imtk::desc
{
    template<typename... ds>
    class variant
    {
    public:
        datapath_link link;

    private:
        std::variant<ds...> variant_;

    public:
        variant(datapath_link link)
            : link(std::move(link))
            , variant_(std::in_place_index<0>, this->link.share())
        {
        }

        template<typename d>
        variant(datapath_link link, d&& val)
            : link(std::move(link))
            , variant_(std::forward<d>(val))
        {
            std::visit([this](auto& v) { v.link = this->link.share(); }, variant_);
        }

        template<typename d>
        d& set()
        {
            variant_ = d(link.share());
            return std::get<d>(variant_);
        }

        template<typename d>
        void set(d&& desc)
        {
            variant_ = std::forward<d>(desc);
            std::visit([this](auto& v) { v.link = link.share(); }, variant_);
        }

        auto visit(auto&& visitor)
        {
            return std::visit([&visitor](auto& desc) { return visitor(desc); }, variant_);
        }

        auto visit(auto&& visitor) const
        {
            return std::visit([&visitor](const auto& desc) { return visitor(desc); }, variant_);
        }

        template<typename d>
        d* try_get()
        {
            return std::get_if<d>(&variant_);
        }

        template<typename d>
        const d* try_get() const
        {
            return std::get_if<d>(&variant_);
        }

        void* resolve(datapath_view path, imp::type_erasure type)
        {
            return std::visit([path, type](auto& desc) { return desc.resolve(path, type); }, variant_);
        }

        void describe(std::ostream& os, datapath_view path) const
        {
            return std::visit([&os, path](auto& desc) { return desc.describe(os, path); }, variant_);
        }

        bool query_dirty(const variant<ds...>& disk) const
        {
            return std::visit(
                [](const auto& lhs, const auto& rhs) {
                    using lty = std::decay_t<decltype(lhs)>;
                    using rty = std::decay_t<decltype(rhs)>;

                    if constexpr (std::is_same_v<lty, rty>)
                        return lhs.query_dirty(rhs);
                    else
                        return true;
                },
                variant_,
                disk.variant_);
        }

        void copy_data(const variant<ds...>& o)
        {
            std::visit(
                [this](auto& lhs, const auto& rhs) {
                    using lty = std::decay_t<decltype(lhs)>;
                    using rty = std::decay_t<decltype(rhs)>;

                    if constexpr (std::is_same_v<lty, rty>)
                        lhs.copy_data(rhs);
                    else
                        set<rty>().copy_data(rhs);
                },
                variant_,
                o.variant_);
        }
    };

    template<typename... ds>
    class subvariant
    {
    public:
        datapath_link link;
        pkey key;

    private:
        std::variant<ds...> variant_;

    public:
        subvariant(pkey key, datapath_link link)
            : link(std::move(link))
            , key(key)
            , variant_(std::in_place_index<0>, key, this->link.share())
        {
        }

        template<typename d>
        subvariant(pkey key, datapath_link link, d&& val)
            : link(std::move(link))
            , key(key)
            , variant_(std::forward<d>(val))
        {
            std::visit([this](auto& v) { v.link = this->link.share(); v.key = this->key; }, variant_);
        }

        template<typename d>
        d& set()
        {
            variant_ = d(key, link.share());
            return std::get<d>(variant_);
        }

        template<typename d>
        void set(d&& desc)
        {
            variant_ = std::forward<d>(desc);
            std::visit([this](auto& v) { v.link = link.share(); v.key = key; }, variant_);
        }

        auto visit(const auto& visitor)
        {
            return std::visit([&visitor](auto& desc) { return visitor(desc); }, variant_);
        }

        auto visit(const auto& visitor) const
        {
            return std::visit([&visitor](const auto& desc) { return visitor(desc); }, variant_);
        }

        template<typename d>
        d* try_get()
        {
            return std::get_if<d>(&variant_);
        }

        template<typename d>
        const d* try_get() const
        {
            return std::get_if<d>(&variant_);
        }

        void* resolve(datapath_view path, imp::type_erasure type)
        {
            return std::visit([path, type](auto& desc) { return desc.resolve(path, type); }, variant_);
        }

        void describe(std::ostream& os, datapath_view path) const
        {
            return std::visit([&os, path](auto& desc) { return desc.describe(os, path); }, variant_);
        }

        bool query_dirty(const subvariant<ds...>& disk) const
        {
            return std::visit(
                [](const auto& lhs, const auto& rhs) {
                    using lty = std::decay_t<decltype(lhs)>;
                    using rty = std::decay_t<decltype(rhs)>;

                    if constexpr (std::is_same_v<lty, rty>)
                        return lhs.query_dirty(rhs);
                    else
                        return true;
                },
                variant_,
                disk.variant_);
        }

        void copy_data(const subvariant<ds...>& o)
        {
            std::visit(
                [this](auto& lhs, const auto& rhs) {
                    using lty = std::decay_t<decltype(lhs)>;
                    using rty = std::decay_t<decltype(rhs)>;

                    if constexpr (std::is_same_v<lty, rty>)
                        lhs.copy_data(rhs);
                    else
                        set<rty>().copy_data(rhs);
                },
                variant_,
                o.variant_);
        }

        toml_node subnode(toml_node node) const
        {
            return node[encode_pkey(key)];
        }

        template<typename value>
        void dump_into(toml::table& table, value&& value_) const
        {
            table.insert_or_assign(encode_pkey(key), std::forward<value>(value_));
        }
    };
}
