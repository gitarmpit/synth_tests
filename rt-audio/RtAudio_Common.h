#ifndef RTAUDIO_COMMON_H
#define RTAUDIO_COMMON_H 

#define RTAUDIO_VERSION "5.1.0"

#if defined(RTAUDIO_EXPORT)
    #define RTAUDIO_DLL_PUBLIC __declspec(dllexport)
#else
    #define RTAUDIO_DLL_PUBLIC
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif


#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <windows.h>
#include <process.h>
#include <stdint.h>

#define MUTEX_INITIALIZE(A) InitializeCriticalSection(A)
#define MUTEX_DESTROY(A)    DeleteCriticalSection(A)
#define MUTEX_LOCK(A)       EnterCriticalSection(A)
#define MUTEX_UNLOCK(A)     LeaveCriticalSection(A)


typedef unsigned long RtAudioFormat;
static const RtAudioFormat RTAUDIO_SINT8 = 0x1;    // 8-bit signed integer.
static const RtAudioFormat RTAUDIO_SINT16 = 0x2;   // 16-bit signed integer.
static const RtAudioFormat RTAUDIO_SINT24 = 0x4;   // 24-bit signed integer.
static const RtAudioFormat RTAUDIO_SINT32 = 0x8;   // 32-bit signed integer.
static const RtAudioFormat RTAUDIO_FLOAT32 = 0x10; // Normalized between plus/minus 1.0.
static const RtAudioFormat RTAUDIO_FLOAT64 = 0x20; // Normalized between plus/minus 1.0.

typedef unsigned int RtAudioStreamFlags;
static const RtAudioStreamFlags RTAUDIO_NONINTERLEAVED = 0x1;    // Use non-interleaved buffers (default = interleaved).
static const RtAudioStreamFlags RTAUDIO_MINIMIZE_LATENCY = 0x2;  // Attempt to set stream parameters for lowest possible latency.
static const RtAudioStreamFlags RTAUDIO_HOG_DEVICE = 0x4;        // Attempt grab device and prevent use by others.
static const RtAudioStreamFlags RTAUDIO_SCHEDULE_REALTIME = 0x8; // Try to select realtime scheduling for callback thread.
static const RtAudioStreamFlags RTAUDIO_ALSA_USE_DEFAULT = 0x10; // Use the "default" PCM device (ALSA only).
static const RtAudioStreamFlags RTAUDIO_JACK_DONT_CONNECT = 0x20; // Do not automatically connect ports (JACK only).

typedef unsigned int RtAudioStreamStatus;
static const RtAudioStreamStatus RTAUDIO_INPUT_OVERFLOW = 0x1;    // Input data was discarded because of an overflow condition at the driver.
static const RtAudioStreamStatus RTAUDIO_OUTPUT_UNDERFLOW = 0x2;  // The output buffer ran low, likely causing a gap in the output sound.

typedef int (*RtAudioCallback)( void *outputBuffer, void *inputBuffer,
                                unsigned int nFrames,
                                double streamTime,
                                RtAudioStreamStatus status,
                                void *userData );


typedef uintptr_t ThreadHandle;
typedef CRITICAL_SECTION StreamMutex;

struct CallbackInfo {
  void *object;    // Used as a "this" pointer.
  ThreadHandle thread;
  void *callback;
  void *userData;
  void *errorCallback;
  void *apiInfo;   // void pointer for API specific callback information
  bool isRunning;
  bool doRealtime;
  int priority;

  CallbackInfo()
  :object(0), callback(0), userData(0), errorCallback(0), apiInfo(0), isRunning(false), doRealtime(false), priority(0) {}
};


#pragma pack(push, 1)
class S24 {

 protected:
  unsigned char c3[3];

 public:
  S24() {}

  S24& operator = ( const int& i ) {
    c3[0] = (i & 0x000000ff);
    c3[1] = (i & 0x0000ff00) >> 8;
    c3[2] = (i & 0x00ff0000) >> 16;
    return *this;
  }

  S24( const double& d ) { *this = (int) d; }
  S24( const float& f ) { *this = (int) f; }
  S24( const signed short& s ) { *this = (int) s; }
  S24( const char& c ) { *this = (int) c; }

  int asInt() {
    int i = c3[0] | (c3[1] << 8) | (c3[2] << 16);
    if (i & 0x800000) i |= ~0xffffff;
    return i;
  }
};
#pragma pack(pop)

/*
inline std::string convertCharPointerToStdString(const char *text)
{
    return std::string(text);
}

inline std::string convertCharPointerToStdString(const wchar_t *text)
{
    int length = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
    std::string s( length-1, '\0' );
    WideCharToMultiByte(CP_UTF8, 0, text, -1, &s[0], length, NULL, NULL);
    return s;
}
*/


#endif
