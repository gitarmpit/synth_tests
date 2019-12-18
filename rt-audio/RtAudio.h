#ifndef _RTAUDIO_H
#define _RTAUDIO_H

#include "RtAudio_Common.h"
#include "RtAudio_Error.h"

class RtApi;

class RTAUDIO_DLL_PUBLIC RtAudio
{
 public:

  //! Audio API specifier arguments.
  enum Api {
    UNSPECIFIED,    /*!< Search for a working compiled API. */
    WINDOWS_WASAPI, /*!< The Microsoft WASAPI API. */
    WINDOWS_ASIO,   /*!< The Steinberg Audio Stream I/O API. */
    WINDOWS_DS,     /*!< The Microsoft DirectSound API. */
    NUM_APIS        /*!< Number of values in this enum. */
  };

  //! The public device information structure for returning queried values.
  struct DeviceInfo {
    bool probed;                  /*!< true if the device capabilities were successfully probed. */
    std::string name;             /*!< Character string device identifier. */
    unsigned int outputChannels;  /*!< Maximum output channels supported by device. */
    unsigned int inputChannels;   /*!< Maximum input channels supported by device. */
    unsigned int duplexChannels;  /*!< Maximum simultaneous input/output channels supported by device. */
    bool isDefaultOutput;         /*!< true if this is the default output device. */
    bool isDefaultInput;          /*!< true if this is the default input device. */
    std::vector<unsigned int> sampleRates; /*!< Supported sample rates (queried from list of standard rates). */
    unsigned int preferredSampleRate; /*!< Preferred sample rate, e.g. for WASAPI the system sample rate. */
    RtAudioFormat nativeFormats;  /*!< Bit mask of supported data formats. */

    // Default constructor.
    DeviceInfo()
      :probed(false), outputChannels(0), inputChannels(0), duplexChannels(0),
       isDefaultOutput(false), isDefaultInput(false), preferredSampleRate(0), nativeFormats(0) {}
  };

  struct StreamParameters {
    unsigned int deviceId;     /*!< Device index (0 to getDeviceCount() - 1). */
    unsigned int nChannels;    /*!< Number of channels. */
    unsigned int firstChannel; /*!< First channel index on device (default = 0). */

    // Default constructor.
    StreamParameters()
      : deviceId(0), nChannels(0), firstChannel(0) {}
  };

  struct StreamOptions {
    RtAudioStreamFlags flags;      /*!< A bit-mask of stream flags (RTAUDIO_NONINTERLEAVED, RTAUDIO_MINIMIZE_LATENCY, RTAUDIO_HOG_DEVICE, RTAUDIO_ALSA_USE_DEFAULT). */
    unsigned int numberOfBuffers;  /*!< Number of stream buffers. */
    std::string streamName;        /*!< A stream name (currently used only in Jack). */
    int priority;                  /*!< Scheduling priority of callback thread (only used with flag RTAUDIO_SCHEDULE_REALTIME). */

    // Default constructor.
    StreamOptions()
    : flags(0), numberOfBuffers(0), priority(0) {}
  };

  static std::string getVersion( void );

  static void getCompiledApi( std::vector<RtAudio::Api> &apis );

  static std::string getApiName( RtAudio::Api api );

  static std::string getApiDisplayName( RtAudio::Api api );

  static RtAudio::Api getCompiledApiByName( const std::string &name );

  RtAudio( RtAudio::Api api=UNSPECIFIED );
  ~RtAudio();

  RtAudio::Api getCurrentApi( void );
  unsigned int getDeviceCount( void );

  RtAudio::DeviceInfo getDeviceInfo( unsigned int device );
 
  unsigned int getDefaultOutputDevice( void );
  unsigned int getDefaultInputDevice( void );
  void openStream( RtAudio::StreamParameters *outputParameters,
                   RtAudio::StreamParameters *inputParameters,
                   RtAudioFormat format, unsigned int sampleRate,
                   unsigned int *bufferFrames, RtAudioCallback callback,
                   void *userData = NULL, RtAudio::StreamOptions *options = NULL, RtAudioErrorCallback errorCallback = NULL );

  void closeStream( void );
  void startStream( void );
  void stopStream( void );
  void abortStream( void );
  bool isStreamOpen( void ) const;
  bool isStreamRunning( void ) const;
  double getStreamTime( void );
  void setStreamTime( double time );
  long getStreamLatency( void );
  unsigned int getStreamSampleRate( void );
  void showWarnings( bool value = true );

 protected:

  void openRtApi( RtAudio::Api api );
  RtApi *rtapi_;
};





#endif
