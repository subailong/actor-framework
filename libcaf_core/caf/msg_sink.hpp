/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2015                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENSE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#ifndef CAF_MSG_SINK_HPP
#define CAF_MSG_SINK_HPP

#include "caf/fwd.hpp"
#include "caf/actor_cast.hpp"
#include "caf/abstract_channel.hpp"

#include "caf/detail/comparable.hpp"

namespace caf {

/**
 * A non-owning handle to a dynamically typed message sink. It is similar
 * to `channel`, except that it does not manipulate the reference count
 * of its target.
 */
class msg_sink : detail::comparable<msg_sink> {
 public:
  template <class T, typename U>
  friend T actor_cast(const U&);

  using pointer = abstract_channel*;

  constexpr msg_sink() noexcept : m_ptr(nullptr) {
    // nop
  }

  msg_sink(const actor& x) noexcept;

  msg_sink(const group& x) noexcept;

  msg_sink(const channel& x) noexcept;

  msg_sink(const scoped_actor& x) noexcept;

  inline msg_sink(pointer x) noexcept : m_ptr(x) {
    // nop
  }

  msg_sink(const msg_sink&) noexcept = default;

  msg_sink& operator=(const msg_sink&) noexcept = default;

  inline explicit operator bool() noexcept {
    return static_cast<bool>(m_ptr);
  }

  inline pointer operator->() noexcept {
    return m_ptr;
  }

  intptr_t compare(msg_sink x) const noexcept;

 private:
  inline abstract_channel* get() const noexcept {
    return m_ptr;
  }

  pointer m_ptr;
};

} // namespace caf

#endif // CAF_MSG_SINK_HPP

