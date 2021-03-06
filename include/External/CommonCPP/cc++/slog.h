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
// of Common C++.
// 
// The exception is that, if you link the Common C++ library with other
// files to produce an executable, this does not by itself cause the
// resulting executable to be covered by the GNU General Public License.
// Your use of that executable is in no way restricted on account of
// linking the Common C++ library code into it.
// 
// This exception does not however invalidate any other reasons why
// the executable file might be covered by the GNU General Public License.
// 
// This exception applies only to the code released under the 
// name Common C++.  If you copy code from other releases into a copy of
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
// 
// If you write modifications of your own for Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.  

#ifndef	__CCXX_SLOG_H__
#define	__CCXX_SLOG_H__

#ifndef CCXX_CONFIG_H_
#include <cc++/config.h>
#endif

#ifndef	__CCXX_THREAD_H__
#include <cc++/thread.h>
#endif

#include <fstream>
#include <iostream>
#include <strstream>

#ifdef	CCXX_NAMESPACES
namespace ost {
#endif

enum slog_class_t
{
	SLOG_SECURITY,
	SLOG_AUDIT,
	SLOG_DAEMON,
	SLOG_USER,
	SLOG_DEFAULT
};
typedef enum slog_class_t slog_class_t;

enum slog_level_t
{
	SLOG_EMERGENCY = 1,
	SLOG_ALERT,
	SLOG_CRITICAL,
	SLOG_ERROR,
	SLOG_WARNING,
	SLOG_NOTICE,
	SLOG_INFO,
	SLOG_DEBUG
};
typedef enum slog_level_t slog_level_t;

/**
 * The slog class is used to stream messages to the system's logging facility (syslogd).
 * A default <code>slog</code> object is used to avoid confusion with the native syslog
 * facility and to imply a logical relationship to the C++ <code>clog()</code>.
 * 
 * The key difference is that the <code>slog</code> object sends it's output to the
 * system logging daemon (typically syslogd) rather than through stderr.
 * <code>slog</code> can be streamed with the <code><<</code> operator just 
 * like <code>clog</code>; a default slog object is pre-initialized, and you stream
 * character data to it.
 *
 * The <code>slog</code> allows one to specify logging levels and other properties through the <code>()</code> operators.
 * Hence, once can do: 
 *
 * <code><pre>
 * slog("mydaemon", SLOG_DAEMON, SLOG_EMERGENCY) << I just died << endl; </pre></code>
 *
 * or things like: 
 *
 * <code><pre>
 * slog("mydaemon", SLOG_DAEMON); 
 * slog(SLOG_INFO) << "daemon initalized" << endl; </pre></code>
 *
 * The intent is to be as common-place and as convenient to use as the stderr based clog facility
 * found in C++, and this is especially useful for C++ daemons.
 *
 * The <code>std::flush</code> manipulator doesn't work.  Either the
 * <code>std::endl</code> or <code>std::ends</code> manipulators
 * must be used to cause the output to be sent to the daemon.
 * 
 * When this class is used on a system that doesn't have the syslog headers
 * (i.e. a non-posix win32 box), the output goes to the a file with the same name
 * as the syslog identifier string with '.log' appended to it.  If the identifier string ends in
 * '.exe', the '.exe' is removed before the '.log' is appened. (e.g. the identifier foo.exe will
 * generate a log file named foo.log)
 * 
 * @author David Sugar <dyfet@ostel.com>
 * <br>Minor docs & hacks by Jon Little <littlej@arlut.utexas.edu>
 *
 * @short system logging facility class.
 */
#if defined(STLPORT) || defined(__KCC)
#define ostream ostream_withassign
#endif
class CCXX_CLASS_EXPORT Slog : public std::streambuf, public std::ostream
{
private:
#ifndef	HAVE_SYSLOG_H
	Mutex lock;
	std::ofstream syslog;
#endif
	int priority;
	slog_level_t  _level;
	bool _enable;
        bool _clogEnable;

protected:
	/**
         * This is the streambuf function that actually outputs the data
         * to the device.  Since all output should be done with the standard
         * ostream operators, this function should never be called directly.
	 */
	int overflow(int c);

public:
	/**
         * Default (and only) constructor.  The default log level is set to
         * SLOG_DEBUG.  There is no default log facility set.  One should be
         * set before attempting any output.  This is done by the <code>open()</code> or the
         * <code>operator()(const char*, slog_class_t, slog_level_t)</code>
         * functions.
	 */
	Slog(void);

	virtual ~Slog(void);

	void close(void);

	/**
         * (re)opens the output stream.
         * @param ident The identifier portion of the message sent to the syslog daemon.
         * @param grp The log facility the message is sent to
	 */
	void open(const char *ident, slog_class_t grp = SLOG_USER);

	/**
         * Sets the log identifier, level, and class to use for subsequent output
         * @param ident The identifier portion of the message
         * @param grp The log facility the message is sent to
         * @param level The log level of the message
	 */
	Slog &operator()(const char *ident, slog_class_t grp = SLOG_USER, 
			 slog_level_t level = SLOG_ERROR);

	/**
         * Changes the log level and class to use for subsequent output
         * @param level The log level of the message
         * @param grp The log facility the message is sent to
	 */
	Slog &operator()(slog_level_t level, slog_class_t = SLOG_DEFAULT);

        /**
         * Does nothing except return *this.
         */
	Slog &operator()(void);	

	/**
         * Sets the logging level.  
         * @param enable is the logging level to use for further output
	 */
	inline void level(slog_level_t enable)
		{_level = enable;};

	/**
         * Enables or disables the echoing of the messages to clog in addition
         * to the syslog daemon.  This is enabled by the default class constructor.
         * @param f true to enable, false to disable clog output
	 */
        inline void clogEnable(bool f=true)
                {_clogEnable = f;};
};

extern Slog	slog;

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
