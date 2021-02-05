#include "Input.hpp"
#include "Widgets.hpp"
#include "Termbox.hpp"
#include <map>

// {{{ Mouse
Mouse::Mouse(Type type, std::function<void(Termbox&, const Vec2i&)> callback)
{
	m_callback = callback;
	m_dim = m_pos = { 0, 0 };
	m_type = type;
}

Mouse::Mouse(const std::pair<Vec2i, Vec2i>& area, Type type, std::function<void(Termbox&, const Vec2i&)> callback)
{
	m_callback = callback;
	SetArea(area);
	m_type = Type::MOUSE_LEFT;
	m_type = type;
}

Mouse::~Mouse()
{
}

bool Mouse::operator==(const Mouse& m) const
{
	return m_pos == m.m_pos && m_dim == m.m_dim && m_type == m.m_type;
}

void Mouse::SetArea(const std::pair<Vec2i, Vec2i>& area)
{
	m_pos = area.first;
	m_dim = area.second;
}

bool Mouse::Match(Termbox& tb, const Widget& w) const
{
	const tb_event& ev = tb.GetContext().ev;
	if (ev.key != m_type)
		return false;

	const Vec2i temp = m_pos + w.GetPosition();
	Vec2i pos(static_cast<int>(ev.x), static_cast<int>(ev.y));

	if ((pos[0] >= w.GetPosition()[0] + temp[0] && pos[0] <= w.GetPosition()[0] + temp[0] + m_dim[0]) &&
		(pos[1] >= w.GetPosition()[1] + temp[1] && pos[1] <= w.GetPosition()[1] + temp[1] + m_dim[1]))
	{
		m_callback(tb, pos);
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
				/*
			case TB_KEY_CTRL_TILDE: // Same as CTRL_2
				return U"aC-~";
				*/
				/*
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
				*/
			case TB_KEY_BACKSPACE: // Same as C-8
				return U"BACKSPACE";
			case TB_KEY_TAB: // Same as C-I
				return U"TAB";
				/*
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
				*/
			case TB_KEY_ESC: // Same as C-[ and ^3
				return U"ESC";
				/*
			case TB_KEY_CTRL_BACKSLASH: // Same as C-4
				return U"C-\\";
			case TB_KEY_CTRL_RSQ_BRACKET: // Same as C-5
				return U"C-]";
			case TB_KEY_CTRL_6:
				return U"C-6";
			case TB_KEY_CTRL_UNDERSCORE: // Same as C-/ and ^7
				return U"C-_";
				*/
			case TB_KEY_SPACE:
				return U"SPC";
		}
	}

	return Settings::empty_key_name;
}
// }}}

// {{{ KeyComb
KeyComb::KeyComb(std::function<void(Termbox&)> callback)
{
	m_keys = nullptr;
	m_keys_num = 0;
	m_callback = callback;
	m_matchState = false;
}

KeyComb::KeyComb(const String& s, std::function<void(Termbox&)> callback)
{
	m_keys = nullptr;
	m_keys_num = 0;
	m_callback = callback;
	m_matchState = false;

	if (auto&& [success, pos] = SetComb(s); !success)
		throw Util::Exception("Could not understand KeyComb '" + Util::StringConvert<char>(s) + "' Error at position " + std::to_string(pos) + ".");
}

KeyComb::~KeyComb()
{
	//if (m_keys != nullptr)
	//delete[] m_keys;
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

enum KC_SPECIAL : std::uint32_t
{
	KC_SPECIAL_CHAR = 0xFFFFFFFF, // Char only
	KC_SPECIAL_SCHAR = 0xFFFFFFFF - 1, // Char only, may have 'S'
};

std::pair<bool, std::size_t>
KeyComb::SetComb(const String& s)
{
	static const std::map<String, std::uint32_t> KeyMap = {
		{ U"F1", 0xFFFF - 0 },
		{ U"F2", 0xFFFF - 1 },
		{ U"F3", 0xFFFF - 2 },
		{ U"F4", 0xFFFF - 3 },
		{ U"F5", 0xFFFF - 4 },
		{ U"F6", 0xFFFF - 5 },
		{ U"F7", 0xFFFF - 6 },
		{ U"F8", 0xFFFF - 7 },
		{ U"F9", 0xFFFF - 8 },
		{ U"F10", 0xFFFF - 9 },
		{ U"F11", 0xFFFF - 10 },
		{ U"F12", 0xFFFF - 11 },
		{ U"INS", 0xFFFF - 12 },
		{ U"DEL", 0xFFFF - 13 },
		{ U"HOME", 0xFFFF - 14 },
		{ U"END", 0xFFFF - 15 },
		{ U"PGUP", 0xFFFF - 16 },
		{ U"PGDN", 0xFFFF - 17 },
		{ U"LEFT", 0xFFFF - 18 },
		{ U"RIGHT", 0xFFFF - 19 },
		{ U"DOWN", 0xFFFF - 20 },
		{ U"UP", 0xFFFF - 21 },
		{ U"BACKSPACE", 0x08 },
		{ U"TAB", 0x09 },
		{ U"ENTER", 0x0D },
		{ U"ESC", 0x1B },
		{ U"SPC", 0x20 },
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

		auto k = getKey();
		if (k)
		{
			keyv.push_back({ k, Key::KEY, meta });
		}
		else
		{
			if (key.size() != 1)
			{
				if (key == U"#CHAR")
					keyv.push_back({ KC_SPECIAL_CHAR, Key::CHAR, meta });
				if (key == U"#SCHAR")
					keyv.push_back({ KC_SPECIAL_SCHAR, Key::CHAR, meta });
				else
					return false;
			}
			else
				keyv.push_back({ key[0], Key::CHAR, meta });
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

bool KeyComb::Match(Termbox& tb)
{
	const tb_event& ev = tb.GetContext().ev;
	if (m_keys_num == 0)
		return 0;

	const Key* k = &m_keys[m_matchState];
	if (k->type == Key::CHAR) //; Match char first
	{
		if ((k->meta == Key::ANY && k->code == ev.ch) ||
			(k->meta == ev.meta && k->code == ev.ch))
			++m_matchState;
		else if (k->code == KC_SPECIAL_CHAR && (k->meta == Key::ANY || k->meta == ev.meta))
			++m_matchState;
		else if (k->code == KC_SPECIAL_SCHAR &&
				 (k->meta == Key::ANY || (k->meta | Key::Meta::SHIFT) == (ev.meta | Key::Meta::SHIFT)))
			++m_matchState;
		else
			m_matchState = 0;
	}
	else
	{
		if ((k->meta == Key::ANY && k->code == ev.key) || (k->code == ev.key && k->meta == ev.meta))
			++m_matchState;
		else
			m_matchState = 0;
	}

	if (m_matchState == Size())
	{
		m_matchState = 0;
		m_callback(tb);
		return true;
	}

	return false;
}
// }}}
