#include "Input.hpp"
#include "Widgets.hpp"
#include "Termbox.hpp"
#include <map>
#include <cstring>

// {{{ Mouse
Mouse::Mouse(Type type, decltype(m_callback) callback)
{
	m_callback = callback;
	m_szOffset = m_posOffset = { 0, 0 };
	m_type = type;
}

Mouse::Mouse(const std::pair<Vec2i, Vec2i>& offset, Type type, decltype(m_callback) callback)
{
	m_callback = callback;
	SetOffset(offset);
	m_type = Type::MOUSE_LEFT;
	m_type = type;
}

Mouse::~Mouse()
{
}

void Mouse::SetOffset(const std::pair<Vec2i, Vec2i>& offset)
{
	m_posOffset = offset.first;
	m_szOffset = offset.second;
}

bool Mouse::Match(Termbox& tb, const Widget& w) const
{
	const tb_event& ev = tb.GetContext().ev;
	if (ev.key != m_type)
		return false;

	const Vec2i pos(static_cast<int>(ev.x), static_cast<int>(ev.y));
	const Vec2i wpos = m_posOffset + w.GetPosition();
	const Vec2i wdim = w.GetSize()+m_szOffset;


	if ((pos[0] >= wpos[0] && pos[0] <= wpos[0] + wdim[0]) &&
		(pos[1] >= wpos[1] && pos[1] <= wpos[1] + wdim[1]))
	{
		m_callback(pos);
		return true;
	}

	return false;
}
// }}}

// {{{ Key
Key::Key()
{
	this->code = 0;
	this->type = Type::CHAR;
	this->meta = Meta::NONE;
}

Key::Key(Char code, Type type, Meta meta)
{
	this->code = code;
	this->type = type;
	this->meta = meta;
}

bool Key::operator==(const Key& m) const
{
	return code == m.code && meta == m.meta;
}

MAKE_CENUMV_Q(KC_SPECIAL, std::uint32_t,
	KC_SPECIAL_CHAR, 0xFFFFFFFF, // Char only
	KC_SPECIAL_SCHAR, 0xFFFFFFFF - 1, // Char only, may have 'S'
	KC_SPECIAL_ANY, 0xFFFFFFFF - 2, // Any key
);

