// Copyright (C) 1999-2001 Open Source Telecom Corporation.
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
// As a special exception to the GNU General Public License, permission is 
// granted for additional uses of the text contained in its release 
// of APE.
// 
// The exception is that, if you link the APE library with other files
// to produce an executable, this does not by itself cause the
// resulting executable to be covered by the GNU General Public License.
// Your use of that executable is in no way restricted on account of
// linking the APE library code into it.
//
// This exception does not however invalidate any other reasons why
// the executable file might be covered by the GNU General Public License.
//
// This exception applies only to the code released under the 
// name APE.  If you copy code from other releases into a copy of
// APE, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
// 
// If you write modifications of your own for APE, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.  

#ifndef	__CCXX_SERIAL_H__
#define	__CCXX_SERIAL_H__

#ifndef CCXX_CONFIG_H_
#include <cc++/config.h>
#endif

#ifndef	__CCXX_THREAD_H__
#include <cc++/thread.h>
#endif

#include <iostream>

#ifdef	CCXX_NAMESPACES
namespace ost {
#endif

enum sioerror_t
{
	SERIAL_SUCCESS = 0,
	SERIAL_OPEN_NOTTY,
	SERIAL_OPEN_FAILED,
	SERIAL_SPEED_INVALID,
	SERIAL_FLOW_INVALID,
	SERIAL_PARITY_INVALID,
	SERIAL_CHARSIZE_INVALID,
	SERIAL_STOPBITS_INVALID,
	SERIAL_OPTION_INVALID,
	SERIAL_RESOURCE_FAILURE,
	SERIAL_OUTPUT_ERROR,
	SERIAL_INPUT_ERROR,
	SERIAL_TIMEOUT_ERROR,
	SERIAL_EXTENDED_ERROR
};
typedef enum sioerror_t sioerror_t;

enum sioflow_t
{
	SERIAL_FLOW_NONE,
	SERIAL_FLOW_SOFT,
	SERIAL_FLOW_HARD,
	SERIAL_FLOW_BOTH
};
typedef enum sioflow_t sioflow_t;

enum sioparity_t
{
	SERIAL_PARITY_NONE,
	SERIAL_PARITY_ODD,
	SERIAL_PARITY_EVEN
};
typedef enum sioparity_t sioparity_t;

enum siopend_t
{
	SERIAL_PENDING_INPUT,
	SERIAL_PENDING_OUTPUT,
	SERIAL_PENDING_ERROR
};
typedef enum siopend_t siopend_t;

/**
 * The Serial class is used as the base for all serial I/O services
 * under APE.  A serial is a system serial port that is used either
 * for line or packet based data input.  Serial ports may also be
 * "streamable" in a derived form.
 * 
 *  Common C++ serial I/O classes are used to manage serial devices and
 *  implement serial device protocols.  From the point of view of Common C++,
 *  serial devices are supported by the underlying Posix specified "termios"
 *  call interface.
 * 
 *  The serial I/O base class is used to hold a descriptor to a serial device
 *  and to provide an exception handling interface for all serial I/O classes.
 *  The base class is also used to specify serial I/O properties such as
 *  communication speed, flow control, data size, and parity.  The "Serial"
 *  base class is not itself directly used in application development,
 *  however.
 * 
 *  Common C++ Serial I/O is itself divided into two conceptual modes; frame
 *  oriented and line oriented I/O.  Both frame and line oriented I/O makes
 *  use of the ability of the underlying tty driver to buffer data and return
 *  "ready" status from when select either a specified number of bytes or
 *  newline record has been reached by manipulating termios c_cc fields
 *  appropriately.  This provides some advantage in that a given thread
 *  servicing a serial port can block and wait rather than have to continually
 *  poll or read each and every byte as soon as it appears at the serial port.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short base class for all serial I/O services.
 */

class Serial
{
private:
	sioerror_t errid;
	char *errstr;

	struct
	{
		bool thrown: 1;
		bool linebuf: 1;
	} flags;

	void *original, *current;

	/**
	 * Used to properly initialize serial object.
	 */
	void initSerial(void);

protected:
	int dev;
	int bufsize;

	/**
	 * This service is used to throw all serial errors which usually
	 * occur during the serial constructor.
	 *
	 * @param error defined serial error id.
	 * @param errstr string or message to optionally pass.
	 */
	sioerror_t Error(sioerror_t error, char *errstr = NULL);

