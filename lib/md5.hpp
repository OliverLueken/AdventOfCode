#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "/usr/lib/cryptopp/md5.h"
#include "/usr/lib/cryptopp/hex.h"
#include "/usr/lib/cryptopp/files.h"

#include <string>
#include <sstream>

// clear && g++ tasks.cpp -std=c++20 -Wpedantic -Wall -Wextra -Wconversion -L/usr/lib/cryptopp/ -lcryptopp

namespace MD5{
    auto getMD5Hash(std::string message){
        using namespace CryptoPP;

        static Weak::MD5 hash;
        static std::string digest;

        std::stringstream stream;
        hash.Update((const byte*) &message[0], message.size());
        digest.resize(hash.DigestSize());
        hash.TruncatedFinal((byte*)&digest[0], digest.size());

        HexEncoder encoder(new FileSink(stream));
        StringSource(digest, true, new Redirector(encoder));
        return stream.str();
    }
}