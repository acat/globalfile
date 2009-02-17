#pragma once

using namespace boost::spirit;

class Ini
{
    map<string, map<string, string> > values_;

public:
    Ini(const char* filename) {
        ifstream is(filename);
        string content;
        if (is) {
            while (!is.eof()) {
                char buf[1024];
                is.read(buf, sizeof(buf));
                if (is.gcount()) {
                    content.append(buf, is.gcount());
                }
            }
        }

        string section_name, key, value;
        map<string, string> values;
        rule<> comment = *((ch_p(';') >> *(anychar_p - eol_p) >> eol_p) | space_p);
        rule<> r = *(comment >> '[' >> (+alpha_p)[assign_a(section_name)] >> ch_p(']')[clear_a(values)] >> *( comment >> (+alpha_p)[assign_a(key)] >> *space_p >> '=' >> *space_p >> (+(~space_p))[assign_a(value)])[insert_at_a(values, key, value)])[insert_at_a(values_, section_name, values)] >> comment;
        parse_info<> hit = parse(content.c_str(), r);
    }

    string get(const string& section, const string& name) {
        map<string, map<string, string> >::const_iterator it = values_.find(section);
        if (it != values_.end()) {
            map<string, string>::const_iterator it2 = it->second.find(name);
            if (it2 != it->second.end())
                return it2->second;
        }
        return string();
    }
};
