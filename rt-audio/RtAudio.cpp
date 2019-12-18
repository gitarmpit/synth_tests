#include "RtAudio.h" 
#include "RtApi.h"
#if defined(__WINDOWS_ASIO__)
#include "RtAudio_ASIO.h"
#endif
#if defined(__WINDOWS_WASAPI__)
#include "RtAudio_WASAPI.h"
#endif
#if defined(__WINDOWS_DS__)
#include "RtAudio_DS.h"
#endif


std::string RtAudio :: getVersion( void )
{
  return RTAUDIO_VERSION;
}

// Define API names and display names.
// Must be in same order as API enum.
extern "C" {
const char* rtaudio_api_names[][2] = {
  { "unspecified" , "Unknown" },
  { "wasapi"      , "WASAPI" },
  { "asio"        , "ASIO" },
  { "ds"          , "DirectSound" },
  { "dummy"       , "Dummy" },
};
const unsigned int rtaudio_num_api_names = 
  sizeof(rtaudio_api_names)/sizeof(rtaudio_api_names[0]);

// The order here will control the order of RtAudio's API search in
// the constructor.
extern "C" const RtAudio::Api rtaudio_compiled_apis[] = {
#if defined(__WINDOWS_ASIO__)
  RtAudio::WINDOWS_ASIO,
#endif
#if defined(__WINDOWS_WASAPI__)
  RtAudio::WINDOWS_WASAPI,
#endif
#if defined(__WINDOWS_DS__)
  RtAudio::WINDOWS_DS,
#endif
  RtAudio::UNSPECIFIED
};
extern "C" const unsigned int rtaudio_num_compiled_apis =
  sizeof(rtaudio_compiled_apis)/sizeof(rtaudio_compiled_apis[0])-1;
}

RtAudio::Api RtAudio :: getCurrentApi( void ) { return rtapi_->getCurrentApi(); }
unsigned int RtAudio :: getDeviceCount( void ) { return rtapi_->getDeviceCount(); }
RtAudio::DeviceInfo RtAudio :: getDeviceInfo( unsigned int device ) { return rtapi_->getDeviceInfo( device ); }
unsigned int RtAudio :: getDefaultInputDevice( void ) { return rtapi_->getDefaultInputDevice(); }
unsigned int RtAudio :: getDefaultOutputDevice( void ) { return rtapi_->getDefaultOutputDevice(); }
void RtAudio :: closeStream( void ) { return rtapi_->closeStream(); }
void RtAudio :: startStream( void ) { return rtapi_->startStream(); }
void RtAudio :: stopStream( void )  { return rtapi_->stopStream(); }
void RtAudio :: abortStream( void ) { return rtapi_->abortStream(); }
bool RtAudio :: isStreamOpen( void ) const { return rtapi_->isStreamOpen(); }
bool RtAudio :: isStreamRunning( void ) const { return rtapi_->isStreamRunning(); }
long RtAudio :: getStreamLatency( void ) { return rtapi_->getStreamLatency(); }
unsigned int RtAudio :: getStreamSampleRate( void ) { return rtapi_->getStreamSampleRate(); }
double RtAudio :: getStreamTime( void ) { return rtapi_->getStreamTime(); }
void RtAudio :: setStreamTime( double time ) { return rtapi_->setStreamTime( time ); }
void RtAudio :: showWarnings( bool value ) { rtapi_->showWarnings( value ); }

/*
inline RtAudio::Api RtAudio :: getCurrentApi( void ) { return rtapi_->getCurrentApi(); }
inline unsigned int RtAudio :: getDeviceCount( void ) { return rtapi_->getDeviceCount(); }
inline RtAudio::DeviceInfo RtAudio :: getDeviceInfo( unsigned int device ) { return rtapi_->getDeviceInfo( device ); }
inline unsigned int RtAudio :: getDefaultInputDevice( void ) { return rtapi_->getDefaultInputDevice(); }
inline unsigned int RtAudio :: getDefaultOutputDevice( void ) { return rtapi_->getDefaultOutputDevice(); }
inline void RtAudio :: closeStream( void ) { return rtapi_->closeStream(); }
inline void RtAudio :: startStream( void ) { return rtapi_->startStream(); }
inline void RtAudio :: stopStream( void )  { return rtapi_->stopStream(); }
inline void RtAudio :: abortStream( void ) { return rtapi_->abortStream(); }
inline bool RtAudio :: isStreamOpen( void ) const { return rtapi_->isStreamOpen(); }
inline bool RtAudio :: isStreamRunning( void ) const { return rtapi_->isStreamRunning(); }
inline long RtAudio :: getStreamLatency( void ) { return rtapi_->getStreamLatency(); }
inline unsigned int RtAudio :: getStreamSampleRate( void ) { return rtapi_->getStreamSampleRate(); }
inline double RtAudio :: getStreamTime( void ) { return rtapi_->getStreamTime(); }
inline void RtAudio :: setStreamTime( double time ) { return rtapi_->setStreamTime( time ); }
inline void RtAudio :: showWarnings( bool value ) { rtapi_->showWarnings( value ); }


*/



