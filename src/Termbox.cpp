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

bool Termbox::AddWidget(Widget* widget)
{
	for (const auto& it : m_widgets)
		if (it.first == widget)
			return false;

	m_widgets.push_back({ widget, true });
	return true;
}

bool Termbox::RemoveWidget(Widget* widget)
{
	for (std::size_t i = 0; i < m_widgets.size(); ++i)
	{
		if (m_widgets[i].first == widget)
		{
			m_widgets.erase(m_widgets.begin() + i);
			return true;
		}
	}

	return false;
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
				if (ev.ch >= U'0' && ev.ch <= U'9')
				{
					OnRepeatChange.Notify<EventWhen::BEFORE>(m_ctx.repeat);
					m_ctx.hasRepeat = true;
					m_ctx.repeat *= 10;
					m_ctx.repeat += ev.ch-U'0';
					OnRepeatChange.Notify<EventWhen::AFTER>(m_ctx.repeat);
					break;
				}
			}
			for (auto& it : m_widgets)
			{
				if (it.first->IsActive())
					it.second = it.first->ProcessKeyboardEvent(*this);
			}
			if constexpr (Settings::enable_repeat)
			{
				if (m_ctx.repeat != 0)
				{
					OnRepeatChange.Notify<EventWhen::BEFORE>(m_ctx.repeat);
					m_ctx.repeat = 0;
					m_ctx.hasRepeat = false;
					OnRepeatChange.Notify<EventWhen::AFTER>(m_ctx.repeat);
				}
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
}

void Termbox::RenderLoop()
{
	while (!m_ctx.stop && tb_poll_event(&m_ctx.ev) != -1)
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
	return m_ctx;
}

Termbox& Termbox::GetTermbox()
{
	return *m_this;
}
