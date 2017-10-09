/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *  
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SYNTH_H
#define _FLUIDSYNTH_SYNTH_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @file synth.h
 * @brief Embeddable SoundFont synthesizer
 *  
 * You create a new synthesizer with new_fluid_synth() and you destroy
 * if with delete_fluid_synth(). Use the settings structure to specify
 * the synthesizer characteristics. 
 *
 * You have to load a SoundFont in order to hear any sound. For that
 * you use the fluid_synth_sfload() function.
 *
 * You can use the audio driver functions described below to open
 * the audio device and create a background audio thread.
 *  
 * The API for sending MIDI events is probably what you expect:
 * fluid_synth_noteon(), fluid_synth_noteoff(), ...
 */


/*******************************************************************
 API: Poly mono mode 
 */

/* Macros interface to poly/mono mode variables */
enum PolyMonoMode
{
	OMNION_POLY,  /* MIDI mode 0 */
	OMNION_MONO,  /* MIDI mode 1 */
	OMNIOFF_POLY, /* MIDI mode 2 */
	OMNIOFF_MONO, /* MIDI mode 3 */
	MODE_NBR
};

/* bits basic channel infos */
#define MONO 0x01            /* b0, 0: poly on , 1: mono on */
#define OMNI 0x02            /* b1, 0: omni on, 1:omni off */
#define MASKMODE (OMNI|MONO)
#define BASIC_CHANNEL  0x04  /* b2, 1: channel is basic channel */
#define ENABLED 0x08         /* b3, 1: channel is listened */

/* access to mode */
#define GetModeMode(mode) (mode &  MASKMODE)
#define IsModeMono(mode)  (mode & MONO)
#define IsModeBasicChan(mode) (mode &  BASIC_CHANNEL)
#define SetModeBasicChan(mode) (mode |= BASIC_CHANNEL)
#define ResetModeBasicChan(mode) (mode &= ~ BASIC_CHANNEL)
#define IsModeChanEn(mode) (mode &  ENABLED)
#define SetModeChanEn(mode) (mode |= ENABLED)
#define ResetModeChanEn(mode) (mode &= ~ENABLED)

#define FLUID_POLYMONO_WARNING (-2) /* returned by fluid_synth_reset_basic_channels() */
struct _fluid_basic_channel_infos_t
{
	int basicchan;  /* MIDI channel numer to set as basic channel */
	int mode;       /* 0:OmniOn_Poly	 1:OmniOn_Mono	2:OmniOff_Poly	3 OmniOff_Mono */
	int val;        /* Number of monophonic channel (Mode 3) */
};
typedef struct  _fluid_basic_channel_infos_t   fluid_basic_channel_infos_t;

FLUIDSYNTH_API int fluid_synth_get_basic_channels(	fluid_synth_t* synth,
								   fluid_basic_channel_infos_t **basicChannelInfos);
FLUIDSYNTH_API int fluid_synth_reset_basic_channels(fluid_synth_t* synth, int n, 
                                          fluid_basic_channel_infos_t *basicChannelInfos);
FLUIDSYNTH_API int fluid_synth_get_channel_mode(fluid_synth_t* synth, int chan,
												fluid_basic_channel_infos_t  *modeInfos);
FLUIDSYNTH_API int fluid_synth_set_basic_channel(fluid_synth_t* synth, 
												 int basicchan, int mode, int val);
/* Interface to mono legato mode  */
/* n1,n2,n3,.. is a legato passage. n1 is the first note, and n2,n3,n4 are played
 legato with previous note. n2,n3,..make use of previous voices if any.
*/
enum LegatoMode
{
	/* Release previous note (fast release), start a new note */
	RETRIGGER_0, /* mode 0 */

	/* Release previous note (normal release), start a new note */
	RETRIGGER_1, /* mode 1 */
	/* On n2,n3,.. retrigger in attack section using  current value and 
	  shape attack using current dynamic  */ 
	MULTI_RETRIGGER, /* mode 2 */

	/* On n2,n3,.stay in current value section and shape current section
	using current dynamic  */
	SINGLE_TRIGGER_0, /* mode 3 */

	/* On n2,n3,.stay in current value section using current dynamic (don't shape adsr)  */
	SINGLE_TRIGGER_1, /* mode 4 */
	LEGATOMODE_NBR
};

FLUIDSYNTH_API int fluid_synth_set_legato_mode(fluid_synth_t* synth, 
												int chan, int legatomode);
FLUIDSYNTH_API int fluid_synth_get_legato_mode(fluid_synth_t* synth,
												int chan, int  *legatomode);