String Key::GetKeyName() const
{
	/* TODO: Finish */
	String metaName(U"");
	bool upperCase = false;
	switch (meta)
	{
		case ALT:
			metaName = U"M-";
			break;
		case ALTSHIFT:
			metaName = U"M-";
			if (type == Key::CHAR)
				upperCase = true;
			else
				metaName += U"S-";
			break;
		case CTRL:
			metaName = U"C-";
			break;
		case ALTCTRL:
			metaName = U"C-M-";
			break;
		case SHIFT:
			metaName = U"";
			if (type == Key::CHAR)
				upperCase = true;
			else
				metaName += U"S-";
			break;
		case CTRLSHIFT:
			metaName = U"C-";
			if (type == Key::CHAR)
				upperCase = true;
			else
				metaName += U"S-";
			break;
		case ALTCTRLSHIFT:
			metaName = U"C-M-";
			if (type == Key::CHAR)
				upperCase = true;
			else
				metaName += U"S-";
			break;
		default:
			break;
	}
	if (type == Key::CHAR)
	{
		if (code == KC_SPECIAL_ANY)
			return metaName + U"(any)";
		else if (code == KC_SPECIAL_CHAR)
			return metaName + U"(char)";
		else if (code == KC_SPECIAL_SCHAR)
			return metaName + U"(char nocase)";
		String ret = U" ";
		ret[0] = (upperCase) ? towupper(code) : code;
		return metaName + ret;
	}
	else if (type == Key::KEY)
	{
		switch (code)
		{
			case TB_KEY_F1:
				return metaName + U"F1";
			case TB_KEY_F2:
				return metaName + U"F2";
			case TB_KEY_F3:
				return metaName + U"F3";
			case TB_KEY_F4:
				return metaName + U"F4";
			case TB_KEY_F5:
				return metaName + U"F5";
			case TB_KEY_F6:
				return metaName + U"F6";
			case TB_KEY_F7:
				return metaName + U"F7";
			case TB_KEY_F8:
				return metaName + U"F8";
			case TB_KEY_F9:
				return metaName + U"F9";
			case TB_KEY_F10:
				return metaName + U"F10";
			case TB_KEY_F11:
				return metaName + U"F11";
			case TB_KEY_F12:
				return metaName + U"F12";
			case TB_KEY_INSERT:
				return metaName + U"INS";
			case TB_KEY_DELETE:
				return metaName + U"DEL";
			case TB_KEY_HOME:
				return metaName + U"HOME";
			case TB_KEY_END:
				return metaName + U"END";
			case TB_KEY_PGUP:
				return metaName + U"PGUP";
			case TB_KEY_PGDN:
				return metaName + U"PGDN";
			case TB_KEY_ARROW_LEFT:
				return metaName + U"←";
			case TB_KEY_ARROW_RIGHT:
				return metaName + U"→";
			case TB_KEY_ARROW_DOWN:
				return metaName + U"↓";
			case TB_KEY_ARROW_UP:
				return metaName + U"↑";
			case TB_KEY_CTRL_TILDE: // Same as CTRL_2
				return U"C-~";
			case TB_KEY_CTRL_A:
				return U"C-A";
			case TB_KEY_CTRL_B:
				return U"C-B";
			case TB_KEY_CTRL_C:
				return U"C-C";
			case TB_KEY_CTRL_D:
				return U"C-D";
			case TB_KEY_CTRL_E:
				return U"C-E";
			case TB_KEY_CTRL_F:
				return U"C-F";
			case TB_KEY_CTRL_G:
				return U"C-G";
			case TB_KEY_BACKSPACE: // Same as C-8
				return U"BACKSPACE";
			case TB_KEY_TAB: // Same as C-I
				return U"TAB";
			case TB_KEY_CTRL_H: // Same as C-BACKSPACE
				return U"C-H";
			case TB_KEY_CTRL_J:
				return U"C-J";
			case TB_KEY_CTRL_K:
				return U"C-K";
			case TB_KEY_CTRL_L:
				return U"C-L";
			case TB_KEY_ENTER: // Same as C-M
				return U"ENTER";
			case TB_KEY_CTRL_N:
				return U"C-N";
			case TB_KEY_CTRL_O:
				return U"C-O";
			case TB_KEY_CTRL_P:
				return U"C-P";
			case TB_KEY_CTRL_Q:
				return U"C-Q";
			case TB_KEY_CTRL_R:
				return U"C-R";
			case TB_KEY_CTRL_S:
				return U"C-S";
			case TB_KEY_CTRL_T:
				return U"C-T";
			case TB_KEY_CTRL_U:
				return U"C-U";
			case TB_KEY_CTRL_V:
				return U"C-V";
			case TB_KEY_CTRL_W:
				return U"C-W";
			case TB_KEY_CTRL_X:
				return U"C-X";
			case TB_KEY_CTRL_Y:
				return U"C-Y";
			case TB_KEY_CTRL_Z:
				return U"C-Z";
			case TB_KEY_ESC: // Same as C-[ and ^3
				return U"ESC";
			case TB_KEY_CTRL_BACKSLASH: // Same as C-4
				return U"C-\\";
			case TB_KEY_CTRL_RSQ_BRACKET: // Same as C-5
				return U"C-]";
			case TB_KEY_CTRL_6:
				return U"C-6";
			case TB_KEY_CTRL_UNDERSCORE: // Same as C-/ and ^7
				return U"C-_";
			case TB_KEY_SPACE:
				return U"SPC";
		}
	}

	return Settings::empty_key_name;
}
// }}}

// {{{ KeyComb
KeyComb::KeyComb(decltype(m_callback) callback)
{
	m_keys = nullptr;
	m_keys_num = 0;
	m_callback = callback;
	m_matchState = false;
}

KeyComb::KeyComb(const String& s, decltype(m_callback) callback)
{
	m_keys = nullptr;
	m_keys_num = 0;
	m_callback = callback;
	m_matchState = false;

	if (auto&& [success, pos] = SetComb(s); !success)
		throw Util::Exception("Could not understand KeyComb '" + Util::StringConvert<char>(s) + "'. Error at position " + std::to_string(pos) + ".");
}