void RtAudio :: getCompiledApi( std::vector<RtAudio::Api> &apis )
{
  apis = std::vector<RtAudio::Api>(rtaudio_compiled_apis,
                                   rtaudio_compiled_apis + rtaudio_num_compiled_apis);
}

std::string RtAudio :: getApiName( RtAudio::Api api )
{
  if (api < 0 || api >= RtAudio::NUM_APIS)
    return "";
  return rtaudio_api_names[api][0];
}

std::string RtAudio :: getApiDisplayName( RtAudio::Api api )
{
  if (api < 0 || api >= RtAudio::NUM_APIS)
    return "Unknown";
  return rtaudio_api_names[api][1];
}

RtAudio::Api RtAudio :: getCompiledApiByName( const std::string &name )
{
  unsigned int i=0;
  for (i = 0; i < rtaudio_num_compiled_apis; ++i)
    if (name == rtaudio_api_names[rtaudio_compiled_apis[i]][0])
      return rtaudio_compiled_apis[i];
  return RtAudio::UNSPECIFIED;
}

void RtAudio :: openRtApi( RtAudio::Api api )
{
  if ( rtapi_ )
    delete rtapi_;
  rtapi_ = 0;

#if defined(__WINDOWS_ASIO__)
  if ( api == WINDOWS_ASIO )
    rtapi_ = new RtApiAsio();
#endif

#if defined(__WINDOWS_WASAPI__)
  if ( api == WINDOWS_WASAPI )
    rtapi_ = new RtApiWasapi();
#endif

#if defined(__WINDOWS_DS__)
  if ( api == WINDOWS_DS )
    rtapi_ = new RtApiDs();
#endif
}

RtAudio :: RtAudio( RtAudio::Api api )
{
  rtapi_ = 0;

  if ( api != UNSPECIFIED ) {
    // Attempt to open the specified API.
    openRtApi( api );
    if ( rtapi_ ) return;

    // No compiled support for specified API value.  Issue a debug
    // warning and continue as if no API was specified.
    std::cerr << "\nRtAudio: no compiled support for specified API argument!\n" << std::endl;
  }

  // Iterate through the compiled APIs and return as soon as we find
  // one with at least one device or we reach the end of the list.
  std::vector< RtAudio::Api > apis;
  getCompiledApi( apis );
  for ( unsigned int i=0; i<apis.size(); i++ ) {
    openRtApi( apis[i] );
    if ( rtapi_ && rtapi_->getDeviceCount() ) break;
  }

  if ( rtapi_ ) return;

  // It should not be possible to get here because the preprocessor
  // definition __RTAUDIO_DUMMY__ is automatically defined if no
  // API-specific definitions are passed to the compiler. But just in
  // case something weird happens, we'll thow an error.
  std::string errorText = "\nRtAudio: no compiled API support found ... critical error!!\n\n";
  throw( RtAudioError( errorText, RtAudioError::UNSPECIFIED ) );
}

RtAudio :: ~RtAudio()
{
  if ( rtapi_ )
    delete rtapi_;
}

void RtAudio :: openStream( RtAudio::StreamParameters *outputParameters,
                            RtAudio::StreamParameters *inputParameters,
                            RtAudioFormat format, unsigned int sampleRate,
                            unsigned int *bufferFrames,
                            RtAudioCallback callback, void *userData,
                            RtAudio::StreamOptions *options,
                            RtAudioErrorCallback errorCallback )
{
  return rtapi_->openStream( outputParameters, inputParameters, format,
                             sampleRate, bufferFrames, callback,
                             userData, options, errorCallback );
}

