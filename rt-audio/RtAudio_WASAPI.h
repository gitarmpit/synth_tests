#ifndef _RTAUDIO_WASAPI_H
#define _RTAUDIO_WASAPI_H

#include "RtAudio.h"
#include "RtApi.h"


struct IMMDeviceEnumerator;

class RtApiWasapi : public RtApi
{
public:
  RtApiWasapi();
  virtual ~RtApiWasapi();

  RtAudio::Api getCurrentApi( void ) { return RtAudio::WINDOWS_WASAPI; }
  unsigned int getDeviceCount( void );
  RtAudio::DeviceInfo getDeviceInfo( unsigned int device );
  unsigned int getDefaultOutputDevice( void );
  unsigned int getDefaultInputDevice( void );
  void closeStream( void );
  void startStream( void );
  void stopStream( void );
  void abortStream( void );

private:
  bool coInitialized_;
  IMMDeviceEnumerator* deviceEnumerator_;

  bool probeDeviceOpen( unsigned int device, StreamMode mode, unsigned int channels,
                        unsigned int firstChannel, unsigned int sampleRate,
                        RtAudioFormat format, unsigned int* bufferSize,
                        RtAudio::StreamOptions* options );

  static DWORD WINAPI runWasapiThread( void* wasapiPtr );
  static DWORD WINAPI stopWasapiThread( void* wasapiPtr );
  static DWORD WINAPI abortWasapiThread( void* wasapiPtr );
  void wasapiThread();
};

#endif