/* End of API: Poly mono mode */

/* Interface to portamento mode  */
/* Macros interface to portamento mode variable */
enum PortamentoMode
{
	/* Portamento on each note (staccato or legato) */
	EACH_NOTE,		/* mode 0 */
	/* Portamento only on legato note  */ 
	LEGATO_ONLY,	/* mode 1 */
	/* Portamento only on staccato note  */ 
	STACCATO_ONLY,	/* mode 2 */
	PORTAMENTOMODE_NBR
};

FLUIDSYNTH_API int fluid_synth_set_portamento_model(fluid_synth_t* synth,
													int chan, int portamentomode);
FLUIDSYNTH_API int fluid_synth_get_portamento_model(fluid_synth_t* synth,
												int chan, int  * portamentomode);

/* End of API: portamento mode */

/* Interface to breath mode   */
/* breath mode bits infos */
#define BREATH_POLY 0x10     /* b4, 1: default breath poly On */
#define BREATH_MONO 0x20     /* b5, 1: default breath mono On */
#define BREATH_SYNC 0x40     /* b6, 1: BreathSyn On */

/* access to breath mode bits */
#define IsPolyDefaultBreath(breath) (breath &  BREATH_POLY)
#define SetPolyDefaultBreath(breath) (breath |= BREATH_POLY)
#define ResetPolyDefaultBreath(breath) (breath &= ~ BREATH_POLY)
#define IsMonoDefaultBreath(breath) (breath &  BREATH_MONO)
#define SetMonoDefaultBreath(breath) (breath |= BREATH_MONO)
#define ResetMonoDefaultBreath(breath) (breath &= ~ BREATH_MONO)

#define IsBreathSync(breath) (breath &  BREATH_SYNC)
#define SetBreathSync(breath) (breath |= BREATH_SYNC)
#define ResetBreathSync(breath) (breath &= ~ BREATH_SYNC)

FLUIDSYNTH_API int fluid_synth_set_breath_mode(fluid_synth_t* synth, 
												int chan, int breathmode);
FLUIDSYNTH_API int fluid_synth_get_breath_mode(fluid_synth_t* synth,
												int chan, int  *breathmode);

/* End of API: breath mode */

#define FLUID_SYNTH_CHANNEL_INFO_NAME_SIZE   32    /**< Length of channel info name field (including zero terminator) */

/**
 * Channel information structure for fluid_synth_get_channel_info().
 * @since 1.1.1
 */
struct _fluid_synth_channel_info_t
{
  int assigned : 1;     /**< TRUE if a preset is assigned, FALSE otherwise */
  /* Reserved flag bits (at the least 31) */
  int sfont_id;         /**< ID of parent SoundFont */
  int bank;             /**< MIDI bank number (0-16383) */
  int program;          /**< MIDI program number (0-127) */
  char name[FLUID_SYNTH_CHANNEL_INFO_NAME_SIZE];     /**< Channel preset name */
  char reserved[32];    /**< Reserved data for future expansion */
};

FLUIDSYNTH_API fluid_synth_t* new_fluid_synth(fluid_settings_t* settings);
FLUIDSYNTH_API int delete_fluid_synth(fluid_synth_t* synth);
FLUIDSYNTH_API fluid_settings_t* fluid_synth_get_settings(fluid_synth_t* synth);


/* MIDI channel messages */

FLUIDSYNTH_API int fluid_synth_noteon(fluid_synth_t* synth, int chan, int key, int vel);
FLUIDSYNTH_API int fluid_synth_noteoff(fluid_synth_t* synth, int chan, int key);
FLUIDSYNTH_API int fluid_synth_cc(fluid_synth_t* synth, int chan, int ctrl, int val);
FLUIDSYNTH_API int fluid_synth_get_cc(fluid_synth_t* synth, int chan, int ctrl, int* pval);
FLUIDSYNTH_API int fluid_synth_sysex(fluid_synth_t *synth, const char *data, int len,
                                     char *response, int *response_len, int *handled, int dryrun);
