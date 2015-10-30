/** Copyright 2015 Christophe Vestri
*
* Convert an ASCII mnt file to a 16bits PNG image
*
*/
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <boost/spirit/include/qi.hpp>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace
{
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace 

int main(int argc, char **argv)
{
// variables
    string fileInname, fileOutname;
    size_t x = 4500, y = 11000;
    size_t w = 6000, h = 4000;
    
    // parsing options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("verbose,v", "print words with verbosity")
        ("x", po::value<int>(), "set x start")
        ("y", po::value<int>(), "set y start")
        ("w", po::value<int>(), "set width")
        ("h", po::value<int>(), "set height")
        ("filein", po::value<string>(&fileInname)->required(), "ASCII file name")
        ("fileout", po::value<string>(&fileOutname)->required(), "PNG image name")
        ;

    po::positional_options_description positionalOptions;
    positionalOptions.add("filein", 1);
    positionalOptions.add("fileout", 1);
    po::variables_map vm;

    // parse options
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc)
            .positional(positionalOptions).run(),
            vm); // throws on error 

        if (vm.count("help")) /* --help option */
        {
            std::cout << "Basic Command Line Parameter App" << std::endl << desc << std::endl;
            return SUCCESS;
        }

        po::notify(vm); // throws on error, so do after help in case there are any problems 
    }
    catch (boost::program_options::error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << "Basic Command Line Parameter App" << std::endl << desc << std::endl;
        return ERROR_IN_COMMAND_LINE;
    }

    // initialise values
    if (vm.count("x")) x = vm["x"].as<int>();
    if (vm.count("y")) y = vm["y"].as<int>();
    if (vm.count("w")) w = vm["w"].as<int>();
    if (vm.count("h")) h = vm["h"].as<int>();


    // read ASCII MNT file
    std::ifstream input(fileInname);
    if (!input.is_open())
    {
        // nothing to do
        return -1;
    }
    
    int win, hin ,itmp;
    string stmp;
    double dtmp, val;
    input >> stmp >> win; cout << "read: " << stmp << " + " << win << endl;
    input >> stmp >> hin; cout << "read: " << stmp << " + " << hin << endl;
    input >> stmp >> itmp; cout << "read: " << stmp << " + " << itmp << endl;
    input >> stmp >> itmp; cout << "read: " << stmp << " + " << itmp << endl;
    input >> stmp >> itmp; cout << "read: " << stmp << " + " << itmp << endl;
    input >> stmp >> itmp; cout << "read: " << stmp << " + " << itmp << endl;

    // Create mat test
    Mat mat(h, w, CV_16UC1);

    // parse file and stop when image is written
    for (int j = 0; j < (y + h); ++j) {
        for (int i = 0; i < win; ++i) {
            input >> dtmp;
            if ((i >= x) && (i < (x+w)) && (j >= y) && (j < (y+h)))
            {
                val = (dtmp <= 0) ? 0 : dtmp * 10;
                mat.at<unsigned short>(j - y, i - x) = saturate_cast<ushort>(val);
            //    cout << dtmp << " -> " << val << endl;
            }
        }
    }

    imwrite(fileOutname, mat);

return 0;
}
