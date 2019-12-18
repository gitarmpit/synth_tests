#ifndef RTAUDIO_ERROR_H
#define RTAUDIO_ERROR_H

#include "RtAudio_Common.h"

class RTAUDIO_DLL_PUBLIC RtAudioError : public std::runtime_error
{
 public:
  //! Defined RtAudioError types.
  enum Type {
    WARNING,           /*!< A non-critical error. */
    DEBUG_WARNING,     /*!< A non-critical error which might be useful for debugging. */
    UNSPECIFIED,       /*!< The default, unspecified error type. */
    NO_DEVICES_FOUND,  /*!< No devices found on system. */
    INVALID_DEVICE,    /*!< An invalid device ID was specified. */
    MEMORY_ERROR,      /*!< An error occured during memory allocation. */
    INVALID_PARAMETER, /*!< An invalid parameter was specified to a function. */
    INVALID_USE,       /*!< The function was called incorrectly. */
    DRIVER_ERROR,      /*!< A system driver error occured. */
    SYSTEM_ERROR,      /*!< A system error occured. */
    THREAD_ERROR       /*!< A thread error occured. */
  };

  //! The constructor.
  RtAudioError( const std::string& message,
                Type type = RtAudioError::UNSPECIFIED )
    : std::runtime_error(message), type_(type) {}

  //! Prints thrown error message to stderr.
  virtual void printMessage( void ) const
    { std::cerr << '\n' << what() << "\n\n"; }

  //! Returns the thrown error message type.
  virtual const Type& getType(void) const { return type_; }

  //! Returns the thrown error message string.
  virtual const std::string getMessage(void) const
    { return std::string(what()); }

 protected:
  Type type_;
};

typedef void (*RtAudioErrorCallback)( RtAudioError::Type type, const std::string &errorText );




#endif