FLUIDSYNTH_API int fluid_synth_pitch_bend(fluid_synth_t* synth, int chan, int val);
FLUIDSYNTH_API int fluid_synth_get_pitch_bend(fluid_synth_t* synth, int chan, int* ppitch_bend);
FLUIDSYNTH_API int fluid_synth_pitch_wheel_sens(fluid_synth_t* synth, int chan, int val);
FLUIDSYNTH_API int fluid_synth_get_pitch_wheel_sens(fluid_synth_t* synth, int chan, int* pval);
FLUIDSYNTH_API int fluid_synth_program_change(fluid_synth_t* synth, int chan, int program);
FLUIDSYNTH_API int fluid_synth_channel_pressure(fluid_synth_t* synth, int chan, int val);
FLUIDSYNTH_API int fluid_synth_bank_select(fluid_synth_t* synth, int chan, unsigned int bank);
FLUIDSYNTH_API int fluid_synth_sfont_select(fluid_synth_t* synth, int chan, unsigned int sfont_id);
FLUIDSYNTH_API
int fluid_synth_program_select(fluid_synth_t* synth, int chan, unsigned int sfont_id,
                               unsigned int bank_num, unsigned int preset_num);
FLUIDSYNTH_API int
fluid_synth_program_select_by_sfont_name (fluid_synth_t* synth, int chan,
                                          const char *sfont_name, unsigned int bank_num,
                                          unsigned int preset_num);
FLUIDSYNTH_API 
int fluid_synth_get_program(fluid_synth_t* synth, int chan, unsigned int* sfont_id, 
                            unsigned int* bank_num, unsigned int* preset_num);
FLUIDSYNTH_API int fluid_synth_unset_program (fluid_synth_t *synth, int chan);
FLUIDSYNTH_API int fluid_synth_get_channel_info (fluid_synth_t *synth, int chan,
                                                 fluid_synth_channel_info_t *info);
