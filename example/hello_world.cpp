// Copied from the [boost].SML examples
//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/sml.hpp>
#include <cassert>
#include <cstdio>
#include <iostream>

namespace sml = boost::sml;

namespace example {
// events
struct release {};
struct ack {};
struct fin {};
struct timeout {};

// guards
const auto is_ack_valid = [](const ack&) { return true; };
const auto is_fin_valid = [](const fin&) { return true; };

// actions
const auto send_fin = [] {};
const auto send_ack = [] {};

struct hello_world {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *"established"_s + event<release> / send_fin = "fin wait 1"_s,
      "fin wait 1"_s + event<ack> [ is_ack_valid ] = "fin wait 2"_s,
      "fin wait 2"_s + event<fin> [ is_fin_valid ] / send_ack = "timed wait"_s,
      "timed wait"_s + event<timeout> / send_ack = X
    );
    // clang-format on
  }
};  // struct hello_world

// logger struct
struct my_logger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent&) {
    printf("[%s][process_event] %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    printf("[%s][guard] %s %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TGuard>(),
           sml::aux::get_type_name<TEvent>(), (result ? "[OK]" : "[Reject]"));
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    printf("[%s][action] %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TAction>(),
           sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
  }
};

}  // namespace example

int main() {
  using namespace sml;
  using namespace example;

  my_logger logger;
  sm<hello_world, sml::logger<my_logger>> sm{logger};
  assert(sm.is("established"_s));

  sm.process_event(release{});
  assert(sm.is("fin wait 1"_s));

  sm.process_event(ack{});
  assert(sm.is("fin wait 2"_s));

  sm.process_event(fin{});
  assert(sm.is("timed wait"_s));

  sm.process_event(timeout{});
  assert(sm.is(X));  //released
}
