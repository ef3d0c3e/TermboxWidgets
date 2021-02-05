#include "Timed.hpp"
#include "Termbox.hpp"

TimedEvent::TimedEvent(decltype(m_event) event, std::size_t interval, std::size_t repeat):
	m_interval(interval),
	m_repeat(repeat),
	m_repeated(0),
	m_lastExecution(0),
	m_event(event)
{
	
}

void TimedEvent::SetInterval(std::size_t interval)
{
	m_interval = interval;
}

std::size_t TimedEvent::GetInterval() const
{
	return m_interval;
}

void TimedEvent::SetRepeat(std::size_t repeat)
{
	m_repeat = repeat;
}

std::size_t TimedEvent::GetRepeat() const
{
	return m_repeat;
}

std::size_t TimedEvent::GetRepeated() const
{
	return m_repeated;
}

std::size_t TimedEvent::GetLastExecution() const
{
	return m_lastExecution;
}

void TimedEvent::SetLastExecution(std::size_t lastExec)
{
	m_lastExecution = lastExec;
}


bool TimedEvent::RunEvent(const std::chrono::microseconds& now, Widget& w, Termbox& tb)
{
	if (m_repeat != 0 && m_repeated >= m_repeat)
		return false;
	if (now.count() - m_lastExecution < 1000*m_interval)

		return false;

	m_event(*this, w, tb);
	m_lastExecution = now.count();
	++m_repeated;

	return true;
}

TimedManager::TimedManager(long int resolution):
	m_resolution(resolution),
	m_lastEpoch(std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch())),
	m_run(false)
{

}

std::size_t TimedManager::AddEvent(TimedEvent&& ev, bool startActive)
{
	m_timed.push_back({std::move(ev), startActive});

	return m_timed.size()-1;
}

bool TimedManager::RemoveEvent(std::size_t id)
{
	if (id >= m_timed.size())
		return false;

	m_timed.erase(m_timed.begin()+id);

	return true;
}

TimedManager::Event& TimedManager::GetEvent(std::size_t id)
{
	if (id >= m_timed.size())
		throw Util::Exception("Tried to get TimedEvent with invalid id.");

	return m_timed[id];
}

const TimedManager::Event& TimedManager::GetEvent(std::size_t id) const
{
	if (id >= m_timed.size())
		throw Util::Exception("Tried to get TimedEvent with invalid id.");

	return m_timed[id];
}

void TimedManager::StartEvents(Widget& w)
{
	m_run = true;
	m_thread = new std::thread([this, &w] {
		while (m_run)
		{
			std::unique_lock<decltype(m_mtx)> l(m_mtx);
			m_cv.wait_for(l, std::chrono::microseconds(m_resolution));

			Termbox::GetTermbox().GetContext().lock = true;

			for (Event& ev : m_timed)
			{
				if (!ev.active)
					continue;

				ev.event.RunEvent(m_lastEpoch, w, Termbox::GetTermbox());
			}
			m_lastEpoch = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch());

			Termbox::GetTermbox().GetContext().lock = false;
		}
	});
}

void TimedManager::StopEvents(bool wait)
{
	m_run = false;
	m_cv.notify_all();
	if (wait)
	{
		if (m_thread->joinable())
			m_thread->join();
		else
			throw Util::Exception("Could not join timed event thread");
	}
}

bool TimedManager::EventsRunning() const
{
	return m_run;
}
