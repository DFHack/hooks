#pragma once

namespace SDL {
    // so we don't need to actually include SDL_events.h
    union Event;
}

// called early from DF's main()
void hooks_init(void);

// called before exiting (if possible)
void hooks_shutdown(void);

// called at the end of each simulation tick (or more often) from the sim thread
void hooks_tick(void);

// called before rendering a frame from the rendering thread
void hooks_prerender(void);

// called for each SDL event, if true is returned, then the event has been
// consumed and further processing shouldn't happen
bool hooks_sdl_event(SDL::Event* event);