FLUIDSYNTH_API int fluid_synth_program_reset(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_system_reset(fluid_synth_t* synth);

FLUIDSYNTH_API int fluid_synth_all_notes_off(fluid_synth_t* synth, int chan);
FLUIDSYNTH_API int fluid_synth_all_sounds_off(fluid_synth_t* synth, int chan);

enum fluid_midi_channel_type
{
  CHANNEL_TYPE_MELODIC = 0,
  CHANNEL_TYPE_DRUM = 1
};

int fluid_synth_set_channel_type(fluid_synth_t* synth, int chan, int type);


/* Low level access */
FLUIDSYNTH_API fluid_preset_t* fluid_synth_get_channel_preset(fluid_synth_t* synth, int chan);
FLUIDSYNTH_API int fluid_synth_start(fluid_synth_t* synth, unsigned int id, 
				     fluid_preset_t* preset, int audio_chan, 
				     int midi_chan, int key, int vel);
FLUIDSYNTH_API int fluid_synth_stop(fluid_synth_t* synth, unsigned int id);


/* SoundFont management */

FLUIDSYNTH_API 
int fluid_synth_sfload(fluid_synth_t* synth, const char* filename, int reset_presets);
FLUIDSYNTH_API int fluid_synth_sfreload(fluid_synth_t* synth, unsigned int id);
FLUIDSYNTH_API int fluid_synth_sfunload(fluid_synth_t* synth, unsigned int id, int reset_presets);
FLUIDSYNTH_API int fluid_synth_add_sfont(fluid_synth_t* synth, fluid_sfont_t* sfont);
FLUIDSYNTH_API void fluid_synth_remove_sfont(fluid_synth_t* synth, fluid_sfont_t* sfont);
FLUIDSYNTH_API int fluid_synth_sfcount(fluid_synth_t* synth);
FLUIDSYNTH_API fluid_sfont_t* fluid_synth_get_sfont(fluid_synth_t* synth, unsigned int num);
FLUIDSYNTH_API fluid_sfont_t* fluid_synth_get_sfont_by_id(fluid_synth_t* synth, unsigned int id);
FLUIDSYNTH_API fluid_sfont_t *fluid_synth_get_sfont_by_name (fluid_synth_t* synth,
                                                             const char *name);
FLUIDSYNTH_API int fluid_synth_set_bank_offset(fluid_synth_t* synth, int sfont_id, int offset);
FLUIDSYNTH_API int fluid_synth_get_bank_offset(fluid_synth_t* synth, int sfont_id);


/* Reverb  */

FLUIDSYNTH_API void fluid_synth_set_reverb(fluid_synth_t* synth, double roomsize, 
					   double damping, double width, double level);
FLUIDSYNTH_API void fluid_synth_set_reverb_on(fluid_synth_t* synth, int on);
FLUIDSYNTH_API double fluid_synth_get_reverb_roomsize(fluid_synth_t* synth);
FLUIDSYNTH_API double fluid_synth_get_reverb_damp(fluid_synth_t* synth);
FLUIDSYNTH_API double fluid_synth_get_reverb_level(fluid_synth_t* synth);
FLUIDSYNTH_API double fluid_synth_get_reverb_width(fluid_synth_t* synth);

#define FLUID_REVERB_DEFAULT_ROOMSIZE 0.2f      /**< Default reverb room size */
#define FLUID_REVERB_DEFAULT_DAMP 0.0f          /**< Default reverb damping */
#define FLUID_REVERB_DEFAULT_WIDTH 0.5f         /**< Default reverb width */
#define FLUID_REVERB_DEFAULT_LEVEL 0.9f         /**< Default reverb level */


/* Chorus */

/**
 * Chorus modulation waveform type.
 */
enum fluid_chorus_mod {
  FLUID_CHORUS_MOD_SINE = 0,            /**< Sine wave chorus modulation */
  FLUID_CHORUS_MOD_TRIANGLE = 1         /**< Triangle wave chorus modulation */
};

FLUIDSYNTH_API void fluid_synth_set_chorus(fluid_synth_t* synth, int nr, double level, 
					 double speed, double depth_ms, int type);
FLUIDSYNTH_API void fluid_synth_set_chorus_on(fluid_synth_t* synth, int on);
FLUIDSYNTH_API int fluid_synth_get_chorus_nr(fluid_synth_t* synth);
FLUIDSYNTH_API double fluid_synth_get_chorus_level(fluid_synth_t* synth);
FLUIDSYNTH_API double fluid_synth_get_chorus_speed_Hz(fluid_synth_t* synth);
FLUIDSYNTH_API double fluid_synth_get_chorus_depth_ms(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_get_chorus_type(fluid_synth_t* synth); /* see fluid_chorus_mod */

#define FLUID_CHORUS_DEFAULT_N 3                                /**< Default chorus voice count */
#define FLUID_CHORUS_DEFAULT_LEVEL 2.0f                         /**< Default chorus level */
#define FLUID_CHORUS_DEFAULT_SPEED 0.3f                         /**< Default chorus speed */
#define FLUID_CHORUS_DEFAULT_DEPTH 8.0f                         /**< Default chorus depth */
#define FLUID_CHORUS_DEFAULT_TYPE FLUID_CHORUS_MOD_SINE         /**< Default chorus waveform type */


/* Audio and MIDI channels */

FLUIDSYNTH_API int fluid_synth_count_midi_channels(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_count_audio_channels(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_count_audio_groups(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_count_effects_channels(fluid_synth_t* synth);


/* Synthesis parameters */

FLUIDSYNTH_API void fluid_synth_set_sample_rate(fluid_synth_t* synth, float sample_rate);
FLUIDSYNTH_API void fluid_synth_set_gain(fluid_synth_t* synth, float gain);
FLUIDSYNTH_API float fluid_synth_get_gain(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_set_polyphony(fluid_synth_t* synth, int polyphony);
FLUIDSYNTH_API int fluid_synth_get_polyphony(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_get_active_voice_count(fluid_synth_t* synth);
FLUIDSYNTH_API int fluid_synth_get_internal_bufsize(fluid_synth_t* synth);

FLUIDSYNTH_API 
int fluid_synth_set_interp_method(fluid_synth_t* synth, int chan, int interp_method);

/**
 * Synthesis interpolation method.
 */
enum fluid_interp {
  FLUID_INTERP_NONE = 0,        /**< No interpolation: Fastest, but questionable audio quality */
  FLUID_INTERP_LINEAR = 1,      /**< Straight-line interpolation: A bit slower, reasonable audio quality */
  FLUID_INTERP_4THORDER = 4,    /**< Fourth-order interpolation, good quality, the default */
  FLUID_INTERP_7THORDER = 7     /**< Seventh-order interpolation */
};

#define FLUID_INTERP_DEFAULT    FLUID_INTERP_4THORDER   /**< Default interpolation method from #fluid_interp. */
#define FLUID_INTERP_HIGHEST    FLUID_INTERP_7THORDER   /**< Highest interpolation method from #fluid_interp. */


/* Generator interface */

FLUIDSYNTH_API 
int fluid_synth_set_gen(fluid_synth_t* synth, int chan, int param, float value);
FLUIDSYNTH_API int fluid_synth_set_gen2 (fluid_synth_t* synth, int chan,
                                         int param, float value,
                                         int absolute, int normalized);
FLUIDSYNTH_API float fluid_synth_get_gen(fluid_synth_t* synth, int chan, int param);


/* Tuning */

FLUIDSYNTH_API 
int fluid_synth_create_key_tuning(fluid_synth_t* synth, int bank, int prog,
				  const char* name, const double* pitch);
FLUIDSYNTH_API
int fluid_synth_activate_key_tuning(fluid_synth_t* synth, int bank, int prog,
                                    const char* name, const double* pitch, int apply);
FLUIDSYNTH_API 
int fluid_synth_create_octave_tuning(fluid_synth_t* synth, int bank, int prog,
                                     const char* name, const double* pitch);
FLUIDSYNTH_API
int fluid_synth_activate_octave_tuning(fluid_synth_t* synth, int bank, int prog,
                                       const char* name, const double* pitch, int apply);
FLUIDSYNTH_API 
int fluid_synth_tune_notes(fluid_synth_t* synth, int bank, int prog,
			   int len, const int *keys, const double* pitch, int apply);
FLUIDSYNTH_API 
int fluid_synth_select_tuning(fluid_synth_t* synth, int chan, int bank, int prog);
FLUIDSYNTH_API
int fluid_synth_activate_tuning(fluid_synth_t* synth, int chan, int bank, int prog,
                                int apply);
FLUIDSYNTH_API int fluid_synth_reset_tuning(fluid_synth_t* synth, int chan);
FLUIDSYNTH_API
int fluid_synth_deactivate_tuning(fluid_synth_t* synth, int chan, int apply);
FLUIDSYNTH_API void fluid_synth_tuning_iteration_start(fluid_synth_t* synth);
FLUIDSYNTH_API 
int fluid_synth_tuning_iteration_next(fluid_synth_t* synth, int* bank, int* prog);
FLUIDSYNTH_API int fluid_synth_tuning_dump(fluid_synth_t* synth, int bank, int prog, 
					   char* name, int len, double* pitch);

/* Misc */

FLUIDSYNTH_API double fluid_synth_get_cpu_load(fluid_synth_t* synth);
FLUIDSYNTH_API char* fluid_synth_error(fluid_synth_t* synth);


/*
 * Synthesizer plugin
 *
 * To create a synthesizer plugin, create the synthesizer as
 * explained above. Once the synthesizer is created you can call
 * any of the functions below to get the audio. 
 */

FLUIDSYNTH_API int fluid_synth_write_s16(fluid_synth_t* synth, int len, 
				       void* lout, int loff, int lincr, 
				       void* rout, int roff, int rincr);
FLUIDSYNTH_API int fluid_synth_write_float(fluid_synth_t* synth, int len, 
					 void* lout, int loff, int lincr, 
					 void* rout, int roff, int rincr);
FLUIDSYNTH_API int fluid_synth_nwrite_float(fluid_synth_t* synth, int len, 
					  float** left, float** right, 
					  float** fx_left, float** fx_right);
FLUIDSYNTH_API int fluid_synth_process(fluid_synth_t* synth, int len,
				     int nin, float** in, 
				     int nout, float** out);

/**
 * Type definition of the synthesizer's audio callback function.
 * @param synth FluidSynth instance
 * @param len Count of audio frames to synthesize
 * @param out1 Array to store left channel of audio to
 * @param loff Offset index in 'out1' for first sample
 * @param lincr Increment between samples stored to 'out1'
 * @param out2 Array to store right channel of audio to
 * @param roff Offset index in 'out2' for first sample
 * @param rincr Increment between samples stored to 'out2'
 */
typedef int (*fluid_audio_callback_t)(fluid_synth_t* synth, int len, 
				     void* out1, int loff, int lincr, 
				     void* out2, int roff, int rincr);

/* Synthesizer's interface to handle SoundFont loaders */

FLUIDSYNTH_API void fluid_synth_add_sfloader(fluid_synth_t* synth, fluid_sfloader_t* loader);
FLUIDSYNTH_API fluid_voice_t* fluid_synth_alloc_voice(fluid_synth_t* synth, 
													  fluid_inst_zone_t* inst_zone,
                                                      int channum, int key, int vel);
FLUIDSYNTH_API void fluid_synth_start_voice(fluid_synth_t* synth, fluid_voice_t* voice);
FLUIDSYNTH_API void fluid_synth_get_voicelist(fluid_synth_t* synth,
                                              fluid_voice_t* buf[], int bufsize, int ID);
FLUIDSYNTH_API int fluid_synth_handle_midi_event(void* data, fluid_midi_event_t* event);
FLUIDSYNTH_API void fluid_synth_set_midi_router(fluid_synth_t* synth,
                                                fluid_midi_router_t* router);

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_SYNTH_H */
