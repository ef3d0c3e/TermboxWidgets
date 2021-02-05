#ifndef TERMBOX_WIDGETS_UTIL_HPP
#define TERMBOX_WIDGETS_UTIL_HPP

#include "Vector.hpp"
#include "Cenum.hpp"
#include <string>

#define ARRAY_LENGTH(__a) (sizeof(__a) / sizeof(__a[0]))

typedef char32_t Char;
typedef std::basic_string<Char> String;
typedef Vector<int, 2, VectorSettings{ .UseFor = false }> Vec2i;


////////////////////////////////////////////////
/// \brief Utility namespace
////////////////////////////////////////////////
namespace Util
{
////////////////////////////////////////////////
/// \brief Exception class
////////////////////////////////////////////////
class Exception
{
	std::function<void()> m_cleanup;
	std::string m_msg;

public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param msg Exception message
	/// \param cleanup Function that will be called with what()
	////////////////////////////////////////////////
	Exception(
		const std::string& msg,
		std::function<void()> cleanup = []() {})
	{
		m_msg = msg;
		m_cleanup = cleanup;
	}

	virtual std::string what() const throw()
	{
		m_cleanup();
		return m_msg;
	}
};


////////////////////////////////////////////////
/// \brief Check if variadic types are the same
/// \code{.cpp}
/// []<typename... Ts>()
/// {
///		static_assert(are_same<int, Ts>()); // Check if all Ts are int
/// };
/// \endcode
/// \returns If every type Ts is the same as T
////////////////////////////////////////////////
template <class T, class... Ts>
struct are_same : std::bool_constant<(std::is_same_v<T, Ts> && ...)>
{};

////////////////////////////////////////////////
/// \brief Array creation
///
/// \param t,ts The elements of the formed array
/// \tparam T The type of the array
/// \tparam Ts... Variadic type
/// \note Every single type Ts has to be the same as T
/// \returns An array containing t and every ts
////////////////////////////////////////////////
template <typename T, typename... Ts>
constexpr std::array<T, sizeof...(Ts) + 1> make_array(T t, Ts... ts);

////////////////////////////////////////////////
/// \brief String conversion
///
/// \param s The string to convert
/// \tparam T the type of the resulting string's elements
/// \tparam U the type of the base string's elements
/// \returns A converted string
////////////////////////////////////////////////
template <typename T, typename U>
std::basic_string<T> StringConvert(const std::basic_string<U>& s);

////////////////////////////////////////////////
/// \brief Strlen for any type of strings
///
/// \param s The string
/// \tparam T the type of the array's elements
/// \returns The index of the first '\0'
////////////////////////////////////////////////
template <typename T>
std::size_t Strlen(const T* s);

////////////////////////////////////////////////
/// \brief Convert a string ot uppercase
///
/// \param s The string to convert
/// \tparam T the type of the resulting string's elements
/// \tparam U the type of the base string's elements
/// \returns An uppercase string
////////////////////////////////////////////////
template <typename T>
std::basic_string<T> ToUpper(std::basic_string<T> s);

////////////////////////////////////////////////
/// \brief Split a string at delim
///
/// \param s The string to split
/// \param delim The delimiter to use for splitting
/// \param pieceHint A hint on the number of pieces
/// \tparam T the type of the string's elements
/// \returns A vector containing the splitted strings
////////////////////////////////////////////////
template <class T>
std::vector<std::basic_string<T>> Split(const std::basic_string<T>& s, const std::basic_string<T>& delim, std::size_t pieceHint = 0);

////////////////////////////////////////////////
/// \brief Get the number of digits
///
/// \param x The number
/// \tparam T the type of the number
/// \tparam B the base
/// \returns The number of digits of x in base B
////////////////////////////////////////////////
template <std::size_t B = 10, class T>
std::uint8_t GetDigitsNum(T x);

////////////////////////////////////////////////
/// \brief Convert an integer to string
///
/// \param x The number
/// \param charFn The function to get a Char from a digit in base B
/// \code{.cpp}
/// // Example charFn for bases 2-36
/// []<typename T>(T d) -> Char
/// {
///		Char c;
/// 	if (d < 10)
/// 		c = '0'+d;
/// 	else
/// 		c = 'A'+(d-10);
/// 	return c;
/// };
/// \endcode
/// \tparam T the type of the number
/// \tparam B the base
/// \returns x as a string in base B
////////////////////////////////////////////////
template <std::size_t B = 10, class T>
String ToString(T x, std::function<Char(T)> charFn = [](T d)
		{
			Char c;
			if (d < 10)
				c = '0'+d;
			else
				c = 'A'+(d-10);
			return c;
		});
}
/** @cond */
#include "Util.tpp"
/** @endcond */

#endif // TERMBOX_WIDGETS_UTIL_HPP
