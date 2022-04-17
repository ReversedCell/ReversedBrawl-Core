#ifndef BYTESTREAM_HPP
#define BYTESTREAM_HPP
#include <string>

class ByteStream {
    private:
    char* buffer;
    int offset;
    int length;
    int bitOffset;
    public:
    ByteStream(int initialCapacity);
    ~ByteStream();
    char* getByteArray();
    void setByteArray(char* buffer, int length);
    int getLength();
    int getOffset();
    char* getDataPointer();
    void ensureCapacity(int capacity);
    bool isAtEnd();
    void resetOffset();

    char readByte();
    bool readBoolean();
    short readShort();
    int readInt();
    int readVInt();
    void readBytes(char* output, int length);
    char* readBytes(int length, int maxCapacity);
    std::string* readString();
    void readString(std::string* str);

    void writeByte(char value);
    void writeBoolean(bool value);
    void writeShort(short value);
    void writeInt(int value);
    void writeVInt(int value);
    void writeBytes(char* buffer, int length);
    void writeString(std::string* str);
};
#endif