#include "Termbox.hpp"
#include "Widgets.hpp"

Termbox::Termbox(Color::COLOR_MODE mode, Color bg)
{
	m_bg = bg;
	int err = tb_init();
	if (err < 0)
		throw Util::Exception("Could not initialize termbox : tb_init() returned " + std::to_string(err));
	Color::SetMode(mode);
	tb_enable_mouse();

	s_dim = Vec2i( tb_width(), tb_height() );
	tb_set_clear_attributes(COLOR_DEFAULT(), bg());

	m_this = this;
}

Termbox::~Termbox()
{
	tb_shutdown();
}

void Termbox::Draw(Drawable& d) const
{
	d.Draw();
}

void Termbox::Clear() const
{
	tb_clear_buffer();
}

void Termbox::Resize()
{
	tb_clear_screen();
	tb_clear_buffer();
	s_dim = { tb_width(), tb_height() };
	m_ctx.clear = true;

	for (auto& it : m_widgets)
	{
		it.first->Resize(s_dim);
		it.second = false;
	}
}

void Termbox::Display()
{
	tb_render();
	++m_ctx.frameCount;
}

void Termbox::SetColor(Color bg)
{
	m_bg = bg;
	tb_set_clear_attributes(COLOR_DEFAULT(), bg());
}

const Color& Termbox::GetColor() const
{
	return m_bg;
}

void Termbox::SetMode(Color::COLOR_MODE mode) const
{
	Color::SetMode(mode);
}

const Color::COLOR_MODE& Termbox::GetMode() const
{
	return Color::GetMode();
}

Vec2i Termbox::GetDim()
{
	return s_dim;
}

struct tb_cell Termbox::At(Vec2i pos)
{
	const auto& [x, y] = pos;
	if (x * y >= s_dim[0] * s_dim[1])
		return { 0, 0, 0 };

	return tb_cell_buffer()[x + y * s_dim[0]];
}

void Termbox::SetCursor(Vec2i pos)
{
	tb_set_cursor(pos[0], pos[1]);
}

std::size_t Termbox::AddWidget(Widget* widget)
{
	// Optional check
	for (const auto& it : m_this->m_widgets)
		if (it.first == widget)
			return static_cast<std::size_t>(-1);

	m_this->m_widgets.push_back({ widget, true });
	return m_this->m_widgets.size()-1;
}

Widget* Termbox::RemoveWidget(std::size_t id)
{
	if (id < m_this->m_widgets.size())
	{
		Widget* w = m_this->m_widgets[id].first;
		m_this->m_widgets.erase(m_this->m_widgets.begin()+id);
		return w;
	}

	return nullptr;
}

Widget* Termbox::GetWidget(std::size_t id)
{
	if (id < m_this->m_widgets.size())
		return m_this->m_widgets[id].first;

	return nullptr;
}

std::size_t Termbox::FindWidget(Widget* ptr)
{
	for (std::size_t id = 0; id < m_this->m_widgets.size(); ++id)
		if (m_this->m_widgets[id].first == ptr)
			return id;
	
	return std::size_t(-1);
}

bool Termbox::SetWidgetExpired(std::size_t id, bool expired)
{
	if (id >= m_this->m_widgets.size())
		return false;

	m_this->m_widgets[id].second = expired;
	return true;
}

void Termbox::ReDraw()
{
	if (m_ctx.clear)
		Clear();

	for (auto& it : m_widgets)
	{
		if (it.second || m_ctx.clear)
		{
			if (it.first->IsVisible())
				this->Draw(*it.first);
			it.second = false;
		}
	}

	if (m_ctx.clear)
		m_ctx.clear = false;
}

void Termbox::ProcessEvent()
{
	tb_event& ev = m_ctx.ev;
	switch (ev.type)
	{
		case TB_EVENT_RESIZE:
			Resize();
		case TB_EVENT_KEY: {
			if constexpr (Settings::enable_repeat)
			{
				if (ev.ch >= U'0' && ev.ch <= U'9' && !m_ctx.noRepeat)
				{
					OnRepeatChange.Notify<EventWhen::BEFORE>(m_ctx.repeat);
					m_ctx.hasRepeat = true;
					m_ctx.repeat *= 10;
					m_ctx.repeat += ev.ch-U'0';
					OnRepeatChange.Notify<EventWhen::AFTER>(m_ctx.repeat);
					break;
				}
			}
			bool matched = false;
			for (auto& it : m_widgets)
			{
				if (it.first->IsActive())
				{
					auto [c, m] = it.first->ProcessKeyboardEvent(*this);
					it.second |= c;
					matched |= m;
				}
			}
			if (matched)
				m_ctx.hasMatched = true;
			else
				m_ctx.hasMatched = false;
			if constexpr (Settings::enable_repeat)
			{
				if (m_ctx.repeat != 0 && !m_ctx.dontResetRepeat)
				{
					OnRepeatChange.Notify<EventWhen::BEFORE>(m_ctx.repeat);
					m_ctx.repeat = 0;
					m_ctx.hasRepeat = false;
					OnRepeatChange.Notify<EventWhen::AFTER>(m_ctx.repeat);
				}
				m_ctx.dontResetRepeat = false;
			}
		}
		break;
		case TB_EVENT_MOUSE: {
			for (auto& it : m_widgets)
			{
				if (it.first->IsActive())
					it.second = it.first->ProcessMouseEvent(*this, *it.first);
			}
		}
		break;
	}
	m_ctx.stopInput = false;
}

void Termbox::RenderLoop()
{
	static std::mutex mtx;
	static std::condition_variable cv;
	static std::unique_lock<std::mutex> l(mtx);

	Clear();
	ReDraw();
	Display();

	while (!m_ctx.stop  && tb_poll_event(&m_ctx.ev) != -1)
	{
		if (m_ctx.lock)
			continue;

		ProcessEvent();

		ReDraw();
		Display();
	}
}

void Termbox::ForceDraw()
{
	for (auto& it : m_widgets)
	{
		if (it.first->IsVisible())
			this->Draw(*it.first);
	}

	if (m_ctx.clear)
		m_ctx.clear = false;
}

Termbox::Context& Termbox::GetContext()
{
	return m_this->m_ctx;
}

Termbox& Termbox::GetTermbox()
{
	return *m_this;
}
