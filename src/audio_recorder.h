/*! \file
 * \author   Lorenzo Miniero <lorenzo@meetecho.com>
 * \author Sergey Radionov <rsatom@gmail.com>
 * \copyright GNU General Public License v3
 */

#pragma once

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "janus/mutex.h"
#include "janus/refcount.h"
#include "janus/rtp.h"
}

namespace ptt_audioroom
{

/*! \brief Structure that represents a recorder */
typedef struct audio_recorder {
	/*! \brief Absolute path to the directory where the recorder file is stored */
	char *dir;
	/*! \brief Filename of this recorder file */
	char *filename;
	/*! \brief Recording file */
	FILE *file;
	/*! \brief Codec the packets to record are encoded in ("opus") */
	char *codec;
	/*! \brief List of RTP extensions (as a hashtable, indexed by ID) in this recording */
	GHashTable *extensions;
	/*! \brief When the recording file has been created and started */
	gint64 created, started;
	/*! \brief In case RED is used for Opus, its payload types */
	int opusred_pt;
	/*! \brief Whether the info header for this recorder instance has already been written or not */
	gboolean header;
	/*! \brief Whether this recorder instance can be used for writing or not */
	gboolean writable;
	/*! \brief RTP switching context for rewriting RTP headers */
	janus_rtp_switching_context context;
	/*! \brief Atomic flag to check if this instance has been destroyed */
	gint destroyed;
	/*! \brief Reference counter for this instance */
	janus_refcount ref;
	bool write_failed;
} audio_recorder;

/*! \brief Create a new recorder
 * \note If no target directory is provided, the current directory will be used. If no filename
 * is passed, a random filename will be used.
 * @param[in] dir Path of the directory to save the recording into (will try to create it if it doesn't exist)
 * @param[in] codec Codec the packets to record are encoded in ("opus")
 * @param[in] filename Filename to use for the recording
 * @returns A valid audio_recorder instance in case of success, NULL otherwise */
audio_recorder *audio_recorder_create(const char *dir, const char *codec, const char *filename);
/*! \brief Pause recording packets
 * \note This is to allow pause and resume recorder functionality.
 * @param[in] recorder The audio_recorder to pause
 * @returns 0 in case of success, a negative integer otherwise */
int audio_recorder_add_extmap(audio_recorder *recorder, int id, const char *extmap);
/*! \brief Mark this recording as using RED for audio
 * \note This will only be possible BEFORE the first frame is written, as it needs to
 * be reflected in the .mjr header: doing this after that will return an error.
 * @param[in] recorder The audio_recorder instance to configure
 * @param[in] red_pt Payload type of RED
 * @returns 0 in case of success, a negative integer otherwise */
int audio_recorder_opusred(audio_recorder *recorder, int red_pt);
/*! \brief Save an RTP frame in the recorder
 * @param[in] recorder The audio_recorder instance to save the frame to
 * @param[in] buffer The frame data to save
 * @param[in] length The frame data length
 * @returns 0 in case of success, a negative integer otherwise */
int audio_recorder_save_frame(audio_recorder *recorder, char *buffer, uint length);
/*! \brief Close the recorder
 * @param[in] recorder The audio_recorder instance to close
 * @returns 0 in case of success, a negative integer otherwise */
int audio_recorder_close(audio_recorder *recorder);
/*! \brief Destroy the recorder instance
 * @param[in] recorder The audio_recorder instance to destroy */
void audio_recorder_destroy(audio_recorder *recorder);

}
