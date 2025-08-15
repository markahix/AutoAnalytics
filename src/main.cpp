#include "utilities.h"
#include "filetypes.h"

extern bool DEBUG;

int main(int argc, char** argv)
{
    // identify type of data coming in:  MD simulation, QM/MM, Optimization, Single Point, etc.
    // Maybe use command line flags?
    // --md
    // --qmmm
    // --opt
    // --spe
    // or just a list of files and then I determine what kind of files they are?
    // The ideal solution would be to build submodules in here like I did for AutoMD, then have a main thing just call the relevant pieces as I process the files I'm given...

    // Okay, so identify the filetype(s) given to the command line.  Then direct appropriately?

    FileList files;
    for (int i = 1; i < argc; i++)
    {
        if (fs::exists(argv[i]))
        {
            files.AddFile(std::to_string(i),argv[i]);
        }
        else if (argv[i] == "--debug")
        {
            DEBUG=true;
        }
        else
        {
            std::cout << argv[i] << " not found. " << std::endl;
        }
    }



}