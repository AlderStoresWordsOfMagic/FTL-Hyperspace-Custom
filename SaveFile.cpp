#include "Global.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
/*
HOOK_STATIC(FileHelper, readBinaryFile, (const std::string& fileName) -> int)
{
    if (boost::algorithm::ends_with(fileName, "ae_prof.sav"))
    {
        std::string newFile = fileName;
        newFile.replace(fileName.size()-11, fileName.size(), "hs_prof.sav");

        if (FileHelper::fileExists(newFile))
        {
            return super(newFile);
        }
        else
        {
            int file = super(fileName);
            int fileLen = FileHelper::fileLength(file);
            char* bin = FileHelper::readBuffer(file, fileLen, false);
            FileHelper::closeBinaryFile(file);

            file = FileHelper::createBinaryFile(newFile);
            FileHelper::writeData(file, bin, fileLen);
            FileHelper::closeBinaryFile(file);

            return super(newFile);
        }
    }

    return super(fileName);
}


HOOK_STATIC(FileHelper, createBinaryFile, (const std::string& fileName) -> int)
{
    if (boost::algorithm::ends_with(fileName, "ae_prof.sav"))
    {
        std::string newFile = fileName;
        newFile.replace(fileName.size()-11, fileName.size(), "hs_prof.sav");

        return super(newFile);
    }

    return super(fileName);
}
*/

HOOK_STATIC(FileHelper, getSaveFile, (std::string& str) -> std::string&)
{
    auto ret = super(str);

    str.replace(str.size()-12, str.size(), "hs_continue.sav");

    return ret;
}