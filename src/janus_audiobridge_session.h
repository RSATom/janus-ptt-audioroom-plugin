#pragma once

#include <glib.h>

extern "C" {
#include "janus/refcount.h"
#include "plugin.h"
}


namespace ptt_audioroom
{
extern GHashTable* sessions;
extern janus_mutex sessions_mutex;

struct janus_audiobridge_participant;

struct janus_audiobridge_session {
	janus_plugin_session *handle;
	gint64 sdp_sessid;
	gint64 sdp_version;
	// once assinged will be alive (and never changed) while janus_audiobridge_session is alive
	janus_audiobridge_participant* participant;
	volatile gint started;
	gint hangingup;
	gint destroyed;
	janus_refcount ref;
};

void janus_audiobridge_session_destroy(janus_audiobridge_session *session);
void janus_audiobridge_session_free(const janus_refcount *session_ref);

janus_audiobridge_session* janus_audiobridge_lookup_session(janus_plugin_session *handle);

}
