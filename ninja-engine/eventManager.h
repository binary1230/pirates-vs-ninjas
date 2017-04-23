#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

class EventManager {
	DECLARE_SINGLETON_CLASS(EventManager)

	public:
		virtual ~EventManager();

		bool Init();
		void Shutdown();

		// triggerable events
		void OnLoad();
		void OnUnLoad();
		void OnEvent(const char* eventName);
		void OnInputEndedPlayback();
		void OnTextboxDone();

	private:
};

#define EVENTS (EventManager::GetInstance())

#endif // EVENTMANAGER_H
