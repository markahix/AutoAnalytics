
#include "utilities.h"
// #include "classes.h"
// #include "slurm.h"

// namespace fs = std::experimental::filesystem;
extern bool DEBUG = false;
void error_log(std::string message,int exit_code)
{
    std::cerr << message << std::endl;
    exit(exit_code);
}

void normal_log(std::string message)
{
    std::cout << message << std::endl;
}

void debug_log(std::string message)
{
    if (DEBUG)
    {
        std::cout << "DEBUG: " << message << std::endl;
    }
}

namespace utils
{
    void splash_screen()
    {
        std::cout << "" << std::endl;
        std::cout << "Welcome to the AutoAnalytics!" << std::endl;
        std::cout << "" << std::endl;
    }
    void silent_shell(const char* cmd)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
                result += buffer.data();
        }
    }

    std::string GetSysResponse(const char* cmd)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
                result += buffer.data();
        }
        return result;
    }

    bool CheckProgAvailable(const char* program)
    {
            std::string result;
            std::string cmd;
            cmd = "which ";
            cmd += program;
            result=GetSysResponse(cmd.c_str());
            if (result.empty())
            {
                std::cout << "Missing program: " << program << std::endl;
                return false;
            }
            return true;
    }

    void write_to_file(std::string inputfilename, std::string buffer)
    {
        std::ofstream outFile;
        outFile.open(inputfilename,std::ios::out);
        outFile << buffer;
        outFile.close();
    }

    void append_to_file(std::string inputfilename, std::string buffer)
    {
        if (!CheckFileExists(inputfilename))
        {
            write_to_file(inputfilename,"");
        }
        std::ofstream outFile;
        outFile.open(inputfilename,std::ios::app);
        outFile << buffer;
        outFile.close();
    }

    bool IsFlag(char* bigstring) //checks if a string is a command line flag or a value.
    {
        if (bigstring[0] == '-')
            return true;
        return false;
    }

    void ReadArgs(int argc, char** argv, std::vector<std::vector<std::string>>& flags)
    {
        int j = 0;
        for (int i = 1; i < argc; i++)
        {
            if (IsFlag(argv[i]))
            {
                j++;
                flags.push_back({argv[i]});
            }
            else
                flags[j].push_back(argv[i]);
        }
    }

    int FindFlag(std::vector<std::vector<std::string>>& flags, char* target)
    {
        for (unsigned int i = 1; i<flags.size(); i++)
        {
            if (flags[i][0] == target)
                return i;
        }
        return 0;
    }
 
    bool CheckFileExists(std::string filename)
    {
        if ( fs::exists(filename) )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
 
    std::string GetTimeAndDate()
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::stringstream buffer;
        buffer.str("");
        buffer << std::put_time(&tm, "%Y.%m.%d %H:%M:%S");
        return buffer.str();
    }
 
    int is_empty(const char *s) 
    {
        while (*s != '\0') 
        {
            if (!isspace((unsigned char)*s))
            return 0;
            s++;
        }
        return 1;
    }
 
    std::string LastLineOfFile(std::string filename)
    {
        std::ifstream fin;
        fin.open(filename);
        std::string line;
        std::vector<std::string> lines_in_order;
        while (std::getline(fin, line))
        {   
            if ( !is_empty(line.c_str()) )
            {
                lines_in_order.push_back(line);
            }
        }
        fin.close();
        return lines_in_order[lines_in_order.size()-1];
    }

    int count_lines_in_file(std::string filename)
    {
        std::string dummy;
        int count = 0;
        std::ifstream file(filename);
        while (getline(file,dummy))
        {
            count ++;
        }
        return count;
    }

    std::string string_between(std::string incoming, std::string first_delim, std::string second_delim)
    {
        unsigned first = incoming.find(first_delim);
        if (first == std::string::npos)
        {
            return incoming;
        }
        incoming = incoming.substr(first+1,incoming.size() - first -1);

        unsigned last = incoming.find(second_delim);
        if (last == std::string::npos)
        {
            return incoming.substr(0, incoming.size());
        }
        return incoming.substr(0, last);
    }
    
    void compress_and_delete(std::string directory)
    {
        std::stringstream buffer;
        buffer.str("");
        buffer << "tar -czvf " << directory << ".tar.gz "<< directory << "/ && rm -r " << directory << "/";
        silent_shell(buffer.str().c_str());
    }

    std::vector<std::string> sort_files_by_timestamp(std::string directory,std::string pattern)
    {
        std::set <fs::path> sort_by_name;
        for (fs::path p : fs::directory_iterator(directory))
        {
            if (p.extension() == pattern) 
            {
                sort_by_name.insert(p);
            }    
        }
        std::vector<std::string> file_list={};

        for (auto p : sort_by_name)
        {
            std::cout << p << std::endl;
            file_list.push_back(p);
        }
        
        return file_list;
    }
}