KeyComb::KeyComb(const KeyComb& kc)
{
	m_keys_num = kc.m_keys_num;
	m_callback = kc.m_callback;
	m_matchState = kc.m_matchState;

	m_keys = new Key[m_keys_num];
	std::memcpy(m_keys, kc.m_keys, m_keys_num*sizeof(Key));
}

KeyComb::~KeyComb()
{
	if (m_keys != nullptr)
		delete[] m_keys;
}

bool KeyComb::operator==(const KeyComb& kc) const
{
	if (Size() != kc.Size())
		return false;

	for (std::size_t i = 0; i < Size(); ++i)
	{
		if (m_keys[i] != kc.m_keys[i])
			return false;
	}

	return true;
}

std::pair<bool, std::size_t>
KeyComb::SetComb(const String& s)
{
	static const std::map<StringView, std::uint32_t> KeyMap = {
		{ U"F1",          TB_KEY_F1 },
		{ U"F2",          TB_KEY_F2 },
		{ U"F3",          TB_KEY_F3 },
		{ U"F4",          TB_KEY_F4 },
		{ U"F5",          TB_KEY_F5 },
		{ U"F6",          TB_KEY_F6 },
		{ U"F7",          TB_KEY_F7 },
		{ U"F8",          TB_KEY_F8 },
		{ U"F9",          TB_KEY_F9 },
		{ U"F10",         TB_KEY_F10 },
		{ U"F11",         TB_KEY_F11 },
		{ U"F12",         TB_KEY_F12 },
		{ U"INS",         TB_KEY_INSERT },
		{ U"DEL",         TB_KEY_DELETE },
		{ U"HOME",        TB_KEY_HOME },
		{ U"TAB",         TB_KEY_TAB },
		{ U"END",         TB_KEY_END },
		{ U"PGUP",        TB_KEY_PGUP },
		{ U"PGDN",        TB_KEY_PGDN },
		{ U"LEFT",        TB_KEY_ARROW_LEFT },
		{ U"RIGHT",       TB_KEY_ARROW_RIGHT },
		{ U"DOWN",        TB_KEY_ARROW_DOWN },
		{ U"UP",          TB_KEY_ARROW_UP },
		{ U"BACKSPACE",   TB_KEY_BACKSPACE },
		{ U"ENTER",       TB_KEY_ENTER },
		{ U"ESC",         TB_KEY_ESC },
		{ U"SPC",         TB_KEY_SPACE },
		{ U"C-BACKSPACE", TB_KEY_CTRL_H },
	};

	static const std::map<StringView, std::uint32_t> CtrlKeyMap = {
		{ U"~",         TB_KEY_CTRL_TILDE },
		{ U"2",         TB_KEY_CTRL_TILDE },
		{ U"A",         TB_KEY_CTRL_A },
		{ U"B",         TB_KEY_CTRL_B },
		{ U"C",         TB_KEY_CTRL_C },
		{ U"D",         TB_KEY_CTRL_D },
		{ U"E",         TB_KEY_CTRL_E },
		{ U"F",         TB_KEY_CTRL_F },
		{ U"G",         TB_KEY_CTRL_G },
		{ U"8",         TB_KEY_BACKSPACE },
		{ U"I",         TB_KEY_TAB },
		{ U"H",         TB_KEY_CTRL_H },
		{ U"J",         TB_KEY_CTRL_J },
		{ U"K",         TB_KEY_CTRL_K },
		{ U"L",         TB_KEY_CTRL_L },
		{ U"M",         TB_KEY_ENTER },
		{ U"N",         TB_KEY_CTRL_N },
		{ U"O",         TB_KEY_CTRL_O },
		{ U"P",         TB_KEY_CTRL_P },
		{ U"Q",         TB_KEY_CTRL_Q },
		{ U"R",         TB_KEY_CTRL_R },
		{ U"S",         TB_KEY_CTRL_S },
		{ U"T",         TB_KEY_CTRL_T },
		{ U"U",         TB_KEY_CTRL_U },
		{ U"V",         TB_KEY_CTRL_V },
		{ U"W",         TB_KEY_CTRL_W },
		{ U"X",         TB_KEY_CTRL_X },
		{ U"Y",         TB_KEY_CTRL_Y },
		{ U"Z",         TB_KEY_CTRL_Z },
		{ U"3",         TB_KEY_ESC },
		{ U"[",         TB_KEY_ESC },
		{ U"\\",        TB_KEY_CTRL_BACKSLASH },
		{ U"4",         TB_KEY_CTRL_BACKSLASH },
		{ U"]",         TB_KEY_CTRL_RSQ_BRACKET },
		{ U"5",         TB_KEY_CTRL_RSQ_BRACKET },
		{ U"6",         TB_KEY_CTRL_6 },
		{ U"_",         TB_KEY_CTRL_UNDERSCORE },
		{ U"/",         TB_KEY_CTRL_UNDERSCORE },
		{ U"7",         TB_KEY_CTRL_UNDERSCORE },
	};

	std::vector<Key> keyv;
	bool m_ctrl = false, m_shift = false, m_alt = false, m_any = false;
	String key = U"";
	std::size_t p = 0;

	const auto getKey = [&]() -> std::uint32_t {
		String upper = Util::ToUpper(key);

		if (auto found = KeyMap.find(upper);
			found != KeyMap.end())
			return found->second;

		return 0;
	};

	const auto addKey = [&]() -> bool {
		Key::Meta meta;
		if (m_any)
		{
			meta = Key::ANY;
		}
		else
		{
			switch (m_ctrl << 2 | m_shift << 1 | m_alt)
			{
				case 0b000:
					meta = Key::NONE;
					break;
				case 0b001:
					meta = Key::ALT;
					break;
				case 0b010:
					meta = Key::SHIFT;
					break;
				case 0b011:
					meta = Key::ALTSHIFT;
					break;
				case 0b100:
					meta = Key::CTRL;
					break;
				case 0b101:
					meta = Key::ALTCTRL;
					break;
				case 0b110:
					meta = Key::CTRLSHIFT;
					break;
				case 0b111:
					meta = Key::ALTCTRLSHIFT;
					break;
			}
		}

		// Special keys
		auto k = getKey();
		if (k)
		{
			if (k == TB_KEY_TAB)
				meta = Key::CTRL;
			keyv.push_back({ k, (k == TB_KEY_SPACE) ? Key::CHAR : Key::KEY, meta });
			return true;
		}
		
		// C-
		if (meta & Key::CTRL)
		{
			if (auto found = CtrlKeyMap.find(Util::ToUpper(key));
				found != KeyMap.end())
			{
				keyv.push_back({ found->second, Key::KEY, meta });
				return true;
			}
		}
		
		// The rest
		{
			if (key.size() != 1)
			{
				if (key == U"#CHAR")
					keyv.push_back({ KC_SPECIAL_CHAR, Key::CHAR, meta });
				else if (key == U"#SCHAR")
					keyv.push_back({ KC_SPECIAL_SCHAR, Key::CHAR, meta });
				else if (key == U"#ANY")
					keyv.push_back({ KC_SPECIAL_ANY, Key::KEY, meta });
				else
					return false;
			}
			else
			{
				if (meta & Key::SHIFT || meta & Key::CTRLSHIFT || meta & Key::ALTSHIFT || meta & Key::ALTCTRLSHIFT)
					keyv.push_back({ std::towupper(key[0]), Key::CHAR, meta });
				else
					keyv.push_back({ std::towlower(key[0]), Key::CHAR, meta });
			}
		}

		return true;
	};

	auto words = Util::Split(s, String(U" "));
	if (words.size() == 0)
		return { false, 0 };

	for (const auto& w : words)
	{
		auto e = Util::Split(w, String(U"-"));
		if (e.size() == 0)
			return { false, p };
		key = e[e.size() - 1];
		for (std::size_t i = 0; i < e.size() - 1; ++i)
		{
			if (e[i] == U"A" && !m_ctrl && !m_shift && !m_alt && !m_any)
				m_any = true;
			else if (e[i] == U"C" && !m_ctrl && !m_any)
				m_ctrl = true;
			else if (e[i] == U"S" && !m_shift && !m_any)
				m_shift = true;
			else if (e[i] == U"M" && !m_alt && !m_any)
				m_alt = true;
			else
				return { false, p };
		}

		if (!addKey())
			return { false, p };
		m_ctrl = m_shift = m_alt = m_any = false;
		key = U"";

		p += w.size();
	}

	m_keys_num = keyv.size();
	m_keys = new Key[m_keys_num];
	for (std::size_t i = 0; i < keyv.size(); ++i)
		m_keys[i] = keyv[i];

	return { true, 0 };
}

