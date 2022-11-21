#ifndef __FILE_READ_H
#define __FILE_READ_H

#include <iostream>
#include <fstream>
#include <string>

#include "data_type.h"

namespace file
{

class FileRead
{
public:
        FileRead()
        {

        }

        ~FileRead()
        {

        }

        bool openFile( const std::string& file_name )
        {
                infile.open( file_name, std::ios::binary );
                if( !infile.is_open() ){
                        std::cerr<<"Failed to Open the TEST File !"<<std::endl;
                        return false;
                }

                std::cerr<<"Open the TEST File !"<<std::endl;
                return true;
        }

	bool readAFrame( sensor::LaserScan& scan )
        {
                if( !infile.eof() ){
                        memset( &scan, 0, sizeof( scan ) );
                        std::cout<<"-------------------------- Read A Frame of data ------------------------"<<std::endl;
                        infile.read( reinterpret_cast<char *>( &scan ), sizeof( scan ) );
                }
                else {
                        return false;
                }

                return true;
        }

	bool readAFrame( sensor::RecordData& data )
        {
                if( !infile.eof() ){
                        memset( &data, 0, sizeof( data ) );
                        std::cout<<"-------------------------- Read A Frame of data ------------------------"<<std::endl;
                        infile.read( reinterpret_cast<char *>( &data ), sizeof( data ) );
                }
                else {
                        return false;
                }

                return true;
        }

        const int  endOfFile()
        {
                return infile.eof();
        }

        void closeFile()
        {
                return infile.close();
        }

private:
        std::ifstream infile;
};

}

#endif