	/**
	 * This service is used to thow application defined serial
	 * errors where the application specific error code is a string.
	 *
	 * @param errstr string or message to pass.
	 */
	inline void Error(char *errstr)
		{Error(SERIAL_EXTENDED_ERROR, errstr);};


	/**
	 * This method is used to turn the error handler on or off for
	 * "throwing" execptions by manipulating the thrown flag.
	 *
	 * @param true to enable handler.
	 */
	inline void setError(bool enable)
		{flags.thrown = !enable;};

	/**
	 * Set packet read mode and "size" of packet read buffer.
	 * This sets VMIN to x.  VTIM is normally set to "0" so that
	 * "isPending()" can wait for an entire packet rather than just
	 * the first byte.
	 *
	 * @return actual buffer size set.
	 * @param size of packet read request.
	 * @param optional inter-byte data packet timeout.
	 */
	int setPacketInput(int size, unsigned char btimer = 0);

	/**
	 * Set "line buffering" read mode and specifies the newline
	 * character to be used in seperating line records.  isPending
	 * can then be used to wait for an entire line of input.
	 *
 	 * @return size of conical input buffer.
	 * @param newline character.
	 */
	int setLineInput(char newline = 13, char nl1 = 0);

	/**
	 * Restore serial device to the original settings at time of open.
	 */
	void Restore(void);

	/**
	 * Used to flush the input waiting queue.
	 */
	void flushInput(void);

	/**
	 * Used to flush any pending output data.
	 */
	void flushOutput(void);

	/**
	 * Used to wait until all output has been sent.
	 */
	void waitOutput(void);

	/**
	 * Used as the default destructor for ending serial I/O
	 * services.  It will restore the port to it's original state.
	 */
	void endSerial(void);

	/**
	 * Used to initialize a newly opened serial file handle.  You
	 * should set serial properties and DTR manually before first
	 * use.
	 */
	void initConfig(void);

	/**
	 * A serial object may be constructed from a named file on the
	 * file system.  This named device must be "isatty()".
	 *
	 * @param name of file.
	 */
	Serial(const char *name);

	/**
	 * This allows later ttystream class to open and close a serial
	 * device.
	 */
	Serial()
		{initSerial();};

public:
	/**
	 * The serial base class may be "thrown" as a result on an error,
	 * and the "catcher" may then choose to destory the object.  By
	 * assuring the socket base class is a virtual destructor, we
	 * can assure the full object is properly terminated.
	 */
	virtual ~Serial()
		{endSerial();};

	/**
	 * Serial ports may also be duplecated by the assignment
	 * operator.
	 */
	Serial &operator=(const Serial &from);
	
	/**
	 * Set serial port speed for both input and output.
	 *
 	 * @return 0 on success.
	 * @param speed to select. 0 signifies modem "hang up".
	 */
	sioerror_t setSpeed(unsigned long speed);

	/**
	 * Set character size.
	 *
	 * @return 0 on success.
	 * @param character size to use (usually 7 or 8).
	 */
	sioerror_t setCharBits(int bits);

	/**
	 * Set parity mode.
	 *
	 * @return 0 on success.
	 * @param parity mode.
	 */
	sioerror_t setParity(sioparity_t parity);

	/**
	 * Set number of stop bits.
	 *
	 * @return 0 on success.
	 * @param stop bits.
	 */
	sioerror_t setStopBits(int bits);

	/**
	 * Set flow control.
	 *
	 * @return 0 on success.
	 * @param flow control mode.
	 */
	sioerror_t setFlowControl(sioflow_t flow);

	/**
	 * Set the DTR mode off momentarily.
	 *
	 * @param number of milliseconds.
	 */
	void toggleDTR(timeout_t millisec);

	/**
	 * Send the "break" signal.
	 */
	void sendBreak(void);

	/**
	 * Often used by a "catch" to fetch the last error of a thrown
	 * serial.
	 *
	 * @return error numbr of last sioerror_t.
	 */
	inline sioerror_t getErrorNumber(void)
		{return errid;};

	/**
	 * Often used by a "catch" to fetch the user set error string
	 * of a thrown serial.
	 *
	 * @return string for error message.
	 */
	inline char *getErrorString(void)
		{return errstr;};

