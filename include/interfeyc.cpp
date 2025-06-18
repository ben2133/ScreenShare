// // g++ -std=c++17 -o tui interfeyc.cpp -lftxui-component -lftxui-dom -lftxui-screen

/// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <string>  // for string, allocator, basic_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for operator|, Maybe, Checkbox, Radiobox, Renderer, Vertical
#include "ftxui/component/component_base.hpp"      // for Component
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, border, color, operator|, text
#include "ftxui/screen/color.hpp"  // for Color, Color::Red

using namespace ftxui;

int main() {
  std::vector<std::string> entries = {
      "entry 1",
      "entry 2",
      "entry 3",
  };
  int menu_1_selected = 0;
  int menu_2_selected = 0;

  bool menu_1_show = false;
  bool menu_2_show = false;

  auto layout = Container::Vertical({
      Checkbox("Show menu_1", &menu_1_show),
      Radiobox(&entries, &menu_1_selected) | border | Maybe(&menu_1_show),
      Checkbox("Show menu_2", &menu_2_show),
      Radiobox(&entries, &menu_2_selected) | border | Maybe(&menu_2_show),

      Renderer([] {
        return text("You found the secret combinaison!") | color(Color::Red);
      }) | Maybe([&] { return menu_1_selected == 1 && menu_2_selected == 2; }),
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(layout);
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
// #include <memory>  // for allocator, __shared_ptr_access
// #include <string>  // for char_traits, operator+, string, basic_string

// #include "ftxui/component/captured_mouse.hpp"  // for ftxui
// #include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
// #include "ftxui/component/component_base.hpp"  // for ComponentBase
// #include "ftxui/component/component_options.hpp"  // for InputOption
// #include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
// #include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
// #include "ftxui/util/ref.hpp"  // for Ref

// int main() {
//   using namespace ftxui;

//   // The data:
//   std::string first_name;
//   std::string last_name;
//   std::string password;
//   std::string phoneNumber;

//   // The basic input components:
//   Component input_first_name = Input(&first_name, "first name");
//   Component input_last_name = Input(&last_name, "last name");

//   // The password input component:
//   InputOption password_option;
//   password_option.password = true;
//   Component input_password = Input(&password, "password", password_option);

//   // The phone number input component:
//   // We are using `CatchEvent` to filter out non-digit characters.
//   Component input_phone_number = Input(&phoneNumber, "phone number");
//   input_phone_number |= CatchEvent([&](Event event) {
//     return event.is_character() && !std::isdigit(event.character()[0]);
//   });
//   input_phone_number |= CatchEvent([&](Event event) {
//     return event.is_character() && phoneNumber.size() > 10;
//   });

//   // The component tree:
//   auto component = Container::Vertical({
//       input_first_name,
//       input_last_name,
//       input_password,
//       input_phone_number,
//   });

//   // Tweak how the component tree is rendered:
//   auto renderer = Renderer(component, [&] {
//     return vbox({
//                hbox(text(" First name : "), input_first_name->Render()),
//                hbox(text(" Last name  : "), input_last_name->Render()),
//                hbox(text(" Password   : "), input_password->Render()),
//                hbox(text(" Phone num  : "), input_phone_number->Render()),
//                separator(),
//                text("Hello " + first_name + " " + last_name),
//                text("Your password is " + password),
//                text("Your phone number is " + phoneNumber),
//            }) |
//            border;
//   });

//   auto screen = ScreenInteractive::TerminalOutput();
//   screen.Loop(renderer);
// }


// #include <memory>  // for shared_ptr, __shared_ptr_access
// #include <string>  // for string, basic_string, operator+, to_string
// #include <vector>  // for vector

// #include "ftxui/component/captured_mouse.hpp"      // for ftxui
// #include "ftxui/component/component.hpp"           // for Radiobox, Renderer
// #include "ftxui/component/component_base.hpp"      // for ComponentBase
// #include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
// #include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, HEIGHT, LESS_THAN

// using namespace ftxui;

// int main() {
//   std::vector<std::string> entries;
//   int selected = 0;

//   for (int i = 0; i < 30; ++i)
//     entries.push_back("RadioBox " + std::to_string(i));
//   auto radiobox = Radiobox(&entries, &selected);
//   auto renderer = Renderer(radiobox, [&] {
//     return radiobox->Render() | vscroll_indicator | frame |
//            size(HEIGHT, LESS_THAN, 10) | border;
//   });

//   auto screen = ScreenInteractive::FitComponent();
//   screen.Loop(renderer);

//   return 0;
// }


// // Copyright 2020 Arthur Sonzogni. All rights reserved.
// // Use of this source code is governed by the MIT license that can be found in
// // the LICENSE file.
// #include <memory>  // for shared_ptr, __shared_ptr_access
// #include <string>  // for string, basic_string, operator+, to_string
// #include <vector>  // for vector

// #include "ftxui/component/captured_mouse.hpp"      // for ftxui
// #include "ftxui/component/component.hpp"           // for Radiobox, Renderer
// #include "ftxui/component/component_base.hpp"      // for ComponentBase
// #include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
// #include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, HEIGHT, LESS_THAN

// using namespace ftxui;

// int main() {
//   std::vector<std::string> entries;
//   int selected = 0;

//   for (int i = 0; i < 100; ++i)
//     entries.push_back(std::to_string(i));
//   auto radiobox = Menu(&entries, &selected, MenuOption::Vertical());
//   auto renderer = Renderer(
//       radiobox, [&] { return radiobox->Render() | hscroll_indicator | border ; });

//   auto screen = ScreenInteractive::FitComponent();
//   screen.Loop(renderer);

//   return 0;
// }

// // Copyright 2021 Arthur Sonzogni. All rights reserved.
// // Use of this source code is governed by the MIT license that can be found in
// // the LICENSE file.
// #include <memory>  // for allocator, shared_ptr, __shared_ptr_access
// #include <functional>  // for function
// #include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
// #include <string>    // for string, basic_string, allocator
// #include <vector>    // for vector

// #include "ftxui/component/captured_mouse.hpp"  // for ftxui
// #include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
// #include "ftxui/component/component_base.hpp"      // for ComponentBase
// #include "ftxui/component/component_options.hpp"   // for MenuOption
// #include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
// #include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

// using namespace ftxui;
// using namespace std;

// // An example of how to compose multiple components into one and maintain their
// // interactiveness.
// int main(int argc, char const *argv[])
// {
//     std::vector<std::string> entries = 
//     {
//         "entry 1",
//         "entry 2",
//         "entry 3",
//     };
//     int selected = 0;
//     auto left_count = 0;
//     auto right_count = 0;


//   // Renderer decorates its child with a new rendering function. The way the
//   // children reacts to events is maintained.
//   auto leftpane = Renderer(left_buttons, [&] 
//   {
//     return vbox({
//                text("This is the left control"),
//                separator(),
//                text("Left button count: " + std::to_string(left_count)),
//                left_buttons->Render(),
//            }) |
//            border;
//   });

//     auto rightpane = Renderer(right_buttons, [&] 
//     {
//         return vbox({
//                text("This is the right control"),
//                separator(),
//                text("Right button count: " + std::to_string(right_count)),
//                right_buttons->Render(),
//            }) |
//            border;
//     });

//   // Container groups components togethers. To render a Container::Horizontal,
//   // it render its children side by side. It maintains their interactiveness and
//   // provide the logic to navigate from one to the other using the arrow keys.
//   auto composition = Container::Vertical({leftpane, rightpane});

//   auto screen = ScreenInteractive::FitComponent();
//   screen.Loop(composition);
//   return 0;
// }

// // Thanks to Chris Morgan for this example!