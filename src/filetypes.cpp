#include "filetypes.h"

FileList::FileList()
{
    files = {};
}
FileList::~FileList()
{
    
}

void FileList::AddFile(std::string key, std::string filename)
{
    files[key] = fs::absolute(filename);
}

std::string FileList::GetFile(std::string key)
{
    return files[key];
}

// class TeraChemOutput
// {
//     public:
//         TeraChemOutput();
//         ~TeraChemOutput();
//         std::string get_calc_type();
//     private:
//         std::string calc_type;
// };

// class PrmtopFile
// {
//     private:
//         std::string filename;
//         int n_atoms;
//         int n_residues;
//         int n_molecules;
//         int n_solvents;
//         int n_counterions;
//         double box_x;
//         double box_y;
//         double box_z;
//     public:
//         PrmtopFile();
//         ~PrmtopFile();
//         std::string get_filename();
// };


std::string get_filetype(std::string filename)
{
    if (is_prmtop(filename))
    {
        return "PRMTOP";
    }
    if (is_trajectory(filename))
    {
        return "TRAJECTORY";
    }
    if (is_terachem(filename))
    {
        return "TERACHEM";
    }
    if (is_amber_output(filename))
    {
        return "AMBER_OUTPUT";
    }
    if (is_amber_restart(filename))
    {
        return "AMBER_RESTART";
    }
}

bool is_prmtop(std::string filename)
{
    // TITLE and POINTERS sections MUST be present in the prmtop.
    bool found_title=false;
    bool found_pointers=false;
    bool found_version=false;
    std::ifstream ifile(filename,std::ios::in);
    std::string line;
    while (getline(ifile,line))
    {
        if (line.find("%VERSION")!= std::string::npos)
        {
            found_version = true;
        }
        else if (line.find("%FLAG POINTERS") != std::string::npos)
        {
            found_pointers = true;
        }
        else if (line.find("%FLAG TITLE") != std::string::npos)
        {
            found_title = true;
        }
        if (found_title && found_pointers && found_version)
        {
            ifile.close();
            return true;
        }
    }
    ifile.close();
    return false;
}

bool is_trajectory(std::string filename)
{
    return true;
}

bool is_terachem(std::string filename)
{   
    bool found_terachem = false;
    bool single_point_energy = false;
    bool resp = false;
    bool optimization = false;
    bool bomd = false;

    std::ifstream ifile(filename, std::ios::in);
    std::string line;
    while (getline(ifile,line))
    {
        if (line.find("TeraChem v") != std::string::npos)
        {
            found_terachem = true;
        }
        else if (line.find("run energy") != std::string::npos)
        {
            single_point_energy = true;
        }
        else if (line.find("resp yes") != std::string::npos)
        {
            resp = true;
        }
        else if (line.find("run minimize")!= std::string::npos)
        {
            optimization = true;
        }
        else if (line.find("run md") != std::string::npos)
        {
            bomd = true;
        }

        if (found_terachem)
        {
            ifile.close();
            if (single_point_energy)
            {
                if (resp)
                {
                    return "RESP";
                }
                return "SPE";
            }               
            if (optimization)
            {
                return "OPT";
            }
            if (bomd)
            {
                return "BOMD";
            }
            return "OTHER_TC";
        }        
    }
    ifile.close();
    return false;
}

bool is_amber_output(std::string filename)
{
    return true;
}

bool is_amber_restart(std::string filename)
{
    return true;
}