	/**
	 * Get the "buffer" size for buffered operations.  This can
	 * be used when setting packet or line read modes to determine
	 * how many bytes to wait for in a given read call.
	 *
	 * @return number of bytes used for buffering.
	 */
	inline int getBufferSize(void)
		{return bufsize;};

	/**
	 * Get the status of pending operations.  This can be used to
	 * examine if input or output is waiting, or if an error has
	 * occured on the serial device.
	 *
	 * @return true if ready, false if timeout.
	 * @param ready check to perform.
	 * @param timeout in milliseconds.
	 */
	virtual bool isPending(siopend_t pend, timeout_t timeout = TIMEOUT_INF);
};

/**
 * TTY streams are used to represent serial connections that are fully
 * "streamable" objects using C++ stream classes and friends.
 * 
 * The first application relevant serial I/O class is the TTYStream class.
 * TTYStream offers a linearly buffered "streaming" I/O session with the
 * serial device.  Furthermore, traditional C++ "stream" operators (<< and
 * >>) may be used with the serial device.  A more "true" to ANSI C++ library
 * format "ttystream" is also available, and this supports an "open" method
 * in which one can pass initial serial device parameters immediately
 * following the device name in a single string, as in
 * "/dev/tty3a:9600,7,e,1", as an example.
 * 
 * The TTYSession aggragates a TTYStream and a Common C++ Thread which is
 * assumed to be the execution context that will be used to perform actual
 * I/O operations.  This class is very anagolous to TCPSession.
 * 
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short streamable tty serial I/O class.
 */

#if defined(STLPORT) || defined(__KCC)
#define iostream iostream_withassign
#endif
class TTYStream : public std::streambuf, public std::iostream, public
Serial
{
private:
	int doallocate();

	friend TTYStream& crlf(TTYStream&);
	friend TTYStream& lfcr(TTYStream&);

protected:
	char *gbuf, *pbuf;
	timeout_t timeout;

	/**
	 * This constructor is used to derive "ttystream", a more
	 * C++ style version of the TTYStream class.
	 */
	TTYStream();

	/**
	 * Used to allocate the buffer space needed for iostream
	 * operations.  This is based on MAX_INPUT.
	 */
	void Allocate(void);

	/**
	 * Used to terminate the buffer space and clean up the tty
	 * connection.  This function is called by the destructor.
	 */
	void endStream(void);

	/**
	 * This streambuf method is used to load the input buffer
	 * through the established tty serial port.
	 *
	 * @return char from get buffer, EOF also possible.
	 */
	int underflow(void);

	/**
	 * This streambuf method is used for doing unbuffered reads
	 * through the establish tty serial port when in interactive mode.
	 * Also this method will handle proper use of buffers if not in
	 * interative mode.
	 *
	 * @return char from tty serial port, EOF also possible.
	 */
	int uflow(void);

	/**
	 * This streambuf method is used to write the output
	 * buffer through the established tty port.
	 *
	 * @param char to push through.
	 * @return char pushed through.
	 */
	int overflow(int ch);

public:
	/**
	 * Create and ipen a tty serial port.
	 *
	 * @param char name of device to open.
	 */
	TTYStream(const char *filename);

	/**
	 * End the tty stream and cleanup.
	 */
	virtual ~TTYStream();

	/**
	 * Set the timeout control.
	 *
	 * @param timeout to use.
	 */
	inline void setTimeout(timeout_t to)
		{timeout = to;};

	/**
	 * Set tty mode to buffered or "interactive".  When interactive,
	 * all streamed I/O is directly sent to the serial port
	 * immediately.
	 *
	 * @param bool set to true to make interactive.
	 */
	void Interactive(bool flag);
	
	/**
	 * Flushes the stream input and out buffers, writes
	 * pending output.
	 *
	 * @return 0 on success.
	 */
	int sync(void);	

	/**
	 * Get the status of pending operations.  This can be used to
	 * examine if input or output is waiting, or if an error has
	 * occured on the serial device.  If read buffer contains data
	 * then input is ready and if write buffer contains data it is
	 * first flushed then checked.
	 *
	 * @return true if ready, false if timeout.
	 * @param ready check to perform.
	 * @param timeout in milliseconds.
	 */
	bool isPending(siopend_t pend, timeout_t timeout = TIMEOUT_INF);
};	

/**
 * A more natural C++ "ttystream" class for use by non-threaded
 * applications.  This class behaves a lot more like fstream and
 * similar classes.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short C++ "fstream" style ttystream class.
 */

class ttystream : public TTYStream
{
public:
	/**
	 * Construct an unopened "ttystream" object.
	 */
	ttystream();

