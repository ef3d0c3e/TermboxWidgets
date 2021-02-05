#ifndef TERMBOX_WIDGETS_SETTINGS_HPP
#define TERMBOX_WIDGETS_SETTINGS_HPP

#include "Text.hpp"

////////////////////////////////////////////////
/// \brief Namespace that holds various settings
/// \ingroup Records
////////////////////////////////////////////////
namespace Settings
{
namespace Colors
{
	constexpr Color background = COLOR_DEFAULT;
	constexpr Color foreground = 0xFFFFFF;
}

////////////////////////////////////////////////
/// \brief String to represent an empty key combination (i.e not bound)
////////////////////////////////////////////////
constexpr Char empty_key_name[] = U"<none>";

////////////////////////////////////////////////
/// \brief Separator that will be printed between keys
/// \code{.cpp}
/// KeyComb kc(U"C-xC-q");
/// auto s = kc.Print(); // U"C-x C-q" // Put a ' ' between the two keys
/// \endcode
////////////////////////////////////////////////
constexpr Char keycomb_separator[] = U" ";

////////////////////////////////////////////////
/// \brief Character to use for filling
////////////////////////////////////////////////
constexpr Char fill_character = U' ';
////////////////////////////////////////////////
/// \brief Character to represent not enough space
////////////////////////////////////////////////
constexpr Char trailing_character = U'…';

////////////////////////////////////////////////
/// \brief Default border of a window
/// \code
/// ╭Window───────────────────╮
/// │                         │
/// │                         │
/// │                         │
/// │                         │
/// ╰─────────────────────────╯
/// \endcode
////////////////////////////////////////////////
constexpr std::array<Char, 8> default_border =
	Util::make_array(U'╭', U'╮', U'╯', U'╰', U'─', U'│', U'─', U'│');

////////////////////////////////////////////////
/// \brief Default style for a window's border
////////////////////////////////////////////////
constexpr TBStyle default_border_style = TBStyle(Settings::Colors::foreground, Settings::Colors::background, TextStyle::None);

////////////////////////////////////////////////
/// \brief Default style for a window's name
////////////////////////////////////////////////
constexpr TBStyle default_window_name_style = TBStyle(Settings::Colors::foreground, Settings::Colors::background, TextStyle::Bold);

////////////////////////////////////////////////
/// \brief Default text style
////////////////////////////////////////////////
constexpr TBStyle default_text_style = TBStyle(0xFFFFFF, COLOR_DEFAULT, TextStyle::None);


// useful settings after this
// Resolution of the event clock, currently a thread will be spawn for each individual widgets
constexpr long int event_clock_resolution = 16666; // 1/60 s

constexpr bool enable_repeat = true;


}

#endif // TERMBOX_WIDGETS_SETTINGS_HPP
