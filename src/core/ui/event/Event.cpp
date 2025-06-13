#include "Event.h"


namespace reflect {
Event::Event() : component(nullptr), eventId(0) { initializeObject() }