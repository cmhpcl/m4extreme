// Copyright (C) 2001 Gianni Mariani
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
//

#ifndef	__CCXX_CMDOPTNS_H__
#define	__CCXX_CMDOPTNS_H__

#ifndef CCXX_CONFIG_H_
#include <cc++/config.h>
#endif

#ifdef	CCXX_NAMESPACES
namespace ost {
#endif

#ifdef HAVE_GETOPT_LONG

/**
 * cmdoptns.h
 *
 * Command line option parsing interface.
 *
 */

class CommandOption;
class CommandOptionParse;

/**
 * This defines a linked list head pointer for all the command line
 * options that use the default list.  It will most likely 
 * be used in most cases without being explicitly referenced in application
 * code.  It is a default value of various method's parameters.
 *
 */
extern CommandOption * DefaultCommandOptionList;

/**
 * CommandOption is the base class for all command line options.  Command
 * line options can be defined statically and used when constructing
 * a command line parser onject using MakeCommandOptionParse.  This serves
 * only as a base class to CommandOptionWithArg, CommandOptionRest or
 * CommandOptionNoArg which can also be used to derive more complex
 * classes or even entire applications.
 *
 * @author Gianni Mariani <gianni@mariani.ws>
 */
class CommandOption {
public:

	/**
	 * Long option name, these will be preceded with "--" on the command line.
	 * e.g. --file foo.x
	 */
	const char	* option_name;
	
	/**
	 * option letter, these will be preceded with "-" on the command line.
	 * e.g. -f foo.x
	 */
	const char	* option_letter;

	/**
	 * A short description of the option for Usage messages.
	 * e.g. Usage: mycommand : blah
	 *			-f, --file <DESCRIPTION here>
	 */
	const char	* description;

	/**
	 * OptionType is for denoting what type of option this is, with an
	 * arg, without an arg or the trailing args.
	 * @short Option type
	 */
	enum OptionType {
		/**
		 * This option is associated with a value
		 */
		HasArg,
		/**
		 * This option is a flag only
		 */
		NoArg,
		/**
		 * Remaining of the command line arguments
		 */
		Trailing,
		/**
		 * Collect values that are not a value to an option
		 */
		Collect
	};

	/**
	 * This command option's OptionType.
	 */
	OptionType	  option_type;	// HasArg, NoArg or Trailing

	/**
	 * True if this parameter is required.  If the parameter is not supplied
	 * and required is true, an error will be flagged in the option processor.
	 */
	bool		  required;		// Option is required - fail without it

	/**
	 * This next CommandOption in this list of options or nil if no more
	 * options exist.
	 */
	CommandOption	* next;
		
	/**
	 * A virtual destructor just in case.
	 */
	virtual ~CommandOption();

	/**
	 * CommandOption contructor.  Note the default values for required and
	 * pp_next.
	 *
	 * @param in_option_name	long option name
	 * @param in_option_letter	short letter name
	 * @param in_description	short description of the option
	 * @param in_option_type	the type of this option
	 * @param in_required		true if option is required
	 * @param pp_next			the linked list header
	 */
	CommandOption(
		const char		* in_option_name,
		const char		* in_option_letter,
		const char		* in_description,
		OptionType		  in_option_type,
		bool			  in_required = false,
		CommandOption  ** pp_next = & DefaultCommandOptionList
	);

	/**
	 * FoundOption is called by the CommandOptionParse object during the parsing
	 * of the command line options.
	 *
	 * @param cop		pointer to the command option parser
	 * @param value		the value of this option
	 */
	virtual	void FoundOption( CommandOptionParse * cop, const char * value = 0 );
	
	/**
	 * FoundOption is called by the CommandOptionParse object during the parsing
	 * of the command line options.
	 *
	 * @param cop		pointer to the command option parser
	 * @param value		an array of values of this option
	 * @param num		number of values in the array
	 */
	virtual	void FoundOption( CommandOptionParse * cop, const char ** value, int num );
	
	/**
	 * Once parsing of command line options is complete, this method is called.
	 * This can be used to perform last minute checks on the options collected.
	 *
	 * @param cop		pointer to the command option parser
	 */
	virtual	void ParseDone( CommandOptionParse * cop );
	
	/**
	 * Once CommandOption objects have completed parsing and there are no
	 * errors they may have some specific tasks to perform.  PerformTask
	 * must return.
	 *
	 * @param cop		pointer to the command option parser
	 */
	virtual	void PerformTask( CommandOptionParse * cop );

	/**
	 * For fields with the required flag set, this method is used to determine
	 * if the Option has satisfied it's required status.  The default methods
	 * simply returns true if any values have been found.  This could be specialized
	 * to return true based on some other criteria.
	 */
	virtual bool HasValue();
	
};

/**
 * Derived class of CommandOption for options that have a value associated with them.
 * Classes CommandOptionRest and CommandOptionArg derive from this class.
 */
class CommandOptionWithArg : public CommandOption {
public:

	/**
	 * Array of list of values collected for this option.
	 */
	const char ** values;

	/**
	 * Number of values in the values array.
	 */
	int			  numvalue;

	/**
	 * CommandOptionWithArg contructor.  Note the default values for required and
	 * pp_next.
	 *
	 * @param in_option_name	long option name
	 * @param in_option_letter	short letter name
	 * @param in_description	short description of the option
	 * @param in_option_type	the type of this option
	 * @param in_required		true if option is required
	 * @param pp_next			the linked list header
	 */
	CommandOptionWithArg(
		const char		* in_option_name,
		const char		* in_option_letter,
		const char		* in_description,
		OptionType		  in_option_type,
		bool			  in_required = false,
		CommandOption  ** pp_next = & DefaultCommandOptionList
	);