	/**
	 * Construct and "open" a tty stream object.  A filename in
	 * the form "device:options[,options]" may be used to pass
	 * device options as part of the open.
	 *
	 * @param name of file and serial options.
	 */
	ttystream(const char *name);

	/**
	 * Open method for a tty stream.
	 *
	 * @param filename to open.
	 */
	void open(const char *name);

	/**
	 * Close method for a tty stream.
	 */
	void close(void);

	/**
	 * Test to see if stream is opened.
	 */
	inline bool operator!()
		{return (dev < 0);};
};

/**
 * 
 * The TTYSession aggragates a TTYStream and a Common C++ Thread which is
 * assumed to be the execution context that will be used to perform actual
 * I/O operations.  This class is very anagolous to TCPSession.
 * 
 * @author David Sugar <dyfet@ostel.com>
 * @short This class is very anagolous to TCPSession.
 */

class TTYSession : public TTYStream, public Thread
{
public:
	/**
	 * Create TTY stream that will be managed by it's own thread.
	 *
	 * @param name of tty device to open.
	 * @param pri execution priority.
	 * @param stack allocation needed on some platforms.
	 */
	TTYSession(const char *name, int pri = 0, int stack = 0);
};

class SerialPort;
class SerialService;

/**
 * The serial port is an internal class which is attached to and then
 * serviced by a specified SerialService thread.  Derived versions of
 * this class offer specific functionality such as serial integration
 * protocols.
 * 
 * The TTYPort and TTYService classes are used to form thread-pool serviced
 * serial I/O protocol sets.  These can be used when one has a large number
 * of serial devices to manage, and a single (or limited number of) thread(s)
 * can then be used to service the tty port objects present.  Each tty port
 * supports a timer control and several virtual methods that the service
 * thread can call when events occur.  This model provides for "callback"
 * event management, whereby the service thread performs a "callback" into
 * the port object when events occur.  Specific events supported include the
 * expiration of a TTYPort timer, pending input data waiting to be read, and
 * "sighup" connection breaks.   
 * 
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short base class for thread pool serviced serial I/O.
 */
class SerialPort: public Serial, public TimerPort
{
private:
	SerialPort *next, *prev;
	SerialService *service;
#ifdef	__CCXX_USE_POLL
	struct pollfd *ufd;
#endif
	bool detect_pending;
	bool detect_output;
	bool detect_disconnect;

	friend class SerialService;

protected:
	/**
	 * Construct a tty serial port for a named serial device.
	 *
	 * @param svc pool thread object.
	 * @param name of tty port.
	 */
	SerialPort(SerialService *svc, const char *name);
	
	/**
	 * Disconnect the Serial Port from the service pool thread
	 * and shutdown the port.
	 */
	virtual ~SerialPort();

	/**
	 * Used to indicate if the service thread should monitor pending
	 * data for us.
	 */
	void setDetectPending( bool );
	
	/**
	 * Get the current state of the DetectPending flag.
	 */
	bool getDetectPending( void ) const
		{ return detect_pending; }
	
	/**
	 * Used to indicate if output ready monitoring should be performed
	 * by the service thread.
	 */
	void setDetectOutput( bool );
	
	/**
	 * Get the current state of the DetectOutput flag.
	 */
	bool getDetectOutput( void ) const
		{ return detect_output; }

	/**
	 * Called by the service thread when the objects timer
	 * has expired.
	 */
	virtual void Expired(void)
		{return;};

	/**
	 * Called by the service thread when input data is pending
	 * for this tty port.  Effected by setPacketInput and by
	 * setLineInput.
	 */
	virtual void Pending(void)
		{return;};

	/**
	 * Called by the service thread when an exception has occured
	 * such as a hangup.
	 */
	virtual void Disconnect(void)
		{return;};

	/**
	 * Transmit "send" data to the serial port.  This is not public
	 * since it's meant to support internal protocols rather than
	 * direct public access to the device.
	 *
	 * @return number of bytes send.
	 * @param address of buffer to send.
	 * @param len of bytes to send.
	 */
	inline int Output(void *buf, int len)
		{return ::write(dev, (char *)buf, len);};

