#ifndef _RTAPI_H
#define _RTAPI_H

#include "RtAudio_Common.h"
#include "RtAudio_Error.h"
#include "RtAudio.h"

#if defined( HAVE_GETTIMEOFDAY )
  #include <sys/time.h>
#endif

#include <sstream>

class RTAUDIO_DLL_PUBLIC RtApi
{
public:

  RtApi();
  virtual ~RtApi();
  
  virtual RtAudio::Api         getCurrentApi( void ) = 0;
  virtual unsigned int         getDeviceCount( void ) = 0;
  virtual RtAudio::DeviceInfo  getDeviceInfo( unsigned int device ) = 0;
  virtual unsigned int         getDefaultInputDevice( void );
  virtual unsigned int         getDefaultOutputDevice( void );

  void openStream( RtAudio::StreamParameters *outputParameters,
                   RtAudio::StreamParameters *inputParameters,
                   RtAudioFormat format, unsigned int sampleRate,
                   unsigned int *bufferFrames, RtAudioCallback callback,
                   void *userData, RtAudio::StreamOptions *options,
                   RtAudioErrorCallback errorCallback );

  virtual void closeStream( void );
  virtual void startStream( void ) = 0;
  virtual void stopStream( void ) = 0;
  virtual void abortStream( void ) = 0;
  
  long            getStreamLatency( void );
  unsigned int    getStreamSampleRate( void );
  virtual double  getStreamTime( void );
  virtual void   setStreamTime( double time );

  bool isStreamOpen( void ) const { return stream_.state != STREAM_CLOSED; }
  bool isStreamRunning( void ) const { return stream_.state == STREAM_RUNNING; }
  void showWarnings( bool value ) { showWarnings_ = value; }

protected:

  static const unsigned int MAX_SAMPLE_RATES;
  static const unsigned int SAMPLE_RATES[];

  enum { FAILURE, SUCCESS };

  enum StreamState {
    STREAM_STOPPED,
    STREAM_STOPPING,
    STREAM_RUNNING,
    STREAM_CLOSED = -50
  };

  enum StreamMode {
    OUTPUT,
    INPUT,
    DUPLEX,
    UNINITIALIZED = -75
  };

  struct ConvertInfo {
    int channels;
    int inJump, outJump;
    RtAudioFormat inFormat, outFormat;
    std::vector<int> inOffset;
    std::vector<int> outOffset;
  };

  struct RtApiStream {
    unsigned int device[2];    // Playback and record, respectively.
    void *apiHandle;           // void pointer for API specific stream handle information
    StreamMode mode;           // OUTPUT, INPUT, or DUPLEX.
    StreamState state;         // STOPPED, RUNNING, or CLOSED
    char *userBuffer[2];       // Playback and record, respectively.
    char *deviceBuffer;
    bool doConvertBuffer[2];   // Playback and record, respectively.
    bool userInterleaved;
    bool deviceInterleaved[2]; // Playback and record, respectively.
    bool doByteSwap[2];        // Playback and record, respectively.
    unsigned int sampleRate;
    unsigned int bufferSize;
    unsigned int nBuffers;
    unsigned int nUserChannels[2];    // Playback and record, respectively.
    unsigned int nDeviceChannels[2];  // Playback and record channels, respectively.
    unsigned int channelOffset[2];    // Playback and record, respectively.
    unsigned long latency[2];         // Playback and record, respectively.
    RtAudioFormat userFormat;
    RtAudioFormat deviceFormat[2];    // Playback and record, respectively.
    StreamMutex mutex;
    CallbackInfo callbackInfo;
    ConvertInfo convertInfo[2];
    double streamTime;         // Number of elapsed seconds since the stream started.

#if defined(HAVE_GETTIMEOFDAY)
    struct timeval lastTickTimestamp;
#endif

    RtApiStream()
      :apiHandle(0), deviceBuffer(0) { device[0] = 11111; device[1] = 11111; }
  };

  typedef S24 Int24;
  typedef signed short Int16;
  typedef signed int Int32;
  typedef float Float32;
  typedef double Float64;

  std::ostringstream errorStream_;
  std::string errorText_;
  bool showWarnings_;
  RtApiStream stream_;
  bool firstErrorOccurred_;

  /*!
    Protected, api-specific method that attempts to open a device
    with the given parameters.  This function MUST be implemented by
    all subclasses.  If an error is encountered during the probe, a
    "warning" message is reported and FAILURE is returned. A
    successful probe is indicated by a return value of SUCCESS.
  */
  virtual bool probeDeviceOpen( unsigned int device, StreamMode mode, unsigned int channels, 
                                unsigned int firstChannel, unsigned int sampleRate,
                                RtAudioFormat format, unsigned int *bufferSize,
                                RtAudio::StreamOptions *options );

  //! A protected function used to increment the stream time.
  void tickStreamTime( void );

  //! Protected common method to clear an RtApiStream structure.
  void clearStreamInfo();
  void verifyStream( void );
  void error( RtAudioError::Type type );
  void convertBuffer( char *outBuffer, char *inBuffer, ConvertInfo &info );
  void byteSwapBuffer( char *buffer, unsigned int samples, RtAudioFormat format );
  unsigned int formatBytes( RtAudioFormat format );
  void setConvertInfo( StreamMode mode, unsigned int firstChannel );
};



#endif