	virtual ~CommandOptionWithArg();
	
	virtual	void FoundOption( CommandOptionParse * cop, const char * value = 0 );
	virtual	void FoundOption( CommandOptionParse * cop, const char ** value, int num );
	virtual bool HasValue();
};
	
/**
 * Class for options with an argument e.g. --option value .
 */
class CommandOptionArg : public CommandOptionWithArg {
public:

	/**
	 * CommandOptionArg contructor.  This sets the option_type for this
	 * object to HasArg.
	 *
	 * @param in_option_name	long option name
	 * @param in_option_letter	short letter name
	 * @param in_description	short description of the option
	 * @param in_required		true if option is required
	 * @param pp_next			the linked list header
	 */
	CommandOptionArg(
		const char		* in_option_name,
		const char		* in_option_letter,
		const char		* in_description,
		bool			  in_required = false,
		CommandOption  ** pp_next = & DefaultCommandOptionList
	);

	virtual ~CommandOptionArg();
	

};
	
/**
 * It only makes sense to have a single one of these set and it is
 * exclusive with CommandOptionCollect. It is the option that takes the rest
 * of the command line options that are not part of any other options.
 * e.g. "strace -ofile command arg1 arg2". The "command arg1 arg2" part is
 * placed in objects of this class.
 * 
 * @short CommandOption to take the rest of the command line
 */
class CommandOptionRest : public CommandOptionWithArg {
public:

	/**
	 * CommandOptionRest contructor.  This sets the option_type for this
	 * object to Trailing.
	 *
	 * @param in_option_name	long option name
	 * @param in_option_letter	short letter name
	 * @param in_description	short description of the option
	 * @param in_required		true if option is required
	 * @param pp_next			the linked list header
	 */
	CommandOptionRest(
		const char		* in_option_name,
		const char		* in_option_letter,
		const char		* in_description,
		bool			  in_required = false,
		CommandOption  ** pp_next = & DefaultCommandOptionList
	);

};
	
/**
 * It only makes sense to have a single one of these set and it is also
 * exclusive with CommandOptionRest.  This makes parameter collecting
 * behave line the Unix "cat" command.
 *
 * @short CommandOption to collect parameters that are not options.
 */
class CommandOptionCollect : public CommandOptionWithArg {
public:

	/**
	 * CommandOptionRest contructor.  This sets the option_type for this
	 * object to Collect.
	 *
	 * @param in_option_name	long option name
	 * @param in_option_letter	short letter name
	 * @param in_description	short description of the option
	 * @param in_required		true if option is required
	 * @param pp_next			the linked list header
	 */
	CommandOptionCollect(
		const char		* in_option_name,
		const char		* in_option_letter,
		const char		* in_description,
		bool			  in_required = false,
		CommandOption  ** pp_next = & DefaultCommandOptionList
	);

};
	
/**
 * CommandOption type for flags.
 */
class CommandOptionNoArg : public CommandOption {
public:

	/**
	 * The number of times this value has been set.
	 */
	int			  numset;	// The number of times this argument is set

	/**
	 * CommandOptionArg contructor.  This sets the option_type for this
	 * object to NoArg.
	 *
	 * @param in_option_name	long option name
	 * @param in_option_letter	short letter name
	 * @param in_description	short description of the option
	 * @param in_required		true if option is required
	 * @param pp_next			the linked list header
	 */
	CommandOptionNoArg(
		const char		* in_option_name,
		const char		* in_option_letter,
		const char		* in_description,
		bool			  in_required = false,
		CommandOption  ** pp_next = & DefaultCommandOptionList
	);

	/**
	 * CommandOptionNoArg::FoundOption will evpect a nil "value" passed in.
	 */
	virtual	void FoundOption( CommandOptionParse * cop, const char * value = 0 );

};

/**
 * This is the CommandOptionParse interface class.  To implement this object you can
 * call MakeCommandOptionParse();  This will instantiate a dynamically allocated
 * version of this class and parse the command line for the list of command options
 * that are passed in.
 *
 * @author Gianni Mariani <gianni@mariani.ws>
 */

class CommandOptionParse {
public:

	/**
	 * Virtual destructor needed so that the object may be correctly deleted.
	 */
	virtual ~CommandOptionParse() = 0;

	/**
	 * Get the value of the error flag set if the parser encountered errors.
	 */
	virtual bool ArgsHaveError() = 0;

	/**
	 * Return a string of text describing the list of errors encountered.
	 */
	virtual const char * PrintErrors() = 0;

	/**
	 * Return a string that contains the usage description of this list of paramaters.
	 */
	virtual const char * PrintUsage() = 0;

	/**
	 * Register an error with this parser.  This string will be appended to the
	 * errors already buffered in this object.
	 */
	virtual void RegisterError( const char * err_msg ) = 0;

	/**
	 * The method should be invoked by the main code once it has determined that
	 * the application should be started.
	 */
	virtual void PerformTask() = 0;

};

/**
 * MakeCommandOptionParse will create an implementation of a CommandOptionParse
 * object.  This particular implementation is a wrapper around getopt_long(3).
 * That interface unfortunatly does not provide enough information to give
 * the best error messages with malformed input.  If the implementation changes
 * there is a good chance that the binary interface will remain the same.
 *
 */
CommandOptionParse * MakeCommandOptionParse(
	int				   argc,
	char			** argv,
	char			 * comment,
	CommandOption	 * options = DefaultCommandOptionList
);
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