	/**
	 * Perform when output is available for sending data.
	 */
	virtual void Output(void)
		{return;};

	/**
	 * Receive "input" for pending data from the serial port.  This
	 * is not a public member since it's meant to support internal
	 * protocols rather than direct external access to the device.
	 *
	 * @return number of bytes received.
	 * @param address of buffer to input.
	 * @param len of input buffer used.
	 */
	inline int Input(void *buf, int len)
		{return ::read(dev, (char *)buf, len);};

public:
	/**
	 * Derived setTimer to notify the service thread pool of changes
	 * in expected timeout.  This allows SerialService to
	 * reschedule all timers.
	 *
	 * @param timeout in milliseconds.
	 */
	void setTimer(timeout_t timeout = 0);

	/**
	 * Derived incTimer to notify the service thread pool of a
	 * change in expected timeout.  This allows SerialService to
	 * reschedule all timers.
	 */
	void incTimer(timeout_t timeout);
};

/**
 * The SerialService is a thead service object that is meant to service
 * attached serial ports.  Multiple pool objects may be created and
 * multiple serial ports may be attached to the same thread of
 * of execution.  This allows one to balance threads and the serial ports
 * they service.
 * 
 *  The TTYPort and TTYService classes are used to form thread-pool serviced
 *  serial I/O protocol sets.  These can be used when one has a large number
 *  of serial devices to manage, and a single (or limited number of) thread(s)
 *  can then be used to service the tty port objects present.  Each tty port
 *  supports a timer control and several virtual methods that the service
 *  thread can call when events occur.  This model provides for "callback"
 *  event management, whereby the service thread performs a "callback" into
 *  the port object when events occur.  Specific events supported include the
 *  expiration of a TTYPort timer, pending input data waiting to be read, and
 *  "sighup" connection breaks.   
 * 
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short Thread pool service for serial ports.
 */
class SerialService : public Thread, private Mutex
{
private:
	fd_set connect;
	int iosync[2];
	int hiwater;
	int count;
	SerialPort *first, *last;

	/**
	 * Attach a new serial port to this service thread.
	 *
	 * @param port of SerialPort derived object to attach.
	 */
	void Attach(SerialPort *port);

	/**
	 * Detach a serial port from this service thread.
	 *
	 * @param port of SerialPort derived object to remove.
	 */
	void Detach(SerialPort *port);

	/**
	 * The service thread itself.
	 */
	void Run(void);

	friend class SerialPort;

protected:
	/**
	 * A virtual handler for processing user defined update
	 * requests (1-254) which have been posted through Update.
	 *
	 * @param flag of update request.
	 */
	virtual void OnUpdate(unsigned char flag)
		{return;};

	/**
	 * A virtual handler for event loop calls.  This can be
 	 * used to extend event loop processing.
	 */
	virtual void OnEvent(void)
		{return;};

	/**
	 * A virtual handler for adding support for additional
	 * callback events into SerialPort.
	 *
	 * @param serial port currently being evaluated.
	 */
	virtual void OnCallback(SerialPort *port)
		{return;};

public:
	/**
	 * Notify service thread that a port has been added or
	 * removed, or a timer changed, so that a new schedule
	 * can be computed for expiring attached ports.  This
	 * can also be used to pass requests to the OnUpdate()
	 * event handler.
	 *
	 * @param flag event for OnUpdate, termination, or reschedule.
	 */
	void Update(unsigned char flag = 0xff);
	
	/**
	 * Create a service thread for attaching serial ports.  The
	 * thread begins execution with the first attached port.
	 *
	 * @param pri of this thread to run under.
	 */
	SerialService(int pri = 0);

	/**
	 * Terminate the service thread and update attached objects.
	 */
	virtual ~SerialService();

	/**
	 * Get current reference count.  This can be used when selecting
	 * the lead used service handler from a pool.
	 *
	 * @return count of active ports.
	 */
	inline int getCount(void)
		{return count;};
};

#ifdef	COMMON_STD_EXCEPTION
class SerException : public IOException
{
public:
	SerException(std::string str) : IOException(str) {};
};
#endif

#ifdef	CCXX_NAMESPACES
};
#endif

#endif
/** EMACS **
 * Local variables:
 * mode: c++
 * c-basic-offset: 8
 * End:
 */
