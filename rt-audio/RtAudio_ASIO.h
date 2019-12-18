#ifndef _RtAudio_ASIO_H
#define _RtAudio_ASIO_H

#include "RtAudio.h"
#include "RtApi.h"

class RtApiAsio: public RtApi
{
public:

  RtApiAsio();
  virtual ~RtApiAsio();
  RtAudio::Api getCurrentApi( void ) { return RtAudio::WINDOWS_ASIO; }
  unsigned int getDeviceCount( void );
  RtAudio::DeviceInfo getDeviceInfo( unsigned int device );
  void closeStream( void );
  void startStream( void );
  void stopStream( void );
  void abortStream( void );

  // This function is intended for internal use only.  It must be
  // public because it is called by the internal callback handler,
  // which is not a member of RtAudio.  External use of this function
  // will most likely produce highly undesireable results!
  bool callbackEvent( long bufferIndex );

  private:

  std::vector<RtAudio::DeviceInfo> devices_;
  void saveDeviceInfo( void );
  bool coInitialized_;
  bool probeDeviceOpen( unsigned int device, StreamMode mode, unsigned int channels, 
                        unsigned int firstChannel, unsigned int sampleRate,
                        RtAudioFormat format, unsigned int *bufferSize,
                        RtAudio::StreamOptions *options );
};


#endif
