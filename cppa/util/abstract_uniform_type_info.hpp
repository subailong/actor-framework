/******************************************************************************\
 *           ___        __                                                    *
 *          /\_ \    __/\ \                                                   *
 *          \//\ \  /\_\ \ \____    ___   _____   _____      __               *
 *            \ \ \ \/\ \ \ '__`\  /'___\/\ '__`\/\ '__`\  /'__`\             *
 *             \_\ \_\ \ \ \ \L\ \/\ \__/\ \ \L\ \ \ \L\ \/\ \L\.\_           *
 *             /\____\\ \_\ \_,__/\ \____\\ \ ,__/\ \ ,__/\ \__/.\_\          *
 *             \/____/ \/_/\/___/  \/____/ \ \ \/  \ \ \/  \/__/\/_/          *
 *                                          \ \_\   \ \_\                     *
 *                                           \/_/    \/_/                     *
 *                                                                            *
 * Copyright (C) 2011-2013                                                    *
 * Dominik Charousset <dominik.charousset@haw-hamburg.de>                     *
 *                                                                            *
 * This file is part of libcppa.                                              *
 * libcppa is free software: you can redistribute it and/or modify it under   *
 * the terms of the GNU Lesser General Public License as published by the     *
 * Free Software Foundation; either version 2.1 of the License,               *
 * or (at your option) any later version.                                     *
 *                                                                            *
 * libcppa is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                       *
 * See the GNU Lesser General Public License for more details.                *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with libcppa. If not, see <http://www.gnu.org/licenses/>.            *
\******************************************************************************/


#ifndef CPPA_ABSTRACT_UNIFORM_TYPE_INFO_HPP
#define CPPA_ABSTRACT_UNIFORM_TYPE_INFO_HPP

#include "cppa/any_tuple.hpp"
#include "cppa/deserializer.hpp"
#include "cppa/uniform_type_info.hpp"

#include "cppa/util/type_traits.hpp"

#include "cppa/detail/to_uniform_name.hpp"
#include "cppa/detail/uniform_type_info_map.hpp"

namespace cppa {
namespace util {

/**
 * @brief Implements all pure virtual functions of {@link uniform_type_info}
 *        except serialize() and deserialize().
 */
template<typename T>
class abstract_uniform_type_info : public uniform_type_info {

 public:

    bool equal_to(const std::type_info& tinfo) const override {
        return typeid(T) == tinfo;
    }

    const char* name() const {
        return m_name.c_str();
    }

    any_tuple as_any_tuple(void* instance) const override {
        return make_any_tuple(deref(instance));
    }

    bool equals(const void* lhs, const void* rhs) const override {
        return eq(deref(lhs), deref(rhs));
    }

    void* new_instance(const void* ptr) const override {
        return (ptr) ? new T(deref(ptr)) : new T();
    }

    void delete_instance(void* instance) const override {
        delete reinterpret_cast<T*>(instance);
    }

 protected:

    abstract_uniform_type_info() {
        auto uname = detail::to_uniform_name<T>();
        auto cname = detail::mapped_name_by_decorated_name(uname.c_str());
        if (cname == uname.c_str()) m_name = std::move(uname);
        else m_name = cname;
    }

    static inline const T& deref(const void* ptr) {
        return *reinterpret_cast<const T*>(ptr);
    }

    static inline T& deref(void* ptr) {
        return *reinterpret_cast<T*>(ptr);
    }

    // can be overridden in subclasses to compare POD types
    // by comparing each individual member
    virtual bool pod_mems_equals(const T&, const T&) const {
        return false;
    }

    std::string m_name;

 private:

    template<class C>
    typename std::enable_if<std::is_empty<C>::value, bool>::type
    eq(const C&, const C&) const {
        return true;
    }

    template<class C>
    typename std::enable_if<
        !std::is_empty<C>::value && util::is_comparable<C, C>::value,
        bool
    >::type
    eq(const C& lhs, const C& rhs) const {
        return lhs == rhs;
    }

    template<class C>
    typename std::enable_if<
        !std::is_empty<C>::value
            && std::is_pod<C>::value
            && !util::is_comparable<C, C>::value,
        bool
    >::type
    eq(const C& lhs, const C& rhs) const {
        return pod_mems_equals(lhs, rhs);
    }

};

} // namespace util
} // namespace cppa


#endif // CPPA_ABSTRACT_UNIFORM_TYPE_INFO_HPP
