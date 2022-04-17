#ifndef STREAMENCRYPTER_HPP
#define STREAMENCRYPTER_HPP

// "abstract" class
class StreamEncrypter {
    public:
    virtual int encrypt(char* input, char* output, int length) = 0;
    virtual int decrypt(char* input, char* output, int length) = 0;
    virtual int getEncryptionOverhead() = 0;
};

#endif