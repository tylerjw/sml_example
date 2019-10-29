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
// robot logic object
struct Robot {
  void Plan() {
    std::cout << "Planning... " << std::endl;
  }
  void Execute() {
    std::cout << "Execute... " << std::endl;
  }
  bool valid_plan() {
    std::cout << "valid_plan: " << counter << std::endl;
    return (counter++ > 0);
  }

  int counter = 0;
};

// events
struct spin { Robot* robot; };

// guards
const auto valid_plan = [](const auto& event) { return event.robot->valid_plan(); };

// actions
const auto plan = [](const auto& event) {event.robot->Plan();};
const auto execute = [](const auto& event) {event.robot->Execute();};

struct robot_sm {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *"idle"_s + event<spin> / plan = "planning"_s,
      "planning"_s + event<spin> [ !valid_plan ] / plan,
      "planning"_s + event<spin> [ valid_plan ] / execute = "execute"_s,
      "execute"_s + event<spin> = "idle"_s
    );
    // clang-format on
  }
};  // struct robot_sm

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

  Robot robot;

  my_logger logger;
  sm<robot_sm, sml::logger<my_logger>> sm{logger};
  assert(sm.is("idle"_s));

  sm.process_event(spin{&robot}); // plan
  sm.process_event(spin{&robot}); // plan
  sm.process_event(spin{&robot}); // execute
  sm.process_event(spin{&robot}); // idle
  assert(sm.is("idle"_s));  //released
}
