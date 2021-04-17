#include "Util.hpp"

#include <codecvt>
#include <locale>

template <typename T, typename ... Ts>
constexpr std::array<T, sizeof...(Ts)+1> Util::make_array(T t, Ts... ts)
{
	static_assert(are_same<T, Ts...>());

	std::array<T, sizeof...(Ts)+1> a;

	std::size_t i = 1;
	a[0] = t;
	((a[i++] = ts), ...);

	return a;
}

template <typename T, typename U>
std::basic_string<T> Util::StringConvert(const std::basic_string<U>& s)
{
	if constexpr (std::is_same_v<T, U>)
		return s;

	if constexpr (std::is_same_v<T, Char>)
	{
		if constexpr (std::is_same_v<U, char>)
		{
			std::wstring_convert<std::codecvt_utf8<Char>, Char> conv;
			return conv.from_bytes(s);
		}
	}
	else if constexpr (std::is_same_v<T, char>)
	{
		if constexpr (std::is_same_v<U, Char>)
		{
			std::wstring_convert<std::codecvt_utf8<Char>, Char> conv;
			return conv.to_bytes(s);
		}
	}
	else if constexpr (std::is_same_v<T, wchar_t>)
	{
		if constexpr (std::is_same_v<U, Char>)
		{
			[]<bool v = false>() // only assert if conversion is tried
			{
				static_assert(v || sizeof(wchar_t) == sizeof(char32_t));
			}();

			std::wstring w(s.size(), L'\0');
			for (std::size_t i = 0; i < s.size(); ++i)
				w[i] = s[i];
			return w;
		}
	}
}

template <typename T>
std::size_t Util::Strlen(const T* s)
{
	std::size_t size = 0;
	while (s[size++]);
	return size - (size != 0 ? 1 : 0);
}

template <typename T>
std::basic_string<T> Util::ToUpper(std::basic_string<T> s)
{
	auto toUpper = [](T c)
	{
		if constexpr (std::is_same_v<T, char>)
			return toupper(c);
		else if constexpr (std::is_same_v<T, Char>)
			return towupper(c);
	};

	for (auto& c : s)
		c = toUpper(c);
	
	return s;
}

template <class T>
std::vector<std::basic_string<T>> Util::Split(const std::basic_string<T>& s, const std::basic_string<T>& delim, std::size_t pieceHint)
{
	using string = std::basic_string<T>;
	std::vector<string> split;
	split.reserve(pieceHint);

	std::size_t i = s.find(delim);
	std::size_t p = 0;
	while (i != string::npos)
	{
		split.push_back(s.substr(p, i-p));
		p = i+1;
		i = s.find(delim, p);
	}
	split.push_back(s.substr(p, i-p));

	return split;
}

template <std::size_t B, class T>
std::uint8_t Util::GetDigitsNum(T x)
{
	if (x == T(B))
		return 2;
	if (x < T(0))
		return 0;
	if (x < T(B))
		return 1;
	
	static constexpr double log_b = std::log(B);
	
	//TODO: use log10i & log2i
	return 1+std::log(x)/log_b;
}

template <std::size_t B, class T>
String Util::ToString(T x, std::function<Char(T)> charFn)
{
	if (x == 0)
	{
		return U"0";
	}

	String s(U"");
	if constexpr (std::is_signed_v<T>)
	{
		if (x < T(0))
		{
			s += U'-';
			x = -x;
		}
	}

	const auto N = GetDigitsNum<B>(x);

	s.resize(s.size()+N);

	std::size_t i = 1;
	while (x != T(0))
	{
		s[s.size()-i] = charFn(x % B);
		x /= B;
		++i;
	}

	return s;
}
