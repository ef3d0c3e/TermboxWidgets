#ifndef TERMBOXWIDGETS_LISTENER_HPP
#define TERMBOXWIDGETS_LISTENER_HPP

#include "Settings.hpp"

MAKE_CENUM_Q(EventWhen, std::uint8_t,
	BEFORE, 0,
	BEFORE_ONCE, 1,
	AFTER, 2,
	AFTER_ONCE, 3);

template <class... Args>
class EventListener
{
	using Fn = std::function<void(Args...)>;
	std::vector<Fn> m_fnBefore;
	std::vector<Fn> m_fnBeforeOnce;
	std::vector<Fn> m_fnAfter;
	std::vector<Fn> m_fnAfterOnce;
public:
	////////////////////////////////////////////////
	/// \brief Constructor
	////////////////////////////////////////////////
	EventListener() {}

	////////////////////////////////////////////////
	/// \brief Add an event listener
	/// \param fn The callback
	/// \param when When the event should be run
	/// \returns The id of the event listener
	////////////////////////////////////////////////
	std::size_t AddEvent(Fn fn, EventWhen when = EventWhen::AFTER)
	{
		switch (when)
		{
			case EventWhen::BEFORE:
				m_fnBefore.push_back(fn);
				return m_fnBefore.size()-1;
				break;
			case EventWhen::BEFORE_ONCE:
				m_fnBeforeOnce.push_back(fn);
				return m_fnBeforeOnce.size()-1;
				break;
			case EventWhen::AFTER:
				m_fnAfter.push_back(fn);
				return m_fnAfter.size()-1;
				break;
			case EventWhen::AFTER_ONCE:
				m_fnAfterOnce.push_back(fn);
				return m_fnAfterOnce.size()-1;
				break;
		}

		throw Util::Exception("Unhandled EventWhen.");
	}

	////////////////////////////////////////////////
	/// \brief Remove an event listener
	/// \param id The id of the event listener
	/// \param when When the event is ran
	/// \returns true if the event listener was removed
	////////////////////////////////////////////////
	bool RemoveEvent(std::size_t id, EventWhen when)
	{
		// TODO...
		if (when == EventWhen::BEFORE && id < m_fnBefore.size())
		{
			m_fnBefore.erase(m_fnBefore.begin()+id);

			return true;
		}
		else if (when == EventWhen::BEFORE_ONCE && id < m_fnBeforeOnce.size())
		{
			m_fnBeforeOnce.erase(m_fnBeforeOnce.begin()+id);

			return true;
		}
		else if (when == EventWhen::AFTER && id < m_fnAfter.size())
		{
			m_fnAfter.erase(m_fnAfter.begin()+id);

			return true;
		}
		else if (when == EventWhen::AFTER_ONCE && id < m_fnAfterOnce.size())
		{
			m_fnAfterOnce.erase(m_fnAfterOnce.begin()+id);

			return true;
		}

		return false;
	}

	////////////////////////////////////////////////
	/// \brief Get an event listener
	/// \param id The id of the event listener
	/// \param when When the event is ran
	/// \returns The event listener
	/// \note Will throw if there is no event listener with id
	////////////////////////////////////////////////
	EventListener& GetEvent(std::size_t id, EventWhen when)
	{
		if (when == EventWhen::BEFORE)
		{
			if (id >= m_fnBefore.size())
				throw Util::Exception("Tried to get Event with invalid id.");

			return m_fnBefore[id];
		}
		else if (when == EventWhen::AFTER)
		{
			if (id >= m_fnAfter.size())
				throw Util::Exception("Tried to get Event with invalid id.");

			return m_fnAfter[id];
		}
		static const std::string msg("Tried to get Event with invalid id");
		switch (when)
		{
			case EventWhen::BEFORE:
				if (id >= m_fnBefore.size())
					throw Util::Exception(msg);
				return m_fnBefore[id];
				break;
			case EventWhen::BEFORE_ONCE:
				if (id >= m_fnBeforeOnce.size())
					throw Util::Exception(msg);
				return m_fnBeforeOnce[id];
				break;
			case EventWhen::AFTER:
				if (id >= m_fnAfter.size())
					throw Util::Exception(msg);
				return m_fnAfter[id];
				break;
			case EventWhen::AFTER_ONCE:
				if (id >= m_fnAfterOnce.size())
					throw Util::Exception(msg);
				return m_fnAfterOnce[id];
				break;
		}

		throw Util::Exception("Unhandled EventWhen.");
	}
	const EventListener& GetEvent(std::size_t id, EventWhen when) const;

	////////////////////////////////////////////////
	/// \brief Notify the callbacks
	/// \param widget The widget that calls it
	/// \param data Some data about the callback
	/// \tparam When the type of event
	////////////////////////////////////////////////
	template <EventWhen When>
	void Notify(Args... args)
	{
		if constexpr (When == EventWhen::BEFORE)
		{
			for (const auto& fn : m_fnBefore)
				fn(args...);
			for (const auto& fn : m_fnBeforeOnce)
				fn(args...);
			m_fnBeforeOnce.clear();
		}
		else if constexpr (When == EventWhen::AFTER)
		{
			for (const auto& fn : m_fnAfter)
				fn(args...);
			for (const auto& fn : m_fnAfterOnce)
				fn(args...);
			m_fnAfterOnce.clear();
		}
		else
		{
			[]<bool v = false>()
			{
				static_assert(v, "Unhandled EventWhen.");
			}();
		}
	}
};


#endif // TERMBOXWIDGETS_LISTENER_HPP
