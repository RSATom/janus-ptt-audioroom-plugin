#pragma once

#include <glib.h>

extern "C" {
#include "plugin.h"
#include "janus/config.h"
#include "janus/mutex.h"
}

#include "janus_audiobridge_session.h"
#include "janus_audiobridge_room.h"


namespace ptt_audioroom
{

extern janus_plugin janus_audiobridge_plugin;

extern janus_callbacks* gateway;

/* Static configuration instance */
extern janus_config* config;
extern const char *config_folder;
extern janus_mutex config_mutex;

extern gint initialized, stopping;
extern gboolean notify_events;
extern gboolean ipv6_disabled;

extern GAsyncQueue *messages;

extern GHashTable* rooms;
extern janus_mutex rooms_mutex;
extern char* admin_key;
extern gboolean lock_rtpfwd;

void* janus_audiobridge_sender_thread(void* data);

int janus_audiobridge_create_udp_socket_if_needed(janus_audiobridge_room *audiobridge);
guint32 janus_audiobridge_rtp_forwarder_add_helper(janus_audiobridge_room *room,
		const gchar *host, uint16_t port, uint32_t ssrc, int pt,
		int srtp_suite, const char *srtp_crypto,
		gboolean always_on, guint32 stream_id);
}