std::size_t KeyComb::Size() const
{
	return m_keys_num;
}

const Key& KeyComb::operator[](std::size_t i) const
{
	return m_keys[i];
}

String KeyComb::GetName() const
{
	String r(U"");

	for (std::size_t i = 0; i < Size(); ++i)
	{
		r += m_keys[i].GetKeyName();
		if (i + 1 < m_keys_num)
			r += Settings::keycomb_separator;
	}

	return r;
}

std::pair<bool, bool> KeyComb::Match(Termbox& tb)
{
	if (m_matchState != tb.GetContext().hasMatched && m_keys[0].code != KC_SPECIAL_ANY && m_keys[0].code != KC_SPECIAL_SCHAR && m_keys[0].code != KC_SPECIAL_CHAR)
		return {false, false};

	const tb_event& ev = tb.GetContext().ev;
	if (m_keys_num == 0)
		return {false, false};

	const auto oldState = m_matchState;
	const Key& k = m_keys[m_matchState];
	if (k.code == KC_SPECIAL_ANY)
	{
		++m_matchState;
	}
	else if (k.type == Key::CHAR) // Match char first
	{
		if ((k.meta == Key::ANY && k.code == ev.ch) ||
			(k.meta == ev.meta && k.code == ev.ch))
			++m_matchState;
		else if (k.code == KC_SPECIAL_CHAR && ev.key == 0 &&
				(k.meta == Key::ANY || k.meta == ev.meta))
			++m_matchState;
		else if (k.code == KC_SPECIAL_SCHAR && ev.key == 0 &&
				 (k.meta == Key::ANY || (k.meta | Key::Meta::SHIFT) == (ev.meta | Key::Meta::SHIFT)))
			++m_matchState;
		else
			m_matchState = 0;
	}
	else
	{
		if ((k.meta == Key::ANY && k.code == ev.key) || (k.code == ev.key && k.meta == ev.meta))
			++m_matchState;
		else
			m_matchState = 0;
	}

	if (m_matchState == Size())
	{
		m_matchState = 0;
		m_callback();
		return {true, false};
	}

	return {false, m_matchState > oldState};
}
// }}}

