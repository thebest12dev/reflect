#pragma once
#include "TypeDefinitions.h"
#include <string>
namespace CinnamonToast {
	class Notification {
		private:
			string title, text;
		public:
			CTOAST_API Notification();
			CTOAST_API Notification(string title, string text);
			CTOAST_API Notification(string text);
			CTOAST_API void SetTitle(string title);
			CTOAST_API void SetText(string text);
			CTOAST_API ~Notification();
			friend class Window;

	};
}

// add more platform support