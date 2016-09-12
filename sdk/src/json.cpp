#include "json.hpp"
#include <string.h>
#include <math.h>
#include <sstream>


#if defined(_WIN32) || defined(_WIN64) 
#define snprintf _snprintf 
#define vsnprintf _vsnprintf 
#define strcasecmp _stricmp 
#define strncasecmp _strnicmp 
#endif



namespace quotek {
  namespace json {

    parser::parser(){}


    node* parser::parse(const char* data)
    {
    	// Skip any preceding whitespace, end of data = no JSON = fail
    	if (!SkipWhitespace(&data))
    		return NULL;

    	// We need the start of a value here now...
    	node *value = node::parse(&data);
    	if (value == NULL)
    		return NULL;
    	
    	// Can be white space now and should be at the end of the string then...
    	if (SkipWhitespace(&data))
    	{
    		delete value;
    		return NULL;
    	}
    	
    	// We're now at the end of the string
    	return value;
    }

    std::string parser::stringify(const node *value)
    {
    	if (value != NULL)
    		return value->stringify();
    	else
    		return "";
    }

    bool parser::SkipWhitespace(const char **data)
    {
    	while (**data != 0 && (**data == L' ' || **data == L'\t' || **data == L'\r' || **data == L'\n'))
    		(*data)++;
    	
    	return **data != 0;
    }


    bool parser::ExtractString(const char **data, std::string &str)
    {
    	str = "";
    	
    	while (**data != 0)
    	{
    		// Save the char so we can change it if need be
    		char next_char = **data;
    		
    		// Escaping something?
    		if (next_char == L'\\')
    		{
    			// Move over the escape char
    			(*data)++;
    			
    			// Deal with the escaped char
    			switch (**data)
    			{
    				case L'"': next_char = L'"'; break;
    				case L'\\': next_char = L'\\'; break;
    				case L'/': next_char = L'/'; break;
    				case L'b': next_char = L'\b'; break;
    				case L'f': next_char = L'\f'; break;
    				case L'n': next_char = L'\n'; break;
    				case L'r': next_char = L'\r'; break;
    				case L't': next_char = L'\t'; break;
    				case L'u':
    				{
    					// We need 5 chars (4 hex + the 'u') or its not valid
    					if (!simplejson_wcsnlen(*data, 5))
    						return false;
    					
    					// Deal with the chars
    					next_char = 0;
    					for (int i = 0; i < 4; i++)
    					{
    						// Do it first to move off the 'u' and leave us on the
    						// final hex digit as we move on by one later on
    						(*data)++;
    						
    						next_char <<= 4;
    						
    						// Parse the hex digit
    						if (**data >= '0' && **data <= '9')
    							next_char |= (**data - '0');
    						else if (**data >= 'A' && **data <= 'F')
    							next_char |= (10 + (**data - 'A'));
    						else if (**data >= 'a' && **data <= 'f')
    							next_char |= (10 + (**data - 'a'));
    						else
    						{
    							// Invalid hex digit = invalid JSON
    							return false;
    						}
    					}
    					break;
    				}
    				
    				// By the spec, only the above cases are allowed
    				default:
    					return false;
    			}
    		}
    		
    		// End of the string?
    		else if (next_char == L'"')
    		{
    			(*data)++;
    			str.reserve(); // Remove unused capacity
    			return true;
    		}
    		
    		// Disallowed char?
    		else if (next_char < L' ' && next_char != L'\t')
    		{
    			// SPEC Violation: Allow tabs due to real world cases
    			return false;
    		}
    		
    		// Add the next char
    		str += next_char;
    		
    		// Move on
    		(*data)++;
    	}
    	
    	// If we're here, the string ended incorrectly
    	return false;
    }


    double parser::ParseInt(const char **data)
    {
    	double integer = 0;
    	while (**data != 0 && **data >= '0' && **data <= '9')
    		integer = integer * 10 + (*(*data)++ - '0');
    	
    	return integer;
    }


    double parser::ParseDecimal(const char **data)
    {
    	double decimal = 0.0;
      double factor = 0.1;
    	while (**data != 0 && **data >= '0' && **data <= '9')
      {
        int digit = (*(*data)++ - '0');
    		decimal = decimal + digit * factor;
        factor *= 0.1;
      }
    	return decimal;
    }


    //**** NODE ROUTINES ****//

    #define FREE_ARRAY(x) { jarray::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete *iter; } }
    #define FREE_OBJECT(x) { jobject::iterator iter; for (iter = x.begin(); iter != x.end(); iter++) { delete (*iter).second; } }

