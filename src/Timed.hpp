#ifndef TERMBOXWIDGETS_TIMED_HPP
#define TERMBOXWIDGETS_TIMED_HPP

#include "Settings.hpp"
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
class Termbox;
class Widget;

class TimedEvent
{
	////////////////////////////////////////////////
	/// \brief TimedEvent class that represent a timed event
	/// timed event are run at a certain interval, a certain number of times
	////////////////////////////////////////////////
	std::size_t m_interval;
	std::size_t m_repeat;

	std::size_t m_repeated;
	std::size_t m_lastExecution;


	std::function<void(const TimedEvent&, Widget&, Termbox&)> m_event;
public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param event The event that will be run
	/// \param interval The minimum interval between each execution (in ms)
	/// \param repeat The number of execution before stopping (set to 0 for infinite execution)
	////////////////////////////////////////////////
	TimedEvent(decltype(m_event) event, std::size_t interval, std::size_t repeat = 0);

	////////////////////////////////////////////////
	/// \brief Set the repetition interval of the event
	/// \param interval The repetition interval
	////////////////////////////////////////////////
	void SetInterval(std::size_t interval);

	////////////////////////////////////////////////
	/// \brief Get the repetition interval of the event
	/// \returns The repetition interval
	////////////////////////////////////////////////
	std::size_t GetInterval() const;

	////////////////////////////////////////////////
	/// \brief Set the maximum number of executions
	/// \param repeat The maximum number of executions
	/// \note Pass ```0``` for infinite execution
	////////////////////////////////////////////////
	void SetRepeat(std::size_t repeat);

	////////////////////////////////////////////////
	/// \brief Get the maximum number of executions
	/// \returns The maximum number of executions
	////////////////////////////////////////////////
	std::size_t GetRepeat() const;

	////////////////////////////////////////////////
	/// \brief Get the number of executions
	/// \returns The number of times the event has been repeated
	////////////////////////////////////////////////
	std::size_t GetRepeated() const;

	////////////////////////////////////////////////
	/// \brief Get the last execution
	/// \returns The last execution
	////////////////////////////////////////////////
	std::size_t GetLastExecution() const;

	////////////////////////////////////////////////
	/// \brief Set the last execution
	/// \param lastExec The last execution
	////////////////////////////////////////////////
	void SetLastExecution(std::size_t lastExec);

	////////////////////////////////////////////////
	/// \brief Run the event
	/// \param now The current time
	/// \param tb The termbox context
	/// \returns true If the event was run
	////////////////////////////////////////////////
	bool RunEvent(const std::chrono::microseconds& now, Widget& w, Termbox& tb);
};

class TimedManager
{
	////////////////////////////////////////////////
	/// \brief TimedManager manages TimedEvent
	////////////////////////////////////////////////
	
	long int m_resolution;
public:
	struct Event
	{
		////////////////////////////////////////////////
		/// \brief Event represents a single timed event
		////////////////////////////////////////////////
		TimedEvent event;
		bool active;

		////////////////////////////////////////////////
		/// \brief Stop an event
		////////////////////////////////////////////////
		void Stop()
		{
			active = false;
		}

		////////////////////////////////////////////////
		/// \brief Start an event
		////////////////////////////////////////////////
		void Start()
		{
			active = true;
		}

		////////////////////////////////////////////////
		/// \brief Toggle an event
		////////////////////////////////////////////////
		void Toggle()
		{
			active = !active;
		}
	};
private:
	std::vector<Event> m_timed;

	std::chrono::high_resolution_clock m_clock;
	std::chrono::microseconds m_lastEpoch;
	bool m_run;
	std::mutex m_mtx;
	std::condition_variable m_cv;
	std::thread* m_thread;
public:
	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param resolution The clock's resolution (in µs)
	////////////////////////////////////////////////
	TimedManager(long int resolution = Settings::event_clock_resolution);

	////////////////////////////////////////////////
	/// \brief Add an event
	/// \param ev The event
	/// \param startActive If set to false, the event will not start immediately
	/// \returns The id of the event
	////////////////////////////////////////////////
	std::size_t AddEvent(TimedEvent&& ev, bool startActive = true);

	////////////////////////////////////////////////
	/// \brief Remove an event
	/// \param id The id of the event
	/// \returns true if the event was removed
	////////////////////////////////////////////////
	bool RemoveEvent(std::size_t id);

	////////////////////////////////////////////////
	/// \brief Get an event
	/// \param id The id of the event
	/// \returns The event
	/// \note Will throw if there is no event with id
	////////////////////////////////////////////////
	Event& GetEvent(std::size_t id);
	const Event& GetEvent(std::size_t id) const;

	////////////////////////////////////////////////
	/// \brief Start the clock
	////////////////////////////////////////////////
	void StartEvents(Widget& w);

	////////////////////////////////////////////////
	/// \brief Stop the clock
	/// \param wait If true, will wait for all timed event to finish
	////////////////////////////////////////////////
	void StopEvents(bool wait = true);

	////////////////////////////////////////////////
	/// \brief Get wether or not the events are ticking
	/// \returns True if they are running
	////////////////////////////////////////////////
	bool EventsRunning() const;
};

#endif // TERMBOXWIDGETS_TIMED_HPP