// {{{ KeyboardInput
std::pair<bool, bool> KeyboardInput::ProcessKeyboardEvent(Termbox& tb)
{
	bool called = false;
	bool matched = false;
	for (auto it = m_keys.begin(); it != m_keys.end(); ++it)
	{
		auto [c, m] = it->Match(tb);
		called |= c;
		matched |= m;
		if (Termbox::GetContext().stopInput)
			break;
	}
	return {called, matched};
}

std::size_t KeyboardInput::AddKeyboardInput(const KeyComb& kc)
{
	m_keys.push_back(kc);
	
	return m_keys.size()-1;
}

std::size_t KeyboardInput::AddKeyboardInput(const String& key, std::function<void()> callback)
{
	return KeyboardInput::AddKeyboardInput({key, callback});
}

bool KeyboardInput::RemoveKeyboardInput(const KeyComb& kc)
{
	return std::erase_if(m_keys, [&](const KeyComb& __kc){ return kc == __kc; }) != 0;
}

void KeyboardInput::RemoveKeyboardInput(std::size_t id)
{
	m_keys.erase(m_keys.begin()+id);
}

void KeyboardInput::RemoveAllKeyboardInput()
{
	m_keys.clear();
}
// }}}

// {{{ MouseInput
bool MouseInput::ProcessMouseEvent(Termbox& tb, const Widget& w)
{
	bool matched = false;
	for (auto& m : m_mouse)
	{
		matched |= m.Match(tb, w);
		if (Termbox::GetContext().stopInput)
			break;
	}
	return matched;
}

////////////////////////////////////////////////
/// \brief Add an input
/// \param m The mouse event
////////////////////////////////////////////////
void MouseInput::AddMouseInput(const Mouse& m)
{
	m_mouse.push_back(m);
}

////////////////////////////////////////////////
/// \brief Remove all input
////////////////////////////////////////////////
void MouseInput::RemoveAllMouseInput()
{
	m_mouse.clear();
}
// }}}