    node *node::parse(const char **data)
    {
    	// Is it a string?
    	if (**data == '"')
    	{
    		std::string str;
    		if (!quotek::json::parser::ExtractString(&(++(*data)), str))
    			return NULL;
    		else
    			return new node(str);
    	}

    	// Is it a boolean?
    	else if ((simplejson_wcsnlen(*data, 4) && strcasecmp(*data, "true") == 0) || (simplejson_wcsnlen(*data, 5) && strcasecmp(*data, "false") == 0))
    	{
    		bool value = strcasecmp(*data, "true") == 0;
    		(*data) += value ? 4 : 5;
    		return new node(value);
    	}

    	// Is it a null?
    	else if (simplejson_wcsnlen(*data, 4) && strcasecmp(*data, "null") == 0)
    	{
    		(*data) += 4;
    		return new node();
    	}

    	// Is it a number?
    	else if (**data == L'-' || (**data >= L'0' && **data <= L'9'))
    	{
    		// Negative?
    		bool neg = **data == L'-';
    		if (neg) (*data)++;

    		double number = 0.0;

    		// Parse the whole part of the number - only if it wasn't 0
    		if (**data == L'0')
    			(*data)++;
    		else if (**data >= L'1' && **data <= L'9')
    			number = quotek::json::parser::ParseInt(data);
    		else
    			return NULL;

    		// Could be a decimal now...
    		if (**data == '.')
    		{
    			(*data)++;

    			// Not get any digits?
    			if (!(**data >= L'0' && **data <= L'9'))
    				return NULL;

    			// Find the decimal and sort the decimal place out
    			// Use ParseDecimal as ParseInt won't work with decimals less than 0.1
    			// thanks to Javier Abadia for the report & fix
    			double decimal = quotek::json::parser::ParseDecimal(data);

    			// Save the number
    			number += decimal;
    		}

    		// Could be an exponent now...
    		if (**data == L'E' || **data == L'e')
    		{
    			(*data)++;

    			// Check signage of expo
    			bool neg_expo = false;
    			if (**data == L'-' || **data == L'+')
    			{
    				neg_expo = **data == L'-';
    				(*data)++;
    			}

    			// Not get any digits?
    			if (!(**data >= L'0' && **data <= L'9'))
    				return NULL;

    			// Sort the expo out
    			double expo = quotek::json::parser::ParseInt(data);
    			for (double i = 0.0; i < expo; i++)
    				number = neg_expo ? (number / 10.0) : (number * 10.0);
    		}

    		// Was it neg?
    		if (neg) number *= -1;

    		return new node(number);
    	}

    	// An object?
    	else if (**data == L'{')
    	{
    		jobject object;

    		(*data)++;

    		while (**data != 0)
    		{
    			// Whitespace at the start?
    			if (!quotek::json::parser::SkipWhitespace(data))
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// Special case - empty object
    			if (object.size() == 0 && **data == L'}')
    			{
    				(*data)++;
    				return new node(object);
    			}

    			// We want a string now...
    			std::string name;
    			if (!quotek::json::parser::ExtractString(&(++(*data)), name))
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// More whitespace?
    			if (!quotek::json::parser::SkipWhitespace(data))
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// Need a : now
    			if (*((*data)++) != L':')
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// More whitespace?
    			if (!quotek::json::parser::SkipWhitespace(data))
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// The value is here
    			node *value = parse(data);
    			if (value == NULL)
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// Add the name:value
    			if (object.find(name) != object.end())
    				delete object[name];
    			object[name] = value;

    			// More whitespace?
    			if (!quotek::json::parser::SkipWhitespace(data))
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			// End of object?
    			if (**data == L'}')
    			{
    				(*data)++;
    				return new node(object);
    			}

    			// Want a , now
    			if (**data != L',')
    			{
    				FREE_OBJECT(object);
    				return NULL;
    			}

    			(*data)++;
    		}

    		// Only here if we ran out of data
    		FREE_OBJECT(object);
    		return NULL;
    	}

    	// An array?
    	else if (**data == L'[')
    	{
    		jarray array;

    		(*data)++;

    		while (**data != 0)
    		{
    			// Whitespace at the start?
    			if (!quotek::json::parser::SkipWhitespace(data))
    			{
    				FREE_ARRAY(array);
    				return NULL;
    			}

    			// Special case - empty array
    			if (array.size() == 0 && **data == L']')
    			{
    				(*data)++;
    				return new node(array);
    			}

    			// Get the value
    			node *value = parse(data);
    			if (value == NULL)
    			{
    				FREE_ARRAY(array);
    				return NULL;
    			}

    			// Add the value
    			array.push_back(value);

    			// More whitespace?
    			if (!quotek::json::parser::SkipWhitespace(data))
    			{
    				FREE_ARRAY(array);
    				return NULL;
    			}

    			// End of array?
    			if (**data == L']')
    			{
    				(*data)++;
    				return new node(array);
    			}

    			// Want a , now
    			if (**data != L',')
    			{
    				FREE_ARRAY(array);
    				return NULL;
    			}

    			(*data)++;
    		}

    		// Only here if we ran out of data
    		FREE_ARRAY(array);
    		return NULL;
    	}

    	// Ran out of possibilites, it's bad!
    	else
    	{
    		return NULL;
    	}
    }

