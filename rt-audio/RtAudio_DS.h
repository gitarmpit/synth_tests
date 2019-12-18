#ifndef _RTAUDIO_DS_H
#define _RTAUDIO_DS_H

#include "RtAudio.h"
#include "RtApi.h"


class RtApiDs: public RtApi
{
public:

  RtApiDs();
  ~RtApiDs();
  RtAudio::Api getCurrentApi( void ) { return RtAudio::WINDOWS_DS; }
  unsigned int getDeviceCount( void );
  unsigned int getDefaultOutputDevice( void );
  unsigned int getDefaultInputDevice( void );
  RtAudio::DeviceInfo getDeviceInfo( unsigned int device );
  void closeStream( void );
  void startStream( void );
  void stopStream( void );
  void abortStream( void );

  void callbackEvent( void );

  private:

  bool coInitialized_;
  bool buffersRolling;
  long duplexPrerollBytes;
  std::vector<struct DsDevice> dsDevices;
  bool probeDeviceOpen( unsigned int device, StreamMode mode, unsigned int channels, 
                        unsigned int firstChannel, unsigned int sampleRate,
                        RtAudioFormat format, unsigned int *bufferSize,
                        RtAudio::StreamOptions *options );
};



#endif
