#ifndef KANGAROON_INLCUDE_FILE_WRITER_H_
#define KANGAROON_INLCUDE_FILE_WRITER_H_
#include <stdint.h>
namespace zoo
{
    namespace kangaroon
    {
        enum class FileWriterType : uint8_t{
            MMAPFILE = 0,
            APPENDFILE
        };
    } // namespace kangaroon
    
} // namespace zoo


#endif