    node::node(/*NULL*/)
    {
    	type = JSONType_Null;
    }

    node::node(const char *m_char_value)
    {
    	type = JSONType_String;
    	string_value = new std::string(std::string(m_char_value));
    }

    node::node(const std::string &m_string_value)
    {
    	type = JSONType_String;
    	string_value = new std::string(m_string_value);
    }

    node::node(bool m_bool_value)
    {
    	type = JSONType_Bool;
    	bool_value = m_bool_value;
    }

    node::node(double m_number_value)
    {
    	type = JSONType_Number;
    	number_value = m_number_value;
    }

  
    node::node(const jarray &m_array_value)
    {
    	type = JSONType_Array;
    	array_value = new jarray(m_array_value);
    }

    node::node(const jobject &m_object_value)
    {
    	type = JSONType_Object;
    	object_value = new jobject(m_object_value);
    }

    node::node(const node &m_source)
    {
    	type = m_source.type;

    	switch (type)
    	{
    		case JSONType_String:
    			string_value = new std::string(*m_source.string_value);
    			break;

    		case JSONType_Bool:
    			bool_value = m_source.bool_value;
    			break;

    		case JSONType_Number:
    			number_value = m_source.number_value;
    			break;

    		case JSONType_Array:
    		{
    			jarray source_array = *m_source.array_value;
    			jarray::iterator iter;
    			array_value = new jarray();
    			for (iter = source_array.begin(); iter != source_array.end(); iter++)
    				array_value->push_back(new node(**iter));
    			break;
    		}

    		case JSONType_Object:
    		{
    			jobject source_object = *m_source.object_value;
    			object_value = new jobject();
    			jobject::iterator iter;
    			for (iter = source_object.begin(); iter != source_object.end(); iter++)
    			{
    				std::string name = (*iter).first;
    				(*object_value)[name] = new node(*((*iter).second));
    			}
    			break;
    		}

    		case JSONType_Null:
    			// Nothing to do.
    			break;
    	}
    }

    node::~node()
    {
    	if (type == JSONType_Array)
    	{
    		jarray::iterator iter;
    		for (iter = array_value->begin(); iter != array_value->end(); iter++)
    			delete *iter;
    		delete array_value;
    	}
    	else if (type == JSONType_Object)
    	{
    		jobject::iterator iter;
    		for (iter = object_value->begin(); iter != object_value->end(); iter++)
    		{
    			delete (*iter).second;
    		}
    		delete object_value;
    	}
    	else if (type == JSONType_String)
    	{
    		delete string_value;
    	}
    }

    bool node::IsNull() const
    {
    	return type == JSONType_Null;
    }

    bool node::IsString() const
    {
    	return type == JSONType_String;
    }


    bool node::IsBool() const
    {
    	return type == JSONType_Bool;
    }

    bool node::IsNumber() const
    {
    	return type == JSONType_Number;
    }

    bool node::IsArray() const
    {
    	return type == JSONType_Array;
    }

    
    bool node::IsObject() const
    {
    	return type == JSONType_Object;
    }

    
    const std::string &node::AsString() const
    {
    	return (*string_value);
    }

   
    bool node::AsBool() const
    {
    	return bool_value;
    }

    
    double node::AsNumber() const
    {
    	return number_value;
    }

    
    const jarray &node::AsArray() const
    {
    	return (*array_value);
    }

   
    const jobject &node::AsObject() const
    {
    	return (*object_value);
    }

    std::size_t node::CountChildren() const
    {
    	switch (type)
    	{
    		case JSONType_Array:
    			return array_value->size();
    		case JSONType_Object:
    			return object_value->size();
    		default:
    			return 0;
    	}
    }

    bool node::HasChild(std::size_t index) const
    {
    	if (type == JSONType_Array)
    	{
    		return index < array_value->size();
    	}
    	else
    	{
    		return false;
    	}
    }


    node* node::child(std::size_t index)
    {
    	if (index < array_value->size())
    	{
    		return (*array_value)[index];
    	}
    	else
    	{
    		return NULL;
    	}
    }

