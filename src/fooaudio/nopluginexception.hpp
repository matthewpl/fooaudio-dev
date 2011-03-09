#ifndef NOPLUGINEXCEPTION_HPP
#define NOPLUGINEXCEPTION_HPP

#include <exception>
#include <string>

class NoPluginException : public std::exception
{
public:
	NoPluginException(const char* msg)
	{
		this->msg = msg;
	}

	virtual ~NoPluginException() throw()
	{
	}

	virtual const char* what() const throw()
	{
		return msg;
	}

private:
	const char* msg;
};

#endif // NOPLUGINEXCEPTION_HPP
