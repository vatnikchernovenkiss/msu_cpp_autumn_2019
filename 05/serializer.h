#include <iostream>
#include <string.h>
#include <cstdlib>
#include <climits>
#pragma once

enum class Error
{
	NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
		return process(std::forward<ArgsT>(args)...);
    }
    
private:
    std::ostream& out_;
    template<class Current, class... Rest>
	Error process(Current&& cur, Rest&&... tail) {
		if (process(std::forward<Current>(cur)) != Error::NoError) 
			return Error::CorruptedArchive;
		return process(std::forward<Rest>(tail)...);
	}

	Error process()
	{
		return Error::NoError;
	}
	
	Error process(bool val)
	{
		if (val)
			out_ << "true" << Separator;
		else
			out_ << "false" << Separator;
		return Error::NoError;
	}
	
	Error process(uint64_t val)
	{
		out_ << val << Separator;
		return Error::NoError;
	}
		
};

class Deserializer
{
public:
	explicit Deserializer(std::istream& in)
		: in_(in)
	{
	}

	template <class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}
	
private:
	std::istream& in_;
	Error process(bool& value)
	{
		std::string text;
		in_ >> text;
		if (text == "true")
			value = true;
		else if (text == "false")
			value = false;
		else
			return Error::CorruptedArchive;
		return Error::NoError;
	}
	
	Error process()
	{
		return Error::NoError;
	}
	
	Error process(uint64_t& value)
	{
		std::string text;
		in_ >> text;
		value = std::stoull(text.c_str());
		if (text[0] == '-' || errno) 
			return Error::CorruptedArchive;
		return Error::NoError;
	}

	template<class Current, class... Rest>
   	Error process(Current &&cur, Rest&&... tail) {
		if (process(std::forward<Current>(cur)) != Error::NoError) 
			return Error::CorruptedArchive;
		return process(std::forward<Rest>(tail)...);
	}
};