    bool node::HasChild(const char* name) const
    {
    	if (type == JSONType_Object)
    	{
    		return object_value->find(name) != object_value->end();
    	}
    	else
    	{
    		return false;
    	}
    }


    node* node::child(const char* name)
    {
    	jobject::const_iterator it = object_value->find(name);
    	if (it != object_value->end())
    	{
    		return it->second;
    	}
    	else
    	{
    		return NULL;
    	}
    }


    std::vector<std::string> node::ObjectKeys() const
    {
    	std::vector<std::string> keys;

    	if (type == JSONType_Object)
    	{
    		jobject::const_iterator iter = object_value->begin();
    		while (iter != object_value->end())
    		{
    			keys.push_back(iter->first);

    			iter++;
    		}
    	}

    	return keys;
    }

    
    std::string node::stringify(bool const prettyprint) const
    {
    	size_t const indentDepth = prettyprint ? 1 : 0;
    	return StringifyImpl(indentDepth);
    }


   
    std::string node::StringifyImpl(size_t const indentDepth) const
    {
    	std::string ret_string;
    	size_t const indentDepth1 = indentDepth ? indentDepth + 1 : 0;
    	std::string const indentStr = Indent(indentDepth);
    	std::string const indentStr1 = Indent(indentDepth1);

    	switch (type)
    	{
    		case JSONType_Null:
    			ret_string = "null";
    			break;

    		case JSONType_String:
    			ret_string = StringifyString(*string_value);
    			break;

    		case JSONType_Bool:
    			ret_string = bool_value ? "true" : "false";
    			break;

    		case JSONType_Number:
    		{
    			if (isinf(number_value) || isnan(number_value))
    				ret_string = "null";
    			else
    			{
    				std::stringstream ss;
    				ss.precision(15);
    				ss << number_value;
    				ret_string = ss.str();
    			}
    			break;
    		}

    		case JSONType_Array:
    		{
    			ret_string = indentDepth ? "[\n" + indentStr1 : "[";
    			jarray::const_iterator iter = array_value->begin();
    			while (iter != array_value->end())
    			{
    				ret_string += (*iter)->StringifyImpl(indentDepth1);

    				// Not at the end - add a separator
    				if (++iter != array_value->end())
    					ret_string += ",";
    			}
    			ret_string += indentDepth ? "\n" + indentStr + "]" : "]";
    			break;
    		}

    		case JSONType_Object:
    		{
    			ret_string = indentDepth ? "{\n" + indentStr1 : "{";
    			jobject::const_iterator iter = object_value->begin();
    			while (iter != object_value->end())
    			{
    				ret_string += StringifyString((*iter).first);
    				ret_string += ":";
    				ret_string += (*iter).second->StringifyImpl(indentDepth1);

    				// Not at the end - add a separator
    				if (++iter != object_value->end())
    					ret_string += ",";
    			}
    			ret_string += indentDepth ? "\n" + indentStr + "}" : "}";
    			break;
    		}
    	}

    	return ret_string;
    }

    
    std::string node::StringifyString(const std::string &str)
    {
    	std::string str_out = "\"";

    	std::string::const_iterator iter = str.begin();
    	while (iter != str.end())
    	{
    		char chr = *iter;

    		if (chr == L'"' || chr == L'\\' || chr == L'/')
    		{
    			str_out += L'\\';
    			str_out += chr;
    		}
    		else if (chr == L'\b')
    		{
    			str_out += "\\b";
    		}
    		else if (chr == L'\f')
    		{
    			str_out += "\\f";
    		}
    		else if (chr == L'\n')
    		{
    			str_out += "\\n";
    		}
    		else if (chr == L'\r')
    		{
    			str_out += "\\r";
    		}
    		else if (chr == L'\t')
    		{
    			str_out += "\\t";
    		}
    		else if (chr < L' ' || chr > 126)
    		{
    			str_out += "\\u";
    			for (int i = 0; i < 4; i++)
    			{
    				int value = (chr >> 12) & 0xf;
    				if (value >= 0 && value <= 9)
    					str_out += (char)('0' + value);
    				else if (value >= 10 && value <= 15)
    					str_out += (char)('A' + (value - 10));
    				chr <<= 4;
    			}
    		}
    		else
    		{
    			str_out += chr;
    		}

    		iter++;
    	}

    	str_out += "\"";
    	return str_out;
    }


    std::string node::Indent(size_t depth)
    {
    	const size_t indent_step = 2;
    	depth ? --depth : 0;
    	std::string indentStr(depth * indent_step, ' ');
    	return indentStr;
    }

  }
}