/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdlib>

namespace quotek {

  namespace json {

    enum JSONType { JSONType_Null, 
    	            JSONType_String, 
    	            JSONType_Bool, 
    	            JSONType_Number, 
    	            JSONType_Array, 
    	            JSONType_Object };


    static inline bool simplejson_wcsnlen(const char *s, size_t n) {
    	if (s == 0)
    		return false;

    	const char *save = s;
    	while (n-- > 0)
    	{
    		if (*(save++) == 0) return false;
    	}

    	return true;
    }


    class parser;
    class node;

    typedef std::vector<quotek::json::node*> jarray;
    typedef std::map<std::string, quotek::json::node*> jobject;

    class node {

    	friend class quotek::json::parser;

    	public:

    		node(/*NULL*/);
    		node(const char *m_char_value);
    		node(const std::string &m_string_value);
    		node(bool m_bool_value);
    		node(double m_number_value);
    		node(const jarray &m_array_value);
    		node(const jobject &m_object_value);
    		node(const node &m_source);
    		~node();

    		bool IsNull() const;
    		bool IsString() const;
    		bool IsBool() const;
    		bool IsNumber() const;
    		bool IsArray() const;
    		bool IsObject() const;

    		const std::string &AsString() const;
    		bool AsBool() const;
    		double AsNumber() const;
    		const jarray &AsArray() const;
    		const jobject &AsObject() const;

    		std::size_t CountChildren() const;
    		bool HasChild(std::size_t index) const;
    		
    		bool HasChild(const char* name) const;
    		node* child(std::size_t index);
    		node* child(const char* name);
    		std::vector<std::string> ObjectKeys() const;

    		std::string stringify(bool const prettyprint = false) const;

            JSONType type;

    	protected:
    		static node *parse(const char **data);

    	private:
    		static std::string StringifyString(const std::string &str);
    		std::string StringifyImpl(size_t const indentDepth) const;
    		static std::string Indent(size_t depth);

    		

    		union
    		{
    			bool bool_value;
    			double number_value;
    			std::string *string_value;
    			jarray *array_value;
    			jobject *object_value;
    		};

    };

    class parser
    {
    	friend class quotek::json::node;
    	
    	public:
    		static node* parse(const char *data);
    		static std::string stringify(const node *value);
    	protected:
    		static bool SkipWhitespace(const char **data);
    		static bool ExtractString(const char **data, std::string &str);
    		static double ParseInt(const char **data);
    		static double ParseDecimal(const char **data);
    	private:
    		parser();
    };

  }
}

#endif
