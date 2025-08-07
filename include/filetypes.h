#ifndef FILETYPES_H
#define FILETYPES_H
#include "utilities.h"


class TeraChemOutput
{
    public:
        TeraChemOutput();
        ~TeraChemOutput();
        std::string get_calc_type();
    private:
        std::string calc_type;
};

class PrmtopFile
{
    private:
        std::string filename;
        int n_atoms;
        int n_residues;
        int n_molecules;
        int n_solvents;
        int n_counterions;
        double box_x;
        double box_y;
        double box_z;
    public:
        PrmtopFile();
        ~PrmtopFile();
        std::string get_filename();
};

class FileList
{
    private:
        std::map<std::string, std::string> files;
    public:
        FileList();
        ~FileList();
        void AddFile(std::string key, std::string filename);
        std::string GetFile(std::string key);
};

bool is_prmtop(std::string filename);
bool is_trajectory(std::string filename);
bool is_terachem(std::string filename);
bool is_amber_output(std::string filename);
bool is_amber_restart(std::string filename);

#endif